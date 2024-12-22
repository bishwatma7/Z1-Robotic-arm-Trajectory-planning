#include "unitree_arm_sdk/control/unitreeArm.h"

using namespace UNITREE_ARM;

class Z1ARM : public unitreeArm{
public:
    Z1ARM():unitreeArm(true){};
    ~Z1ARM(){};
    
};

int main() {
    
    Z1ARM arm;
    arm.sendRecvThread->start();
    arm.backToStart();
    arm.setFsm(ArmFSMState::PASSIVE);
    arm.sendRecvThread->shutdown();
    
    return 0;
}