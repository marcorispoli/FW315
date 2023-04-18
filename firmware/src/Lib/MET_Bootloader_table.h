/* ************************************************************************** */
/*    Dichiarazione Strutture dati e costanti di pasaggio applicazione-btl    */
/* ************************************************************************** */
#include "definitions.h"                // SYS function prototypes

#ifndef _MET_BOOTLOADER_TABLE_H    /* Guard against multiple inclusion */
#define _MET_BOOTLOADER_TABLE_H

/* Definizione dimensione Bootloader */
#define _MET_BOOTLOADER_SIZE     (8192)

/* INIZIO APPLICAZIONE */
#define _MET_BOOTLOADER_APP_START_ADDRESS   (0x2200UL)

/* FLASH TABLE */
#define _MET_BOOTLOADER_TABLE_BASE          (0x2000UL)
#define _MET_BOOTLOADER_CAN_ID_OFFSET       (_MET_BOOTLOADER_TABLE_BASE + 0)
#define _MET_BOOTLOADER_APP_RELEASE_OFFSET  (_MET_BOOTLOADER_TABLE_BASE + 4)

/* RAM TABLE */
#define _MET_BOOTLOADER_RAM_START           (0x20000000)
#define _MET_BOOTLOADER_APP_REQUEST_1       (0)
#define _MET_BOOTLOADER_APP_REQUEST_2       (1)
#define _MET_BOOTLOADER_REVISION            (2)
#define _MET_BOOTLOADER_SPARE1              (3)

/* REQUEST PATTERN */
#define _MET_BOOTLOADER_TRIGGER_PATTERN     0x5048434D

/* Dichiarazione puntatore array di scambio */
volatile uint32_t *ramBootloaderTable;

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
