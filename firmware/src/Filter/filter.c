#define _FILETR_C

#include "application.h"
#include "filter.h"
#include "Protocol/protocol.h" 

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

static void filterCallback(TC_COMPARE_STATUS status, uintptr_t context); //!< Callback every STEP pin changes
static volatile FILTER_MOTOR_t filterMotor; //!< Motor main structure variable declaration

/**
 * This function sets the Motor Torque
 *   
 * @param val: this is the requested motor torque
 */
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

/**
 * This function sets the Motor micro stepping mode
 * 
 * @param val: This is the requested micro stepping mode
 */
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

/**
 * This is the function to activate the Motor Driver.
 * 
 * When the function is called, every step will cause a motor rotation.
 * 
 * @param direction: this is the direction of the shaft
 * @param mc_mode: this is the micro-stepping mode
 * @param final_period: this is the final pwm period (maximum speed)
 * @param init_period: this is the initial pwm period (minimum speed)
 * @param ramp: this is the period decrease for every period (speed ramp)
 */
void startMotor(DIRECTION_t direction, MICROSTEP_t mc_mode, uint16_t final_period, uint16_t init_period, uint16_t ramp){
    
    // Speed regulation
    filterMotor.final_period = final_period;
    filterMotor.init_period = init_period;
    filterMotor.ramp_rate = ramp;
    filterMotor.running = true;
    
    // Direction
    if(direction == MOTOR_DIR_HOME) MOTOR_CW;
    else MOTOR_CCW;

    // Step mode
    setMicroStep(mc_mode);
    setFaseCurrentMode(_CURLIM_HIGH); 
    
    TC1_CompareStop();    
    TC1_Compare16bitPeriodSet(init_period);
    TC1_CompareStart();
    
    // Motor activation output pin
    MOTOR_RST_OFF; 
    MOTOR_SLEEP_OFF;
    MOTOR_LED_ON;
    
}

/**
 * Module Filter initialization
 */
void FilterInit(void){
    filterMotor.command_activated = false;
    filterMotor.slot_valid = false;
    filterMotor.command_sequence = 0;
    filterMotor.running = false; 
  
    // Initializes the Protocol
    SETBYTE_SLOT_SELECTED(SYSTEM_OUT_POSITION);
    
    // Disables the motor driver
    setFaseCurrentMode(_CURLIM_DISABLE);    
    MOTOR_RST_OFF;
    MOTOR_SLEEP_OFF;

    // Stop the counter
    TC1_CompareStop();
    
    // Registers the working callback
    TC1_CompareCallbackRegister(filterCallback, 0);

    
}


bool FilterIsRunning(void){
     return filterMotor.command_activated;
}

bool FilterIsError(void){
     return  (!filterMotor.slot_valid) ;
}
 
bool FilterIsTarget(uint8_t filter){
    if(!filterMotor.slot_valid) return false;
    if(filterMotor.command_activated) return false;
    if(filter != filterMotor.target_filter) return false;
    
    return true;
}

bool FilterSelect(uint8_t filter){

    // Command Busy
    if(filterMotor.command_activated ) return false;
    
    // Assignes the current target position
    if(filter == POSITIONER_SELECT_FILTER1){ 
        filterMotor.target_slot = FILTER1_SLOT;
        filterMotor.target_slot_position[FILTER1_SLOT] = umToSteps(GETWORD_PARAMETER_FILTER1_POSITION);        
    }  else if(filter == POSITIONER_SELECT_FILTER2){ 
        filterMotor.target_slot = FILTER2_SLOT;
        filterMotor.target_slot_position[FILTER2_SLOT] = umToSteps(GETWORD_PARAMETER_FILTER2_POSITION);
    }  else if(filter == POSITIONER_SELECT_FILTER3){ 
        filterMotor.target_slot = FILTER3_SLOT;
        filterMotor.target_slot_position[FILTER3_SLOT] = umToSteps(GETWORD_PARAMETER_FILTER3_POSITION);
    }  else if(filter == POSITIONER_SELECT_FILTER4){ 
        filterMotor.target_slot = FILTER4_SLOT;
        filterMotor.target_slot_position[FILTER4_SLOT] = umToSteps(GETWORD_PARAMETER_FILTER4_POSITION);
    }  else if(filter == POSITIONER_SELECT_MIRROR){ 
        filterMotor.target_slot = MIRROR_SLOT;
        filterMotor.target_slot_position[MIRROR_SLOT] = umToSteps(GETWORD_PARAMETER_MIRROR_POSITION);
    }  else return false; 
    
 
    filterMotor.target_filter = filter;
    filterMotor.command_activated = true;        
    filterMotor.command_sequence = 0;
    filterMotor.slot_valid = false;
    
    // Set the Protocol Filter status to running mode
    SETBYTE_SLOT_SELECTED(SYSTEM_SELECTION_PENDING);
    
    startMotor(MOTOR_DIR_HOME,MOTOR_SPEED_HOME);                  
    return true;
}

