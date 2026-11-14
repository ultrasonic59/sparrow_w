/*
 * libelf.c
 *
 *  Created on: 22 мая 2023 г.
 *      Author: vova
 */


#include "lib_elf.h"

int _elf_errno=0;
int errno=0;

unsigned _elf_version = EV_NONE;

static const Elf _elf_init = INIT_ELF;
static const char fmag[] = ARFMAG;

static int
xread(int fd, char *buffer, size_t len) {
    size_t done = 0;
    size_t n;

    while (done < len) {
	n = read(fd, buffer + done, len - done);
	if (n == 0) {
	    /* premature end of file */
	    return -1;
	}
	else if (n != (size_t)-1) {
	    /* some bytes read, continue */
	    done += n;
	}
	else if (errno != EAGAIN && errno != EINTR) {
	    /* real error */
	    return -1;
	}
    }
    return 0;
}


void*
_elf_read(Elf *elf, void *buffer, size_t off, size_t len) {
    void *tmp;

    elf_assert(elf);
    elf_assert(elf->e_magic == ELF_MAGIC);
    elf_assert(off >= 0 && off + len <= elf->e_size);
    if (elf->e_disabled) {
	seterr(ERROR_FDDISABLED);
    }
    else if (len) {
	off += elf->e_base;
	if (lseek(elf->e_fd, (off_t)off, SEEK_SET) != (off_t)off) {
	    seterr(ERROR_IO_SEEK);
	}
	else if (!(tmp = buffer) && !(tmp = malloc(len))) {
	    seterr(ERROR_IO_2BIG);
	}
	else if (xread(elf->e_fd, tmp, len)) {
	    seterr(ERROR_IO_READ);
	    if (tmp != buffer) {
		free(tmp);
	    }
	}
	else {
	    return tmp;
	}
    }
    return NULL;
}

static unsigned long
getnum(const char *str, size_t len, int base, size_t *err) {
    unsigned long result = 0;

    while (len && *str == ' ') {
	str++; len--;
    }
    while (len && *str >= '0' && (*str - '0') < base) {
	result = base * result + *str++ - '0'; len--;
    }
    while (len && *str == ' ') {
	str++; len--;
    }
    if (len) {
	*err = len;
    }
    return result;
}
static void
_elf_init_ar(Elf *elf) {
    struct ar_hdr *hdr;
    size_t offset;
    size_t size;
    size_t err = 0;

    elf->e_kind = ELF_K_AR;
    elf->e_idlen = SARMAG;
    elf->e_off = SARMAG;

    /* process special members */
    offset = SARMAG;
    while (!elf->e_strtab && offset + sizeof(*hdr) <= elf->e_size) {
	hdr = (struct ar_hdr*)(elf->e_data + offset);
	if (memcmp(hdr->ar_fmag, fmag, sizeof(fmag) - 1)) {
	    break;
	}
	if (hdr->ar_name[0] != '/') {
	    break;
	}
	size = getnum(hdr->ar_size, sizeof(hdr->ar_size), 10, &err);
	if (err || !size) {
	    break;
	}
	offset += sizeof(*hdr);
	if (offset + size > elf->e_size) {
	    break;
	}
	if (hdr->ar_name[1] == '/' && hdr->ar_name[2] == ' ') {
	    elf->e_strtab = elf->e_data + offset;
	    elf->e_strlen = size;
	    break;
	}
	if (hdr->ar_name[1] != ' ') {
	    break;
	}
	/*
	 * Windows (.lib) archives provide two symbol tables
	 * The first one is the one we want.
	 */
	if (!elf->e_symtab) {
	    elf->e_symtab = elf->e_data + offset;
	    elf->e_symlen = size;
	}
	offset += size + (size & 1);
    }
}

