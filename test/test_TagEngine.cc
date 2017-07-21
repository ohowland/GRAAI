#include "TagEngine.hh"
#include "modbus.hh"

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

  typedef typename graComm::Library<graComm::ModbusServer, graComm::ModbusPkg> ModbusLibrary;

  graComm::TagEngine engine1;
 
  // Create new ModbusLib 
  std::shared_ptr<graComm::LibraryBase> lib1(new ModbusLibrary());
  std::shared_ptr<graComm::LibraryBase> lib2(new ModbusLibrary());
 
  // why can't the type <ModbusLibrary> be deduced?
  lib1->addServer<ModbusLibrary>(std::shared_ptr<graComm::ModbusServer>(new graComm::ModbusServer("127.0.0.1", 1502)));
  lib2->addServer<ModbusLibrary>(std::shared_ptr<graComm::ModbusServer>(new graComm::ModbusServer("127.0.0.1", 1502)));

  std::fstream fs; // Create a ModbusPkg from file
  lib1->addPkg<ModbusLibrary>(std::shared_ptr<graComm::ModbusPkg>(new graComm::ModbusPkg(fs, "ModbusConfig.txt")));
  lib1->addPkg<ModbusLibrary>(std::shared_ptr<graComm::ModbusPkg>(new graComm::ModbusPkg(fs, "ModbusConfig.txt")));
  
  lib2->addPkg<ModbusLibrary>(std::shared_ptr<graComm::ModbusPkg>(new graComm::ModbusPkg(fs, "ModbusConfig.txt")));
  lib2->addPkg<ModbusLibrary>(std::shared_ptr<graComm::ModbusPkg>(new graComm::ModbusPkg(fs, "ModbusConfig.txt")));

  engine1.addLibrary(lib1); // Add ModbusPkg to the TagEngine
  engine1.addLibrary(lib2);

  //engine1.update(&running);

  return 0;
}
