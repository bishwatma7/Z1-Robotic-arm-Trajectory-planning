#include "unitree_arm_sdk/control/unitreeArm.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace UNITREE_ARM;
using namespace std::chrono;

class Z1ARM : public unitreeArm {
public:
    Z1ARM() : unitreeArm(true), startTime(high_resolution_clock::now()) {};
    ~Z1ARM() {};

    void armCtrlByFSM();
    void armCtrlInJointCtrl();
    void armCtrlInCartesian();
    void printState();

    void RecordOperationTime();
    void RecordNetworkPerformance();

    time_point<high_resolution_clock> startTime;
    time_point<high_resolution_clock> endTime;
    std::vector<double> loopDurations; // Store individual loop durations

    double gripper_pos = 0.0;
    double joint_speed = 2.0;
    double cartesian_speed = 0.5;

    double totalBytesSent = 0.0; // To track total data sent in bytes
    double totalBytesReceived = 0.0; // To track total data received in bytes
    double totalLatency = 0.0; // Total latency for latency measurements
    int latencySamples = 0; // Number of latency samples taken

    void measureLatencyAndBandwidth();
};

void Z1ARM::measureLatencyAndBandwidth() {
    // Create a simple UDP socket for latency measurement
    int sockfd;
    struct sockaddr_in serverAddr;
    char message[] = "ping";
    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Failed to create socket." << std::endl;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8082);
    inet_pton(AF_INET, "192.168.123.220", &serverAddr.sin_addr);

    // Sending packet to measure latency
    auto sendStart = high_resolution_clock::now();
    sendto(sockfd, message, strlen(message), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    socklen_t addrLen = sizeof(serverAddr);

    ssize_t recvLen = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddr, &addrLen);
    auto sendEnd = high_resolution_clock::now();

    if (recvLen > 0) {
        double latency = duration_cast<microseconds>(sendEnd - sendStart).count() / 1000.0;
        totalLatency += latency;
        latencySamples++;
    }

    totalBytesSent += strlen(message);
    totalBytesReceived += recvLen;

    close(sockfd);
}

int main() {
    std::cout << std::fixed << std::setprecision(3);

    // Initialize the arm
    Z1ARM arm;
    arm.sendRecvThread->start();

    // Record the start time
    arm.startTime = high_resolution_clock::now();

    // Move the arm to the start position
    auto loopStart = high_resolution_clock::now();
    arm.backToStart();
    auto loopEnd = high_resolution_clock::now();
    arm.loopDurations.push_back(duration_cast<milliseconds>(loopEnd - loopStart).count());

    arm.measureLatencyAndBandwidth(); // Measure latency and bandwidth during operation

    // Execute the trajectory from the CSV file named "Traj_hello1.csv"
    std::string label = "hello1";
    loopStart = high_resolution_clock::now();
    arm.teachRepeat(label);
    loopEnd = high_resolution_clock::now();
    arm.loopDurations.push_back(duration_cast<milliseconds>(loopEnd - loopStart).count());

    arm.measureLatencyAndBandwidth(); // Measure latency and bandwidth during operation

    sleep(1);

    // Move the arm back to the start position
    loopStart = high_resolution_clock::now();
    arm.backToStart();
    loopEnd = high_resolution_clock::now();
    arm.loopDurations.push_back(duration_cast<milliseconds>(loopEnd - loopStart).count());

    // Record the end time
    arm.endTime = high_resolution_clock::now();

    arm.measureLatencyAndBandwidth(); // Measure latency and bandwidth during operation

    // Set the arm to passive state and shut down the communication thread
    arm.setFsm(ArmFSMState::PASSIVE);
    arm.sendRecvThread->shutdown();

    // Record and output the total operation time after the arm completes the task
    arm.RecordOperationTime();
    arm.RecordNetworkPerformance(); // Output network performance metrics

    return 0;
}

void Z1ARM::RecordOperationTime() {
    // Calculate and print total operation time
    double totalOperationTime = duration_cast<milliseconds>(endTime - startTime).count() / 1000.0;
    std::cout << "Total operation time: " << totalOperationTime << " seconds" << std::endl;

    // Calculate average loop duration
    double totalLoopDuration = 0.0;
    for (const auto& duration : loopDurations) {
        totalLoopDuration += duration;
    }
    double averageLoopDuration = loopDurations.empty() ? 0.0 : totalLoopDuration / loopDurations.size();
    std::cout << "Average loop duration: " << averageLoopDuration << " milliseconds" << std::endl;

    // Number of loops performed
    std::cout << "Total loops performed: " << loopDurations.size() << std::endl;
}

void Z1ARM::RecordNetworkPerformance() {
    // Calculate and print average latency
    double averageLatency = latencySamples == 0 ? 0.0 : totalLatency / latencySamples;
    std::cout << "Average latency: " << averageLatency << " milliseconds" << std::endl;

    // Calculate and print total bytes sent and received
    std::cout << "Total data sent: " << totalBytesSent << " bytes" << std::endl;
    std::cout << "Total data received: " << totalBytesReceived << " bytes" << std::endl;

    // Calculate and print bandwidth
    double totalOperationTime = duration_cast<milliseconds>(endTime - startTime).count() / 1000.0; // in seconds
    double bandwidth = totalOperationTime == 0.0 ? 0.0 : (totalBytesSent + totalBytesReceived) / totalOperationTime; // bytes per second
    std::cout << "Average bandwidth: " << bandwidth << " bytes per second" << std::endl;
}
