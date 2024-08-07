/*!
 * \defgroup  applicationModule Application Modules section
 * 
 */


/*!
 * \defgroup  definitions Application Constants
 * 
 * \ingroup applicationModule
 */


/*!
 * \defgroup  libraryModules Library Modules Used in the Application.
 */


/*!
 * \mainpage
 *
 * # Overview
 *
 * This Firmware implements the functions of the FW315 Filter controller device 
 * software specification.
 * 
 * 
 * # Dependencies test
 * 
 * The Application has been developed with the use of the Microchip Harmony 3
 * libraries. The libraries are a set of source files auto generated by the 
 * Harmony 3 configurator tools and contained into the following directories:
 * - src/config;
 * - src/packs;
 * 
 * ***NOTE: those directories shall never be directly modified since 
 * they are auto-rigenerated every time the generator tool is launched.*** 
 *
 * The libraries make use of the CMSIS 5.4.0 for ARM processors
 * and SAME51_DFP 3.6.130
 *  
 * 
 * # Modules
 *
 * The Application implements the following modules:
 * - @ref applicationModule : defines common objects to the application modules;
 * - @ref protocolModule : module implementing CAN communication protocol;
 * - @ref libraryModules : this is the set of the libraries used in the current application;
 *
 * # Compilation
 * 
 * This software is compiled with the MPLAB-X IDE 6.05:
 * - Compiler: XC32 Vs 3.1
 * 
 * 
 * # Licensing
 *
 * The Application has been developed with the Microchip Harmony 3 libraries 
 * with the Microchip License Agreement conditions as described in:
 * - @ref licenseAgreement 
 *
 * # Documentation
 * 
 * This documentation has been produced with Doxygen 
 */



#ifndef _APPLICATION_H    /* Guard against multiple inclusion */
#define _APPLICATION_H

// Defines the Application revision code

/** \addtogroup definitions
 *  @{
*/

// Application Revision 
static const unsigned char  APPLICATION_MAJ_REV =  0 ;  //!< Revision Major Number
static const unsigned char  APPLICATION_MIN_REV =  2 ;  //!< Revision Minor Number
static const unsigned char  APPLICATION_SUB_REV =  1 ;  //!< Revision build Number

/** @}*/
        
#endif 