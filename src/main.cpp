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

// FIXME: use correct ports
bumper TLBumper = bumper(Brain.ThreeWirePort.A);
bumper TRBumper = bumper(Brain.ThreeWirePort.B);
bumper BLBumper = bumper(Brain.ThreeWirePort.C);
bumper BRBumper = bumper(Brain.ThreeWirePort.D);

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

typedef enum START_SIDE {
  SIDE_LEFT,
  SIDE_RIGHT,
} START_SIDE;

typedef enum A_STATE {
  MOVE_OUT_START,
  ALIGN_MOBILE_GOAL,
  COLLECT_BLOCK,
  MOVE_TO_LOW_GOAL,
  STOP,
} A_STATE;

void autonomous(START_SIDE side) {
  A_STATE state = MOVE_OUT_START;

  motor opp_motor = side == SIDE_LEFT ? RightDriveSmart : LeftDriveSmart;
  motor same_motor = side == SIDE_LEFT ? LeftDriveSmart : RightDriveSmart;

  while(true) {
   switch(state) {
    case MOVE_OUT_START:
      Drivetrain.spin(forward);
      if(TRBumper.pressing() || TLBumper.pressing()) {
        V_PRINTF("aligning mobile goal");
        state = ALIGN_MOBILE_GOAL;
      }
      break;
    case ALIGN_MOBILE_GOAL:
      opp_motor.spin(forward, 100, percent); 
      // TODO: wait until we hit the wall--maybe light sensor?
      wait(2, sec);

      V_PRINTF("collecting block");

      state = COLLECT_BLOCK;

      ClawMotor.spin(CLAW_OPEN);
      ClawShaftMotor.spin(CLAW_GO_DOWN);

      break;
    case COLLECT_BLOCK_CLAW_DOWN:
      if(ClawDownBumper.pressing()) {
        state = MOVE_TO_LOW_GOAL;

        ClawMotor.spin(CLAW_CLOSE);
        wait(750, msec);
        ClawShaftMotor.spin(CLAW_GO_UP);

        V_PRINTF("moving to low goal");
      }
      break;
    case MOVE_TO_LOW_GOAL:
      opp_motor.spin(reverse, 100, percent);
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

  RightDriveSmart.setVelocity(50, percent);
  LeftDriveSmart.setVelocity(50, percent);

  ClawUpBumper.pressed(ClawShaftMotor.stop);
  ClawDownMotor.pressed(ClawShaftMotor.stop);

  Competition.autonomous(autonomous);
  Competition.drivercontrol(driver_control);
}