/**
 * This function stops the motor driver.
 * 
 * The cause of the stop and the Torque applied in stop, will be passed as parameters. 
 * @param cause: this is the cause of the stop
 * @param torque: this is the motor torque setting 
 */
void stopMotor(STOPMODE_t cause, FASE_CURRENT_MODE_t torque){
        setFaseCurrentMode(torque);
        filterMotor.cause = cause;
        filterMotor.running = false;
        MOTOR_LED_OFF;       
       
}


void FilterTest2(void){
    static uint8_t filtro = SYSTEM_FILTER1_SELECTED;
    if(FilterSelect(filtro)){
        filtro++;
        if(filtro > SYSTEM_MIRROR_SELECTED ) filtro = SYSTEM_FILTER1_SELECTED;        
    }
}

void FilterTest(void){
    return;
    static bool button_stat = false;

    if(uc_OPTO_Get()){
        uc_DL7_Set();
    }else{
        uc_DL7_Clear();
    }
    
    if(button_stat != uc_PUSHBUTTON_Get()){
        button_stat = uc_PUSHBUTTON_Get();
        if(button_stat) FilterTest2();        
    }        
}



/**
 * This is the positioning Worker callback.
 * 
 * The function execute the positioning sequence.
 * 
 * @param status
 * @param context
 */
