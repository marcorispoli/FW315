#ifndef _FILTER_H    
#define _FILTER_H

#include "definitions.h"  
#include "application.h"  

#undef ext
#undef ext_static

#ifdef _FILTER_C
    #define ext
    #define ext_static static 
#else
    #define ext extern
    #define ext_static extern
#endif

/*!
 * \defgroup filterModule Filter Selection management module
 *
 * \ingroup applicationModule
 * 
 * 
 * This Module implements the functions of the Filter specifications in the 
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
    * \defgroup filterApiModule API Module
    *  @{
    */
        
        ext void FilterInit(void);
    
        
        ext void FilterLoop(void);
        ext void FilterTest(void);
        
        
        
    /** @}*/ // filterApiModule
        


         
/** @}*/ // filterModule
        
        
#endif 