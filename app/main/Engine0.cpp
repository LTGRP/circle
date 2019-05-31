//
// File: Engine0.cpp
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

#include "Engine0.h"
#include "solver_zc.h"
#include "zero_crossing_types.h"
#ifndef slZcHadEvent
#define slZcHadEvent(ev, zcsDir)       (((ev) & (zcsDir)) != 0x00 )
#endif

#ifndef slZcUnAliasEvents
#define slZcUnAliasEvents(evL, evR)    ((((slZcHadEvent((evL), (SL_ZCS_EVENT_N2Z)) && slZcHadEvent((evR), (SL_ZCS_EVENT_Z2P))) || (slZcHadEvent((evL), (SL_ZCS_EVENT_P2Z)) && slZcHadEvent((evR), (SL_ZCS_EVENT_Z2N)))) ? (SL_ZCS_EVENT_NUL) : (evR)))
#endif

#define NumBitsPerChar                 8U

// Block signals and states (auto storage)
DW rtDW;

// Previous zero-crossings (trigger) states
PrevZCX rtPrevZCX;

// External inputs (root inport signals with auto storage)
ExtU rtU;

// External outputs (root outports fed by signals with auto storage)
ExtY rtY;

// Real-time model
RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;
extern "C" {
  extern ZCEventType rt_ZCFcn(ZCDirection zcDir, ZCSigState *prevZc, real_T
    currValue);
}                                      // extern "C"
  extern "C"
{
  extern real_T rtInf;
  extern real_T rtMinusInf;
  extern real_T rtNaN;
  extern real32_T rtInfF;
  extern real32_T rtMinusInfF;
  extern real32_T rtNaNF;
  extern void rt_InitInfAndNaN(size_t realSize);
  extern boolean_T rtIsInf(real_T value);
  extern boolean_T rtIsInfF(real32_T value);
  extern boolean_T rtIsNaN(real_T value);
  extern boolean_T rtIsNaNF(real32_T value);
  typedef struct {
    struct {
      uint32_T wordH;
      uint32_T wordL;
    } words;
  } BigEndianIEEEDouble;

  typedef struct {
    struct {
      uint32_T wordL;
      uint32_T wordH;
    } words;
  } LittleEndianIEEEDouble;

  typedef struct {
    union {
      real32_T wordLreal;
      uint32_T wordLuint;
    } wordL;
  } IEEESingle;
}                                      // extern "C"

extern "C" {
  real_T rtInf;
  real_T rtMinusInf;
  real_T rtNaN;
  real32_T rtInfF;
  real32_T rtMinusInfF;
  real32_T rtNaNF;
}
  extern "C"
{
  extern real_T rtGetInf(void);
  extern real32_T rtGetInfF(void);
  extern real_T rtGetMinusInf(void);
  extern real32_T rtGetMinusInfF(void);
}                                      // extern "C"

