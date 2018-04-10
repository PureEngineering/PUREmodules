#ifndef _HAL_IF_H_
#define _HAL_IF_H_


//dwm1000 dev board
#if INTERFACE_NUMBER == 0
#include "dw1000_spi.h"
#define IF_Tx           spiWrite
#define IF_Rx           spiRead
#define IF_Init         SPI_Init
#define IF_WaitRX		SPI_waitforRX
#define IF_STR         "dwm1000DEVboard"
#endif


//dw1000 module
#if INTERFACE_NUMBER == 1
#include "dw1000_spi.h"
#define IF_Tx           spiWrite
#define IF_Rx           spiRead
#define IF_Init         spiRead
#define IF_WaitRX		spiRead
#define IF_STR         "dw1000Module"
#endif 

//#if INTERFACE_NUMBER == 2
//#define HAL_IF_Tx          
//#define HAL_IF_Rx          
//#define HAL_IF_STR         
//#endif   

#endif //_HAL_IF_H_
