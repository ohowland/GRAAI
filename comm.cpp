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

#include "config.hpp"

namespace graComm {

// CommunicationCore
CommunicationCore::CommunicationCore()
: processName_("UNINITIALIZED")
{ 
	std::cout << "COMMMUNICATION_CORE: Constructor"
			 	 << std::endl;
}

// ModbusCommunication
ModbusCommunication::ModbusCommunication()
:  ipAddress_("127.0.0.1"),
  port_(1502),
  //destination_(NULL),
  destinationSize_(0)
{
	std::cout << "MODBUS_COMMUNICATION: Constructor" << std::endl;
}

ModbusCommunication& ModbusCommunication::open() {
	/* begin polling modbus slave */
	std::cout << "MODBUS_COMMUNICATION: Opening connection..." << std::endl
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
	Read modbus registers defined in ModbusConfig object.
	This function currenty uses a single destination pointer held in
	the ModbusCommunication object. Future development would be to give read()
	a ModbusConfig, and a second object linking the destination to a data model.
	
	This highlights the next step, which is to move the data return to destination_
	into a model object.
		
		1. should the transfer take place in the read command?
		2. how to ensure the allocated space in destination_ is freed?
		3. how to link the modbus register tag 'name' to a parameter in
		   the model object?

*/

int ModbusCommunication::read(ModbusConfig& c) 
{
	createDestination(c);
	
	int rc = -1; // registers recieved
	if (destination_) {
		std::cout << "MODBUS COMMUNICATION: read " << destinationSize_ 
				  << " registers to " << destination_ << std::endl;
		
		rc = modbus_read_registers(ctx_, 0, destinationSize_, destination_);
	}

	if (rc == -1) {
		std::cout << "Read failed: " << modbus_strerror(errno)
				  << std::endl;
		return -1;
	}

	// when reading the registers back out, it would be nice to know what their
	// data types are, so that they can be reconstructed on this end.

	for (size_t i = 0; i <= destinationSize_; i++) {
		std::cout << "UINT16_T " << i 
				  << "--> " << *(destination_+i) 
				  << std::endl;
	}
	return rc;
}

/**
	Find the total space required to recieve data listed in a ModbusConfig
    object. Create that space and return a pointer to it.

	@params ModbusConfig object with data for modbus call.
	@return pointer to start of destination space to be used in libmodbus
		    read_tcp call.	
*/
uint16_t& ModbusCommunication::createDestination(const ModbusConfig& cfg)
{
	std::cout << "CALL: createDestination()" << std::endl;
	if (destination_)
		uncreateDestination();

	for (ModbusConfig::const_iterator it = cfg.begin(); 
		 it <= cfg.end(); it++)
	{
		switch(it->dataType) {
		case U16:
			destinationSize_ += (sizeof(uint16_t)/2);
			std::cout << "FOUND: U16, destinationSize: " << destinationSize_;
			break;
		case U32:
			destinationSize_ += (sizeof(uint32_t)/2);
			std::cout << "FOUND: U32, destinationSize: " << destinationSize_;
			break;
		case FLOAT:
			destinationSize_ += (sizeof(float)/2);
			std::cout << "FOUND: float, destinationSize: " << destinationSize_;
			break;
		case DOUBLE:
			destinationSize_ += (sizeof(double)/2);
			std::cout << "FOUND: double, destinationSize: " << destinationSize_;
			break;
		default:
			std::cout << "WARNING! FOUND: undefined type. behavior undefined!";
			break;
		}
		std::cout << std::endl;
	}
	
	destination_ = new uint16_t[destinationSize_]; // allocate memory in destination_

	return *destination_;
}
	
int ModbusCommunication::uncreateDestination() {
	
	std::cout << "CALL: uncreateDestination()" << std::endl;
	delete[] destination_;
	destinationSize_ = 0;
	return 0;
}	

}
