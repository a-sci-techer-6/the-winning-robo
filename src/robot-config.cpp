#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

competition Competition;

// VEXcode device constructors
motor LeftDriveSmart = motor(PORT20, ratio18_1, false);
motor RightDriveSmart = motor(PORT11, ratio18_1, true);
drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, 319.19, 295, 40, mm, 1);

motor ClawMotor = motor(PORT17, ratio18_1, true);

motor ClawShaftMotor = motor(PORT18, ratio18_1, false);
#define CLAW_GO_UP forward
#define CLAW_GO_DOWN reverse

bumper ClawUpBumper = bumper(Brain.ThreeWirePort.F);
bumper ClawDownBumper = bumper(Brain.ThreeWirePort.E);

// VEXcode generated functions

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}