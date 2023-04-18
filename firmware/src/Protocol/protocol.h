#ifndef _PROTOCOL_H    
#define _PROTOCOL_H

#include "definitions.h"  
#include "application.h"  
#include "Shared/CAN/MET_can_protocol.h"

#undef ext
#undef ext_static

#ifdef _PROTOCOL_C
    #define ext
    #define ext_static static 
#else
    #define ext extern
    #define ext_static extern
#endif

/*!
 * \defgroup protocolModule CAN Communication Protocol Module
 *
 * \ingroup applicationModule
 * 
 * 
 * This Module implements the functions 
 * of the PCB/22-301 Software Communication protocol specifications.
 * 
 * ## Dependencies
 * 
 * This module requires the following library modules:
 * - Shared/CAN/MET_Can_Protocol.c
 * - Shared/CAN/MET_Can_Protocol.h
 * 
 * The application shall define the following labels into 
 * the application.h header file in the project root directory, where the main.c file
 * is located:
 * 
 * + #define MET_CAN_APP_MAJ_REV : Revision Major Number
 * + #define MET_CAN_APP_MIN_REV : Revision Minor Number
 * + #define MET_CAN_APP_SUB_REV : Revision build Number
 * + #define MET_CAN_APP_DEVICE_ID : Application DEVICE CAN Id address
 * + #define MET_CAN_STATUS_REGISTERS : Defines the total number of implemented STATUS registers 
 * + #define MET_CAN_DATA_REGISTERS : Defines the total number of implemented Application DATA registers 
 * + #define MET_CAN_PARAM_REGISTERS :  Defines the total number of implemented PARAMETER registers 
 * 
 * ## Communication setting
 * 
 * The CAN communication is based on the following  characteristics:
 * 
 * - Standard (not Flexible) communication;
 * - Standard frame (11 bit ID);
 * - Baude Rate: 1Mb/s;
 * - Can ID reception address: 0x211;
 * 
 * ## Protocol description
 * 
 * The Application iplements the protocol Items 
 * described in the PCB/22-301 Software Communication protocol specifications.
 * 
 * In this chapter it will be briefly described the protocol Items.
 * For details refer to the document specification.
 * 
 * ### Application STATUS Registers
 * 
 * The requested status registers are :
 * 
 * |REGISTER NAME|IDX|DESCRIPTION|
 * |:--|:--|:--|
 * |SYSTEM|0|Internal status bit (Input lines and other status)|
 * |BATTERY|1|Baackup battery monitored level|
 * ||
 * 
 * ### DATA Registers
 * 
 * The available DATA registers are:
 *
 * |REGISTER NAME|IDX|DESCRIPTION|
 * |:--|:--|:--|
 * |OUTPUTS|0|Output lines setting and other internal flags|
 * ||
 * 
 * ### PARAMETER Registers
 * 
 * ### COMMAND Register
 * 
 *  @{
 * 
 */

    /**
     * \defgroup moduleInterface CAN Protocol API interface
     * 
     * This section describes the funtions implementing the Protocol module.
     * 
     *  @{
     */
        /// This is the Protocol initialization function
        ext void ApplicationProtocolInit ( void);

        /// This is the Main Loop protocol function
        ext void  ApplicationProtocolLoop(void);

    /** @}*/  // moduleInterface
    

    /** \defgroup STATUS_Register STATUS Registers Definition
     *  
     *  This section describes the implementation of the Application STATUS Registers 
     *  @{
     */
        
        #define PROTOCOL_STATUS_SYSTEM_IDX 0          //!< STATUS-SYSTEM Register Index definition
        #define PROTOCOL_STATUS_BATTERY_IDX 1         //!< STATUS-BATTERY Register Index definition
        #define PROTOCOL_STATUS_ARM_POSITION_IDX 2    //!< STATUS-ARM-POS Register Index definition

        
        #define PROTOCOL_SYSTEM_ERROR(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 0, 0x1, val) //!< This is the error bit in the system register
        #define PROTOCOL_SYSTEM_EMERGENCY(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 0, 0x2, val) //!< This is the emergency_button bit in the system register
        #define PROTOCOL_SYSTEM_POWERDOWN(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 0, 0x4, val) //!< This is the powerdown_detected bit in the system register
        #define PROTOCOL_SYSTEM_MOTOR_SAFETY(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 0, 0x8, val) //!< This is the motor_safety bit in the system register
        #define PROTOCOL_SYSTEM_SOFT_POWEROFF(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 0, 0x10, val) //!< This is the  soft_poweroff bit in the system register
        #define PROTOCOL_SYSTEM_BATT1_LOW(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 0, 0x20, val) //!< This is the  batt1_low bit in the system register
        #define PROTOCOL_SYSTEM_BATT2_LOW(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 0, 0x40, val) //!< This is the  batt2_low bit in the system register
        #define PROTOCOL_SYSTEM_BATTENA(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 0, 0x80, val) //!< This is the  battery_enable_button bit in the system register
        
        #define PROTOCOL_SYSTEM_CARM_UP(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 1, 0x1, val) //!< This is the c-arm up request flag
        #define PROTOCOL_SYSTEM_CARM_DWN(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 1, 0x2, val) //!< This is the c-arm down request flag
        #define PROTOCOL_SYSTEM_SLIDE_UP(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 1, 0x4, val) //!< This is the slide up request flag
        #define PROTOCOL_SYSTEM_SLIDE_DWN(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 1, 0x8, val) //!< This is the slide down request flag
        #define PROTOCOL_SYSTEM_ROT_CW(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 1, 0x10, val) //!< This is the c-arm rot cw request flag
        #define PROTOCOL_SYSTEM_ROT_CCW(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 1, 0x20, val) //!< This is the c-arm rot ccw request flag
        #define PROTOCOL_SYSTEM_BODY_CW(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 1, 0x40, val) //!< This is the body rot cw request flag
        #define PROTOCOL_SYSTEM_BODY_CCW(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 1, 0x80, val) //!< This is the body rot ccw request flag
        
        #define PROTOCOL_SYSTEM_MOT_DCOK(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 2, 0x1, val) //!< This is the motor DC-OK power DC flag
        #define PROTOCOL_SYSTEM_MOT_POWER_ON(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 2, 0x2, val) //!< This is the Motor power activatin status
        #define PROTOCOL_SYSTEM_COMPRESSION_ON(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 2, 0x4, val) //!< This is Compression On status
        #define PROTOCOL_SYSTEM_XRAY_BUTTON(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 2, 0x8, val) //!< This is the X-RAY button activation status
        #define PROTOCOL_SYSTEM_CLOSED_DOOR(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 2, 0x10, val) //!< This is the Closed-Door activation status
        #define PROTOCOL_SYSTEM_BURNIN_JUMPER(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 2, 0x20, val) //!< This is the burnin jumper presence
        #define PROTOCOL_SYSTEM_POWER_LOCK(val)  MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 2, 0x40, val) //!< This is the power lock activation feedback
        
        #define PROTOCOL_SYSTEM_PEDAL_UP(val)       MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 3, 0x1, val) //!< This is the Pedal Up input status
        #define PROTOCOL_SYSTEM_PEDAL_DWN(val)       MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 3, 0x2, val) //!< This is the Pedal Down input status
        #define PROTOCOL_SYSTEM_PEDAL_CMP_UP(val)    MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 3, 0x4, val) //!< This is the Pedal Compression Up input status
        #define PROTOCOL_SYSTEM_PEDAL_CMP_DWN(val)   MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 3, 0x8, val) //!< This is the Pedal Compression Down input status
        #define PROTOCOL_SYSTEM_MOTORS_IDLE(val)   MET_Can_Protocol_SetStatusBit(PROTOCOL_STATUS_SYSTEM_IDX, 3, 0x10, val) //!< This is the Motor IDLE condition

        #define PROTOCOL_BATTERY_VBATT1(val)  MET_Can_Protocol_SetStatusReg(PROTOCOL_STATUS_BATTERY_IDX, 0, val )     //!< This is the voltage of the Battery 1
        #define PROTOCOL_BATTERY_VBATT2(val)  MET_Can_Protocol_SetStatusReg(PROTOCOL_STATUS_BATTERY_IDX, 1, val ) //!< This is the voltage of the Battery 2

        #define PROTOCOL_ARMPOS_POTL(val)  MET_Can_Protocol_SetStatusReg(PROTOCOL_STATUS_ARM_POSITION_IDX, 0, val )     //!< This is the Pot value (low byte)
        #define PROTOCOL_ARMPOS_POTH(val)  MET_Can_Protocol_SetStatusReg(PROTOCOL_STATUS_ARM_POSITION_IDX, 1, val )     //!< This is the Pot value (h byte)
        #define PROTOCOL_ARMPOS_ENCODERL(val)  MET_Can_Protocol_SetStatusReg(PROTOCOL_STATUS_ARM_POSITION_IDX, 2, val ) //!< This is the Encoder value (L byte)
        #define PROTOCOL_ARMPOS_ENCODERH(val)  MET_Can_Protocol_SetStatusReg(PROTOCOL_STATUS_ARM_POSITION_IDX, 3, val ) //!< This is the Encoder value (H byte)


    /** @}*/  // STATUS_Register

    /** \defgroup DATA_Register DATA Registers Definition
     *  
     *  This section describes the implementation of the Application DATA Registers 
     *  @{
     */

        
        #define PROTOCOL_DATA_OUTPUTS_IDX 0            //!< DATA-OUTPUT Register Index definition

        #define PROTOCOL_PROGRAMMING_OUT  MET_Can_Protocol_TestData(PROTOCOL_DATA_OUTPUTS_IDX, 0,0x1) //!< D0.0 - This flag activates the power auto sustain during a Loader firmware download
        #define PROTOCOL_MOT_ENA_OUT  MET_Can_Protocol_TestData(PROTOCOL_DATA_OUTPUTS_IDX, 0,0x2) //!< D0.1 - This flag activates ALL the Motor power supply
        #define PROTOCOL_MOT_SWITCH_OUT  MET_Can_Protocol_TestData(PROTOCOL_DATA_OUTPUTS_IDX, 0,0x4) //!< D0.2 - This flag enable the power supply external switch
        #define PROTOCOL_COMPRESSOR_ENA_OUT  MET_Can_Protocol_TestData(PROTOCOL_DATA_OUTPUTS_IDX, 0,0x8) //!< D0.3 - This flag enables the compressor driver
        #define PROTOCOL_CALIB_ENA_OUT  MET_Can_Protocol_TestData(PROTOCOL_DATA_OUTPUTS_IDX, 0,0x10) //!< D0.4 - This flag enables devices to operate calibrations
        #define PROTOCOL_XRAY_ENA_OUT  MET_Can_Protocol_TestData(PROTOCOL_DATA_OUTPUTS_IDX, 0,0x20) //!< D0.5 - This flag activates the XRAY-ENA signal on the generator interface

        #define PROTOCOL_BURNIN_OUT  MET_Can_Protocol_TestData(PROTOCOL_DATA_OUTPUTS_IDX, 1,0x1) //!< D1.0 - This flag activates the burnin output
        #define PROTOCOL_MAN_BUZZER_OUT  MET_Can_Protocol_TestData(PROTOCOL_DATA_OUTPUTS_IDX, 1,0x2) //!< D1.1 - This flag manually activates the buzzer
        #define PROTOCOL_BUZZER_MODE_OUT  MET_Can_Protocol_TestData(PROTOCOL_DATA_OUTPUTS_IDX, 1,0x4) //!< D1.2 - This flag set the buzzer activation mode
        #define PROTOCOL_XRAY_LED_OUT  MET_Can_Protocol_TestData(PROTOCOL_DATA_OUTPUTS_IDX, 1,0x8) //!< D1.3 - This flag activate the XRAY-LED output
        #define PROTOCOL_XRAY_LAMP1_OUT  MET_Can_Protocol_TestData(PROTOCOL_DATA_OUTPUTS_IDX, 1,0x10) //!< D1.4 - This flag activates the external Lamp 1 during Exposure
        #define PROTOCOL_XRAY_LAMP2_OUT  MET_Can_Protocol_TestData(PROTOCOL_DATA_OUTPUTS_IDX, 1,0x20) //!< D1.5 - This flag activates the external Lamp 2 during Exposure

    /** @}*/  // DATA_Register

    /** \defgroup PARAM_Register PARAMETER Registers Definition
     *  
     *  This section describes the implementation of the Application PARAMETER Registers 
     *  @{
     */
        
    /**
     * Parameter index enumeration
     */            
    typedef enum{
        POWER_ON_OFF_DELAY = 0, //!< This is the parameter setting the delay between power On Off events
        HARDWARE_POWER_OFF_TIME, //!< This is the parameter setting the button pression time to activate a Hard Power Off
        KEEP_ALIVE_POWER_OFF,//!< This is the parameter setting the Keep Alive time during a Powerdown condition
        SOFT_POWER_OFF_DELAY,//!< This is the parameter setting the delay waiting the soft power off
        LOW_BATT_LEVELS,//!< This is the parameter setting the battery voltage level for low batt signaling
        BODY_ROTATION_LOCK_TIME,//!< This is the parameter setting the lock/unlock time of the BODY ROTATION activation
        PEDALBOARD_LOCK_TIME,//!< This is the parameter setting the lock/unlock time of the PEDALBOARD activation
        MANUAL_ROT_LOCK_TIME,//!< This is the parameter setting the lock/unlock time of the MANUAL ROTATION activation
        XRAY_BUTTON_LOCK_TIME,//!< This is the parameter setting the lock/unlock time of the XRAY BUTTON
        ARM_ZERO_POSITION,
        LIFT_ZERO_POSITION,        
        BODY_ZERO_POSITION,                
    }PROTO_PARAMETERS_t;
    
        #define PARAMETER_POWER_ON_OFF_DELAY MET_Can_Protocol_GetParameter(POWER_ON_OFF_DELAY,0)
        #define PARAMETER_HARDWARE_POWER_OFF_TIME MET_Can_Protocol_GetParameter(HARDWARE_POWER_OFF_TIME,0)
        #define PARAMETER_KEEP_ALIVE_POWER_OFF MET_Can_Protocol_GetParameter(KEEP_ALIVE_POWER_OFF,0)
        #define PARAMETER_SOFT_POWER_OFF_DELAY MET_Can_Protocol_GetParameter(SOFT_POWER_OFF_DELAY,0)
        #define PARAMETER_LOW_BATT1_LEVEL MET_Can_Protocol_GetParameter(LOW_BATT_LEVELS,0)
        #define PARAMETER_LOW_BATT2_LEVEL MET_Can_Protocol_GetParameter(LOW_BATT_LEVELS,1)

    
        
    /** @}*/  // PARAM_Register
        
     /** \defgroup COMMAND_exec COMMAND Execution Definition
     *  
     *  This section describes the Application Command Execution codes
     *  @{
     */
    typedef enum{
      ACTIVATE_SOFT_POWEROFF = 1,    
      ACTIVATE_MOTORS
    }PROTO_COMMAND_ENUM_t;
    
    typedef enum{
        PROTO_COMMAND_NO_ERR = 0,
        PROTO_ERR_MOTORS_SAFETY_ABORT = 100, // This is Mandatory
        PROTO_ERR_MOTORS_BUTTON_ABORT,        
    }PROTO_COMMAND_ERROR_ENUM_t;

        ext void Protocol_7280_us_callback(void);
        
        
    /** @}*/  // COMMAND_exec

        


         
/** @}*/ // protocolModule
#endif 