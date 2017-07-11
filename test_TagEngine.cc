#include "ModbusServer.hh"
#include "TagEngine.hh"
#include "ModbusPkg.hh"

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
 
  /* Create new ModbusLib */
  std::shared_ptr<graComm::ModbusLib> lib1(new graComm::ModbusLib());
  std::shared_ptr<graComm::ModbusLib> lib2(new graComm::ModbusLib());
  lib1->addServer(std::shared_ptr<graComm::ModbusServer>(new graComm::ModbusServer("127.0.0.1", 1502)));
  lib2->addServer(std::shared_ptr<graComm::ModbusServer>(new graComm::ModbusServer("127.0.0.1", 1504)));

  std::fstream fs; /* Create a ModbusPkg from file */ 
  lib1->addPkg(std::shared_ptr<graComm::ModbusPkg>(new graComm::ModbusPkg(fs, "ModbusConfig.txt")));
  lib2->addPkg(std::shared_ptr<graComm::ModbusPkg>(new graComm::ModbusPkg(fs, "ModbusConfig.txt")));

  engine1.addLibrary(lib1); /* Add ModbusPkg to the TagEngine */
  engine1.addLibrary(lib2);

  engine1.updateTags(&running);

  return 0;
}
