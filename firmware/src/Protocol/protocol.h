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
 * This Module implements the functions of the PCB/22-303 Software Communication protocol specifications.
 * 
 * ## Dependencies
 * 
 * This module requires the following library modules:
 * - Shared/CAN/MET_Can_Protocol.c
 * - Shared/CAN/MET_Can_Protocol.h
 * 
 * 
 * ## Protocol Communication setting
 *  
 * The Application implements the communication protocol  
 * described in the PCB/22-303 Software Communication protocol specifications.
 * 
 *  @{
 * 
 */

    /**
     * \defgroup moduleConstants Constants module definition
     * 
     * This section describes the module constants
     * 
     *  @{
     */
        // Can Module Definitions
        static const unsigned char   MET_CAN_APP_DEVICE_ID    =  0x13 ;     //!< Application DEVICE CAN Id address
        static const unsigned char   MET_CAN_STATUS_REGISTERS =  1 ;        //!< Defines the total number of implemented STATUS registers 
        static const unsigned char   MET_CAN_DATA_REGISTERS   =  0 ;        //!< Defines the total number of implemented Application DATA registers 
        static const unsigned char   MET_CAN_PARAM_REGISTERS  =  6 ;        //!< Defines the total number of implemented PARAMETER registers 

     /// @}   moduleConstants

    /**
     * \defgroup moduleApiInterface CAN Protocol API interface
     * 
     * This section describes the functions implementing the Protocol module.
     * 
     *  @{
     */
        /// This is the Protocol initialization function
        ext void ApplicationProtocolInit ( void);

        /// This is the Main Loop protocol function
        ext void  ApplicationProtocolLoop(void);

        ext void setStatorErrorHigh(bool stat);
        ext void setStatorErrorShort(bool stat);
        ext void setStatorErrorLow(bool stat);
        ext void setBulbErrorHigh(bool stat);
        ext void setBulbErrorShort(bool stat);
        ext void setBulbErrorLow(bool stat);
        ext void setFilterError(bool stat);

        /// @}   moduleApiInterface
    
    /** \defgroup ErrorRegisterGroup ERROR REGISTER Definition
     *  
     *  This section describes the implementation of the Protocol Error Register
     *  @{
     */

        #define PERS0_BULB_SENS_LOW     0x1
        #define PERS0_BULB_SENS_HIGH    0x2
        #define PERS0_BULB_SENS_SHORT   0x4
        #define PERS0_STATOR_SENS_LOW   0x8
        #define PERS0_STATOR_SENS_HIGH  0x10
        #define PERS0_STATOR_SENS_SHORT 0x20
        #define PERS0_FILTER_SEL_FAIL   0x40

        


        
    /// @}   ErrorRegisterGroup
        
    /** \defgroup StatusRegisterGroup STATUS REGISTERS Definition
     *  
     *  This section describes the implementation of the Application STATUS Registers 
     *  @{
     */
        
     /// This is the list of the implemented STATUS REGISTERS    
     typedef enum{
        SYSTEM_STATUS_REGISTER = 0, //!< This is the Internal Status  
              
     }PROTO_STATUS_t;
    #define SYSTEM_FILTER_STATUS_BYTE 0
    #define SYSTEM_STATOR_TEMP_BYTE 1
    #define SYSTEM_BULB_TEMP_BYTE 2
    #define SYSTEM_FLAGS_BYTE 3

    typedef enum{
        SYSTEM_OUT_POSITION = 0, //!< The Filter is currently out of a valid selected position
        SYSTEM_FILTER1_SELECTED, //!< The Filter1 is successfully selected
        SYSTEM_FILTER2_SELECTED, //!< The Filter2 is successfully selected
        SYSTEM_FILTER3_SELECTED, //!< The Filter3 is successfully selected
        SYSTEM_FILTER4_SELECTED, //!< The Filter4 is successfully selected
        SYSTEM_MIRROR_SELECTED, //!< The Mirror is successfully selected
        SYSTEM_SELECTION_PENDING, //!< The Module is executing a Slot selection

    }PROTO_FILTER_SELECTION_STATUS_t;
    
    #define SETBYTE_SLOT_SELECTED(val)  MET_Can_Protocol_SetStatusReg(SYSTEM_STATUS_REGISTER, SYSTEM_FILTER_STATUS_BYTE, val) //!< This is Status of the current slot selection  
    #define GETBYTE_SLOT_SELECTED  MET_Can_Protocol_GetStatus(SYSTEM_STATUS_REGISTER, SYSTEM_FILTER_STATUS_BYTE) //!< This is Status of the current slot selection    

    #define SETBYTE_STATOR_PERCENT(val)  MET_Can_Protocol_SetStatusReg(SYSTEM_STATUS_REGISTER, SYSTEM_STATOR_TEMP_BYTE, val) //!< This is the Stator Percentage
    #define GETBYTE_STATOR_PERCENT  MET_Can_Protocol_GetStatus(SYSTEM_STATUS_REGISTER, SYSTEM_STATOR_TEMP_BYTE) //!< This is the Stator Percentage

    #define SETBYTE_BULB_PERCENT(val)  MET_Can_Protocol_SetStatusReg(SYSTEM_STATUS_REGISTER, SYSTEM_BULB_TEMP_BYTE, val) //!< This is the Bulb Percentage    
    #define GETBYTE_BULB_PERCENT  MET_Can_Protocol_GetStatus(SYSTEM_STATUS_REGISTER, SYSTEM_BULB_TEMP_BYTE) //!< This is the Bulb Percentage    

    #define SETBIT_FLAGS_LIGHT_ON(val)  MET_Can_Protocol_SetStatusBit(SYSTEM_STATUS_REGISTER, SYSTEM_FLAGS_BYTE, 0x1, val) //!< This bit is the status of the power light
    #define GETBIT_FLAGS_LIGHT_ON(val)  MET_Can_Protocol_TestStatus(SYSTEM_STATUS_REGISTER, SYSTEM_FLAGS_BYTE, 0x1) //!< This bit is the status of the power light
    
    #define SETBIT_FLAGS_ERRORS(val)  MET_Can_Protocol_SetStatusBit(SYSTEM_STATUS_REGISTER, SYSTEM_FLAGS_BYTE, 0x80, val) //!< This bit is the Filter in error condition
    #define GETBIT_FLAGS_ERRORS(val)  MET_Can_Protocol_TestStatus(SYSTEM_STATUS_REGISTER, SYSTEM_FLAGS_BYTE, 0x80) //!< This bit is the Filter in error condition
     
     
    
     /// @}   StatusRegisterGroup

     
    /** \defgroup DataRegisterGroup DATA REGISTERS Definition
     *  
     *  This section describes the implementation of the Protocol DATA Registers 
     *  @{
     */

         /// This is the list of the implemented DATA REGISTERS    
         typedef enum{
            RESERVED_DATA_REGISTER = 0, //!< This is the parameter setting the delay between power On Off events
         }PROTO_DATA_t;
        
    /// @}   DataRegisterGroup

    /** \defgroup ParamRegisterGroup PARAMETER Registers Definition
     *  
     *  This section describes the implementation of the Application PARAMETER Registers 
     *  @{
     */
        
        /// This is the list of the implemented PARAMETER REGISTERS        
        typedef enum{
            // RESERVED_PARAM_REGISTER = 0, //!< This is the parameter setting the delay between power On Off events
            PROTO_PARAM_FILTER1_POSITION = 0, //!< This is the parameter setting the delay between power On Off events
            PROTO_PARAM_FILTER2_POSITION,
            PROTO_PARAM_FILTER3_POSITION,
            PROTO_PARAM_FILTER4_POSITION,
            PROTO_PARAM_MIRROR_POSITION,
            PROTO_PARAM_LIGHT_TIMEOUT,
                    
        }PROTO_PARAMETERS_t;
        
        
        
        #define GETWORD_PARAMETER_FILTER1_POSITION (MET_Can_Protocol_GetParameter(PROTO_PARAM_FILTER1_POSITION,0) + 256 * MET_Can_Protocol_GetParameter(PROTO_PARAM_FILTER1_POSITION,1))
        #define GETWORD_PARAMETER_FILTER2_POSITION (MET_Can_Protocol_GetParameter(PROTO_PARAM_FILTER2_POSITION,0) + 256 * MET_Can_Protocol_GetParameter(PROTO_PARAM_FILTER2_POSITION,1))
        #define GETWORD_PARAMETER_FILTER3_POSITION (MET_Can_Protocol_GetParameter(PROTO_PARAM_FILTER3_POSITION,0) + 256 * MET_Can_Protocol_GetParameter(PROTO_PARAM_FILTER3_POSITION,1))
        #define GETWORD_PARAMETER_FILTER4_POSITION (MET_Can_Protocol_GetParameter(PROTO_PARAM_FILTER4_POSITION,0) + 256 * MET_Can_Protocol_GetParameter(PROTO_PARAM_FILTER4_POSITION,1))
        #define GETWORD_PARAMETER_MIRROR_POSITION (MET_Can_Protocol_GetParameter(PROTO_PARAM_MIRROR_POSITION,0) + 256 * MET_Can_Protocol_GetParameter(PROTO_PARAM_MIRROR_POSITION,1))
        #define GETBYTE_PARAMETER_LIGHT_TIMEOUT (MET_Can_Protocol_GetParameter(PROTO_PARAM_LIGHT_TIMEOUT,0))


    /// @}   ParamRegisterGroup
        
        
     /** \defgroup CommandGroup COMMAND Execution Definition
     *  
     *  This section describes the Application Command Execution codes
     *  @{
     */
        
    /// This is the list of the implemented COMMANDS
    typedef enum{
      RESERVED_COMMAND = 0,    
      SET_POSITIONER,
      SET_RAW_POSITIONER,
      SET_LIGHT,
    }PROTO_COMMAND_ENUM_t;
    
    #define POSITIONER_SELECT_FILTER1 1
    #define POSITIONER_SELECT_FILTER2 2
    #define POSITIONER_SELECT_FILTER3 3
    #define POSITIONER_SELECT_FILTER4 4
    #define POSITIONER_SELECT_MIRROR  5

    #define SET_LIGHT_ON  1
    #define SET_LIGHT_OFF 0


    
    /// This is the list of the implemented ERRORS
    typedef enum{
        COMMAND_ERROR_FILTER_SELECTION_FAILED = MET_CAN_COMMAND_APPLICATION_ERRORS,      
                
    }PROTO_COMMAND_ERROR_ENUM_t;

     /// @}   CommandGroup

        


         
/** @}*/ // protocolModule
#endif 