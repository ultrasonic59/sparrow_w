#ifndef _OCTOPUZ_H_
#define _OCTOPUZ_H_

#include "stdio.h"
#include "ctype.h"

#define CURR_VERS_PAR   0
#define ID_MAX_LEN    0x1000
#define DEF_LEN_ID    265

#include "board_cnf.h"
/*
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
*/
#include "types.h"
#include "my_misc.h"

////#include "rtos.h"
#include "octopuz_types.h"
///#include "drv_utrk.h"
///================================
///#define SYS_PAR_NAME "/param/sys_par.ini"
///#define FILE_NAME_LIST_PAR "list_par.bin"
///#define DIR_PARAM_NAME "param"
///=================================
///=======================================

///#define LMSP_STACK_SIZE         (( unsigned short )256)
#define DEF_STACK_SIZE	        ( ( unsigned short ) 1024 )
///#define BT_STACK_SIZE	        ( ( unsigned short ) 1500 )
///#define BT_RX_STACK_SIZE	        ( ( unsigned short ) 512 )
#define BT_RX_STACK_SIZE	        ( ( unsigned short ) 1024 )
///#define BT_TX_STACK_SIZE	        ( ( unsigned short ) 512 )
#define BT_TX_STACK_SIZE	        ( ( unsigned short ) 1024 )
///#define VCP_RX_STACK_SIZE	        1024////( ( unsigned short ) 512 )
///#define VCP_TX_STACK_SIZE	       1024//// ( ( unsigned short ) 512 )
///#define UDP_STACK_SIZE	               1024//// (( unsigned short ) 512 )
///#define HTTP_SERVER_STACK_SIZE	       1024//// (( unsigned short ) 512 )

#define APP_PRIORITY   7

#define NUM_DEV         sys_par.num_dev
///======================================
#endif
