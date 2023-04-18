#ifndef _XRAY_TUBE_H    
#define _XRAY_TUBE_H

#include "definitions.h"  
#include "application.h"  

#undef ext
#undef ext_static

#ifdef _XRAY_TUBE_C
    #define ext
    #define ext_static static 
#else
    #define ext extern
    #define ext_static extern
#endif

/*!
 * \defgroup xrayTubeModule Xray Tube Fan-coil and temperature management module
 *
 * \ingroup applicationModule
 * 
 * 
 * This Module implements the functions of the Xray Tube device specifications in the 
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
    * \defgroup xrayApiModule API Module
    *  @{
    */
        
        ext void XrayInit(void);
    
        
        ext void XrayLoop(void);
        
        
        
    /** @}*/ // xrayApiModule
        


         
/** @}*/ // xrayTubeModule
        
        
#endif 