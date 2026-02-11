#include "autons.hpp"
#include "main.h"
#include "pros/rtos.hpp"
#include "subsystems.hpp"

/////
// For installation, upgrading, documentations, and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////

///
// Turn Example
///

void Match_Program_Left() {
    
    DirectoryMotor.move_relative(-500, 80);
 pros::delay(100);
 
 leftMotors.move(50);
 rightMotors.move(50);
 FrontBottomMotor.move(90);
 BottomBackMotor.move(  40);
 MiddleBackMotor.move(80);
 TopFrontMotor.move(70);
 TopChanelMotor.move(-100);
 pros::delay(1230);

 leftMotors.move(50);
 rightMotors.move(-50);
pros::delay(350);

 leftMotors.move(50);
 rightMotors.move(50);
pros::delay(250);

leftMotors.brake();
 rightMotors.brake();
DirectoryMotor.move_relative(500, 80);
pros::delay(150);

FrontBottomMotor.move(70);
BottomBackMotor.move(-40);
MiddleBackMotor.move(60);
TopFrontMotor.move(-60);
TopChanelMotor.move(-80);   
pros::delay(2900);

leftMotors.brake();
rightMotors.brake();
pros::delay(1700);

leftMotors.move(-50);
rightMotors.move(-50);
pros::delay(1500);

leftMotors.brake();
 rightMotors.brake();
 FrontBottomMotor.brake();
 BottomBackMotor.brake();
 MiddleBackMotor.brake();
 TopFrontMotor.brake();
 TopChanelMotor.brake();

}

void Match_Left_2 (){

leftMotors.move(50);
rightMotors.move(50);
FrontBottomMotor.move(90);
BottomBackMotor.move(  40);
MiddleBackMotor.move(80);
TopFrontMotor.move(70);
TopChanelMotor.move(-100);
pros::delay(500);

leftMotors.move(-50);
rightMotors.move(50);
pros::delay(100);

leftMotors.move(50);
 rightMotors.move(50);
pros::delay(100);

leftMotors.move(50);
rightMotors.move(-50);
pros::delay(100);

leftMotors.move(50);
rightMotors.move(50);
pros::delay(660);

leftMotors.move(50);
 rightMotors.move(-50);
pros::delay(400);

 leftMotors.move(50);
 rightMotors.move(50);
pros::delay(325);

leftMotors.brake();
 rightMotors.brake();
DirectoryMotor.move_relative(500, 80);
pros::delay(150);

FrontBottomMotor.move(70);
BottomBackMotor.move(-40);
MiddleBackMotor.move(60);
TopFrontMotor.move(-60);
TopChanelMotor.move(-80);   
pros::delay(2900);

leftMotors.brake();
rightMotors.brake();
pros::delay(1700);

leftMotors.move(-50);
rightMotors.move(-50);
pros::delay(1500);

leftMotors.brake();
 rightMotors.brake();
 FrontBottomMotor.brake();
 BottomBackMotor.brake();
 MiddleBackMotor.brake();
 TopFrontMotor.brake();
 TopChanelMotor.brake();
}


void Match_Right_2 () {
DirectoryMotor.move_relative(-500, 80);
 pros::delay(100);

    leftMotors.move(50);
rightMotors.move(50);
FrontBottomMotor.move(90);
BottomBackMotor.move(  40);
MiddleBackMotor.move(80);
TopFrontMotor.move(70);
TopChanelMotor.move(-100);
pros::delay(500);

leftMotors.move(50);
rightMotors.move(-50);
pros::delay(100);

leftMotors.move(50);
 rightMotors.move(50);
pros::delay(100);

leftMotors.move(-50);
rightMotors.move(50);
pros::delay(100);

leftMotors.move(50);
rightMotors.move(50);
pros::delay(660);

leftMotors.move(-50);
 rightMotors.move(50);
pros::delay(400);

 leftMotors.move(50);
 rightMotors.move(50);
pros::delay(325);

leftMotors.brake();
 rightMotors.brake();
DirectoryMotor.move_relative(500, 80);
pros::delay(150);

FrontBottomMotor.move(-60);
BottomBackMotor.move(-60);
MiddleBackMotor.move(70);
TopFrontMotor.move(-100);  
pros::delay(2900);

leftMotors.brake();
rightMotors.brake();
pros::delay(1700);

leftMotors.move(-50);
rightMotors.move(-50);
pros::delay(1500);

leftMotors.brake();
 rightMotors.brake();
 FrontBottomMotor.brake();
 BottomBackMotor.brake();
 MiddleBackMotor.brake();
 TopFrontMotor.brake();
 TopChanelMotor.brake();
}

