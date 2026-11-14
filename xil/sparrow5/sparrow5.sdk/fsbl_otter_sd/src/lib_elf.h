/*
 * libelf.h - public header file for libelf.
 * Copyright (C) 1995 - 2008 Michael Riepe
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * 
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */

/* @(#) $Id: libelf.h,v 1.29 2009/07/07 17:57:43 michael Exp $ */

#ifndef _LIB_ELF_H
#define _LIB_ELF_H

#include <stddef.h>	/* for size_t */
#include <sys/types.h>

#include "sys_elf.h"

#ifndef SEEK_SET
#define SEEK_SET	0
#endif /* SEEK_SET */
#ifndef SEEK_CUR
#define SEEK_CUR	1
#endif /* SEEK_CUR */
#ifndef SEEK_END
#define SEEK_END	2
#endif /* SEEK_END */

#define EAGAIN          11
#define EINTR           4

typedef enum error
{
  /* The job was ended sucessfully */
  SUCCESS = 0,

  /* Common error codes */
  ERROR_NOMEM = 1,
  ERROR_CANT_READ,
  ERROR_CANT_WRITE,
  ERROR_ITERATION_END,

  /* BIF parser specific errors */
  ERROR_BIF_NOFILE,
  ERROR_BIF_PARSER,
  ERROR_BIF_UNSUPPORTED_ATTR,
  ERROR_BIF_UNINITIALIZED,
  ERROR_BIF_UNSUPPORTED_VAL,
  ERROR_BIF_LEXER,

  /* Bootrom compiler specific errors */
  ERROR_BOOTROM_NOFILE,
  ERROR_BOOTROM_BITSTREAM,
  ERROR_BOOTROM_ELF,
  ERROR_BOOTROM_SEC_OVERLAP,
  ERROR_BOOTROM_UNSUPPORTED,
  ERROR_BOOTROM_NOMEM,

  /* Bootrom parser specific errors */
  ERROR_BIN_FILE_EXISTS,
  ERROR_BIN_NOFILE,
  ERROR_BIN_WADDR,
} error;

#define ERROR_VERSION_UNSET    4 ////????
#define ERROR_INVALID_CMD      5  ////????
#define ERROR_CMDMISMATCH      6  ///???
#define ERROR_MEMBERWRITE      7  ///???
#define ERROR_FDMISMATCH       8   ///???
#define  ERROR_IO_GETSIZE      9   ///???
#define ERROR_MEM_ELF          10  ///???
#define ERROR_BOOTROM_ELF      11  ///???
#define ERROR_OUTSIDE          12  ///???
#define ERROR_TRUNC_ARHDR      13   ///???
#define ERROR_ARFMAG           14   ///???
#define ERROR_ARSTRTAB         15
#define ERROR_ARSPECIAL        16
#define ERROR_ARHDR            17
#define ERROR_MEM_ARHDR        18
#define ERROR_TRUNC_MEMBER     19
#define ERROR_FDDISABLED        20   ///???
#define ERROR_IO_SEEK        21   ///???
#define ERROR_IO_2BIG        22   ///???
#define ERROR_IO_READ        23   ///???

/*
 * e_ident
 */
#define EI_MAG0		0
#define EI_MAG1		1
#define EI_MAG2		2
#define EI_MAG3		3
#define EI_CLASS	4
#define EI_DATA		5
#define EI_VERSION	6
#define EI_OSABI	7
#define EI_ABIVERSION	8
#define EI_PAD		9

#define ARMAG	"!ARM\n"
#define SARMAG	8


#define ELFMAG0		0x7f
#define ELFMAG1		'E'
#define ELFMAG2		'L'
#define ELFMAG3		'F'
#define ELFMAG		"\177ELF"
#define SELFMAG		4


#define EI_NIDENT	16


#define ELFCLASSNONE	0
#define ELFCLASS32	1
#define ELFCLASS64	2
#define ELFCLASSNUM	3

#define ELFDATANONE	0
#define ELFDATA2LSB	1
#define ELFDATA2MSB	2
#define ELFDATANUM	3

#define EV_NONE		0
#define EV_CURRENT	1
#define EV_NUM		2

#define ARFMAG	"`\n"

/*
 * File types
 */
typedef enum {
    ELF_K_NONE = 0,	/* must be first, 0 */
    ELF_K_AR,
    ELF_K_COFF,
    ELF_K_ELF,
    ELF_K_NUM		/* must be last */
} Elf_Kind;

typedef struct Elf_Scn	Elf_Scn;
/*
 * Elf descriptor
 */