extern "C" {
  extern real_T rtGetNaN(void);
  extern real32_T rtGetNaNF(void);
}                                      // extern "C"
  extern "C"
{
  // Detect zero crossings events.
  ZCEventType rt_ZCFcn(ZCDirection zcDir, ZCSigState* prevZc, real_T currValue)
  {
    slZcEventType zcsDir;
    slZcEventType tempEv;
    ZCEventType zcEvent = NO_ZCEVENT;  // assume

    // zcEvent matrix
    static const slZcEventType eventMatrix[4][4] = {
      //          ZER              POS              NEG              UNK
      { SL_ZCS_EVENT_NUL, SL_ZCS_EVENT_Z2P, SL_ZCS_EVENT_Z2N, SL_ZCS_EVENT_NUL },// ZER

      { SL_ZCS_EVENT_P2Z, SL_ZCS_EVENT_NUL, SL_ZCS_EVENT_P2N, SL_ZCS_EVENT_NUL },// POS

      { SL_ZCS_EVENT_N2Z, SL_ZCS_EVENT_N2P, SL_ZCS_EVENT_NUL, SL_ZCS_EVENT_NUL },// NEG

      { SL_ZCS_EVENT_NUL, SL_ZCS_EVENT_NUL, SL_ZCS_EVENT_NUL, SL_ZCS_EVENT_NUL }// UNK
    };

    // get prevZcEvent and prevZcSign from prevZc
    slZcEventType prevEv = (slZcEventType)(((uint8_T)(*prevZc)) >> 2);
    slZcSignalSignType prevSign = (slZcSignalSignType)(((uint8_T)(*prevZc)) &
      (uint8_T)0x03);

    // get current zcSignal sign from current zcSignal value
    slZcSignalSignType currSign = (slZcSignalSignType)((currValue) > 0.0 ?
      SL_ZCS_SIGN_POS :
      ((currValue) < 0.0 ? SL_ZCS_SIGN_NEG : SL_ZCS_SIGN_ZERO));

    // get current zcEvent based on prev and current zcSignal value
    slZcEventType currEv = eventMatrix[prevSign][currSign];

    // get slZcEventType from ZCDirection
    switch (zcDir) {
     case ANY_ZERO_CROSSING:
      zcsDir = SL_ZCS_EVENT_ALL;
      break;

     case FALLING_ZERO_CROSSING:
      zcsDir = SL_ZCS_EVENT_ALL_DN;
      break;

     case RISING_ZERO_CROSSING:
      zcsDir = SL_ZCS_EVENT_ALL_UP;
      break;

     default:
      zcsDir = SL_ZCS_EVENT_NUL;
      break;
    }

    //had event, check if double zc happend remove double detection.
    if (slZcHadEvent(currEv, zcsDir)) {
      currEv = (slZcEventType)(slZcUnAliasEvents(prevEv, currEv));
    } else {
      currEv = SL_ZCS_EVENT_NUL;
    }

    // Update prevZc
    tempEv = (slZcEventType)(currEv << 2);// shift left by 2 bits
    *prevZc = (ZCSigState)((currSign) | (tempEv));
    if ((currEv & SL_ZCS_EVENT_ALL_DN) != 0) {
      zcEvent = FALLING_ZCEVENT;
    } else if ((currEv & SL_ZCS_EVENT_ALL_UP) != 0) {
      zcEvent = RISING_ZCEVENT;
    } else {
      zcEvent = NO_ZCEVENT;
    }

    return zcEvent;
  }                                    // rt_ZCFcn
}

extern "C" {
  //
  // Initialize the rtInf, rtMinusInf, and rtNaN needed by the
  // generated code. NaN is initialized as non-signaling. Assumes IEEE.
  //
  void rt_InitInfAndNaN(size_t realSize)
  {
    (void) (realSize);
    rtNaN = rtGetNaN();
    rtNaNF = rtGetNaNF();
    rtInf = rtGetInf();
    rtInfF = rtGetInfF();
    rtMinusInf = rtGetMinusInf();
    rtMinusInfF = rtGetMinusInfF();
  }

  // Test if value is infinite
  boolean_T rtIsInf(real_T value)
  {
    return (boolean_T)((value==rtInf || value==rtMinusInf) ? 1U : 0U);
  }

  // Test if single-precision value is infinite
  boolean_T rtIsInfF(real32_T value)
  {
    return (boolean_T)(((value)==rtInfF || (value)==rtMinusInfF) ? 1U : 0U);
  }

  // Test if value is not a number
  boolean_T rtIsNaN(real_T value)
  {
    return (boolean_T)((value!=value) ? 1U : 0U);
  }

  // Test if single-precision value is not a number
  boolean_T rtIsNaNF(real32_T value)
  {
    return (boolean_T)(((value!=value) ? 1U : 0U));
  }
}
  extern "C"
{
  //
  // Initialize rtInf needed by the generated code.
  // Inf is initialized as non-signaling. Assumes IEEE.
  //
  real_T rtGetInf(void)
  {
    size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
    real_T inf = 0.0;
    if (bitsPerReal == 32U) {
      inf = rtGetInfF();
    } else {
      union {
        LittleEndianIEEEDouble bitVal;
        real_T fltVal;
      } tmpVal;

      tmpVal.bitVal.words.wordH = 0x7FF00000U;
      tmpVal.bitVal.words.wordL = 0x00000000U;
      inf = tmpVal.fltVal;
    }

    return inf;
  }

  //
  // Initialize rtInfF needed by the generated code.
  // Inf is initialized as non-signaling. Assumes IEEE.
  //
  real32_T rtGetInfF(void)
  {
    IEEESingle infF;
    infF.wordL.wordLuint = 0x7F800000U;
    return infF.wordL.wordLreal;
  }

  //
  // Initialize rtMinusInf needed by the generated code.
  // Inf is initialized as non-signaling. Assumes IEEE.
  //
  real_T rtGetMinusInf(void)
  {
    size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
    real_T minf = 0.0;
    if (bitsPerReal == 32U) {
      minf = rtGetMinusInfF();
    } else {
      union {
        LittleEndianIEEEDouble bitVal;
        real_T fltVal;
      } tmpVal;

      tmpVal.bitVal.words.wordH = 0xFFF00000U;
      tmpVal.bitVal.words.wordL = 0x00000000U;
      minf = tmpVal.fltVal;
    }

    return minf;
  }

  //
  // Initialize rtMinusInfF needed by the generated code.
  // Inf is initialized as non-signaling. Assumes IEEE.
  //
  real32_T rtGetMinusInfF(void)
  {
    IEEESingle minfF;
    minfF.wordL.wordLuint = 0xFF800000U;
    return minfF.wordL.wordLreal;
  }
}