///
// 
///
void Match_Program_Right() {
 DirectoryMotor.move_relative(-500, 80);
 pros::delay(100);
 
 leftMotors.move(50);
 rightMotors.move(50);
 FrontBottomMotor.move(90);
 BottomBackMotor.move(  40);
 MiddleBackMotor.move(80);
 TopFrontMotor.move(70);
 TopChanelMotor.move(-100);
 pros::delay(1330);

 leftMotors.move(-50);
 rightMotors.move(50);
pros::delay(350);

 leftMotors.move(50);
 rightMotors.move(50);
pros::delay(75);

 leftMotors.brake();
 rightMotors.brake();
pros::delay(100);

leftMotors.brake();
 rightMotors.brake();
DirectoryMotor.move_relative(500, 80);
pros::delay(150);

FrontBottomMotor.move(-60);
BottomBackMotor.move(-60);
MiddleBackMotor.move(70);
TopFrontMotor.move(-100);
pros::delay(1900);

leftMotors.move(50);
rightMotors.move(50);
pros::delay(200);

leftMotors.brake();
rightMotors.brake();
pros::delay(1700);

leftMotors.move(-50);
rightMotors.move(-50);
pros::delay(1500);

leftMotors.brake();
 rightMotors.brake();
 FrontBottomMotor.brake();
 BottomBackMotor.brake();
 MiddleBackMotor.brake();
 TopFrontMotor.brake();
 TopChanelMotor.brake();

}

///
// 
void Skills() {
    leftMotors.move(-110);
    rightMotors.move(-110);
    FrontBottomMotor.move(80);
    BottomBackMotor.move(  30);
    MiddleBackMotor.move(80);
    TopFrontMotor.move(70);
    TopChanelMotor.move(-100);
    pros::delay(1050);

    leftMotors.brake();
    rightMotors.brake();
    FrontBottomMotor.brake();
    BottomBackMotor.brake();
    MiddleBackMotor.brake();
    TopFrontMotor.brake();
    TopChanelMotor.brake();

    //if time then get it to clear both parking zones
    //If time later in season then get it to empty the side tube  
}

void WorkInProgressSkills() {
// set flapper
DirectoryMotor.move_relative(-500, 80);
 pros::delay(100);
 
 //drive forward to ceter troughs
 leftMotors.move(50);
 rightMotors.move(50);
 FrontBottomMotor.move(90);
 BottomBackMotor.move(40);
 MiddleBackMotor.move(80);
 TopFrontMotor.move(70);
 TopChanelMotor.move(-100);
 pros::delay(1330);
//turn
 leftMotors.move(-50);
 rightMotors.move(50);
pros::delay(350);


 leftMotors.move(50);
 rightMotors.move(50);
pros::delay(75);

 leftMotors.brake();
 rightMotors.brake();
pros::delay(100);

leftMotors.brake();
 rightMotors.brake();
DirectoryMotor.move_relative(500, 80);
pros::delay(150);

FrontBottomMotor.move(-60);
BottomBackMotor.move(-60);
MiddleBackMotor.move(70);
TopFrontMotor.move(-100);
pros::delay(1900);

leftMotors.move(50);
rightMotors.move(50);
pros::delay(200);

leftMotors.move(-50);
rightMotors.move(-50);
pros::delay(300);

leftMotors.move(50);
 rightMotors.move(-50);
pros::delay(200);

leftMotors.move(-60);
rightMotors.move(-60);
pros::delay(1400);

leftMotors.move(50);
 rightMotors.move(-50);
pros::delay(750);

leftMotors.move(-40);
rightMotors.move(-40);
pros::delay(1200);

leftMotors.move(-110);
rightMotors.move(-110);
pros::delay(1050);

leftMotors.brake();
rightMotors.brake();

}
