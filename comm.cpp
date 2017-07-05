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
#include <errno.h>

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
	
	// should this be wrapped in a smart pointer?
	// need to look at modbus's source code to see if there
	// is a new/delete	
	ctx_ = modbus_new_tcp(ipAddress().c_str(), port());
	if (ctx_ == NULL) {
		std::cout << "Unable to allocate libmodbus context"
				  << std::endl;
	}

	if (modbus_connect(ctx_) == -1) {
		std::cout << "Connection failed: " << modbus_strerror(errno)
				  << std::endl;
	modbus_free(ctx_);
	}
	return *this;
}

void ModbusCommunication::close() {
	/* stop polling modbus slave */
	modbus_close(ctx_);
	modbus_free(ctx_);
	return;
}

/**
*/
void ModbusCommunication::waitOnQueue() {
	if(!commQueue_->empty()) {
		    read(commQueue_->operator[](0));
		    commQueue_->pop_front();
	}
}

int ModbusCommunication::read(std::shared_ptr<ModbusPkg>& spPkg)
{
	int rc = -1; // registers recieved
	if (spPkg->size()) {
		std::cout << "ModbusCommunication: call read()" << std::endl
			      << "...# registers: " << spPkg->size()
				  << ", beginning address: " << spPkg->operator[](0).address()
				  << ", destination: " << spPkg->destination() 
				  << std::endl;
		
		rc = modbus_read_registers(ctx_, spPkg->operator[](0).address(), spPkg->size(), spPkg->destination());
	}	
	if (rc == -1) {
		std::cout << "ModbusCommmunication: Read failed: " << modbus_strerror(errno)
				  << std::endl;
		return -1;
	}

	return rc;
}

}
