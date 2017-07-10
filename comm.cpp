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
  /* create libmodbus context */ 
  ctx_ = modbus_new_tcp(ipAddress().c_str(), port());
  if (ctx_ == NULL) {
    std::cout << "Unable to allocate libmodbus context"
              << std::endl;
  }
 
  /* set recovery type */ 
  modbus_set_error_recovery(ctx_, MODBUS_ERROR_RECOVERY_LINK);
}

/**
begin polling modbus slave */
ModbusCommunication& ModbusCommunication::open() {
  print("Opening connection");
  
  if (modbus_connect(ctx_) == -1) {
    std::cout << "Connection failed: " << modbus_strerror(errno)
              << std::endl;
  }
  return *this;
}

/** 
stop polling modbus slave */
void ModbusCommunication::close() {
  modbus_close(ctx_);
  modbus_free(ctx_);
  print("Connection closed");
}

/**
*/
int ModbusCommunication::run(bool* running) {
  print("Server running");
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
    rc = modbus_read_registers(ctx_, spPkg->front().address(), spPkg->size(), spPkg->localDestination());
  }	
  
  if (rc == -1)
    print(modbus_strerror(errno));
  else
    print("TX Read"); 
  return rc;
}

void ModbusCommunication::print(const std::string& s) const {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t); 
  std::cout << "[" << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "] " 
	    << "ModbusCommunication: " << "(" << ipAddress() << ") "
	    << s  <<  std::endl;
}

}
