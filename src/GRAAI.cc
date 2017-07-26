#include "GRAAI.hh"

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

int main(int argc, char* argv[]) {

  fprintf(stdout, "%s version %d.%d\n",
        argv[0],
        GRAAI_VERSION_MAJOR,
        GRAAI_VERSION_MINOR);

  signal(SIGINT, stopHandler);
  signal(SIGTERM, stopHandler);

  typedef typename graComm::Library<graComm::ModbusServer, graComm::ModbusPkg> ModbusLibrary;
  typedef std::shared_ptr<graComm::ModbusServer> spModbusServer;
  typedef std::shared_ptr<graComm::ModbusPkg> spModbusPkg;
  
  graComm::TagEngine engine1;
 
  // Create new ModbusLib 
  std::shared_ptr<graComm::LibraryBase> lib1(new ModbusLibrary());
  std::shared_ptr<graComm::LibraryBase> lib2(new ModbusLibrary());
 
  // why can't the type <ModbusLibrary> be deduced?
  lib1->addServer<ModbusLibrary>(spModbusServer(new graComm::ModbusServer("127.0.0.1", 1502)));
  lib2->addServer<ModbusLibrary>(spModbusServer(new graComm::ModbusServer("127.0.0.1", 1504)));

  std::fstream fs; // Create a ModbusPkg from file
  std::string cfgDIR = PROJECT_CONFIG_DIR;
  lib1->addPkg<ModbusLibrary>(spModbusPkg(new graComm::ModbusPkg(fs, cfgDIR + "/ModbusConfig.txt")));
  lib1->addPkg<ModbusLibrary>(spModbusPkg(new graComm::ModbusPkg(fs, cfgDIR + "/ModbusConfig.txt")));
  
  lib2->addPkg<ModbusLibrary>(spModbusPkg(new graComm::ModbusPkg(fs, cfgDIR + "/ModbusConfig.txt")));
  lib2->addPkg<ModbusLibrary>(spModbusPkg(new graComm::ModbusPkg(fs, cfgDIR + "/ModbusConfig.txt")));

  engine1.addLibrary(lib1); // Add ModbusPkg to the TagEngine
  engine1.addLibrary(lib2);

  engine1.update(&running);

  return 0;
}
