#ifndef _POWER_LED_H    
#define _POWER_LED_H

#include "definitions.h"  
#include "application.h"  

#undef ext
#undef ext_static

#ifdef _POWER_LED_C
    #define ext
    #define ext_static static 
#else
    #define ext extern
    #define ext_static extern
#endif

/*!
 * \defgroup powerLedModule Power Led management module
 *
 * \ingroup applicationModule
 * 
 * 
 * This Module implements the functions of the Power Led specifications in the 
 * FW315 Software Detailed Specifications document
 * 
 * ## Dependencies
 * 
 * 
 * ## Harmony 3 configurator setting
 * 
 * ### Pin Assignement
 * 
 * |Signal name|Pin|Direction|Driver|Description|
 * |:---|:---:|:---:|:---:|:---|
 * |uc_CMP_ENA|38|Out|Strong|This is the Compression enable signal to Compressor device|
 * ||
 * 
 * ## Module Function Description
 *  
 * 
 *  @{
 * 
 */

     /**
    * \defgroup powerLedApiModule API Module
    *  @{
    */
        
        ext void PowerLedInit(void);
    
        
        ext void PowerLedLoop(void);
        
        ext void PowerLedOff(void);
        ext void PowerLedOn(void);
        
        
        
    /** @}*/ // powerLedApiModule
        


         
/** @}*/ // powerLedModule
        
        
#endif 