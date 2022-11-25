#define S_FUNCTION_NAME  sfun_pssecomm
#define S_FUNCTION_LEVEL 2

#include <windows.h>
#include <string>
#include <iostream>
#include <memory>

#include "pipeclient.hpp"
#include "simstruc.h"

#define STEP_SIZE_P  0
#define NUM_PRMS     1

#define MDL_CHECK_PARAMETERS
#if defined(MDL_CHECK_PARAMETERS) && defined(MATLAB_MEX_FILE)
// Validate our parameters to verify they are okay
static void mdlCheckParameters(SimStruct *S)
{

    bool isValid = mxIsDouble(ssGetSFcnParam(S,STEP_SIZE_P)) &&
        mxGetNumberOfElements(ssGetSFcnParam(S,STEP_SIZE_P)) == 1 &&
        !mxIsComplex(ssGetSFcnParam(S,STEP_SIZE_P));

    if (isValid) {
        double *v = reinterpret_cast<double *>(mxGetData(ssGetSFcnParam(S,STEP_SIZE_P)));
        if (*v < 0) isValid = false;
    }
    if (!isValid) {
        ssSetErrorStatus(S,"Communication interval parameter must be a positive real scalar of double data type.");
        return;
    }
    return;
}
#endif

// Method to check parameters and configure the S-Function block.
static void mdlInitializeSizes(SimStruct *S)
{
    // Register the number of expected parameters
    ssSetNumSFcnParams(S, NUM_PRMS);

#if defined(MATLAB_MEX_FILE)
    if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) {
        mdlCheckParameters(S);
        if (ssGetErrorStatus(S) != NULL) {
            return;
        }
    } else {
        return; // Parameter mismatch will be reported by Simulink
    }
#endif

    ssSetSFcnParamTunable(S, STEP_SIZE_P, false);

    // Setting properties of inputs 
    if (!ssSetNumInputPorts(S, 1)) return;
    
    //ssSetInputPortWidth(S, 0, 2);
    ssSetInputPortWidth(S, 0, DYNAMICALLY_SIZED);
    ssSetInputPortDataType(S, 0, SS_DOUBLE);
    ssSetInputPortComplexSignal(S, 0, COMPLEX_NO);
    ssSetInputPortRequiredContiguous(S, 0, 1);
    ssSetInputPortDirectFeedThrough(S, 0, 0);
    
    if (!ssSetNumOutputPorts(S, 1)) return;
    
    ssSetOutputPortWidth(S, 0, 20);
    ssSetOutputPortDataType(S, 0, SS_DOUBLE);
    ssSetOutputPortComplexSignal(S, 0, COMPLEX_NO);   


    ssSetNumSampleTimes(S, 1);

    // Specify the sim state compliance to be same as Simulink built-in block
    ssSetSimStateCompliance(S, HAS_NO_SIM_STATE);
    
    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);
    
    ssSetModelReferenceNormalModeSupport(S, MDL_START_AND_MDL_PROCESS_PARAMS_OK);
}

#define MDL_SET_INPUT_PORT_DIMENSION_INFO
#if defined(MATLAB_MEX_FILE) && defined(MDL_SET_INPUT_PORT_DIMENSION_INFO)
static void mdlSetInputPortDimensionInfo(SimStruct *S, int_T port, const DimsInfo_T *dimsInfo)
{
    if(!ssSetInputPortDimensionInfo(S, port, dimsInfo)) return;
}
#endif

#define MDL_SET_OUTPUT_PORT_DIMENSION_INFO
#if defined(MATLAB_MEX_FILE) && defined(MDL_SET_OUTPUT_PORT_DIMENSION_INFO)
static void mdlSetOutputPortDimensionInfo(SimStruct *S, int_T port, const DimsInfo_T *dimsInfo)
{
    if(!ssSetOutputPortDimensionInfo(S, port, dimsInfo)) return;
}
#endif


//   This function is used to specify the sample time(s) for your
//   S-function. You must register the same number of sample times as
//   specified in ssSetNumSampleTimes.
static void mdlInitializeSampleTimes(SimStruct *S)
{
    double *stepSizeP = reinterpret_cast<double *>(mxGetData(ssGetSFcnParam(S,STEP_SIZE_P)));
    
    ssSetSampleTime(S, 0, *stepSizeP);
    ssSetOffsetTime(S, 0, 0.0);
    ssSetModelReferenceSampleTimeDefaultInheritance(S);
}

#define MDL_SET_WORK_WIDTHS
#if defined(MDL_SET_WORK_WIDTHS) && defined(MATLAB_MEX_FILE)
static void mdlSetWorkWidths(SimStruct *S)
{
    ssSetNumPWork(S, 1);
    ssSetNumDWork(S, 0);
}
#endif

#define GET_ZM_PTR(S) ssGetPWorkValue(S,0)

#define MDL_SETUP_RUNTIME_RESOURCES
// Called at the beginning of one or multiple simulations.
void mdlSetupRuntimeResources(SimStruct *S)
{
    std::cout << "PSSE Communication: Connecting to the pipe" << std::endl;
    ssSetPWorkValue(S, 0, setupruntimeresources_wrapper());
}

static void mdlOutputs(SimStruct *S, int_T tid)
{
    double   *y_ptr = reinterpret_cast<double *>(ssGetOutputPortSignal(S,0));
    read_inputs_helper(GET_ZM_PTR(S), y_ptr);
}

#define MDL_UPDATE
static void mdlUpdate(SimStruct *S, int_T tid)
{
    const double   *u_ptr    = reinterpret_cast<const double *>(ssGetInputPortSignal(S,0));
    write_outputs_helper(GET_ZM_PTR(S), u_ptr);                  
}

#define MDL_CLEANUP_RUNTIME_RESOURCES
// Called at the end of one or multiple simulations. This method is NOT called
// at the end of each subsequent Fast Restart.
static void mdlCleanupRuntimeResources(SimStruct *S)
{
    std::cout << "PSSE Communication: Closing the pipe" << std::endl;
    cleanupruntimeresouces_wrapper(GET_ZM_PTR(S));
}

// Called at the end of every simulation. Called at the end of every Fast Restart.
static void mdlTerminate(SimStruct *S)
{
    // No need to do anything here. The pipe will be closed in mdlCleanupRuntimeResources
}

#ifdef  MATLAB_MEX_FILE    // Is this file being compiled as a MEX-file?
#include "simulink.c"      // MEX-file interface mechanism
#else
#include "cg_sfun.h"       // Code generation registration function
#endif