typedef struct Elf	Elf;
typedef struct Scn_Data Scn_Data;

typedef uint32_t	    Elf32_Addr;
typedef uint32_t		Elf32_Half;
typedef uint32_t		Elf32_Off;
typedef int32_t		    Elf32_Sword;
typedef uint32_t		Elf32_Word;

/*
 * Section header
 */
typedef struct {
    Elf32_Word		sh_name;
    Elf32_Word		sh_type;
    Elf32_Word		sh_flags;
    Elf32_Addr		sh_addr;
    Elf32_Off		sh_offset;
    Elf32_Word		sh_size;
    Elf32_Word		sh_link;
    Elf32_Word		sh_info;
    Elf32_Word		sh_addralign;
    Elf32_Word		sh_entsize;
} Elf32_Shdr;


typedef Elf32_Shdr	GElf_Shdr;			////????
/*
 * Archive member header
 */
typedef struct {
    char*		ar_name;
    time_t		ar_date;
    long		ar_uid;
    long 		ar_gid;
    unsigned long	ar_mode;
    off_t		ar_size;
    char*		ar_rawname;
} Elf_Arhdr;
/*
 * Section descriptor
 */
typedef struct Elf_Scn	Elf_Scn;
struct ar_hdr {
    char    ar_name[16];
    char    ar_date[12];
    char    ar_uid[6];
    char    ar_gid[6];
    char    ar_mode[8];
    char    ar_size[10];
    char    ar_fmag[2];
};
/*
 * Data types
 */
typedef enum {
    ELF_T_BYTE = 0,	/* must be first, 0 */
    ELF_T_ADDR,
    ELF_T_DYN,
    ELF_T_EHDR,
    ELF_T_HALF,
    ELF_T_OFF,
    ELF_T_PHDR,
    ELF_T_RELA,
    ELF_T_REL,
    ELF_T_SHDR,
    ELF_T_SWORD,
    ELF_T_SYM,
    ELF_T_WORD,
    /*
     * New stuff for 64-bit.
     *
     * Most implementations add ELF_T_SXWORD after ELF_T_SWORD
     * which breaks binary compatibility with earlier versions.
     * If this causes problems for you, contact me.
     */
    ELF_T_SXWORD,
    ELF_T_XWORD,
    /*
     * Symbol versioning.  Sun broke binary compatibility (again!),
     * but I won't.
     */
    ELF_T_VDEF,
    ELF_T_VNEED,
    ELF_T_NUM		/* must be last */
} Elf_Type;

/*
 * Data descriptor
 */
typedef struct {
    void*		d_buf;
    Elf_Type		d_type;
    size_t		d_size;
    off_t		d_off;
    size_t		d_align;
    unsigned		d_version;
} Elf_Data;


typedef struct Scn_Data Scn_Data;
/*
 * Data descriptor
 */
struct Scn_Data {
    Elf_Data	sd_data;		/* must be first! */
    Scn_Data*	sd_link;		/* pointer to next Scn_Data */
    Elf_Scn*	sd_scn;			/* pointer to section */
    char*	sd_memdata;		/* memory image of section */
    unsigned	sd_data_flags;		/* data flags (ELF_F_*) */
    /* misc flags */
    unsigned	sd_freeme : 1;		/* this Scn_Data was malloc'ed */
    unsigned	sd_free_data : 1;	/* sd_memdata is malloc'ed */
    /* magic number for debugging */
    long	sd_magic;
};


/*
 * ELF descriptor
 */
