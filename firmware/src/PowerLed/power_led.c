#define _POWER_LED_C

#include "application.h"
#include "power_led.h"
#include "Protocol/protocol.h" 

static int light_tmo = 0;

void PowerLedInit(void){
    uc_POWERLED_Clear();
    SETBIT_FLAGS_LIGHT_ON(false);
    light_tmo = 0;
    
}

/**
 * This routine is called every 15ms
 */
void PowerLedLoop(void){
    
    if(light_tmo){
        light_tmo--;
        if(light_tmo == 0){
            SETBIT_FLAGS_LIGHT_ON(false);
            uc_POWERLED_Clear();
        }
    }
}

void PowerLedOff(void){    
    uc_POWERLED_Clear();
    SETBIT_FLAGS_LIGHT_ON(false);
    light_tmo = 0;
}

void PowerLedOn(void){
    uc_POWERLED_Set();
    light_tmo = (int) GETBYTE_PARAMETER_LIGHT_TIMEOUT * (int) 66; // 15ms to seconds
    SETBIT_FLAGS_LIGHT_ON(true);
    
}

