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

// ModbusCommunication
ModbusCommunication::ModbusCommunication()
: ipAddress_("127.0.0.1"),
  port_(1502)
{ std::cout << "MODBUS_COMMUNICATION: Constructor" << std::endl; }

ModbusCommunication& ModbusCommunication::open() {

	/* begin polling modbus slave */
	std::cout << "MODBUS_COMMUNICATION: Opening connection..." << std::endl
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
int ModbusCommunication::read(const ModbusConfig& pkg,
							  std::map<std::string, uint16_t>& rMap) 
{
//	createDestination(pkg);
	
	int rc = -1; // registers recieved
	if (destination_) {
		std::cout << "MODBUS COMMUNICATION: read " << pkg.size() 
				  << " registers to " << destination_ << std::endl;
		
		rc = modbus_read_registers(ctx_, pkg[0].registerAddr, pkg.size(), destination_);
	}	
	if (rc == -1) {
		std::cout << "Read failed: " << modbus_strerror(errno)
				  << std::endl;
		return -1;
	}

	rMap = mapReturnedData(pkg);
//	uncreateDestination();
	return rc;
}

/**
	@params ModbusConfig
	@return <name, value> map of modbus data.
*/
std::map<std::string, uint16_t>
ModbusCommunication::mapReturnedData(const ModbusConfig& pkg)
{
	std::map<std::string, uint16_t> pkgMap;

	for (ModbusConfig::const_iterator it = pkg.begin(); it != pkg.end(); it++)
	{
		std::pair<std::string, uint16_t> registerPair(it->name, destination_[it->registerAddr]);
		pkgMap.insert(registerPair);
	}

	return pkgMap;
}
}