struct Elf {
    /* common */
    size_t	e_size;			/* file/member size */
    size_t	e_dsize;		/* size of memory image */
    Elf_Kind	e_kind;			/* kind of file */
    char*	e_data;			/* file/member data */
    char*	e_rawdata;		/* file/member raw data */
    size_t	e_idlen;		/* identifier size */
    int		e_fd;			/* file descriptor */
    unsigned	e_count;		/* activation count */
    /* archive members (still common) */
    Elf*	e_parent;		/* NULL if not an archive member */
    size_t	e_next;			/* 0 if not an archive member */
    size_t	e_base;			/* 0 if not an archive member */
    Elf*	e_link;			/* next archive member or NULL */
    Elf_Arhdr*	e_arhdr;		/* archive member header or NULL */
    /* archives */
    size_t	e_off;			/* current member offset (for elf_begin) */
    Elf*	e_members;		/* linked list of active archive members */
    char*	e_symtab;		/* archive symbol table */
    size_t	e_symlen;		/* length of archive symbol table */
    char*	e_strtab;		/* archive string table */
    size_t	e_strlen;		/* length of archive string table */
    /* ELF files */
    unsigned	e_class;		/* ELF class */
    unsigned	e_encoding;		/* ELF data encoding */
    unsigned	e_version;		/* ELF version */
    char*	e_ehdr;			/* ELF header */
    char*	e_phdr;			/* ELF program header table */
    size_t	e_phnum;		/* size of program header table */
    Elf_Scn*	e_scn_1;		/* first section */
    Elf_Scn*	e_scn_n;		/* last section */
    unsigned	e_elf_flags;		/* elf flags (ELF_F_*) */
    unsigned	e_ehdr_flags;		/* ehdr flags (ELF_F_*) */
    unsigned	e_phdr_flags;		/* phdr flags (ELF_F_*) */
    /* misc flags */
    unsigned	e_readable : 1;		/* file is readable */
    unsigned	e_writable : 1;		/* file is writable */
    unsigned	e_disabled : 1;		/* e_fd has been disabled */
    unsigned	e_cooked : 1;		/* e_data was modified */
    unsigned	e_free_syms : 1;	/* e_symtab is malloc'ed */
    unsigned	e_unmap_data : 1;	/* e_data is mmap'ed */
    unsigned	e_memory : 1;		/* created by elf_memory() */
    /* magic number for debugging */
    long	e_magic;
};
/*
 * Section descriptor
 */
struct Elf_Scn {
    Elf_Scn*	s_link;			/* pointer to next Elf_Scn */
    Elf*	s_elf;			/* pointer to elf descriptor */
    size_t	s_index;		/* number of this section */
    unsigned	s_scn_flags;		/* section flags (ELF_F_*) */
    unsigned	s_shdr_flags;		/* shdr flags (ELF_F_*) */
    Scn_Data*	s_data_1;		/* first data buffer */
    Scn_Data*	s_data_n;		/* last data buffer */
    Scn_Data*	s_rawdata;		/* raw data buffer */
    /* data copied from shdr */
    unsigned	s_type;			/* section type */
    size_t	s_offset;		/* section offset */
    size_t	s_size;			/* section size */
    /* misc flags */
    unsigned	s_freeme : 1;		/* this Elf_Scn was malloc'ed */
    /* section header */
    union {
#if __LIBELF64
	Elf64_Shdr	u_shdr64;
#endif /* __LIBELF64 */
	Elf32_Shdr	u_shdr32;
    }		s_uhdr;
    /* magic number for debugging */
    long	s_magic;
};


#define ELF_MAGIC	0x012b649e

#define INIT_ELF	{\
    /* e_size */	0,\
    /* e_dsize */	0,\
    /* e_kind */	ELF_K_NONE,\
    /* e_data */	NULL,\
    /* e_rawdata */	NULL,\
    /* e_idlen */	0,\
    /* e_fd */		-1,\
    /* e_count */	1,\
    /* e_parent */	NULL,\
    /* e_next */	0,\
    /* e_base */	0,\
    /* e_link */	NULL,\
    /* e_arhdr */	NULL,\
    /* e_off */		0,\
    /* e_members */	NULL,\
    /* e_symtab */	NULL,\
    /* e_symlen */	0,\
    /* e_strtab */	NULL,\
    /* e_strlen */	0,\
    /* e_class */	ELFCLASSNONE,\
    /* e_encoding */	ELFDATANONE,\
    /* e_version */	EV_NONE,\
    /* e_ehdr */	NULL,\
    /* e_phdr */	NULL,\
    /* e_phnum */	0,\
    /* e_scn_1 */	NULL,\
    /* e_scn_n */	NULL,\
    /* e_elf_flags */	0,\
    /* e_ehdr_flags */	0,\
    /* e_phdr_flags */	0,\
    /* e_readable */	0,\
    /* e_writable */	0,\
    /* e_disabled */	0,\
    /* e_cooked */	0,\
    /* e_free_syms */	0,\
    /* e_unmap_data */	0,\
    /* e_memory */	0,\
    /* e_magic */	ELF_MAGIC\
}


#if defined __GNUC__ && !defined __cplusplus
#define DEPRECATED	__attribute__((deprecated))
#else
#define DEPRECATED	/* nothing */
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef __P
# if (__STDC__ + 0) || defined(__cplusplus) || defined(_WIN32)
#  define __P(args) args
# else /* __STDC__ || defined(__cplusplus) */
#  define __P(args) ()
# endif /* __STDC__ || defined(__cplusplus) */
#endif /* __P */

