/*
 * load_uboot.c
 *
 *  Created on: 22 мая 2023 г.
 *      Author: vova
 */
#include "fsbl.h"
#include "image_mover.h"
#include "xil_printf.h"
#include "xstatus.h"

#include "ff.h"
#include "sd.h"
#include "lib_elf.h"

///===================================

////static
extern FIL fil;		/* File object */
///static
extern FATFS fatfs;
///static
extern char buffer[32];
////static
extern char *boot_file ;
#if 1
///===================================
u32 Init_SD(void)
{
FRESULT rc;
TCHAR *path = "0:/"; /* Logical drive number is 0 */

/* Register volume work area, initialize device */
rc = f_mount(&fatfs, path, 0);

fsbl_printf(DEBUG_INFO,"SD: rc= %.8x\n\r", rc);

if (rc != FR_OK) {
	return XST_FAILURE;
	}

#if 0
	strcpy_rom(buffer, filename);
	boot_file = (char *)buffer;
	FlashReadBaseAddress = XPAR_PS7_SD_0_S_AXI_BASEADDR;

	rc = f_open(&fil, boot_file, FA_READ);
	if (rc) {
		fsbl_printf(DEBUG_GENERAL,"SD: Unable to open file %s: %d\n", boot_file, rc);
		return XST_FAILURE;
	}
#endif
	return XST_SUCCESS;

}
#endif
u32 open_file(char* name)
{
FRESULT rc;
rc = f_open(&fil, name, FA_READ);
if (rc) {
	fsbl_printf(DEBUG_GENERAL,"SD: Unable to open file %s: %d\n", name, rc);
	return XST_FAILURE;
	}
return XST_SUCCESS;
}
static error elf_get_startaddr_endaddr(Elf *elf, uint32_t *start_addr, uint32_t *end_addr) {
  Elf_Scn *elf_scn = NULL;
  GElf_Shdr elf_shdr;
  *start_addr = -1;
  *end_addr = 0;

  while ((elf_scn = elf_nextscn(elf, elf_scn)) != NULL) {
    if (gelf_getshdr(elf_scn, &elf_shdr) != &elf_shdr)
      return ERROR_BOOTROM_ELF;

    if (!elf_is_loadable_section(&elf_shdr))
      continue;

    if (*start_addr > elf_shdr.sh_addr)
      *start_addr = elf_shdr.sh_addr;

    if (elf_shdr.sh_addr + elf_shdr.sh_size > *end_addr)
      *end_addr = elf_shdr.sh_addr + elf_shdr.sh_size;
  }

  return SUCCESS;
}
int load_uboot(PartHeader *HeaderPtr)
{
error err;
Elf *elf;
int fd_elf=1;
////GElf_Ehdr elf_ehdr;
uint32_t start_addr;
uint32_t end_addr;

fsbl_printf(DEBUG_INFO,
			"\r\n===load_uboot====");
if(Init_SD()!=XST_SUCCESS)
	return -1;
if(open_file("u-boot.elf")!=XST_SUCCESS)
	return -1;
/* Init elf */
if ((elf = elf_begin(fd_elf, ELF_C_READ, NULL)) == NULL) {
////    close(fd_elf);
    return ERROR_BOOTROM_ELF;
  }
/* Make sure it is an elf (despite magic byte check) */
 if (elf_kind(elf) != ELF_K_ELF) {
   elf_end(elf);
 ////  close(fd_elf);
   return ERROR_BOOTROM_ELF;
 }
 if ((err = elf_get_startaddr_endaddr(elf, &start_addr, &end_addr))) {
     elf_end(elf);
     close(fd_elf);
     return err;
   }

return 0;
}

