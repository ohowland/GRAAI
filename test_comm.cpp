#include "comm.hpp"
#include "TagEngine.hpp"
#include "ModbusPkg.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <thread>
#include <signal.h>
#include <unistd.h>

bool running = true;
void stopHandler(int) {
  std::cout << "***STOPPING***" << std::endl;
  running = false;
}

int main() {
  signal(SIGINT, stopHandler);
  signal(SIGTERM, stopHandler);

  /* Create and start the ModbusCommunications loop */
  graComm::ModbusCommunication comm1;
  comm1.open();
  std::thread t2(&graComm::ModbusCommunication::run, &comm1, &running);

  graComm::TagEngine engine1(comm1.commQueue()); /* Create the TagEngine */
 
  std::fstream fs; /* Create a ModbusPkg from file */ 
  std::shared_ptr<graComm::ModbusPkg> pkg1(new graComm::ModbusPkg(fs, "ModbusConfig.txt"));

  engine1.addPkg(pkg1); /* Add ModbusPkg to the TagEngine */

  std::thread t1(&graComm::TagEngine::run, &engine1, &running); /* Start the TagEngine */

  t1.join();
  t2.join();

  comm1.close();

  return 0;
}
