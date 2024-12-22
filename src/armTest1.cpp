#include "unitree_arm_sdk/control/unitreeArm.h"

using namespace UNITREE_ARM;

class Z1ARM : public unitreeArm{
public:
    Z1ARM():unitreeArm(true){};
    ~Z1ARM(){};
    void armCtrlByFSM();
    void printState();
};

 
void Z1ARM::armCtrlByFSM() {
    Vec6 posture[1];
    std::cout << "[MOVEJ]" << std::endl;
    setFsm(ArmFSMState::JOINTCTRL);
    labelRun("show_left");
    
    sleep(1);

    posture[0]<<-1,-0.3,-0.4,0.45,0.3,0.4;
    MoveJ(posture[0], -M_PI/3.0, 1.0);

    //Gripper opens with the middle value -M_PI/3.0. 0 means closed.
   
    //These are from savedArmStates.csv:
    //labelRun("forward");
    //labelRun("show_left");
    //labelRun("show_mid");
    //labelRun("show_right");
    //labelRun("startFlat");
    

}

int main() {
    std::cout << std::fixed << std::setprecision(3);
    Z1ARM arm;
    arm.sendRecvThread->start();

    arm.backToStart();
    
    arm.armCtrlByFSM();
      
    
    arm.backToStart();
    arm.setFsm(ArmFSMState::PASSIVE);
    arm.sendRecvThread->shutdown();
    return 0;
}