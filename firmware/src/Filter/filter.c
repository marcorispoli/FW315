#define _FILETR_C

#include "application.h"
#include "filter.h"
#include "Protocol/protocol.h" 

#define _CURLIM_LOW 0
#define _CURLIM_MED 1
#define _CURLIM_HIGH 2

#define _uSTEP_1 0
#define _uSTEP_2 1
#define _uSTEP_4 2
#define _uSTEP_16 3

#define MOTOR_ENA uc_ENA_Clear()
#define MOTOR_DIS uc_ENA_Set()

#define MOTOR_RST_ON  uc_RESET_Clear()
#define MOTOR_RST_OFF uc_RESET_Set()

#define MOTOR_SLEEP_ON  uc_SLEEP_Clear()
#define MOTOR_SLEEP_OFF uc_SLEEP_Set()

#define MOTOR_CW  uc_DIR_Clear()
#define MOTOR_CCW uc_DIR_Set()


void setCurrentLimit(uint8_t val){
    if(val == _CURLIM_LOW){
        uc_REFA_Set();
        uc_REFB_Set();
    }else if(val == _CURLIM_MED){
        uc_REFA_Set();
        uc_REFB_Clear();        
    }else{
        uc_REFA_Clear();
        uc_REFB_Set();        
    }
}

void setMicroStep(uint8_t val){
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
    MOTOR_DIS;
    MOTOR_RST_ON;
    MOTOR_SLEEP_ON;
    setCurrentLimit(_CURLIM_LOW); // Set the motor fase current limit
    TC1_CompareStop();

}
    
void FilterLoop(void){
    
}

void FilterTest(void){
    /*
    static bool stat = false;
    static bool button_stat = false;
    static bool test = false;
    
    if(button_stat != uc_PUSHBUTTON_Get()){
        button_stat = uc_PUSHBUTTON_Get();
        
        if(button_stat) test = !test;
    }
    
    if(!test){
        uc_DL9_Clear();
        MOTOR_DIS;
        MOTOR_RST_ON;
        TC1_CompareStop();
        stat = false;
  
        
        return;
    }else{
        uc_DL9_Set();
        if(!stat){
            setCurrentLimit(_CURLIM_HIGH); 
            setMicroStep(_uSTEP_16);
            MOTOR_CCW;
            MOTOR_ENA;
            MOTOR_RST_OFF; 
            MOTOR_SLEEP_OFF;
            TC1_CompareStart();
            stat = true;

            return;
        }
        
        
    }
     
     */
}