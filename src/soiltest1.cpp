
   // posture[0] = {1.76843, 2.61420, -1.62358, 0.52477, -0.00504, 0.00137}; // Define the posture for opening gripper
   
   // posture[1] = {1.77852, 2.74734, -1.63973, 0.52379, -0.00527, 0.00136}; // Define the posture for closing gripper   

#include "unitree_arm_sdk/control/unitreeArm.h"
#include <iostream>
#include <iomanip>
#include <string>


using namespace UNITREE_ARM;


class Z1ARM : public unitreeArm{
public:
    Z1ARM():unitreeArm(true){};
    ~Z1ARM(){};
    void armCtrlByFSM();
    void armCtrlInJointCtrl();
    void armCtrlInCartesian();
    void printState();
    
    

    double gripper_pos = 0.0;
    double joint_speed = 2.0;
    double cartesian_speed = 0.5;
};


int main() {
    std::cout << std::fixed << std::setprecision(3);

    // Initialize the arm
    unitreeArm arm(true);
    arm.sendRecvThread->start();

    // Move the arm to the start position
    //arm.backToStart();
   
    // Execute the trajectory from the CSV file named "Traj_soil4.csv"
    std::string label = "soil4";
    arm.teachRepeat(label);
   
   sleep(1);
    // Execute the trajectory from the CSV file named "Traj_gripper1.csv"
    label = "soil7";
    arm.teachRepeat(label);

    sleep(1);
    
    // Move the arm back to the start position
    arm.backToStart();

    // Set the arm to passive state and shut down the communication thread
    arm.setFsm(ArmFSMState::PASSIVE);
    arm.sendRecvThread->shutdown();

    return 0;
}