/*
 * Commands
 */
typedef enum {
    ELF_C_NULL = 0,	/* must be first, 0 */
    ELF_C_READ,
    ELF_C_WRITE,
    ELF_C_CLR,
    ELF_C_SET,
    ELF_C_FDDONE,
    ELF_C_FDREAD,
    ELF_C_RDWR,
    ELF_C_NUM		/* must be last */
} Elf_Cmd;

/*
 * Flags
 */
#define ELF_F_DIRTY	0x1
#define ELF_F_LAYOUT	0x4
/*
 * Allow sections to overlap when ELF_F_LAYOUT is in effect.
 * Note that this flag ist NOT portable, and that it may render
 * the output file unusable.  Use with extreme caution!
 */
#define ELF_F_LAYOUT_OVERLAP	0x10000000

#if 0
/*
 * Data types
 */
typedef enum {
    ELF_T_BYTE = 0,	/* must be first, 0 */
    ELF_T_ADDR,
    ELF_T_DYN,
    ELF_T_EHDR,
    ELF_T_HALF,
    ELF_T_OFF,
    ELF_T_PHDR,
    ELF_T_RELA,
    ELF_T_REL,
    ELF_T_SHDR,
    ELF_T_SWORD,
    ELF_T_SYM,
    ELF_T_WORD,
    /*
     * New stuff for 64-bit.
     *
     * Most implementations add ELF_T_SXWORD after ELF_T_SWORD
     * which breaks binary compatibility with earlier versions.
     * If this causes problems for you, contact me.
     */
    ELF_T_SXWORD,
    ELF_T_XWORD,
    /*
     * Symbol versioning.  Sun broke binary compatibility (again!),
     * but I won't.
     */
    ELF_T_VDEF,
    ELF_T_VNEED,
    ELF_T_NUM		/* must be last */
} Elf_Type;

#endif


/*
 * Archive symbol table
 */
typedef struct {
    char*		as_name;
    size_t		as_off;
    unsigned long	as_hash;
} Elf_Arsym;
#if 0
/*
 * Data descriptor
 */
typedef struct {
    void*		d_buf;
    Elf_Type		d_type;
    size_t		d_size;
    off_t		d_off;
    size_t		d_align;
    unsigned		d_version;
} Elf_Data;
#endif
#if 0
/*
 * Function declarations
 */
extern Elf *elf_begin __P((int __fd, Elf_Cmd __cmd, Elf *__ref));
extern Elf *elf_memory __P((char *__image, size_t __size));
extern int elf_cntl __P((Elf *__elf, Elf_Cmd __cmd));
extern int elf_end __P((Elf *__elf));
extern const char *elf_errmsg __P((int __err));
extern int elf_errno __P((void));
extern void elf_fill __P((int __fill));
extern unsigned elf_flagdata __P((Elf_Data *__data, Elf_Cmd __cmd,
	unsigned __flags));
extern unsigned elf_flagehdr __P((Elf *__elf, Elf_Cmd __cmd,
	unsigned __flags));
extern unsigned elf_flagelf __P((Elf *__elf, Elf_Cmd __cmd,
	unsigned __flags));
extern unsigned elf_flagphdr __P((Elf *__elf, Elf_Cmd __cmd,
	unsigned __flags));
extern unsigned elf_flagscn __P((Elf_Scn *__scn, Elf_Cmd __cmd,
	unsigned __flags));
extern unsigned elf_flagshdr __P((Elf_Scn *__scn, Elf_Cmd __cmd,
	unsigned __flags));
extern size_t elf32_fsize __P((Elf_Type __type, size_t __count,
	unsigned __ver));