extern "C" {
  //
  // Initialize rtNaN needed by the generated code.
  // NaN is initialized as non-signaling. Assumes IEEE.
  //
  real_T rtGetNaN(void)
  {
    size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
    real_T nan = 0.0;
    if (bitsPerReal == 32U) {
      nan = rtGetNaNF();
    } else {
      union {
        LittleEndianIEEEDouble bitVal;
        real_T fltVal;
      } tmpVal;

      tmpVal.bitVal.words.wordH = 0xFFF80000U;
      tmpVal.bitVal.words.wordL = 0x00000000U;
      nan = tmpVal.fltVal;
    }

    return nan;
  }

  //
  // Initialize rtNaNF needed by the generated code.
  // NaN is initialized as non-signaling. Assumes IEEE.
  //
  real32_T rtGetNaNF(void)
  {
    IEEESingle nanF = { { 0 } };

    nanF.wordL.wordLuint = 0xFFC00000U;
    return nanF.wordL.wordLreal;
  }
}
// Model step function
  void Engine0_step(void)
{
  real_T rtb_DiscreteTimeIntegrator;
  boolean_T rtb_RelationalOperator;
  ZCEventType zcEvent;
  real_T rtb_Switch;
  real_T rtb_Pumping;
  real_T rtb_ftheta;
  real_T rtb_Init;

  // DiscreteIntegrator: '<S4>/Discrete-Time Integrator'
  rtb_DiscreteTimeIntegrator = rtDW.DiscreteTimeIntegrator_DSTATE;

  // Outport: '<Root>/crank speed (rad//sec)' incorporates:
  //   DiscreteIntegrator: '<S4>/Discrete-Time Integrator'
  rtY.crankspeedradsec = rtDW.DiscreteTimeIntegrator_DSTATE;

  // Gain: '<S1>/rad//s to rpm' incorporates:
  //   DiscreteIntegrator: '<S4>/Discrete-Time Integrator'
  rtDW.EngineSpeed = 9.5492965855137211//@>68
    * rtDW.DiscreteTimeIntegrator_DSTATE;

  // Outport: '<Root>/RPM '
  rtY.RPM = rtDW.EngineSpeed;

  // RelationalOperator: '<S9>/Relational Operator' incorporates:
  //   Constant: '<S9>/Constant'
  rtb_RelationalOperator = (rtDW.DiscreteTimeIntegrator_DSTATE_b//@>76
    >= 3.1415926535897931);

  // Outputs for Triggered SubSystem: '<S6>/positive edge to dual edge conversion' incorporates:
  //   TriggerPort: '<S10>/Trigger'
  if (rtb_RelationalOperator           //@>80
      && (rtPrevZCX.positiveedgetodualedgeconversio//@>80
          != POS_ZCSIG)) {
    // UnitDelay: '<S10>/Unit Delay'
    rtDW.UnitDelay_m = rtDW.UnitDelay_DSTATE_a;

    // Update for UnitDelay: '<S10>/Unit Delay' incorporates:
    //   Gain: '<S10>/Gain'
    rtDW.UnitDelay_DSTATE_a =          //@>81
      -rtDW.UnitDelay_m;
  }

  rtPrevZCX.positiveedgetodualedgeconversio = rtb_RelationalOperator;

  // End of Outputs for SubSystem: '<S6>/positive edge to dual edge conversion'

  // Outputs for Triggered SubSystem: '<S1>/Compression' incorporates:
  //   TriggerPort: '<S3>/Trigger'
  zcEvent = rt_ZCFcn(ANY_ZERO_CROSSING,&rtPrevZCX.Compression_Trig_ZCE,
                     (rtDW.UnitDelay_m));
  if (zcEvent                          //@>13
      != NO_ZCEVENT) {
    rtDW.Trigger = (real_T)zcEvent;

    // InitialCondition: '<S3>/Init'
    if (rtDW.Init_FirstOutputTime) {
      rtDW.Init_FirstOutputTime = false;
      rtb_Init = 0.152;
    } else {
      rtb_Init = rtDW.DiscreteTimeIntegrator_DSTATE_a;
    }

    // End of InitialCondition: '<S3>/Init'

    // UnitDelay: '<S3>/Unit Delay'
    rtDW.UnitDelay = rtDW.UnitDelay_DSTATE;

    // Update for UnitDelay: '<S3>/Unit Delay'
    rtDW.UnitDelay_DSTATE = rtb_Init;
  }

  // End of Outputs for SubSystem: '<S1>/Compression'

  // Gain: '<S2>/Stoichiometric Fuel'
  rtb_Init = 0.0684931506849315        //@>6
    * rtDW.UnitDelay;

  // Saturate: '<S5>/0 <= theta <= 90' incorporates:
  //   Inport: '<Root>/Angle '
  if (rtU.ThrottleAngle > 90.0) {
    rtb_Switch = 90.0;
  } else if (rtU.ThrottleAngle < 0.0) {
    rtb_Switch = 0.0;
  } else {
    rtb_Switch = rtU.ThrottleAngle;
  }

  // End of Saturate: '<S5>/0 <= theta <= 90'

  // DiscreteIntegrator: '<S5>/Discrete-Time Integrator'
  if (((rtDW.Trigger                   //@>104
        > 0.0)                         //@>104
       && (rtDW.DiscreteTimeIntegrator_PrevRese//@>104
           <= 0))                      //@>104
      || ((rtDW.Trigger                //@>104
           <= 0.0)                     //@>104
          && (rtDW.DiscreteTimeIntegrator_PrevRese//@>104
              == 1))) {
    rtDW.DiscreteTimeIntegrator_DSTATE_a = 0.0;
  }

  // End of DiscreteIntegrator: '<S5>/Discrete-Time Integrator'

  // Fcn: '<S7>/Pumping' incorporates:
  //   DiscreteIntegrator: '<S4>/Discrete-Time Integrator'
  //   DiscreteIntegrator: '<S7>/Discrete-Time Integrator'
  rtb_Pumping = ((0.08979              //@>37
                  * rtDW.DiscreteTimeIntegrator_DSTATE_i//@>37
                  * rtDW.DiscreteTimeIntegrator_DSTATE//@>37
                  + -0.366)            //@>37
                 - 0.0337              //@>37
                 * rtDW.DiscreteTimeIntegrator_DSTATE//@>37
                 * rtDW.DiscreteTimeIntegrator_DSTATE_i//@>37
                 * rtDW.DiscreteTimeIntegrator_DSTATE_i)//@>37
    + 0.0001                           //@>37
    * rtDW.DiscreteTimeIntegrator_DSTATE_i//@>37
    * rtDW.DiscreteTimeIntegrator_DSTATE//@>37
    * rtDW.DiscreteTimeIntegrator_DSTATE;

  // Fcn: '<S8>/f(theta)'
  rtb_ftheta = ((2.821                 //@>55
                 - 0.05231             //@>55
                 * rtb_Switch)         //@>55
                + 0.10299              //@>55
                * rtb_Switch           //@>55
                * rtb_Switch)          //@>55
    - 0.00063                          //@>55
    * rtb_Switch                       //@>55
    * rtb_Switch                       //@>55
    * rtb_Switch;

  // Product: '<S8>/Product2' incorporates:
  //   DiscreteIntegrator: '<S7>/Discrete-Time Integrator'
  rtb_Switch = 1.0                     //@>51
    / rtDW.DiscreteTimeIntegrator_DSTATE_i;

  // MinMax: '<S8>/MinMax' incorporates:
  //   DiscreteIntegrator: '<S7>/Discrete-Time Integrator'
  if ((rtDW.DiscreteTimeIntegrator_DSTATE_i <= rtb_Switch) || rtIsNaN(rtb_Switch))
  {
    rtb_Switch = rtDW.DiscreteTimeIntegrator_DSTATE_i;
  }

  // End of MinMax: '<S8>/MinMax'

  // Switch: '<S8>/Switch' incorporates:
  //   Constant: '<S8>/Sonic Flow '
  //   Fcn: '<S8>/g(pratio)'
  if (rtb_Switch                       //@>54
      >= 0.5) {
    // Fcn: '<S8>/g(pratio)'
    rtb_Switch                         //@>57
      -= rtb_Switch                    //@>57
      * rtb_Switch;
    if (rtb_Switch                     //@>57
        < 0.0) {
      rtb_Switch =                     //@>57
        -sqrt(                         //@>57
              -rtb_Switch);
    } else {
      rtb_Switch = sqrt(rtb_Switch);
    }

    rtb_Switch                         //@>54
      *= 2.0;
  } else {
    rtb_Switch = 1.0;
  }

  // End of Switch: '<S8>/Switch'

  // DiscreteIntegrator: '<S9>/Discrete-Time Integrator'
  if (rtb_RelationalOperator           //@>107
      && (rtDW.DiscreteTimeIntegrator_PrevRe_k//@>107
          <= 0)) {
    rtDW.DiscreteTimeIntegrator_DSTATE_b = 0.0;
  }

  // End of DiscreteIntegrator: '<S9>/Discrete-Time Integrator'

  // Update for DiscreteIntegrator: '<S4>/Discrete-Time Integrator' incorporates:
  //   DiscreteIntegrator: '<S4>/Discrete-Time Integrator'
  //   Fcn: '<S2>/Torque Gen'
  //   Fcn: '<S2>/Torque Gen2'
  //   Gain: '<S4>/Inverse Inertia, 1//J'
  //   Inport: '<Root>/Load'
  //   Inport: '<Root>/Spark Advance (BTDC)'
  //   Sum: '<S2>/Sum'
  //   Sum: '<S4>/Sum'
  rtDW.DiscreteTimeIntegrator_DSTATE   //@>108
    += (((((((379.36                   //@>8
              * rtDW.UnitDelay         //@>8
              + -181.3)                //@>8
             + 21.91                   //@>8
             * rtDW.UnitDelay          //@>8
             / rtb_Init)               //@>8
            - 0.85                     //@>8
            * rtDW.UnitDelay           //@>8
            * rtDW.UnitDelay           //@>8
            / (rtb_Init                //@>8
               * rtb_Init))            //@>8
           + 0.26                      //@>8
           * rtU.SparkAdvanceBTDC)     //@>8
          - 0.0028                     //@>8
          * rtU.SparkAdvanceBTDC       //@>8
          * rtU.SparkAdvanceBTDC)      //@>7
         + ((((0.027                   //@>9
               * rtDW.DiscreteTimeIntegrator_DSTATE//@>9
               - 0.000107              //@>9
               * rtDW.DiscreteTimeIntegrator_DSTATE//@>9
               * rtDW.DiscreteTimeIntegrator_DSTATE)//@>9
              + 0.00048                //@>9
              * rtDW.DiscreteTimeIntegrator_DSTATE//@>9
              * rtU.SparkAdvanceBTDC)  //@>9
             + 2.55                    //@>9
             * rtU.SparkAdvanceBTDC    //@>9
             * rtDW.UnitDelay)         //@>9
            - 0.05                     //@>9
            * rtU.SparkAdvanceBTDC     //@>9
            * rtU.SparkAdvanceBTDC     //@>9
            * rtDW.UnitDelay))         //@>65
        - rtU.Load)                    //@>64
    * 7.1428571428571423               //@>108
    * 0.0001;

  // Update for DiscreteIntegrator: '<S5>/Discrete-Time Integrator'
  rtDW.DiscreteTimeIntegrator_DSTATE_a //@>104
    += 0.0001                          //@>104
    * rtb_Pumping;
  if (rtDW.Trigger                     //@>104
      > 0.0) {
    rtDW.DiscreteTimeIntegrator_PrevRese = 1;
  } else if (rtDW.Trigger              //@>104
             < 0.0) {
    rtDW.DiscreteTimeIntegrator_PrevRese = -1;
  } else if (rtDW.Trigger              //@>104
             == 0.0) {
    rtDW.DiscreteTimeIntegrator_PrevRese = 0;
  } else {
    rtDW.DiscreteTimeIntegrator_PrevRese = 2;
  }

  // End of Update for DiscreteIntegrator: '<S5>/Discrete-Time Integrator'

  // Signum: '<S8>/flow direction' incorporates:
  //   Constant: '<S5>/Atmospheric Pressure, Pa (bar)'
  //   DiscreteIntegrator: '<S7>/Discrete-Time Integrator'
  //   Sum: '<S8>/Sum'
  if (1.0                              //@>53
      - rtDW.DiscreteTimeIntegrator_DSTATE_i < 0.0) {
    rtb_Init = -1.0;
  } else if (1.0                       //@>53
             - rtDW.DiscreteTimeIntegrator_DSTATE_i > 0.0) {
    rtb_Init = 1.0;
  } else if (1.0                       //@>53
             - rtDW.DiscreteTimeIntegrator_DSTATE_i == 0.0) {
    rtb_Init = 0.0;
  } else {
    rtb_Init = 1.0                     //@>53
      - rtDW.DiscreteTimeIntegrator_DSTATE_i;
  }

  // Update for DiscreteIntegrator: '<S7>/Discrete-Time Integrator' incorporates:
  //   Gain: '<S7>/RT//Vm'
  //   Product: '<S8>/Product'
  //   Signum: '<S8>/flow direction'
  //   Sum: '<S7>/Sum'
  rtDW.DiscreteTimeIntegrator_DSTATE_i //@>106
    += (rtb_ftheta                     //@>49
        * rtb_Switch                   //@>49,56
        * rtb_Init                     //@>39
        - rtb_Pumping)                 //@>38
    * 0.41328                          //@>106
    * 0.0001;

  // Update for DiscreteIntegrator: '<S9>/Discrete-Time Integrator'
  rtDW.DiscreteTimeIntegrator_DSTATE_b //@>107
    += 0.0001                          //@>107
    * rtb_DiscreteTimeIntegrator;
  rtDW.DiscreteTimeIntegrator_PrevRe_k = (int8_T)rtb_RelationalOperator;
}

