#define _PROTOCOL_C

#include "application.h"
#include "protocol.h"
#include "../Filter/filter.h"
#include "../PowerLed/power_led.h"


static void ApplicationProtocolCommandHandler(uint8_t cmd, uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3 ); //!< This is the Command protocol callback
static volatile unsigned char current_command = 0;



/**
 * This function initializes the CAN Protocol module.
 * 
 * The function calls the library API  MET_Can_Protocol_Init() in order to:
 * + Set the Device Identifier;
 * + The number of implemented STATUS registers;
 * + The number of implemented DATA registers;
 * + The number of implemented PARAMETER registers;
 * + The Application revision 
 * + The protocol command handler;
 * 
 * The function initializes the Parameters with the default value   
 * with the library MET_Can_Protocol_SetDefaultParameter() function.
 * 
 */
void ApplicationProtocolInit ( void )
{
     
    // Initialize the Met Can Library
    MET_Can_Protocol_Init(MET_CAN_APP_DEVICE_ID, MET_CAN_STATUS_REGISTERS, MET_CAN_DATA_REGISTERS, MET_CAN_PARAM_REGISTERS, APPLICATION_MAJ_REV, APPLICATION_MIN_REV, APPLICATION_SUB_REV, ApplicationProtocolCommandHandler);
    
    // Assignes the default parameter here ..
    MET_Can_Protocol_SetDefaultParameter(PROTO_PARAM_FILTER1_POSITION,0,0,0,0);
    MET_Can_Protocol_SetDefaultParameter(PROTO_PARAM_FILTER2_POSITION,0,0,0,0);
    MET_Can_Protocol_SetDefaultParameter(PROTO_PARAM_FILTER3_POSITION,0,0,0,0);
    MET_Can_Protocol_SetDefaultParameter(PROTO_PARAM_FILTER4_POSITION,0,0,0,0);
    MET_Can_Protocol_SetDefaultParameter(PROTO_PARAM_MIRROR_POSITION,0,0,0,0);
    MET_Can_Protocol_SetDefaultParameter(PROTO_PARAM_LIGHT_TIMEOUT,5,0,0,0);
    
}
  
/**
 * This function shall be called by the MAIN loop application 
 * in order to manage the reception/transmission protocol activities.
 * 
 * The function merely call the library function  MET_Can_Protocol_Loop().
 */
void inline ApplicationProtocolLoop(void){

    MET_Can_Protocol_Loop();

    if(!current_command) return;
    
    // Command completion
    if(current_command == SET_POSITIONER) {
        if(FilterIsRunning()) return;
        
        if(FilterIsError())   MET_Can_Protocol_returnCommandError(COMMAND_ERROR_FILTER_SELECTION_FAILED); 
        else MET_Can_Protocol_returnCommandExecuted(0,0);
        
        current_command = 0;
        return;
    }
}

/**
 * This is the Command Handler implementation
 *  
 */
void ApplicationProtocolCommandHandler(uint8_t cmd, uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3 ){

    switch(cmd){
        case MET_COMMAND_ABORT:  // This is the Library mandatory 
            
            break;
        
        // This is the command implementing the Slot selection
        case SET_POSITIONER:
            
            // Anyway if the requested command is not a MIRROR the power light is switched Off
            if( d0 != POSITIONER_SELECT_MIRROR ) PowerLedOff();
            
            if(FilterIsRunning()) MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_BUSY);
            else if(FilterIsTarget(d0)){ 
                MET_Can_Protocol_returnCommandExecuted(d0,0); // The Slot requested is already selected
                if( d0 == POSITIONER_SELECT_MIRROR ) PowerLedOn();
            } else {
                // The Filter activation routine is in charge to send the MET_Can_Protocol_returnCommandExecuted() command                               
                if(FilterSelect(d0)){   
                    MET_Can_Protocol_returnCommandExecuting();
                    current_command = cmd; 
                    if( d0 == POSITIONER_SELECT_MIRROR ) PowerLedOn();
                }else MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_INVALID_DATA);
            }
            break;
            
        // This is the command implements the power light manual control    
        case SET_LIGHT:
            if( d0 == SET_LIGHT_ON ) PowerLedOn();
            else PowerLedOff();
            MET_Can_Protocol_returnCommandExecuted(0,0); // The Command is immediatelly executed
            break;
            
        default:
            MET_Can_Protocol_returnCommandError(MET_CAN_COMMAND_NOT_AVAILABLE);
    }
    
    return;
}

void setStatorErrorLow(bool stat){
    unsigned char error_pers0;
    unsigned char error_pers1;
    MET_Can_Protocol_GetErrors(0, 0, &error_pers0, &error_pers1);
    
    error_pers0 &= ~PERS0_STATOR_SENS_LOW;
    error_pers0 &= ~PERS0_STATOR_SENS_SHORT;
    error_pers0 &= ~PERS0_STATOR_SENS_HIGH;
    
    if(stat)  error_pers0 |= PERS0_STATOR_SENS_LOW;   
    MET_Can_Protocol_SetErrors(0, 0, &error_pers0, &error_pers1);
    
    if((error_pers0) ||(error_pers1)) SETBIT_FLAGS_ERRORS(true); // Sets the error bit in the Status Flags 
    else SETBIT_FLAGS_ERRORS(false); // Sets the error bit in the Status Flags 
    
}
    
