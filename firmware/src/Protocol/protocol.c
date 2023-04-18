#define _PROTOCOL_C

#include "application.h"
#include "protocol.h"

static void ApplicationProtocolCommandHandler(uint8_t cmd, uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3 );

/**
 * This function initiate the CAN Protocol communication.
 * 
 * + Library protocol initilization procedure;
 * + Command Handler assignment;
 * + Parameter default initialization
 * 
 * 
 */
void ApplicationProtocolInit ( void )
{
    // Initialize the Met Can Library
    MET_Can_Protocol_Init(ApplicationProtocolCommandHandler);
    
    // Assignes the default parameter values
    MET_Can_Protocol_SetDefaultParameter(POWER_ON_OFF_DELAY,40,0,0,0);
    MET_Can_Protocol_SetDefaultParameter(HARDWARE_POWER_OFF_TIME,50,0,0,0);
    MET_Can_Protocol_SetDefaultParameter(KEEP_ALIVE_POWER_OFF,200,0,0,0);
    MET_Can_Protocol_SetDefaultParameter(SOFT_POWER_OFF_DELAY,100,0,0,0);
    MET_Can_Protocol_SetDefaultParameter(LOW_BATT_LEVELS,100,100,0,0);
    
    MET_Can_Protocol_SetDefaultParameter(BODY_ROTATION_LOCK_TIME,0,0,0,0);
    MET_Can_Protocol_SetDefaultParameter(PEDALBOARD_LOCK_TIME,0,0,0,0);
    MET_Can_Protocol_SetDefaultParameter(MANUAL_ROT_LOCK_TIME,0,0,0,0);
    MET_Can_Protocol_SetDefaultParameter(XRAY_BUTTON_LOCK_TIME,0,0,0,0);
   
    MET_Can_Protocol_SetDefaultParameter(ARM_ZERO_POSITION,0x00,0x02,0,0);
    MET_Can_Protocol_SetDefaultParameter(LIFT_ZERO_POSITION,0x00,0x0,0,0);
    MET_Can_Protocol_SetDefaultParameter(BODY_ZERO_POSITION,0,0,0,0);
}
  
/**
 * This function shall be called by the MAIN loop application 
 * in order to manage the reception/transmission protocol activities.
 * 
 * The function merely call the library function  MET_Can_Protocol_Loop().
 */
void inline ApplicationProtocolLoop(void){
    MET_Can_Protocol_Loop();
}

/**
 * This function is called every 7.82ms from the main loop
 */
void Protocol_7280_us_callback(void){
    
}

/**
 * This is the Application Command Execution Handler
 *  
 */
void ApplicationProtocolCommandHandler(uint8_t cmd, uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3 ){

    switch(cmd){
        case MET_COMMAND_ABORT:    
            
            break;
        

        default:
            MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_NOT_AVAILABLE);
    }
    
    return;
}