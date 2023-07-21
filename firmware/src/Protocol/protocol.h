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
        static const unsigned char   MET_CAN_APP_DEVICE_ID    =  0x12 ;     //!< Application DEVICE CAN Id address
        static const unsigned char   MET_CAN_STATUS_REGISTERS =  0 ;        //!< Defines the total number of implemented STATUS registers 
        static const unsigned char   MET_CAN_DATA_REGISTERS   =  0 ;        //!< Defines the total number of implemented Application DATA registers 
        static const unsigned char   MET_CAN_PARAM_REGISTERS  =  0 ;        //!< Defines the total number of implemented PARAMETER registers 

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

     /// @}   moduleApiInterface
    

    /** \defgroup StatusRegisterGroup STATUS REGISTERS Definition
     *  
     *  This section describes the implementation of the Application STATUS Registers 
     *  @{
     */
        
     /// This is the list of the implemented STATUS REGISTERS    
     typedef enum{
        RESERVED_STATUS_REGISTER = 0, //!< This is the parameter setting the delay between power On Off events
     }PROTO_STATUS_t;
     
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
            RESERVED_PARAM_REGISTER = 0, //!< This is the parameter setting the delay between power On Off events
        }PROTO_PARAMETERS_t;

    /// @}   ParamRegisterGroup
        
        
     /** \defgroup CommandGroup COMMAND Execution Definition
     *  
     *  This section describes the Application Command Execution codes
     *  @{
     */
        
    /// This is the list of the implemented COMMANDS
    typedef enum{
      RESERVED_COMMAND = 0,    
    }PROTO_COMMAND_ENUM_t;
    
    /// This is the list of the implemented ERRORS
    typedef enum{
        PROTO_COMMAND_FIRST_ERROR = MET_CAN_COMMAND_APPLICATION_ERRORS,            
    }PROTO_COMMAND_ERROR_ENUM_t;

     /// @}   CommandGroup

        


         
/** @}*/ // protocolModule
#endif 