static Elf_Arhdr*
_elf_arhdr(Elf *arf) {
    struct ar_hdr *hdr;
    Elf_Arhdr *arhdr;
    size_t namelen;
    size_t tmp;
    char *name;
    size_t err = 0;

    if (arf->e_off == arf->e_size) {
	/* no error! */
	return NULL;
    }
    if (arf->e_off < 0 || arf->e_off > arf->e_size) {
	seterr(ERROR_OUTSIDE);
	return NULL;
    }
    if (arf->e_off + sizeof(*hdr) > arf->e_size) {
	seterr(ERROR_TRUNC_ARHDR);
	return NULL;
    }
    elf_assert(arf->e_data != NULL);
    hdr = (struct ar_hdr*)(arf->e_data + arf->e_off);
    if (memcmp(hdr->ar_fmag, fmag, sizeof(fmag) - 1)) {
	seterr(ERROR_ARFMAG);
	return NULL;
    }

    name = hdr->ar_name;
    for (namelen = sizeof(hdr->ar_name); namelen > 0; namelen--) {
	if (name[namelen - 1] != ' ') {
	    break;
	}
    }
    if (name[0] == '/') {
	if (name[1] >= '0' && name[1] <= '9') {
	    if (!arf->e_strtab) {
		seterr(ERROR_ARSTRTAB);
		return NULL;
	    }
	    tmp = getnum(&name[1], namelen - 1, 10, &err);
	    if (err) {
		seterr(ERROR_ARSPECIAL);
		return NULL;
	    }
	    if (tmp < 0 || tmp >= arf->e_strlen) {
		seterr(ERROR_ARSTRTAB);
		return NULL;
	    }
	    for (namelen = tmp; namelen < arf->e_strlen; namelen++) {
		if (arf->e_strtab[namelen] == '/') {
		    break;
		}
	    }
	    if (namelen == arf->e_strlen) {
		seterr(ERROR_ARSTRTAB);
		return NULL;
	    }
	    name = arf->e_strtab + tmp;
	    namelen -= tmp;
	}
	else if (namelen != 1 && !(namelen == 2 && name[1] == '/')) {
	    seterr(ERROR_ARSPECIAL);
	    return NULL;
	}
    }
    else if (namelen > 0 && name[namelen - 1] == '/') {
	namelen--;
    }
    /* XXX some broken software omits the trailing slash
    else {
	namelen = 0;
    }
    */

    if (!(arhdr = (Elf_Arhdr*)malloc(sizeof(*arhdr) +
		     sizeof(hdr->ar_name) + namelen + 2))) {
	seterr(ERROR_MEM_ARHDR);
	return NULL;
    }

    arhdr->ar_name = NULL;
    arhdr->ar_rawname = (char*)(arhdr + 1);
    arhdr->ar_date = getnum(hdr->ar_date, sizeof(hdr->ar_date), 10, &err);
    arhdr->ar_uid = getnum(hdr->ar_uid, sizeof(hdr->ar_uid), 10, &err);
    arhdr->ar_gid = getnum(hdr->ar_gid, sizeof(hdr->ar_gid), 10, &err);
    arhdr->ar_mode = getnum(hdr->ar_mode, sizeof(hdr->ar_mode), 8, &err);
    arhdr->ar_size = getnum(hdr->ar_size, sizeof(hdr->ar_size), 10, &err);
    if (err) {
	free(arhdr);
	seterr(ERROR_ARHDR);
	return NULL;
    }
    if (arf->e_off + sizeof(struct ar_hdr) + arhdr->ar_size > arf->e_size) {
	free(arhdr);
	seterr(ERROR_TRUNC_MEMBER);
	return NULL;
    }

    memcpy(arhdr->ar_rawname, hdr->ar_name, sizeof(hdr->ar_name));
    arhdr->ar_rawname[sizeof(hdr->ar_name)] = '\0';

    if (namelen) {
	arhdr->ar_name = arhdr->ar_rawname + sizeof(hdr->ar_name) + 1;
	memcpy(arhdr->ar_name, name, namelen);
	arhdr->ar_name[namelen] = '\0';
    }

    return arhdr;
}
static void
_elf_check_type(Elf *elf, size_t size) {
    elf->e_idlen = size;
    if (size >= EI_NIDENT && !memcmp(elf->e_data, ELFMAG, SELFMAG)) {
	elf->e_kind = ELF_K_ELF;
	elf->e_idlen = EI_NIDENT;
	elf->e_class = elf->e_data[EI_CLASS];
	elf->e_encoding = elf->e_data[EI_DATA];
	elf->e_version = elf->e_data[EI_VERSION];
    }
    else if (size >= SARMAG && !memcmp(elf->e_data, ARMAG, SARMAG)) {
	_elf_init_ar(elf);
    }
}