// Model initialize function
void Engine0_initialize(void)
{
  // Registration code

  // initialize non-finites
  rt_InitInfAndNaN(sizeof(real_T));

  // Start for Triggered SubSystem: '<S1>/Compression'
  // Start for InitialCondition: '<S3>/Init'
  rtDW.Init_FirstOutputTime = true;

  // End of Start for SubSystem: '<S1>/Compression'
  rtPrevZCX.Compression_Trig_ZCE = UNINITIALIZED_ZCSIG;
  rtPrevZCX.positiveedgetodualedgeconversio = POS_ZCSIG;

  // InitializeConditions for DiscreteIntegrator: '<S4>/Discrete-Time Integrator'
  rtDW.DiscreteTimeIntegrator_DSTATE = 209.48;

  // InitializeConditions for DiscreteIntegrator: '<S5>/Discrete-Time Integrator'
  rtDW.DiscreteTimeIntegrator_PrevRese = 2;

  // InitializeConditions for DiscreteIntegrator: '<S7>/Discrete-Time Integrator'
  rtDW.DiscreteTimeIntegrator_DSTATE_i = 0.543;

  // InitializeConditions for DiscreteIntegrator: '<S9>/Discrete-Time Integrator'
  rtDW.DiscreteTimeIntegrator_PrevRe_k = 2;

  // SystemInitialize for Triggered SubSystem: '<S6>/positive edge to dual edge conversion'
  // InitializeConditions for UnitDelay: '<S10>/Unit Delay'
  rtDW.UnitDelay_DSTATE_a = 1.0F;

  // SystemInitialize for Outport: '<S10>/trigger'
  rtDW.UnitDelay_m = -1.0F;

  // End of SystemInitialize for SubSystem: '<S6>/positive edge to dual edge conversion'

  // SystemInitialize for Triggered SubSystem: '<S1>/Compression'
  // InitializeConditions for UnitDelay: '<S3>/Unit Delay'
  rtDW.UnitDelay_DSTATE = 0.152;

  // SystemInitialize for Outport: '<S3>/mass(k)'
  rtDW.UnitDelay = 0.152;

  // SystemInitialize for Outport: '<S3>/trigger'
  rtDW.Trigger = -1.0;

  // End of SystemInitialize for SubSystem: '<S1>/Compression'
}

//
// File trailer for generated code.
//
// [EOF]
//