void filterCallback(TC_COMPARE_STATUS status, uintptr_t context){
   
  
   static uint32_t stp=0;
   bool error = false;
   bool completed = false;
   
   // Updates the opto current status
   filterMotor.opto_status = uc_OPTO_Get();
   
   switch(filterMotor.command_sequence){
       case 0:
           error = false;
           completed = false;
           
           // Activates the motor driver to move to Home position
           filterMotor.command_sequence++;
           stp = 0;
           return;
       
       case 1: // Wait for the opto = ENGAGED           
           stp++;
           if(stp > 5 * MAX_STEPS_BETWEEN_SLOTS ){ error = true; break;}
            
           if(!filterMotor.opto_status) break;
           
           // Set the pulses that identifies the dark-home position from the dark slots (smaller))
           // The Home dark should be almost 1mm larger than the dark slots
           filterMotor.current_pulses = umToSteps(dark_slot_dim + 1000);
           filterMotor.command_sequence++;
           break;
       
       case 2: // Count extra pulses to validate
           stp++;
           if(stp > 5 * MAX_STEPS_BETWEEN_SLOTS ){ error = true; break;}
                      
           filterMotor.current_pulses--;
           if(filterMotor.current_pulses != 0) break;
           
           // If the opto is in light it means that the home slot is not yet reached
           if(!filterMotor.opto_status){ 
               filterMotor.command_sequence = 1;
               break;
           }
           
           // The Home position has been correctly reached: invert the motor
           startMotor(MOTOR_DIR_OUT,MOTOR_SPEED_OUT);
           filterMotor.command_sequence++;   
           stp = 0;
           return;           
           
       case 3: // Wait for the opto = FREE
           stp++;
           if(stp > MAX_STEPS_BETWEEN_SLOTS ){ error = true; break;}
           if(uc_OPTO_Get()) break;

           stp = 0;
           filterMotor.current_pulses = filterMotor.target_slot_position[0] + 1; // Slot 0 pulses
           filterMotor.command_sequence++;
           break;
           
        case 4: // Count the slot 0 position pulses
            stp++;                        
            filterMotor.current_pulses--;
            if(filterMotor.current_pulses != 0) break;
            
            // If the requested slot is 0 ends..
            if(filterMotor.target_slot == 0 ) {
                completed = true;
                break;    
            }            
            filterMotor.command_sequence++;
            break;
                   
        case 5: // Wait for the opto = ENGAGED
            stp++; 
            if(stp > MAX_STEPS_BETWEEN_SLOTS ){ error = true; break;}
            
            if(!filterMotor.opto_status) break;
            filterMotor.measured_light_slot[0] = stp;
            stp=0;
            
            // Wait almost half of the dark slot dimension before to check the opto light transition 
            filterMotor.current_pulses = umToSteps(dark_slot_dim/2);
            filterMotor.command_sequence++;
            break;
            
       case 6: // Wait for the opto = FREE
            stp++;                                    
            if(filterMotor.current_pulses){ 
                filterMotor.current_pulses--;
                break;
            }            
            if(filterMotor.opto_status) break;
            filterMotor.measured_dark_slot[0] = stp;
            
            stp = 0;
            filterMotor.current_pulses = filterMotor.target_slot_position[1] + 1; // Slot 1 pulses
            filterMotor.command_sequence++;
            break;
           
        case 7: // Count the slot 0 position pulses
            stp++;                        
            filterMotor.current_pulses--;
            if(filterMotor.current_pulses != 0) break;
            
            // If the requested slot is 0 ends..
            if(filterMotor.target_slot == 1 ) {
                completed = true;
                break;    
            }            
            filterMotor.command_sequence++;
            break;
                   
        case 8: // Wait for the opto = ENGAGED
            stp++; 
            if(stp > MAX_STEPS_BETWEEN_SLOTS ){ error = true; break;}
            
            if(!filterMotor.opto_status) break;
            filterMotor.measured_light_slot[1] = stp;
            stp=0;
            
            // Wait almost half of the dark slot dimension before to check the opto light transition 
            filterMotor.current_pulses = umToSteps(dark_slot_dim/2);
            filterMotor.command_sequence++;
            break;
            
       case 9: // Wait for the opto = FREE
            stp++;                                    
            if(filterMotor.current_pulses){ 
                filterMotor.current_pulses--;
                break;
            }            
            if(filterMotor.opto_status) break;
            filterMotor.measured_dark_slot[1] = stp;
            
            stp = 0;
            filterMotor.current_pulses = filterMotor.target_slot_position[2] + 1; // Slot 2 pulses
            filterMotor.command_sequence++;
            break;
           
        case 10: // Count the slot 0 position pulses
            stp++;                        
            filterMotor.current_pulses--;
            if(filterMotor.current_pulses != 0) break;
            
            // If the requested slot is 0 ends..
            if(filterMotor.target_slot == 2 ) {
                completed = true;
                break;    
            }            
            filterMotor.command_sequence++;
            break;
                   
        case 11: // Wait for the opto = ENGAGED
            stp++; 
            if(stp > MAX_STEPS_BETWEEN_SLOTS ){ error = true; break;}
            
            if(!filterMotor.opto_status) break;
            filterMotor.measured_light_slot[2] = stp;
            stp=0;
            
            // Wait almost half of the dark slot dimension before to check the opto light transition 
            filterMotor.current_pulses = umToSteps(dark_slot_dim/2);
            filterMotor.command_sequence++;
            break;
            
       case 12: // Wait for the opto = FREE
            stp++;                                    
            if(filterMotor.current_pulses){ 
                filterMotor.current_pulses--;
                break;
            }            
            if(filterMotor.opto_status) break;
            filterMotor.measured_dark_slot[2] = stp;
            
            stp = 0;
            filterMotor.current_pulses = filterMotor.target_slot_position[3] + 1; // Slot 3 pulses
            filterMotor.command_sequence++;
            break;
           
        case 13: // Count the slot 0 position pulses
            stp++;                        
            filterMotor.current_pulses--;
            if(filterMotor.current_pulses != 0) break;
            
            // If the requested slot is 0 ends..
            if(filterMotor.target_slot == 3 ) {
                completed = true;
                break;    
            }            
            filterMotor.command_sequence++;
            break;
                   
        case 14: // Wait for the opto = ENGAGED
            stp++; 
            if(stp > MAX_STEPS_BETWEEN_SLOTS ){ error = true; break;}
            
            if(!filterMotor.opto_status) break;
            filterMotor.measured_light_slot[3] = stp;
            stp=0;
            
            // Wait almost half of the dark slot dimension before to check the opto light transition 
            filterMotor.current_pulses = umToSteps(dark_slot_dim/2);
            filterMotor.command_sequence++;
            break;
            
       case 15: // Wait for the opto = FREE
            stp++;                                    
            if(filterMotor.current_pulses){ 
                filterMotor.current_pulses--;
                break;
            }            
            if(filterMotor.opto_status) break;
            filterMotor.measured_dark_slot[3] = stp;
            
            stp = 0;
            filterMotor.current_pulses = filterMotor.target_slot_position[4] + 1; // Slot 4 pulses
            filterMotor.command_sequence++;
            break;
           
        case 16: // Count the slot 0 position pulses
            stp++;                        
            filterMotor.current_pulses--;
            if(filterMotor.current_pulses != 0) break;
            
            // If the requested slot is 0 ends..
            completed = true;           
            break;                         
   }
   
   // If an error condition was detected, the activation ends in 
   if(error){
        error = false;
        stopMotor(_STOP_BECAUSE_ERROR, _CURLIM_LOW);
        TC1_CompareStop();
        filterMotor.command_activated = false;            
        filterMotor.slot_valid = false;
        
        unsigned char error_pers0;
        MET_Can_Protocol_GetErrors(0, 0, &error_pers0, 0);
        error_pers0 |= PERS0_FILTER_SEL_FAIL;       
        MET_Can_Protocol_SetErrors(0, 0, &error_pers0, 0);
        
        SETBYTE_SLOT_SELECTED(SYSTEM_OUT_POSITION); // Sets the OUT OF POSITION on the Status register
        setFilterError(true);        
        return;
   }
   
   // If the complete activation has been detected, the activation ends
   if(completed){
       stopMotor(_STOP_BECAUSE_TARGET, _CURLIM_LOW);
       TC1_CompareStop();
       filterMotor.command_activated = false;            
       filterMotor.slot_valid = true;       
       
       // Sets the current selection on the STATUS register
        if(filterMotor.target_filter == POSITIONER_SELECT_FILTER1) SETBYTE_SLOT_SELECTED(SYSTEM_FILTER1_SELECTED);
        else if(filterMotor.target_filter == POSITIONER_SELECT_FILTER2) SETBYTE_SLOT_SELECTED(SYSTEM_FILTER2_SELECTED);
        else if(filterMotor.target_filter == POSITIONER_SELECT_FILTER3) SETBYTE_SLOT_SELECTED(SYSTEM_FILTER3_SELECTED);
        else if(filterMotor.target_filter == POSITIONER_SELECT_FILTER4) SETBYTE_SLOT_SELECTED(SYSTEM_FILTER4_SELECTED);
        else if(filterMotor.target_filter == POSITIONER_SELECT_MIRROR) SETBYTE_SLOT_SELECTED(SYSTEM_MIRROR_SELECTED);
        else{
            setFilterError(true);
            SETBYTE_SLOT_SELECTED(SYSTEM_OUT_POSITION);
            return;
        } 
        setFilterError(false);
        
       return;
   }
   
   
   
    // Acceleration ramp
    uint16_t current_period = TC1_Compare16bitPeriodGet();
    if(current_period > filterMotor.final_period){
        TC1_CompareStop();
        TC1_Compare16bitPeriodSet(current_period - filterMotor.ramp_rate);
        TC1_CompareStart();
    }
    
}

