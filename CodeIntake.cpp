float CaoThap;
#include "main.h"

// Controller
pros::Controller Controller1(pros::E_CONTROLLER_MASTER);

// Distance Sensor
pros::Distance Distance1(1);

// Intake Motor
pros::Motor Intake(2);

// Pneumatic - 3-wire port A
pros::adi::DigitalOut PistonNangIntake('A');

// Pneumatic - 3-wire port B
pros::adi::DigitalOut PistonNangQuaySau('B');

event HutCao = event();
event HutThap = event();

// "when started" hat block
int whenStarted1() {
  CaoThap = 0.0;
  PistonNangIntake.set(false);
  while (true) {
    if (Controller1.ButtonL1.pressing()) {
      if (fmod(CaoThap,2.0) == 0.0) {
        HutCao.broadcast();
      }
      else {
        HutThap.broadcast();
      }
      CaoThap = CaoThap + 1.0;
      wait(0.1, seconds);
    }
  wait(5, msec);
  }
  return 0;
}

// "when I receive HutCao" hat block
void onevent_HutCao_0() {
  PistonNangIntake.set(true);
  PistonNangQuaySau.set(true);
  while ((fmod(CaoThap,2.0) == 0.0)) {
    if (Controller1.ButtonR1.pressing()) {
      Intake.spin(forward);
    }
    else {
      if (Controller1.ButtonR2.pressing()) {
        Intake.spin(reverse);
      }
      else {
        Intake.stop();
      }
    }
    if (Distance1.isObjectDetected()) {
      PistonNangQuaySau.set(false);
    }
    else {
      PistonNangQuaySau.set(true);
    }
  wait(5, msec);
  }
}

// "when I receive HutThap" hat block
void onevent_HutThap_0() {
  PistonNangIntake.set(false);
  PistonNangQuaySau.set(true);
  while ((!(fmod(CaoThap,2.0) == 0.0))) {
    if (Controller1.ButtonR1.pressing()) {
      Intake.spin(forward);
    }
    else {
      if (Controller1.ButtonR2.pressing()) {
        Intake.spin(reverse);
      }
      else {
        Intake.stop();
      }
    }
  wait(5, msec);
  }
}


int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // register event handlers
  HutCao(onevent_HutCao_0);
  HutThap(onevent_HutThap_0);

  wait(15, msec);
  whenStarted1();
}