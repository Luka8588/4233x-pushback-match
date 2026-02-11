#pragma once
#include "lemlib/chassis/chassis.hpp"
#include "EZ-Template/api.hpp"
#include "api.h"

//inline lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);

// Your motors, sensors, etc. should go here.  Below are examples

inline pros::MotorGroup leftMotors({-9, -8}, pros::MotorGearset::blue);
inline pros::MotorGroup rightMotors({16, 15}, pros::MotorGearset::blue); // right motor group - ports 6, 7, 9 (reversed)
inline pros::Motor BottomBackMotor(-10, pros::MotorGearset::blue); // under motor in bringing the balls into the scoreing channel - port 10
inline pros::Motor TopChanelMotor(-12, pros::MotorGearset::blue); // motors for channel - ports 20 
inline pros::Motor TopFrontMotor(11, pros::MotorGearset::blue); // motors for channel - port 12
inline pros::Motor MiddleBackMotor(-7, pros::MotorGearset::blue);
inline pros::Motor FrontBottomMotor(20, pros::MotorGearset::blue);
inline pros::Motor DirectoryMotor(13, pros::MotorGearset::red); // Directing motor in back that decides the spinner level. - port 13

// inline pros::Motor intake(1);
// inline pros::adi::DigitalIn limit_switch('A');
