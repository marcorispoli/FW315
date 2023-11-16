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
 * # Harmony 3 Configurator Settings
 * 
 * This Module makes use of the following Processor modules:
 * - TC1 module;
 * - TC1 Clock assignment;
 * - PIN Assignement;
 * 
 * ## TC1 Settings
 * 
 * - Counter mode: 16 bit mode
 * - Prescaler: GCLK_TC (1us Timer resolution)
 * - Operating Mode: Compare
 * - Waveform: Match Frequency
 * - Compare 0 Value: 1
 * - Enable Double Buffering: Yes
 * - Enable Period Interrupt: Yes
 * 
 * ## TC1 Peripheral Clock:
 * 
 * - TC0, TC1: GCLK2 at 1MHz;
 *  
 * 
 * ## Pin Assignement
 * 
 * |Signal name|Pin|Direction|Driver|Description|
 * |:---|:---:|:---:|:---:|:---|
 * |uc_MS1|3|Out|Strong|Micro stepping mode(1)|
 * |uc_MS2|4|Out|Strong|Micro stepping mode (2)|
 * |uc_RESET|13|Out|Strong|Driver Reset|
 * |uc_SLEEP|14|Out|Strong|Driver Sleep|
 * |uc_STEP|15|TC1_WO0|Strong|Driver Step|
 * |uc_ENA|16|Out|Strong|Driver Ena|
 * |uc_DIR|17|Out|Strong|Driver direction|
 * |uc_REFA|18|Out|Strong|Current Limit Pin(1)|
 * |uc_REFB|19|Out|Strong|Current Limit Pin(2)|
 * |uc_OPTO|20|Out|Strong|Opto for position detection|
 * 
 * 
 * ## Technical data
 * 
 * The Filter motor is activated with the following settings:
 * 
 * - Min PWM period (run): 190 us, 1500, 40 
 * - Max PWM period (initial speed): 1500 us, 40 
 * - Ramp delta period: 40us decrement every period;
 * - Step Mode: 16 u-step;
 * - mm/step = about 0.013
 * 
 * Measure of the slider ticks:
 * - Light slots: 1380 steps (18mm);
 * - Dark slots: 130/140 steps;
 *  
 *  @{
 * 
 */
    

    /**
    * \defgroup filterApiModule Module's API
    *  @{
    */
        
    ext void FilterInit(void);            
    ext void FilterLoop(void);
    ext void FilterTest(void);
    ext bool FilterSelect(uint8_t filter);
    ext bool FilterIsTarget(uint8_t filter);
    ext bool FilterIsRunning(void);
    ext bool FilterIsError(void);
    
    /** @}*/ // filterApiModule
    
     /**
    * \defgroup filterStructModule Module Data structures
    *  @{
    */
    
    /// Defines the pin code to set the driver current limit
    typedef enum{
    _CURLIM_DISABLE = 0,    //!< No torque applied
    _CURLIM_LOW,            //!< minimum torque applied
    _CURLIM_MED,            //!< medium torque applied
    _CURLIM_HIGH            //!< activation torque
    }FASE_CURRENT_MODE_t;

    /// Defines the Driver micro-step mode 
    typedef enum{
        _uSTEP_1 = 0,   //!< Full step driver mode
        _uSTEP_2,       //!< 1/2 step driver mode
        _uSTEP_4,       //!< 1/4 step driver mode
        _uSTEP_16       //!< 1/16 step driver mode
    }MICROSTEP_t;

    /// Defines the Driver shaft rotation direction 
    typedef enum{
        MOTOR_DIR_HOME = 0, //!< Motor rotation to Home position
        MOTOR_DIR_OUT       //!< Motor rotation to Out position
    }DIRECTION_t;

    typedef enum{
        _STOP_BECAUSE_TARGET = 0,
        _STOP_BECAUSE_ERROR,
        _STOP_BECAUSE_HOME
    }STOPMODE_t;

    /// Module data structure
    typedef struct{
    
        // Filter positioning data
        bool    command_activated;  //!< A command is in execution
        bool    slot_valid;         //!< A valid slot is selected
        uint8_t target_slot;        //!< Target slot selected 
        uint8_t target_filter;      //!< This is the Filter code requested
        uint32_t target_slot_position[5]; //!< Define the calibrated position for every slot
        
        // Slot detection        
        bool     opto_status; //!< This is a copy of the current Opto status
        uint8_t  current_slot;//!< This is the current slot during positioning
        uint32_t current_pulses;//!< This is the current pulse count during transitions
        
        // Speed regulation
        uint16_t init_period;   //!< This is the initial period for the PWM
        uint16_t final_period;  //!< This is the final period for the PWM
        uint16_t ramp_rate;     //!< This is the ramp value (delta period for every period) PWM
                
        uint8_t command_sequence;//!< This is the execution sequence index
        bool     running;        //!< This is the motor activated flag
        STOPMODE_t cause;        //!< This is cause of the command termination
  
        uint32_t  measured_light_slot[5]; //!< Measures the light slot pulses for diagnosys and test
        uint32_t  measured_dark_slot[5]; //!< Measures the dark slot pulses for diagnosys and test
        
    }FILTER_MOTOR_t;
    
    /** @}*/ // filterStructModule

    /**
    * \defgroup filterMacroModule Module's Macros
    *  @{
    */
    
    #define FILTER1_SLOT    0
    #define FILTER2_SLOT    1
    #define FILTER3_SLOT    3
    #define FILTER4_SLOT    4
    #define MIRROR_SLOT     2

    
    #define um_step_dimension   ((uint32_t) 13)   //!< (micro-meter) linear space per pulse  
    #define umToSteps(val) (((uint32_t) val) / ( (uint32_t) um_step_dimension)) //!< Macro conversion from um to pulse
    #define stepsToum(val) (((uint32_t) val) * ( (uint32_t) um_step_dimension)) //!< Macro conversion from step to um

    #define dark_slot_dim  (2000)     //!< (micro-meter) dark slot dimension 
    #define light_slot_dim (18000)    //!< //!< (micro-meter) light slot dimension
    
    #define MAX_STEPS_BETWEEN_SLOTS umToSteps(dark_slot_dim + light_slot_dim) //!< Max number of steps the module shall count between slots
    #define MOTOR_SPEED_HOME _uSTEP_16, 150, 1500, 20 //!< Motor speed: (run-period, init-period, ramp delta-period), with period in us
    #define MOTOR_SPEED_OUT _uSTEP_16, 150, 2000, 20 //!< Motor speed: (run-period, init-period, ramp delta-period), with period in us
    /** @}*/ // filterMacroModule

    
    
/** @}*/ // filterModule
        
        
#endif 