#if 1
Elf*
elf_begin(int fd, Elf_Cmd cmd, Elf *ref) {
    Elf_Arhdr *arhdr = NULL;
    size_t size = 0;
    off_t off;
    Elf *elf;

    elf_assert(_elf_init.e_magic == ELF_MAGIC);
    if (_elf_version == EV_NONE) {
    	seterr(ERROR_VERSION_UNSET);
    	return NULL;
    	}
#if 1
    else if (cmd == ELF_C_NULL) {
    	return NULL;
    	}
    else if (cmd == ELF_C_WRITE) {
    	ref = NULL;
    	}
    else if (cmd != ELF_C_READ && cmd != ELF_C_RDWR) {
    	seterr(ERROR_INVALID_CMD);
    	return NULL;
    	}
    else if (ref) {
    	elf_assert(ref->e_magic == ELF_MAGIC);
    	if (!ref->e_readable || (cmd == ELF_C_RDWR && !ref->e_writable)) {
    		seterr(ERROR_CMDMISMATCH);
    		return NULL;
    	}
	if (ref->e_kind != ELF_K_AR) {
	    ref->e_count++;
	    return ref;
	}
	if (cmd == ELF_C_RDWR) {
	    seterr(ERROR_MEMBERWRITE);
	    return NULL;
	}
	if (ref->e_memory) {
	    fd = ref->e_fd;
	}
	else if (fd != ref->e_fd) {
	    seterr(ERROR_FDMISMATCH);
	    return NULL;
	}
	if (!(arhdr = _elf_arhdr(ref))) {
	    return NULL;
	}
	size = arhdr->ar_size;
    }
    else if ((off = lseek(fd, (off_t)0, SEEK_END)) == (off_t)-1
	  || (off_t)(size = off) != off) {
	seterr(ERROR_IO_GETSIZE);
	return NULL;
    }

    if (!(elf = (Elf*)malloc(sizeof(Elf)))) {
	seterr(ERROR_MEM_ELF);
	return NULL;
    }
    *elf = _elf_init;
    elf->e_fd = fd;
    elf->e_parent = ref;
    elf->e_size = elf->e_dsize = size;

    if (cmd != ELF_C_READ) {
	elf->e_writable = 1;
    }
    if (cmd != ELF_C_WRITE) {
	elf->e_readable = 1;
    }
    else {
	return elf;
    }

    if (ref) {
	size_t offset = ref->e_off + sizeof(struct ar_hdr);
	Elf *xelf;

	elf_assert(arhdr);
	elf->e_arhdr = arhdr;
	elf->e_base = ref->e_base + offset;
	/*
	 * Share the archive's memory image. To avoid
	 * multiple independent elf descriptors if the
	 * same member is requested twice, scan the list
	 * of open members for duplicates.
	 *
	 * I don't know how SVR4 handles this case. Don't rely on it.
	 */
	for (xelf = ref->e_members; xelf; xelf = xelf->e_link) {
	    elf_assert(xelf->e_parent == ref);
	    if (xelf->e_base == elf->e_base) {
		free(arhdr);
		free(elf);
		xelf->e_count++;
		return xelf;
	    }
	}
	if (size == 0) {
	    elf->e_data = NULL;
	}
#if 1
	else {
	    /*
	     * Archive members may be misaligned.  Freezing them will
	     * cause libelf to allocate buffers for translated data,
	     * which should be properly aligned in all cases.
	     */
	    elf_assert(!ref->e_cooked);
	    elf->e_data = elf->e_rawdata = ref->e_data + offset;
	}
#else
	else if (ref->e_data == ref->e_rawdata) {
	    elf_assert(!ref->e_cooked);
	    /*
	     * archive is frozen - freeze member, too
	     */
	    elf->e_data = elf->e_rawdata = ref->e_data + offset;
	}
	else {
	    elf_assert(!ref->e_memory);
	    elf->e_data = ref->e_data + offset;
	    /*
	     * The member's memory image may have been modified if
	     * the member has been processed before. Since we need the
	     * original image, we have to re-read the archive file.
	     * Will fail if the archive's file descriptor is disabled.
	     */
	    if (!ref->e_cooked) {
		ref->e_cooked = 1;
	    }
	    else if (!_elf_read(ref, elf->e_data, offset, size)) {
		free(arhdr);
		free(elf);
		return NULL;
	    }
	}
#endif
	elf->e_next = offset + size + (size & 1);
	elf->e_disabled = ref->e_disabled;
	elf->e_memory = ref->e_memory;
	/* parent/child linking */
	elf->e_link = ref->e_members;
	ref->e_members = elf;
	ref->e_count++;
	/* Slowaris compatibility - do not rely on this! */
	ref->e_off = elf->e_next;
    }
    else if (size) {
#if HAVE_MMAP
	/*
	 * Using mmap on writable files will interfere with elf_update
	 */
	if (!elf->e_writable && (elf->e_data = _elf_mmap(elf))) {
	    elf->e_unmap_data = 1;
	}
	else
#endif /* HAVE_MMAP */
	if (!(elf->e_data = _elf_read(elf, NULL, 0, size))) {
	    free(elf);
	    return NULL;
	}
    }

    _elf_check_type(elf, size);
#endif
    return elf;
}
#endif

