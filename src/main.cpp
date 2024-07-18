#include "vex.h"

using namespace vex;

using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;
competition Competition;

controller Controller = controller(primary);

// VEXcode device constructors
motor LeftDriveSmart = motor(PORT20, ratio18_1, false);
motor RightDriveSmart = motor(PORT11, ratio18_1, true);
drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, 319.19, 290, 215, mm, 1);

motor ClawMotor = motor(PORT17, ratio18_1, true);
#define CLAW_CLOSE forward
#define CLAW_OPEN reverse
bool ClawClosed = false;

motor ClawShaftMotor = motor(PORT18, ratio18_1, false);
#define CLAW_GO_UP forward
#define CLAW_GO_DOWN reverse

bumper ClawUpBumper = bumper(Brain.ThreeWirePort.F);
bumper ClawDownBumper = bumper(Brain.ThreeWirePort.E);

#define V_PRINTF(...) Controller.Screen.print(__VA_ARGS__); \
                      Controller.Screen.newLine();


void toggle_claw() {
  V_PRINTF("toggling claw (ClawClosed = %d)", ClawClosed);
  ClawMotor.spin(ClawClosed ? CLAW_OPEN : CLAW_CLOSE);
  ClawClosed = !ClawClosed;
}

void driver_control() {
  uint32_t last_toggled_claw = 0;

  while(true) {
    uint32_t now = Brain.Timer.time();
    if(Controller.ButtonUp.pressing()) {
      V_PRINTF("moving claw up shaft");
      ClawShaftMotor.spin(CLAW_GO_UP);
    } else if (Controller.ButtonDown.pressing()) {
      V_PRINTF("moving claw down shaft");
      ClawShaftMotor.spin(CLAW_GO_DOWN);
    } else {
      ClawShaftMotor.stop();
    }

    if(Controller.ButtonA.pressing() && now - last_toggled_claw >= 1000) {
      last_toggled_claw = now;
      toggle_claw();
    }

    // not rotating, then we can drive using controller
  
    // read percent from controller axis
    int leftSpeed  = Controller.Axis3.position();
    int rightSpeed = Controller.Axis2.position();
    // deadband
    if( abs(leftSpeed)  < 10 ) leftSpeed  = 0;
    if( abs(rightSpeed) < 10 ) rightSpeed = 0;
  
    // send to motors
    LeftDriveSmart.spin( forward, leftSpeed, percent );
    RightDriveSmart.spin( forward, rightSpeed, percent );

    wait(5, msec);
  }
}

typedef enum A_STATE {
  MOVE_OUT_START,
  ALIGN_MOBILE_GOAL,
  COLLECT_BLOCK,
  SCORE_LOW_GOAL,
  STOP,
} A_STATE;

void autonomous() {
  A_STATE state = MOVE_OUT_START;
  while(true) {
   switch(state) {
    case MOVE_OUT_START:
      break;
    case ALIGN_MOBILE_GOAL:
      break;
    case COLLECT_BLOCK:
      break;
    case SCORE_LOW_GOAL:
      break;
    case STOP:
      Drivetrain.stop();
      ClawShaftMotor.stop();
      ClawMotor.stop();
      break;
    default:
      state = STOP;
   } 

    wait(5, msec);
  }
}

int main()
{
  V_PRINTF("started\n");

  ClawShaftMotor.setStopping(brake);
  ClawShaftMotor.setVelocity(100, percent);

  Competition.autonomous(autonomous);
  Competition.drivercontrol(driver_control);
}
