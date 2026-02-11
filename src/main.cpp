#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "pros/adi.h"
#include "pros/misc.h"
#include "pros/rtos.hpp"

// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor groups
//pros::MotorGroup leftMotors({-9, -8},
//                            pros::MotorGearset::green); // left motor group - ports 3 (reversed), 4, 5 (reversed)

pros::Imu imu(3);

pros::ADIDigitalOut doinker('G');

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

// Chassis constructor
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize() {
  ez::ez_template_print();
    

  pros::delay(500);  // Stop the user from doing anything while legacy ports configure
  chassis.calibrate(); // calibrate sensors

  // Look at your horizontal tracking wheel and decide if it's in front of the midline of your robot or behind it
  //  - change `back` to `front` if the tracking wheel is in front of the midline
  //  - ignore this if you aren't using a horizontal tracker
  // chassis.odom_tracker_back_set(&horiz_tracker);
  // Look at your vertical tracking wheel and decide if it's to the left or right of the center of the robot
  //  - change `left` to `right` if the tracking wheel is to the right of the centerline
  //  - ignore this if you aren't using a vertical tracker
  // chassis.odom_tracker_left_set(&vert_tracker);

  // Configure your chassis controls
  
  // These are already defaulted to these buttons, but you can change the left/right curve buttons here!
  // chassis.opcontrol_curve_buttons_left_set(pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT);  // If using tank, only the left side is used.
  // chassis.opcontrol_curve_buttons_right_set(pros::E_CONTROLLER_DIGITAL_Y, pros::E_CONTROLLER_DIGITAL_A);

  // Autonomous Selector using LLEMU
  ez::as::auton_selector.autons_add({
      {"Match Right\n\nRight side of drivers perspective", Match_Program_Right}, 
      {"Match Left\n\nLeft side of drivers perspective.", Match_Program_Left},
      {"Skills\n\nRun on skills program only.", Skills},
      {"Match skills work in progress, If working at comp then run. set up like match right", WorkInProgressSkills},
      {"Left side work in progress", Match_Left_2}, 
      {"Match side left three blocks.", Match_Right_2},
  });

  // Initialize chassis and auton selector
  ez::as::initialize();
  //master.rumble(chassis.drive_imu_calibrated() ? "." : "---");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
  // . . .
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
  // . . .
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {

  ez::as::auton_selector.selected_auton_call();  // Calls selected auton from autonomous selector
}

/**
 * Simplifies printing tracker values to the brain screen
 */
void screen_print_tracker(ez::tracking_wheel *tracker, std::string name, int line) {
  std::string tracker_value = "", tracker_width = "";
  // Check if the tracker exists
  if (tracker != nullptr) {
    tracker_value = name + " tracker: " + util::to_string_with_precision(tracker->get());             // Make text for the tracker value
    tracker_width = "  width: " + util::to_string_with_precision(tracker->distance_to_center_get());  // Make text for the distance to center
  }
  ez::screen_print(tracker_value + tracker_width, line);  // Print final tracker text
}

/**
 * Ez screen task
 * Adding new pages here will let you view them during user control or autonomous
 * and will help you debug problems you're having
 */
void ez_screen_task() {
  while (true) {
    // Only run this when not connected to a competition switch
    if (!pros::competition::is_connected()) {
      // Blank page for odom debugging
      //if (chassis.odom_enabled() && !chassis.pid_tuner_enabled()) {
        // If we're on the first blank page...
      //  if (ez::as::page_blank_is_on(0)) {
          // Display X, Y, and Theta
          //ez::screen_print("x: " + util::to_string_with_precision(chassis.odom_x_get()) +
          //                     "\ny: " + util::to_string_with_precision(chassis.odom_y_get()) +
          //                     "\na: " + util::to_string_with_precision(chassis.odom_theta_get()),
          //                 1);  // Don't override the top Page line

          // Display all trackers that are being used
          //screen_print_tracker(chassis.odom_tracker_left, "l", 4);
          //screen_print_tracker(chassis.odom_tracker_right, "r", 5);
          //screen_print_tracker(chassis.odom_tracker_back, "b", 6);
          //screen_print_tracker(chassis.odom_tracker_front, "f", 7);
      //  }
      //}
    }

    // Remove all blank pages when connected to a comp switch
    else {
      if (ez::as::page_blank_amount() > 0)
        ez::as::page_blank_remove_all();
    }

    pros::delay(ez::util::DELAY_TIME);
  }
}
pros::Task ezScreenTask(ez_screen_task);

/**
 * Gives you some extras to run in your opcontrol:
 * - run your autonomous routine in opcontrol by pressing DOWN and B
 *   - to prevent this from accidentally happening at a competition, this
 *     is only enabled when you're not connected to competition control.
 * - gives you a GUI to change your PID values live by pressing X
 */
void ez_template_extras() {
  // Only run this when not connected to a competition switch
  if (!pros::competition::is_connected()) {
    // PID Tuner
    // - after you find values that you're happy with, you'll have to set them in auton.cpp

    // Enable / Disable PID Tuner
    //  When enabled:
    //  * use A and Y to increment / decrement the constants

  }

  // Disable PID Tuner when connected to a comp switch
  else {

  }
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  // This is preference to what you like to drive on
  // controller
    // loop to continuously update motors
    while (true) {
        // get joystick positions
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int leftX = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

        if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)){
            //motors 10, 11, 12, 20 = R, R, R, R goes out and into the side tubes.
            FrontBottomMotor.move(80);
            BottomBackMotor.move(60);
            MiddleBackMotor.move(60);
            TopFrontMotor.move(60);
            TopChanelMotor.move(80);

        } else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
            //motors 10, 11, 12, 20 = F, F, F, F. goes out of the robot by going down
            FrontBottomMotor.move(-90);
            BottomBackMotor.move(-90);
            MiddleBackMotor.move(-90);
            TopFrontMotor.move(-90);
            TopChanelMotor.move(-50);

        } else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)){
            //motors 10, 11, 12, 20 = F, R, R. goes up then into the central tube
            FrontBottomMotor.move(100);
            BottomBackMotor.move(100);
            MiddleBackMotor.move(100);
            TopFrontMotor.move(-120);
            TopChanelMotor.move(-50);

        } else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)){
            FrontBottomMotor.move(80);
            BottomBackMotor.move(80);
            MiddleBackMotor.move(80);
            TopFrontMotor.move(70);
            TopChanelMotor.move(-100);

        } else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)){
            FrontBottomMotor.move(-85);
            BottomBackMotor.move(-90);
            MiddleBackMotor.move(70);
            TopFrontMotor.move(-100);

        } else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)){
            FrontBottomMotor.move(100);
            BottomBackMotor.move(-20);
            MiddleBackMotor.move(100);
            TopFrontMotor.move(100);
            TopChanelMotor.move(110);

        } else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)){
            FrontBottomMotor.move(90);
            BottomBackMotor.move(-40);
            MiddleBackMotor.move(60);
            TopFrontMotor.move(-90);
            TopChanelMotor.move(-80);   

            } else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)){
            FrontBottomMotor.move(60);
            BottomBackMotor.move(-40);
            MiddleBackMotor.move(60);
            TopFrontMotor.move(80);
            TopChanelMotor.move(-100);   

        } else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
            DirectoryMotor.move_relative(500, 80);

        } else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
            DirectoryMotor.move_relative(-500, 80);

        } else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
            doinker.set_value(LOW);

        } else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
            doinker.set_value(HIGH);

        } else {
            // stops all motors when no buttons are pressed.
            FrontBottomMotor.move(0);
            BottomBackMotor.move(0);
            MiddleBackMotor.move(0);
            TopFrontMotor.move(0);
            TopChanelMotor.move(0);
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
