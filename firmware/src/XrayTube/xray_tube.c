#define _XRAY_TUBE_C

#include "application.h"
#include "xray_tube.h"
#include "Protocol/protocol.h" 

#define FAN_ON uc_FAN_Clear()
#define FAN_OFF uc_FAN_Set()

static uint16_t tubeTemp;
static uint16_t bulbTemp;

void XrayInit(void){
    FAN_OFF;
    ADC0_Enable();
    ADC1_Enable();
    ADC0_ConversionStart();
    ADC1_ConversionStart();
}
    
void XrayLoop(void){
    /*
    static bool button_stat = false;
    
    
    if(button_stat != uc_PUSHBUTTON_Get()){
        button_stat = uc_PUSHBUTTON_Get();
        
        if(button_stat) FAN_ON;
        else FAN_OFF;
    }
    */
    
    // 10 bit Temperature input
    tubeTemp = ADC0_ConversionResultGet();
    bulbTemp = ADC1_ConversionResultGet();
    
    
}