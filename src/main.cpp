#include "vex.h"
#include "robot-config.h"

using namespace vex;

void onAutonomous() {
  Brain.Screen.print("autonomous\n");
  while(true) {
    if()
  }
}

void onDriverControl() {
  Brain.Screen.print("driver control\n");
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  Drivetrain.drive(forward);

  Competition.autonomous(onAutonomous);
  Competition.drivercontrol(onDriverControl);
}
