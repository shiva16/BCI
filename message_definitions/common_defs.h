//////////////////////////////////////////////////////////////////////////////////////////////////
//
// common_defs.h - Common definitions that are shared between programs in the DK project
//
// Meel Velliste
// Aug 27, 2008

#ifndef _COMMON_DEFS_H_
#define _COMMON_DEFS_H_



// Utility functions for dealing with bitmasks
#define check_flag_bits( A, bitmask)  (((A) & (bitmask)) == (bitmask))
#define clear_flag_bits( A, bitmask)  ((A) &= ~(bitmask))
#define set_flag_bits( A, bitmask)    ((A) |= (bitmask))

// Header for all samples (i.e. spike count, EM output, robot command, robot feedback).
// Header for each sample is generated by the spike count module, and then propagated
// through the chain of processing.

typedef struct {
	int SerialNo;
	int Flags;
	double DeltaTime;
} SAMPLE_HEADER;
//Flags
#define SF_ALIGNMENT         0x01 // Signifies that this is an alignment sample (i.e. one where the low-frequency alignment clock pulsed). This is used for calculating system delay.
#define SF_UNFREEZE          0x02 // Signifies that this is the first sample after EM has unfrozen
#define SF_IRREGULAR         0x04 // Signifies that there was in irregularity in the generation of this sample (missing RZ2 packets)
#define SF_FRACTINT          0x08 // Signifies that this sample is based on a fractional interval count (as opposed to regular spike count)


// Data format for all movement commands that go to the arbitrator that controls input to the robot
#define MAX_CONTROL_DIMS   18
#ifndef MAX_EM_DIMS
#define MAX_EM_DIMS MAX_CONTROL_DIMS
#endif

#define TAG_LENGTH 64
#define NUM_D_COLS 16
#define MAX_D_COLS 32


typedef struct {
  SAMPLE_HEADER sample_header;
  double sample_interval;
  double pos[MAX_CONTROL_DIMS];
  double vel[MAX_CONTROL_DIMS];
  int controlledDims[MAX_CONTROL_DIMS]; 
  char tag[TAG_LENGTH];
} MOVEMENT_COMMAND_DATA;

#define MAX_INPUT_DOFS 21

typedef struct {
  SAMPLE_HEADER sample_header;
  char tag[TAG_LENGTH];
  double dof_vals[MAX_INPUT_DOFS];
} INPUT_DOF_DATA;

// sent to graspy stuff  in command config 
typedef struct {
  double event_time; // RTMA AbsTime when event occurred
  int event_code;    // One of the TE_ codes below
  int reserved;
} TRIAL_EVENT;
// Trial event codes

typedef struct {
  double D[MAX_CONTROL_DIMS * MAX_D_COLS];
} PLAN_PROCESSOR_STATE;

typedef struct {
  double virtualPadPosition[MAX_CONTROL_DIMS];
  double centralPointPosition[3];
  double posMin[MAX_CONTROL_DIMS];
  double posMax[MAX_CONTROL_DIMS];
  double velMin[MAX_CONTROL_DIMS];
  double velMax[MAX_CONTROL_DIMS];
  double graspingGraspApertureTarget;
  double graspingOpenApertureTarget;  
  double graspingIntermediateApertureTarget;
  double maxApproachSwingAngle;
  double graspTooCloseIfClosedRadius;
} AUTOMAGIC_CONTROLLER_PARAMS;

typedef struct { 
  double target[MAX_CONTROL_DIMS];
  double coriMatrix[9];
  //TASK_SPACE_TARGET target;
  TRIAL_EVENT trialEvent;
  AUTOMAGIC_CONTROLLER_PARAMS autoControlParams;
  double autoPosControlFraction[MAX_CONTROL_DIMS];
  double autoVelControlFraction[MAX_CONTROL_DIMS];
  double orthPosImpedance[MAX_CONTROL_DIMS];
  double orthVelImpedance[MAX_CONTROL_DIMS];
  int extrinsicVelControlled[MAX_CONTROL_DIMS];
  int extrinsicPosControlled[MAX_CONTROL_DIMS];
  int importantDOF[MAX_CONTROL_DIMS]; // MCD must be even
  //GRASP_EVENT_THRESHOLDS graspEventThresholds;
} ROBOT_CONTROL_CONFIG;
 
// particular to WAM with Barrett Hand
typedef struct {
  SAMPLE_HEADER sample_header;
  double RTMA_received_time;
  double Jpos[7];
  double Jvel[7];
  double Cpos[3];
  double Cori[3];
  double Cforce[3];
  double Ctrq[3];  
  double Cvel[3];
  double CangVel[3];
  double Hpos[4];
  double Hstrain[4];  /// saving velocity here!!! change sometime!
  double Jtrq[3];
} ROBOT_ACTUAL_STATE;

typedef struct {
  SAMPLE_HEADER sample_header;
  double RTMA_received_time;
  double pos[MAX_CONTROL_DIMS];
  double vel[MAX_CONTROL_DIMS];
  double CoriMatrix[9];
} ROBOT_CONTROL_SPACE_ACTUAL_STATE;

typedef struct {
  SAMPLE_HEADER sample_header;
  double targetPos[MAX_CONTROL_DIMS];
} PLANNER_STATE;

typedef struct {
  double cmd[11];
} JVEL_COMMAND;

typedef struct {
  double DOFCommand[MAX_CONTROL_DIMS];
  double JposCommand[7];
  int controllerId;  
  int j0nks;
} OVERRIDE_COMMAND;

// put MCD in there to get the norm2real ratio in there
typedef struct {
  SAMPLE_HEADER sample_header;
  PLANNER_STATE autoControl;
  OVERRIDE_COMMAND overrideCommand;
  double RTMA_received_time;
  double actualControlPos[MAX_CONTROL_DIMS]; //should be MOVEE  ENT_COMMAND_DATA 
  double actualControlVel[MAX_CONTROL_DIMS];// except that has extra headers
  double autoControlPos[MAX_CONTROL_DIMS];
  double autoControlVel[MAX_CONTROL_DIMS];
  double autoPosControlFraction[MAX_CONTROL_DIMS];
  double autoVelControlFraction[MAX_CONTROL_DIMS];
  double orthPosImpedance[MAX_CONTROL_DIMS];
  double orthVelImpedance[MAX_CONTROL_DIMS];
  int intrinsicVelControlled[MAX_CONTROL_DIMS];
  int intrinsicPosControlled[MAX_CONTROL_DIMS];
  int extrinsicVelControlled[MAX_CONTROL_DIMS];
  int extrinsicPosControlled[MAX_CONTROL_DIMS];
  int overrideDimensions[MAX_CONTROL_DIMS];
  unsigned int actualCommandComposition;
  int blank;
} ROBOT_CONTROL_STATE;

#endif //#ifndef _COMMON_DEFS_H_