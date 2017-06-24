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
: ipAddress_("127.0.0.1"),
  port_(1502)
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
int ModbusCommunication::read(const ModbusConfig& pkg,
							  std::map<std::string, uint16_t>& rMap) 
{
	createDestination(pkg);
	
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
	uncreateDestination();
	return rc;
}

/**
	Find the total space required to recieve data listed in a ModbusConfig
    object. Create that space and return a pointer to it.

	@params ModbusConfig object with data for modbus call.
	@return ref to ModbusCommunication object.
*/
ModbusCommunication&
ModbusCommunication::createDestination(const ModbusConfig& pkg)
{
	std::cout << "CALL: createDestination()" << std::endl;
	if (destination_)
		uncreateDestination();

	destination_ = new uint16_t[pkg.size()]; // allocate memory in destination_

	return *this;
}

/**
	Destroy the space allocated in destination_
*/ 
ModbusCommunication&
ModbusCommunication::uncreateDestination()
{
	
	std::cout << "CALL: uncreateDestination()" << std::endl;
	delete[] destination_;
	return *this;
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
