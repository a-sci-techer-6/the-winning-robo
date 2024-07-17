#include "vex.h"
#include "robot-config.h"

using namespace vex;

#define V_PRINTF(...) Brain.Screen.print(__VA_ARGS__)

void onAutonomous()
{
  Brain.Screen.print("autonomous\n");
}

void onDriverControl()
{
  Brain.Screen.print("driver control\n");
}

void spin_up() {
  ClawShaftMotor.spin(forward);
}
void spin_down() {
  ClawShaftMotor.spin(reverse);
}

int main()
{
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  V_PRINTF("started\n");

  //Drivetrain.drive(forward);

  ClawMotor.spin(forward);

  ClawShaftMotor.setMaxTorque(100, percent);

  ClawShaftMotor.spin(forward);
  V_PRINTF("go?\n");

  ClawUpBumper.pressed(spin_down);
  ClawDownBumper.pressed(spin_up);

  //Competition.autonomous(onAutonomous);
  //Competition.drivercontrol(onDriverControl);
}