void setStatorErrorShort(bool stat){
    unsigned char error_pers0;
    unsigned char error_pers1;
    MET_Can_Protocol_GetErrors(0, 0, &error_pers0, &error_pers1);
    
    error_pers0 &= ~PERS0_STATOR_SENS_LOW;
    error_pers0 &= ~PERS0_STATOR_SENS_SHORT;
    error_pers0 &= ~PERS0_STATOR_SENS_HIGH;

    if(stat)  error_pers0 |= PERS0_STATOR_SENS_SHORT;
    MET_Can_Protocol_SetErrors(0, 0, &error_pers0, &error_pers1);
    
    if((error_pers0) ||(error_pers1)) SETBIT_FLAGS_ERRORS(true); // Sets the error bit in the Status Flags 
    else SETBIT_FLAGS_ERRORS(false); // Sets the error bit in the Status Flags 
    
}

void setStatorErrorHigh(bool stat){
    unsigned char error_pers0;
    unsigned char error_pers1;
    MET_Can_Protocol_GetErrors(0, 0, &error_pers0, &error_pers1);

    error_pers0 &= ~PERS0_STATOR_SENS_LOW;
    error_pers0 &= ~PERS0_STATOR_SENS_SHORT;
    error_pers0 &= ~PERS0_STATOR_SENS_HIGH;

    if(stat)  error_pers0 |= PERS0_STATOR_SENS_HIGH;
    MET_Can_Protocol_SetErrors(0, 0, &error_pers0, &error_pers1);
    
    if((error_pers0) ||(error_pers1)) SETBIT_FLAGS_ERRORS(true); // Sets the error bit in the Status Flags 
    else SETBIT_FLAGS_ERRORS(false); // Sets the error bit in the Status Flags 
    
}
    
void setBulbErrorLow(bool stat){
    unsigned char error_pers0;
    unsigned char error_pers1;
    MET_Can_Protocol_GetErrors(0, 0, &error_pers0, &error_pers1);

    error_pers0 &= ~PERS0_BULB_SENS_LOW;
    error_pers0 &= ~PERS0_BULB_SENS_SHORT;
    error_pers0 &= ~PERS0_BULB_SENS_HIGH;

    if(stat)  error_pers0 |= PERS0_BULB_SENS_LOW;
    MET_Can_Protocol_SetErrors(0, 0, &error_pers0, &error_pers1);
    
    if((error_pers0) ||(error_pers1)) SETBIT_FLAGS_ERRORS(true); // Sets the error bit in the Status Flags 
    else SETBIT_FLAGS_ERRORS(false); // Sets the error bit in the Status Flags 
    
}

void setBulbErrorShort(bool stat){
    unsigned char error_pers0;
    unsigned char error_pers1;
    MET_Can_Protocol_GetErrors(0, 0, &error_pers0, &error_pers1);

    error_pers0 &= ~PERS0_BULB_SENS_LOW;
    error_pers0 &= ~PERS0_BULB_SENS_SHORT;
    error_pers0 &= ~PERS0_BULB_SENS_HIGH;

    if(stat)  error_pers0 |= PERS0_BULB_SENS_SHORT;
    MET_Can_Protocol_SetErrors(0, 0, &error_pers0, &error_pers1);
    
    if((error_pers0) ||(error_pers1)) SETBIT_FLAGS_ERRORS(true); // Sets the error bit in the Status Flags 
    else SETBIT_FLAGS_ERRORS(false); // Sets the error bit in the Status Flags 
    
}

void setBulbErrorHigh(bool stat){
    unsigned char error_pers0;
    unsigned char error_pers1;
    MET_Can_Protocol_GetErrors(0, 0, &error_pers0, &error_pers1);
    error_pers0 &= ~PERS0_BULB_SENS_LOW;
    error_pers0 &= ~PERS0_BULB_SENS_SHORT;
    error_pers0 &= ~PERS0_BULB_SENS_HIGH;

    if(stat)  error_pers0 |= PERS0_BULB_SENS_HIGH;
    MET_Can_Protocol_SetErrors(0, 0, &error_pers0, &error_pers1);
    
    if((error_pers0) ||(error_pers1)) SETBIT_FLAGS_ERRORS(true); // Sets the error bit in the Status Flags 
    else SETBIT_FLAGS_ERRORS(false); // Sets the error bit in the Status Flags 
    
}

void setFilterError(bool stat){
    unsigned char error_pers0;
    unsigned char error_pers1;
    MET_Can_Protocol_GetErrors(0, 0, &error_pers0, &error_pers1);
    
    if(stat)  error_pers0 |= PERS0_FILTER_SEL_FAIL;
    else error_pers0 &= ~PERS0_FILTER_SEL_FAIL;
    MET_Can_Protocol_SetErrors(0, 0, &error_pers0, &error_pers1);
    
    if((error_pers0) ||(error_pers1)) SETBIT_FLAGS_ERRORS(true); // Sets the error bit in the Status Flags 
    else SETBIT_FLAGS_ERRORS(false); // Sets the error bit in the Status Flags 
    
}