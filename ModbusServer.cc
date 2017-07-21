/**
	ModbusServer.cc
	Wraps libmodbus.c in a C++

	@author Owen Edgerton
	@version 1.0 6/17/17

*/

#include "modbus.hh"

#include <string>
#include <iostream>
#include <iomanip>
#include <errno.h>
#include <unistd.h>

namespace graComm {

// ModbusServer
ModbusServer::ModbusServer(const std::string& ip, int p)
: ipAddress_(ip),
  port_(p),
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
//  modbus_set_response_timeout(ctx, 0, 200000);
}

/* Open connection to modbus slave */
int ModbusServer::open() {
  print_("Opening connection");
  
  if (modbus_connect(ctx_) == -1) {
    std::cout << "Connection failed: " << modbus_strerror(errno)
              << std::endl;
    return -1;
  }
  return 0;
}

/* Close connection to modbus slave */
void ModbusServer::close() {
  modbus_close(ctx_);
  modbus_free(ctx_);
  print_("Connection closed");
}

/* Run Modbus server consumer loop */
int ModbusServer::run() {
  for(auto pkg : *(commQueue_.get())) {
    read(commQueue_->front());
    commQueue_->pop_front();
  }
  return 0;
}

int ModbusServer::read(std::shared_ptr<ModbusPkg>& spPkg) {
  int rc = -1; // registers recieved
  if (spPkg->size()) {  
    rc = modbus_read_registers(ctx_, spPkg->front().address(), spPkg->size(), spPkg->localDestination());
  }	
  
  if (rc == -1)
    print_(modbus_strerror(errno));
  else
    print_("TX Read"); 
  return rc;
}

void ModbusServer::print_(const std::string& s) const {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t); 
  std::cout << "[" << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "] " 
	    << "ModbusServer: " << "(" << ipAddress() << ") "
	    << s  <<  std::endl;
}

}
