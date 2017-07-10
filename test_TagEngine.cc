#include "TagEngine.hpp"
#include "ModbusPkg.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <signal.h>

bool running = true;
void stopHandler(int sig) { 
  std::cout << "Main: Shutting down" << std::endl;
  running = false;
}

int main() {
  signal(SIGINT, stopHandler);
  signal(SIGTERM, stopHandler);

  std::shared_ptr<std::deque<std::shared_ptr<graComm::ModbusPkg> > >
  spcq(new std::deque<std::shared_ptr<graComm::ModbusPkg> >);

  graComm::TagEngine engine1(spcq);
	
  std::fstream fs;
  std::shared_ptr<graComm::ModbusPkg> pkg1(new graComm::ModbusPkg(fs, "ModbusConfig.txt"));

  engine1.addPkg(pkg1);
  engine1.run(&running);
	
  return 0;
}