Elf_Kind
elf_kind(Elf *elf) {
    if (!elf) {
	return ELF_K_NONE;
    }
    elf_assert(elf->e_magic == ELF_MAGIC);
    return elf->e_kind;
}
static void
_elf_free(void *ptr) {
    if (ptr) {
	free(ptr);
    }
}
static void
_elf_free_scns(Elf *elf, Elf_Scn *scn) {
    Scn_Data *sd, *tmp;
    Elf_Scn *freescn;

    for (freescn = NULL; scn; scn = scn->s_link) {
	elf_assert(scn->s_magic == SCN_MAGIC);
	elf_assert(scn->s_elf == elf);
	for (sd = scn->s_data_1; sd; sd = tmp) {
	    elf_assert(sd->sd_magic == DATA_MAGIC);
	    elf_assert(sd->sd_scn == scn);
	    tmp = sd->sd_link;
	    if (sd->sd_free_data) {
		_elf_free(sd->sd_memdata);
	    }
	    if (sd->sd_freeme) {
		free(sd);
	    }
	}
	if ((sd = scn->s_rawdata)) {
	    elf_assert(sd->sd_magic == DATA_MAGIC);
	    elf_assert(sd->sd_scn == scn);
	    if (sd->sd_free_data) {
		_elf_free(sd->sd_memdata);
	    }
	    if (sd->sd_freeme) {
		free(sd);
	    }
	}
	if (scn->s_freeme) {
	    _elf_free(freescn);
	    freescn = scn;
	}
    }
    _elf_free(freescn);
}

int
elf_end(Elf *elf) {
    Elf **siblings;

    if (!elf) {
	return 0;
    }
    elf_assert(elf->e_magic == ELF_MAGIC);
    if (--elf->e_count) {
	return elf->e_count;
    }
    if (elf->e_parent) {
	elf_assert(elf->e_parent->e_magic == ELF_MAGIC);
	elf_assert(elf->e_parent->e_kind == ELF_K_AR);
	siblings = &elf->e_parent->e_members;
	while (*siblings) {
	    if (*siblings == elf) {
		*siblings = elf->e_link;
		break;
	    }
	    siblings = &(*siblings)->e_link;
	}
	elf_end(elf->e_parent);
	_elf_free(elf->e_arhdr);
    }
#if HAVE_MMAP
    else if (elf->e_unmap_data) {
	munmap(elf->e_data, elf->e_size);
    }
#endif /* HAVE_MMAP */
    else if (!elf->e_memory) {
	_elf_free(elf->e_data);
    }
    _elf_free_scns(elf, elf->e_scn_1);
    if (elf->e_rawdata != elf->e_data) {
	_elf_free(elf->e_rawdata);
    }
    if (elf->e_free_syms) {
	_elf_free(elf->e_symtab);
    }
    _elf_free(elf->e_ehdr);
    _elf_free(elf->e_phdr);
    free(elf);
    return 0;
}


