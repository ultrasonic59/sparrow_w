/*
 * thk_sd_loader — second-stage app from BOOT_SD.BIN.
 * Loads thk_rx.elf from the SD FAT and jumps to its entry.
 */
#include "ff.h"
#include "xil_cache.h"
#include "xil_printf.h"
#include "xstatus.h"

#include <string.h>

#define SD_APP_NAME	"thk_rx.elf"
#define PT_LOAD		1U
#define DDR_LO		0x00100000U
#define DDR_HI		0x1FFFFFFFU

typedef struct {
	u8  e_ident[16];
	u16 e_type;
	u16 e_machine;
	u32 e_version;
	u32 e_entry;
	u32 e_phoff;
	u32 e_shoff;
	u32 e_flags;
	u16 e_ehsize;
	u16 e_phentsize;
	u16 e_phnum;
	u16 e_shentsize;
	u16 e_shnum;
	u16 e_shstrndx;
} Elf32_Ehdr;

typedef struct {
	u32 p_type;
	u32 p_offset;
	u32 p_vaddr;
	u32 p_paddr;
	u32 p_filesz;
	u32 p_memsz;
	u32 p_flags;
	u32 p_align;
} Elf32_Phdr;

static FATFS fatfs;
static FIL fil;

static u32 load_thk_rx_elf(u32 *EntryOut)
{
	FRESULT rc;
	UINT br;
	Elf32_Ehdr eh;
	Elf32_Phdr ph;
	u16 pi;
	u32 dest;

	*EntryOut = 0;

	rc = f_mount(&fatfs, "0:/", 0);
	if (rc != FR_OK) {
		xil_printf("loader: mount fail %d\r\n", rc);
		return XST_FAILURE;
	}

	rc = f_open(&fil, SD_APP_NAME, FA_READ);
	if (rc != FR_OK) {
		xil_printf("loader: open %s fail %d\r\n", SD_APP_NAME, rc);
		return XST_FAILURE;
	}

	rc = f_read(&fil, &eh, sizeof(eh), &br);
	if (rc != FR_OK || br != sizeof(eh) ||
	    eh.e_ident[0] != 0x7FU || eh.e_ident[1] != 'E' ||
	    eh.e_ident[2] != 'L' || eh.e_ident[3] != 'F') {
		xil_printf("loader: bad ELF\r\n");
		f_close(&fil);
		return XST_FAILURE;
	}

	for (pi = 0; pi < eh.e_phnum; pi++) {
		rc = f_lseek(&fil, eh.e_phoff + (u32)pi * eh.e_phentsize);
		if (rc != FR_OK) {
			f_close(&fil);
			return XST_FAILURE;
		}
		rc = f_read(&fil, &ph, sizeof(ph), &br);
		if (rc != FR_OK || br != sizeof(ph)) {
			f_close(&fil);
			return XST_FAILURE;
		}
		if (ph.p_type != PT_LOAD || ph.p_memsz == 0U)
			continue;

		dest = ph.p_paddr;
		if (dest < DDR_LO || (dest + ph.p_memsz - 1U) > DDR_HI) {
			xil_printf("loader: PT_LOAD out of DDR 0x%08lx\r\n", dest);
			f_close(&fil);
			return XST_FAILURE;
		}

		if (ph.p_filesz) {
			rc = f_lseek(&fil, ph.p_offset);
			if (rc != FR_OK) {
				f_close(&fil);
				return XST_FAILURE;
			}
			rc = f_read(&fil, (void *)dest, ph.p_filesz, &br);
			if (rc != FR_OK || br != ph.p_filesz) {
				xil_printf("loader: PT_LOAD read fail\r\n");
				f_close(&fil);
				return XST_FAILURE;
			}
		}
		if (ph.p_memsz > ph.p_filesz)
			memset((void *)(dest + ph.p_filesz), 0,
			       ph.p_memsz - ph.p_filesz);

		Xil_DCacheFlushRange((INTPTR)dest, ph.p_memsz);
		xil_printf("loader: PT_LOAD 0x%08lx file=0x%lx mem=0x%lx\r\n",
			   dest, ph.p_filesz, ph.p_memsz);
	}

	f_close(&fil);
	*EntryOut = eh.e_entry;
	return XST_SUCCESS;
}

int main(void)
{
	u32 entry = 0;
	void (*go)(void);

	xil_printf("thk_sd_loader: load %s\r\n", SD_APP_NAME);

	if (load_thk_rx_elf(&entry) != XST_SUCCESS || entry == 0) {
		xil_printf("thk_sd_loader: FAIL\r\n");
		while (1)
			;
	}

	xil_printf("thk_sd_loader: jump 0x%08lx\r\n", entry);
	Xil_DCacheDisable();
	Xil_ICacheDisable();
	go = (void (*)(void))entry;
	go();

	while (1)
		;
	return 0;
}
