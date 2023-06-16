#define _FILETR_C

#include "application.h"
#include "filter.h"
#include "Protocol/protocol.h" 

typedef enum{
    _CURLIM_DISABLE = 0,
    _CURLIM_LOW,
    _CURLIM_MED,
    _CURLIM_HIGH
}FASE_CURRENT_MODE_t;

typedef enum{
    _uSTEP_1 = 0,
    _uSTEP_2,
    _uSTEP_4,
    _uSTEP_16
}MICROSTEP_t;

typedef enum{
    _DIRCW = 0,
    _DIRCCW
}DIRECTION_t;

typedef enum{
    _STOP_BECAUSE_PULSES = 0,
    _STOP_BECAUSE_OPTO,
    _STOP_BECAUSE_ELSE
}STOPMODE_t;

typedef struct{
        uint16_t init_period;
        uint16_t final_period;
        uint8_t  ramp_rate;
        uint32_t pulses;
        bool     pulse_ena;
        bool     running;
        STOPMODE_t cause;
        
}FILTER_MOTOR_t;

#define MOTOR_LED_ON uc_DL9_Set();
#define MOTOR_LED_OFF uc_DL9_Clear();

#define MOTOR_ENA uc_ENA_Clear()
#define MOTOR_DIS uc_ENA_Set()

#define MOTOR_RST_ON  uc_RESET_Clear()
#define MOTOR_RST_OFF uc_RESET_Set()

#define MOTOR_SLEEP_ON  uc_SLEEP_Clear()
#define MOTOR_SLEEP_OFF uc_SLEEP_Set()

#define MOTOR_CW  uc_DIR_Clear()
#define MOTOR_CCW uc_DIR_Set()

static void filterCallback(TC_COMPARE_STATUS status, uintptr_t context);
static FILTER_MOTOR_t filterMotor;

void setFaseCurrentMode(FASE_CURRENT_MODE_t val){
    switch(val){
        case _CURLIM_DISABLE:
            MOTOR_DIS;
            break;
        case  _CURLIM_LOW:
            uc_REFA_Set();
            uc_REFB_Set();
            MOTOR_ENA;
            break;
        case  _CURLIM_MED:
            uc_REFA_Set();
            uc_REFB_Clear();        
            MOTOR_ENA;    
            break;
        case  _CURLIM_HIGH:
            uc_REFA_Clear();
            uc_REFB_Set();        
            MOTOR_ENA;
            break;
    }
    
}

void setMicroStep(MICROSTEP_t val){
    if(val == _uSTEP_1){
        uc_MS1_Clear();
        uc_MS2_Clear();
    }else if(val == _uSTEP_2){
        uc_MS1_Set();
        uc_MS2_Clear();
    }else if(val == _uSTEP_4){
        uc_MS1_Clear();
        uc_MS2_Set();
    }else{
        uc_MS1_Set();
        uc_MS2_Set();
    }
}

void FilterInit(void){
    
    setFaseCurrentMode(_CURLIM_DISABLE);    
    MOTOR_RST_OFF;
    MOTOR_SLEEP_OFF;

    TC1_CompareStop();
    TC1_CompareCallbackRegister(filterCallback, 0);

    filterMotor.running = false;

}
    
void FilterLoop(void){
    
}


void startFilterMotor(MICROSTEP_t mc_mode,  DIRECTION_t direction, uint16_t period, uint16_t init_period, uint8_t ramp, uint32_t pulses){
    
    filterMotor.final_period = period;
    filterMotor.init_period = init_period;
    filterMotor.ramp_rate = ramp;
        
    if(pulses){
        filterMotor.pulses = pulses;
        filterMotor.pulse_ena = true;        
    }else filterMotor.pulse_ena = false;        

    if(direction == _DIRCW) MOTOR_CW;
    else MOTOR_CCW;

    setMicroStep(mc_mode);
    setFaseCurrentMode(_CURLIM_HIGH);     
    MOTOR_RST_OFF; 
    MOTOR_SLEEP_OFF;
    MOTOR_LED_ON;
    
    TC1_Compare16bitPeriodSet(filterMotor.init_period);
    filterMotor.running = true;
    TC1_CompareStart();
}

void stopFilterMotor(STOPMODE_t stopmode, FASE_CURRENT_MODE_t fasemode){
        TC1_CompareStop();
        setFaseCurrentMode(fasemode);
        filterMotor.cause = stopmode;
        filterMotor.running = false;
        MOTOR_LED_OFF;
        
}

void FilterTest(void){
    
    static bool button_stat = false;

    
    if(uc_OPTO_Get()){
        uc_DL7_Set();
    }else{
        uc_DL7_Clear();
    }
    
    if(button_stat != uc_PUSHBUTTON_Get()){
        button_stat = uc_PUSHBUTTON_Get();
        
        if(button_stat) startFilterMotor(_uSTEP_16,_DIRCW,50,500,1,60000);
        else stopFilterMotor(_STOP_BECAUSE_ELSE, _CURLIM_LOW);
    }
    
    if(button_stat && (!filterMotor.running)){
        button_stat = false;
    }
}

void filterCallback(TC_COMPARE_STATUS status, uintptr_t context){
    static bool front = false;    
    if(!front) return;
    front = !front;
    
    // Acceleration ramp
    uint16_t current_period = TC1_Compare16bitPeriodGet();
    if(current_period > filterMotor.final_period){
        TC1_Compare16bitPeriodSet(current_period - filterMotor.ramp_rate);
    }
    
    // Test if the pulses expires
    if(filterMotor.pulse_ena){
        filterMotor.pulses--;
        if(filterMotor.pulses == 0){
            stopFilterMotor(_STOP_BECAUSE_PULSES, _CURLIM_LOW);
            return;
        }
    }
    
}