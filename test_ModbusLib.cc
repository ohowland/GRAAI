#include "ModbusServer.hh"
#include "TagEngine.hh"
#include "ModbusPkg.hh"

#include <iostream>
#include <fstream>
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

  std::fstream fs; /* Create a ModbusPkg from file */ 
  std::shared_ptr<graComm::ModbusPkg> pkg1(new graComm::ModbusPkg(fs, "ModbusConfig.txt"));
  std::shared_ptr<graComm::ModbusServer> serv1(new graComm::ModbusServer("127.0.0.1", 1504)); 

  std::shared_ptr<graComm::ModbusLib> lib1(new graComm::ModbusLib());

  lib1->addServer(serv1);
  lib1->addPkg(pkg1);

  graComm::TagEngine engine1;
  engine1.addLibrary(lib1); /* Add ModbusPkg to the TagEngine */

  return 0;
}
