#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep

// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor groups
pros::MotorGroup leftMotors({-9, -8},
                            pros::MotorGearset::green); // left motor group - ports 3 (reversed), 4, 5 (reversed)
pros::MotorGroup rightMotors({16, 15}, pros::MotorGearset::green); // right motor group - ports 6, 7, 9 (reversed)
pros::MotorGroup ChannelMotors({-10, 20}, pros::MotorGearset::blue); // motors for channel - ports 10, 11 
pros::Motor topchanelmotor(-12, pros::MotorGearset::blue); // motors for channel - ports 20 
pros::Motor middleMotor(11, pros::MotorGearset::blue); // motors for channel - port 12

pros::ADIDigitalOut doinker('A');

// Inertial Sensor on port 10
pros::Imu imu(3);

// tracking wheels
// horizontal tracking wheel encoder. Rotation sensor, port 20, not reversed
//pros::Rotation horizontalEnc(20);
// vertical tracking wheel encoder. Rotation sensor, port 11, reversed
//pros::Rotation verticalEnc(-11);
// horizontal tracking wheel. 2.75" diameter, 5.75" offset, back of the robot (negative)
//lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_275, -5.75);
// vertical tracking wheel. 2.75" diameter, 2.5" offset, left of the robot (negative)
//lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_275, -2.5);

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              10, // 10 inch track width
                              lemlib::Omniwheel::NEW_275, // using new 275" omnis
                              360, // drivetrain rpm is 360
                              2 // horizontal drift is 2. If we had traction wheels, it would have been 8
);

// lateral motion controller
lemlib::ControllerSettings linearController(10, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            3, // derivative gain (kD)
                                            3, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            3, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            20 // maximum acceleration (slew)
);

// angular motion controller
lemlib::ControllerSettings angularController(2, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             10, // derivative gain (kD)
                                             3, // anti windup
                                             1, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             3, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             0 // maximum acceleration (slew)
);

// sensors for odometry
lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel
                            nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                            nullptr, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors

    // the default rate is 50. however, if you need to change the rate, you
    // can do the following.
    // lemlib::bufferedStdout().setRate(...);
    // If you use bluetooth or a wired connection, you will want to have a rate of 10ms

    // for more information on how the formatting for the loggers
    // works, refer to the fmtlib docs

    // thread to for brain screen and position logging
    pros::Task screenTask([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            // log position telemetry
            lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
            // delay to save resources
            pros::delay(50);
        }
    });
}

/**
 * Runs while the robot is disabled
 */
void disabled() {}

/**
 * runs after initialize if the robot is connected to field control
 */
void competition_initialize() {}

// get a path used for pure pursuit
// this needs to be put outside a function
ASSET(example_txt); // '.' replaced with "_" to make c++ happy

/**
 * Runs during auto
 *
 * This is an example autonomous routine which demonstrates a lot of the features LemLib has to offer
 */
void autonomous() {
    // Back up untill robot has 3 wheels hanging out of park zone
    leftMotors.move(100);
    rightMotors.move(100);
    ChannelMotors.move(40);
    middleMotor.move(30);
    pros::delay(1700);

    //drive fowards into the park zone to park
    leftMotors.move(-80);
    rightMotors.move(80);
    pros::delay(450);

    leftMotors.move(80);
    rightMotors.move(80);
    pros::delay(500);

    ChannelMotors.move(-80);
    middleMotor.move(-100);
    topchanelmotor.move(-100);
}

/**
 * Runs in driver control
 */
void opcontrol() {
    // controller
    // loop to continuously update motors
    bool pneumatic_state = false;
    while (true) {
        // get joystick positions
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int leftX = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
        if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)){
            //motors 10, 11, 12, 20 = R, R, R, R goes out and into the side tubes.
            middleMotor.move(60);
            ChannelMotors.move(50);
            topchanelmotor.move(100);
            } else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
            //motors 10, 11, 12, 20 = F, F, F, F. goes out of the robot by going down
          ChannelMotors.move(-90);
          middleMotor.move(-60);
          topchanelmotor.move(-100);
        } else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)){
         //motors 10, 11, 12, 20 = F, R, R. goes up then into the central tube
         topchanelmotor.move(100);
         middleMotor.move(-60);
         ChannelMotors.move(50);
        } else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)){
         //motors 10, 11, 12, 20 = F,F,R. Goes up the into the back container 
         topchanelmotor.move(-100);
         middleMotor.move(60);
         ChannelMotors.move(50);
         } 
         else {
            // stops all motors when no buttons are pressed.
            middleMotor.move(0);
            ChannelMotors.move(0);
            topchanelmotor.move(0);
        }
        if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
            pneumatic_state = !pneumatic_state;
            doinker.set_value(pneumatic_state);
        } 
        // move the chassis with curvature drive
        chassis.arcade(leftY, leftX);
        // delay to save resources
        pros::delay(5);
    }
}