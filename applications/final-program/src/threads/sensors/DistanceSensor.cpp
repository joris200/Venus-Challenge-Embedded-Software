#include "DistanceSensor.hpp"

void DistanceSensor::start(boost::lockfree::queue<std::function<void()> *> *managerToSensorQueue, boost::lockfree::queue<std::function<void()> *> *sensorToManagerQueue)
{
    running_ = true;
    thread_ = std::thread(&DistanceSensor::readData, this);
}

void DistanceSensor::stop()
{
    running_ = false;
    if (thread_.joinable()) {
        thread_.join();
    }
}

void DistanceSensor::readData() {
    while (running_) {
        // int distance = readDistance(); // Calls private helper function
        // sendData(distance);
        std::cout << "Reading distance data..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate reading every second
    }
}

// int DistanceSensor::readDistance() {
//     // Simulated function to read distance data
//     // Replace with actual code to read from sensor
//     static int distance = 0;
//     distance += 10; // Incrementing distance for demonstration
//     return distance;
// }