extern Elf_Arhdr *elf_getarhdr __P((Elf *__elf));
extern Elf_Arsym *elf_getarsym __P((Elf *__elf, size_t *__ptr));
extern off_t elf_getbase __P((Elf *__elf));
extern Elf_Data *elf_getdata __P((Elf_Scn *__scn, Elf_Data *__data));
extern Elf32_Ehdr *elf32_getehdr __P((Elf *__elf));
extern char *elf_getident __P((Elf *__elf, size_t *__ptr));
extern Elf32_Phdr *elf32_getphdr __P((Elf *__elf));
extern Elf_Scn *elf_getscn __P((Elf *__elf, size_t __index));
extern Elf32_Shdr *elf32_getshdr __P((Elf_Scn *__scn));
extern unsigned long elf_hash __P((const unsigned char *__name));
extern Elf_Kind elf_kind __P((Elf *__elf));
extern size_t elf_ndxscn __P((Elf_Scn *__scn));
extern Elf_Data *elf_newdata __P((Elf_Scn *__scn));
extern Elf32_Ehdr *elf32_newehdr __P((Elf *__elf));
extern Elf32_Phdr *elf32_newphdr __P((Elf *__elf, size_t __count));
extern Elf_Scn *elf_newscn __P((Elf *__elf));
extern Elf_Cmd elf_next __P((Elf *__elf));
extern Elf_Scn *elf_nextscn __P((Elf *__elf, Elf_Scn *__scn));
extern size_t elf_rand __P((Elf *__elf, size_t __offset));
extern Elf_Data *elf_rawdata __P((Elf_Scn *__scn, Elf_Data *__data));
extern char *elf_rawfile __P((Elf *__elf, size_t *__ptr));
extern char *elf_strptr __P((Elf *__elf, size_t __section, size_t __offset));
extern off_t elf_update __P((Elf *__elf, Elf_Cmd __cmd));
extern unsigned elf_version __P((unsigned __ver));
extern Elf_Data *elf32_xlatetof __P((Elf_Data *__dst, const Elf_Data *__src,
	unsigned __encode));
extern Elf_Data *elf32_xlatetom __P((Elf_Data *__dst, const Elf_Data *__src,
	unsigned __encode));
#endif
/*
 * Additional functions found on Solaris
 */
extern long elf32_checksum __P((Elf *__elf));

#if 0
/*
 * ELF format extensions
 *
 * These functions return 0 on failure, 1 on success.  Since other
 * implementations of libelf may behave differently (there was quite
 * some confusion about the correct values), they are now officially
 * deprecated and should be replaced with the three new functions below.
 */
DEPRECATED extern int elf_getphnum __P((Elf *__elf, size_t *__resultp));
DEPRECATED extern int elf_getshnum __P((Elf *__elf, size_t *__resultp));
DEPRECATED extern int elf_getshstrndx __P((Elf *__elf, size_t *__resultp));
/*
 * Replacement functions (return -1 on failure, 0 on success).
 */
extern int elf_getphdrnum __P((Elf *__elf, size_t *__resultp));
extern int elf_getshdrnum __P((Elf *__elf, size_t *__resultp));
extern int elf_getshdrstrndx __P((Elf *__elf, size_t *__resultp));

/*
 * Convenience functions
 *
 * elfx_update_shstrndx is elf_getshstrndx's counterpart.
 * It should be used to set the e_shstrndx member.
 * There is no update function for e_shnum or e_phnum
 * because libelf handles them internally.
 */
extern int elfx_update_shstrndx __P((Elf *__elf, size_t __index));

/*
 * Experimental extensions:
 *
 * elfx_movscn() moves section `__scn' directly after section `__after'.
 * elfx_remscn() removes section `__scn'.  Both functions update
 * the section indices; elfx_remscn() also adjusts the ELF header's
 * e_shnum member.  The application is responsible for updating other
 * data (in particular, e_shstrndx and the section headers' sh_link and
 * sh_info members).
 *
 * elfx_movscn() returns the new index of the moved section.
 * elfx_remscn() returns the original index of the removed section.
 * A return value of zero indicates an error.
 */
extern size_t elfx_movscn __P((Elf *__elf, Elf_Scn *__scn, Elf_Scn *__after));
extern size_t elfx_remscn __P((Elf *__elf, Elf_Scn *__scn));

/*
 * elf_delscn() is obsolete.  Please use elfx_remscn() instead.
 */
extern size_t elf_delscn __P((Elf *__elf, Elf_Scn *__scn));
#endif
extern int _elf_errno;

#define seterr(err)	(_elf_errno = (err))

/*
 * Debugging
 */
#if ENABLE_DEBUG
extern void __elf_assert __P((const char*, unsigned, const char*));
# if (__STDC__ + 0)
#  define elf_assert(x)	do{if(!(x))__elf_assert(__FILE__,__LINE__,#x);}while(0)
# else /* __STDC__ */
#  define elf_assert(x)	do{if(!(x))__elf_assert(__FILE__,__LINE__,"x");}while(0)
# endif /* __STDC__ */
#else /* ENABLE_DEBUG */
# define elf_assert(x)	do{}while(0)
#endif /* ENABLE_DEBUG */


extern Elf*
elf_begin(int fd, Elf_Cmd cmd, Elf *ref) ;
extern Elf_Kind
elf_kind(Elf *elf);

extern int
elf_end(Elf *elf) ;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _LIBELF_H */
