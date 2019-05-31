//
// File: Engine0.h
//
// Code generated for Simulink model 'Engine0'.
//
// Model version                  : 1.93
// Simulink Coder version         : 8.10 (R2016a) 10-Feb-2016
// C/C++ source code generated on : Tue Dec 26 11:29:08 2017
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM 10
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
// Validation result: Not run
//
#ifndef RTW_HEADER_Engine0_h_
#define RTW_HEADER_Engine0_h_
#include <stddef.h>
#include <Arduino.h>
#ifndef Engine0_COMMON_INCLUDES_
# define Engine0_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "zero_crossing_types.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 // Engine0_COMMON_INCLUDES_

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

// Forward declaration for rtModel
typedef struct tag_RTM RT_MODEL;

// Block signals and states (auto storage) for system '<Root>'
typedef struct {
  real_T EngineSpeed;                  // '<S1>/rad//s to rpm'
  real_T UnitDelay;                    // '<S3>/Unit Delay'
  real_T Trigger;                      // '<S3>/Trigger'
  real_T DiscreteTimeIntegrator_DSTATE;// '<S4>/Discrete-Time Integrator'
  real_T DiscreteTimeIntegrator_DSTATE_a;// '<S5>/Discrete-Time Integrator'
  real_T DiscreteTimeIntegrator_DSTATE_i;// '<S7>/Discrete-Time Integrator'
  real_T DiscreteTimeIntegrator_DSTATE_b;// '<S9>/Discrete-Time Integrator'
  real_T UnitDelay_DSTATE;             // '<S3>/Unit Delay'
  real32_T UnitDelay_m;                // '<S10>/Unit Delay'
  real32_T UnitDelay_DSTATE_a;         // '<S10>/Unit Delay'
  int8_T DiscreteTimeIntegrator_PrevRese;// '<S5>/Discrete-Time Integrator'
  int8_T DiscreteTimeIntegrator_PrevRe_k;// '<S9>/Discrete-Time Integrator'
  boolean_T Init_FirstOutputTime;      // '<S3>/Init'
} DW;

// Zero-crossing (trigger) state
typedef struct {
  ZCSigState positiveedgetodualedgeconversio;// '<S6>/positive edge to dual edge conversion' 
  ZCSigState Compression_Trig_ZCE;     // '<S1>/Compression'
} PrevZCX;

// External inputs (root inport signals with auto storage)
typedef struct {
  real_T ThrottleAngle;                // '<Root>/Angle '
  real_T SparkAdvanceBTDC;             // '<Root>/Spark Advance (BTDC)'
  real_T Load;                         // '<Root>/Load'
} ExtU;

// External outputs (root outports fed by signals with auto storage)
typedef struct {
  real_T crankspeedradsec;             // '<Root>/crank speed (rad//sec)'
  real_T RPM;                          // '<Root>/RPM '
} ExtY;

// Real-time Model Data Structure
struct tag_RTM {
  const char_T * volatile errorStatus;
};

// Block signals and states (auto storage)
extern DW rtDW;

#ifdef __cplusplus

extern "C" {

#endif

  // External inputs (root inport signals with auto storage)
  extern ExtU rtU;

  // External outputs (root outports fed by signals with auto storage)
  extern ExtY rtY;

#ifdef __cplusplus

}
#endif

#ifdef __cplusplus

extern "C" {

#endif

  // Model entry point functions
  extern void Engine0_initialize(void);
  extern void Engine0_step(void);

#ifdef __cplusplus

}
#endif

// Real-time Model object
#ifdef __cplusplus

extern "C" {

#endif

  extern RT_MODEL *const rtM;

#ifdef __cplusplus

}
#endif

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Note that this particular code originates from a subsystem build,
//  and has its own system numbers different from the parent model.
//  Refer to the system hierarchy for this subsystem below, and use the
//  MATLAB hilite_system command to trace the generated code back
//  to the parent model.  For example,
//
//  hilite_system('Engine/Engine')    - opens subsystem Engine/Engine
//  hilite_system('Engine/Engine/Kp') - opens and selects block Kp
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'Engine'
//  '<S1>'   : 'Engine/Engine'
//  '<S2>'   : 'Engine/Engine/Combustion'
//  '<S3>'   : 'Engine/Engine/Compression'
//  '<S4>'   : 'Engine/Engine/Engine Dynamics'
//  '<S5>'   : 'Engine/Engine/Throttle & Manifold'
//  '<S6>'   : 'Engine/Engine/valve timing'
//  '<S7>'   : 'Engine/Engine/Throttle & Manifold/Intake Manifold'
//  '<S8>'   : 'Engine/Engine/Throttle & Manifold/Throttle'
//  '<S9>'   : 'Engine/Engine/valve timing/TDC and BDC detection'
//  '<S10>'  : 'Engine/Engine/valve timing/positive edge to dual edge conversion'

#endif                                 // RTW_HEADER_Engine0_h_

//
// File trailer for generated code.
//
// [EOF]
//
