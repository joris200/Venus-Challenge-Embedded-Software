#include <iostream>
#include <boost/lockfree/queue.hpp> // Include Boost.Lockfree
#include <thread>
#include <cstdlib> // Include for EXIT_FAILURE and EXIT_SUCCESS
#include <memory> // Include for std::unique_ptr

#include "threads/StepperThread.hpp"
#include "threads/ComManagerThread.hpp"
#include "threads/SensorManager.hpp"
#include "threads/sensors/DistanceSensor.hpp"
#include "threads/sensors/ColorSensor.hpp"
#include "utils/structs.hpp"
// #include <libpynq.h>

/**
 * Maybe add a robot class to keep track of the robot's state and position
*/

// Define all queues here
SensorManagerQueue comToSensorQueue(100); // Create a queue to store pointers to s_StepperThread
SensorManagerQueue sensorToComQueue(100); // Create a queue to store pointers to s_StepperThread

StepperQueue comToStepperQueue(100); // Create a queue to store pointers to s_StepperThread
StepperQueue stepperToComQueue(100); // Create a queue to store pointers to s_StepperThread

int main(void) {
  // Initialize the Pynq library
  // pynq_init();

  // Create a queue to store pointers to s_StepperThread
  // QueueType comToSensorQueue(100); // Create a queue to store pointers to s_StepperThread
  // QueueType sensorToComQueue(100); // Create a queue to store pointers to s_StepperThread

  SensorManager sensorManager(comToSensorQueue, sensorToComQueue);

  DistanceSensor distanceSensor;
  ColorSensor colorSensor;

  sensorManager.addSensor(&distanceSensor);
  sensorManager.addSensor(&colorSensor);

  sensorManager.start();


  sensorManager.amountOfSensors();

  colorSensor.requestCapture();


  // sensorManager.joinSensors();

  // // Create a queue to store pointers to s_StepperThread
  // boost::lockfree::queue<s_StepperThread*> comThreadQueue(100); // Create a queue to store pointers to s_StepperThread
  // boost::lockfree::queue<s_StepperThread*> stepperThreadQueue(100); // Create a queue to store pointers to s_StepperThread

  // Create a thread to handle the stepper motor
  Stepper stepperThread(comToStepperQueue, stepperToComQueue);
  stepperThread.start();

  // Create a thread to handle the communication manager
  CommunicationManager comManager(comToStepperQueue, stepperToComQueue, comToSensorQueue, sensorToComQueue);
  comManager.start();

  // join the threads

  std::this_thread::sleep_for(std::chrono::seconds(10));

  sensorManager.stopSensorThreads();
  sensorManager.stop();

  sensorManager.join();

  std::cout << "Stopping com and stepper thread!" << std::endl;
  comManager.stop();
  stepperThread.stop();
  stepperThread.join();
  comManager.joinThreads();

  std::cout << "Ending!!" << std::endl;


  // std::thread comManagerThread(Com, std::ref(threadQueue));

 

  // Clean up the Pynq library resources
  // pynq_destroy();

  // Return a success status code
  return EXIT_SUCCESS;
}
