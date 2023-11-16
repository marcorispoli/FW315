#define _XRAY_TUBE_C

#include "application.h"
#include "xray_tube.h"
#include "Protocol/protocol.h" 

#define FAN_ON uc_FAN_Clear()
#define FAN_OFF uc_FAN_Set()

static unsigned char statorPerc;
static unsigned char bulbPerc;

/*
 * Lookup Table 
|	71	|	0	|
|	78	|	5	|
|	85	|	10	|
|	92	|	15	|
|	100	|	20	|
|	107	|	25	|
|	114	|	30	|
|	121	|	35	|
|	127	|	40	|
|	134	|	45	|
|	140	|	50	|
|	146	|	55	|
|	151	|	60	|
|	156	|	65	|
|	161	|	70	|
|	166	|	75	|
|	170	|	80	|
|	174	|	85	|
|	178	|	90	|
|	181	|	95	|
|	184	|	100	|
|	187	|	105	|
 */
void XrayInit(void){
    FAN_OFF;
    ADC0_Enable();
    ADC1_Enable();
    ADC0_ConversionStart();
    ADC1_ConversionStart();
}

typedef struct{
    unsigned char sensor;
    unsigned char perc;
}lookupT;


static const lookupT sensToPerc[] = {
    {71,0}, //!< 0°C
    {78,0}, //!< 5°C
    {85,0}, //!< 10°C
    {92,0}, //!< 15°C
    {100,0},//!< 20°C
    {107,6},//!< 25°C
    {114,12},//!< 30°C
    {121,18},//!< 35°C
    {127,24},//!< 40°C
    {134,30},//!< 45°C
    {140,36},//!< 50°C
    {146,42},//!< 55°C
    {151,48},//!< 60°C
    {156,54},//!< 65°C
    {161,60},//!< 70°C
    {166,66},//!< 75°C
    {170,72},//!< 80°C
    {174,78},//!< 85°C
    {178,84},//!< 90°C
    {181,90},//!< 95°C
    {184,96},//!< 100°C
    {187,100}//!< 105°C
               
};

static unsigned char analogToPerc(unsigned char sensor){
    
    if(sensor <= 71 ) return 0;
    if(sensor >= 187 ) return 100;
    
    for(int i = 1; i < sizeof(sensToPerc) / sizeof(lookupT); i++){
        if(sensor < sensToPerc[i].sensor) return sensToPerc[i-1].perc;
    }
    
    return 0;    
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
    
    
    
    // Evaluates the Stator Sensor
    unsigned char statorSens = ADC0_ConversionResultGet();
    setStatorErrorShort(false);
    if( statorSens < 71 ){
        // Error cable Open or sensor open
        statorPerc = 0;        
        setStatorErrorLow(true);
        
    }else if( statorSens > 210 ){
        // Error cable short or sensor short
        statorPerc = 0;
        setStatorErrorShort(true);       
    }
    else {
        if( statorSens >= 84 ) setStatorErrorHigh(true);       
        statorPerc = analogToPerc(statorSens);
    }
    
    // Evaluates the Bulb Sensor
    unsigned char bulbSens = ADC1_ConversionResultGet();
    setBulbErrorLow(false);
    if( bulbSens < 71 ){
        // Error cable Open or sensor open
        bulbPerc = 0;
        setBulbErrorLow(true);
    }else if( bulbSens > 210 ){
        // Error cable short or sensor short
        bulbPerc = 0;
        setBulbErrorShort(true);
    }else {
        if( bulbSens >=84 ) setBulbErrorHigh(true);        
        bulbPerc = analogToPerc(bulbSens);       
    }    
    
}