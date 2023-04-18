/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "application.h"
#include "Protocol/protocol.h"




// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
static uint8_t trigger_time = 0;

#define _1024_ms_TriggerTime 0x1
#define _7820_us_TriggerTime 0x2
#define _15_64_ms_TriggerTime 0x4


static void rtcEventHandler (RTC_TIMER32_INT_MASK intCause, uintptr_t context)
{
    // Periodic Interval Handler: Freq = 1024 / 2 ^ (n+3)
    
    if (intCause & RTC_TIMER32_INT_MASK_PER0) trigger_time |= _7820_us_TriggerTime;  // 7.82ms Interrupt
    if (intCause & RTC_TIMER32_INT_MASK_PER1) trigger_time |= _15_64_ms_TriggerTime; // 15.64ms Interrupt
    if (intCause & RTC_TIMER32_INT_MASK_PER7) trigger_time |= _1024_ms_TriggerTime;  // 1024 ms Interrupt
    
}

int main ( void )
{
  
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
  
    // Registers the RTC interrupt routine to the RTC module
    RTC_Timer32CallbackRegister(rtcEventHandler, 0);
    RTC_Timer32Start(); // Start the RTC module
            
    // Start the TCo to start the Vitality LED
    TC0_CompareStart();
    
    
    
    // Application Protocol initialization
    ApplicationProtocolInit();
    
    
    
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        
        // Protocol management
        ApplicationProtocolLoop();
        
        // CanOpen management
       
        
        // Timer events activated into the RTC interrupt
        if(trigger_time & _7820_us_TriggerTime){
            trigger_time &=~ _7820_us_TriggerTime;
            
             
            Protocol_7280_us_callback();
            
        }

        if(trigger_time & _15_64_ms_TriggerTime){
            trigger_time &=~ _15_64_ms_TriggerTime;
               
        }
        
        
                
        if(trigger_time & _15_64_ms_TriggerTime){
            trigger_time &=~ _15_64_ms_TriggerTime;
             //MET_Can_Open_Send_WriteData(1,0x20,0,0,0);
                //VITALITY_LED_Toggle();
                /*
                if(GeneratorGetHvOn()) VITALITY_LED_Set();
                else VITALITY_LED_Clear();
                */   
        }        
     

    }

    
    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

