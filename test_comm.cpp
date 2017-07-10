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

  graComm::TagEngine engine1;
  std::fstream fs; /* Create a ModbusPkg from file */ 
  std::shared_ptr<graComm::ModbusPkg> pkg1(new graComm::ModbusPkg(fs, "ModbusConfig.txt"));
  std::shared_ptr<graComm::ModbusPkg> pkg2(new graComm::ModbusPkg(fs, "ModbusConfig2.txt"));
  
  engine1.addPkg(pkg1); /* Add ModbusPkg to the TagEngine */
  engine1.addPkg(pkg2);

  /* Create servers required by TagEngine ModbusPkgs */
  std::vector<graComm::ModbusCommunication> servers;
  
  std::vector<std::thread *> serverThreads;
  for(auto s : servers) {
    serverThreads.push_back(new std::thread(&graComm::ModbusCommunication::run, &s, &running)); 
  } 

  std::thread te1(&graComm::TagEngine::run, &engine1, &running); /* Start the TagEngine */

  te1.join();
  
  for(auto thread : serverThreads) {
    thread->join();
  }

  for(auto server : servers) {
    server.close();
  }

  return 0;
}
