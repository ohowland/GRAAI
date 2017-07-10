/**
	comm.cpp
	Purpose: Implements the CommunicationCore and 
	ModbusCommunication class. Wraps libmodbus.c in a C++
	class. Meant to operate on a ModbusConfig class containing
	modbus register data.	

	@author Owen Edgerton
	@version 1.0 6/17/17

*/

#include "comm.hpp"

#include <string>
#include <iostream>
#include <iomanip>
#include <errno.h>
#include <unistd.h>

namespace graComm {

// ModbusCommunication
ModbusCommunication::ModbusCommunication()
: ipAddress_("127.0.0.1"),
  port_(1502),
  commQueue_(new std::deque<std::shared_ptr<ModbusPkg> >)
{ 
  std::cout << "ModbusCommunication: Constructor" << std::endl;
}

ModbusCommunication& ModbusCommunication::open() {

/* begin polling modbus slave */
  std::cout << "ModbusCommunication: Opening connection. "
            << "IP: " << ipAddress_ << " Port: " << port_
            << std::endl;
	
  ctx_ = modbus_new_tcp(ipAddress().c_str(), port());
  if (ctx_ == NULL) {
    std::cout << "Unable to allocate libmodbus context"
              << std::endl;
  }
  if (modbus_connect(ctx_) == -1) {
    std::cout << "Connection failed: " << modbus_strerror(errno)
              << std::endl;
  }
  return *this;
}

void ModbusCommunication::close() {
/* stop polling modbus slave */
  modbus_close(ctx_);
  modbus_free(ctx_);
}

/**
*/
int ModbusCommunication::run(bool* running) {
  std::cout << "ModbusCommunication: call run()" << std::endl;
  while(*running) {
    if(!(commQueue_->empty())) {
      read(commQueue_->front());
      commQueue_->pop_front();
    }
  usleep(1000);
  }
  return 0;
}

int ModbusCommunication::read(std::shared_ptr<ModbusPkg>& spPkg) {
  int rc = -1; // registers recieved
  if (spPkg->size()) {  
    rc = modbus_read_registers(ctx_, spPkg->front().address(), spPkg->size(), spPkg->destination());
  }	
  
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t); 
  if (rc == -1) {
    std::cout << "[" << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "] " 
	      << "ModbusCommunication: " << "(" << ipAddress() << ") "
	      << modbus_strerror(errno) <<  std::endl;
    return -1;
  } else {
    std::cout << "[" << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "] " 
	      << "ModbusCommunication: " << "(" << ipAddress() << ") "
	      << "TX Read" <<  std::endl;
  }
  return rc;
}

}
