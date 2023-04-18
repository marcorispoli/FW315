#define _MET_BOOTLOADER_TABLE_C
#include "MET_Bootloader_table.h"

volatile uint32_t *ramBootloaderTable = (uint32_t *)_MET_BOOTLOADER_RAM_START;

void MET_Bootloader_Activation_Request(void){
    ramBootloaderTable[_MET_BOOTLOADER_APP_REQUEST_1] = _MET_BOOTLOADER_TRIGGER_PATTERN;
    ramBootloaderTable[_MET_BOOTLOADER_APP_REQUEST_2] = _MET_BOOTLOADER_TRIGGER_PATTERN;
    NVIC_SystemReset();    
}