#ifndef GUARD_GRAAI_COMMIMPL_HPP_
#define GUARD_GRAAI_COMMIMPL_HPP_

#include <string>
#include <modbus.h>

#include "config.hpp"

namespace graComm {

class ModbusConfig;

class CommunicationCore {
/* Base class for objects handling pysical layer communication */

public:
	CommunicationCore();
	virtual ~CommunicationCore() { }

	virtual CommunicationCore& open() { return *this; }
	virtual void close() = 0; 

	std::string processName() const { return processName_; }

protected:

private:
	std::string processName_; // communication process name

};

class ModbusCommunication: public CommunicationCore {
/* Child class for modbus communication */

public:
	ModbusCommunication();
	std::string ipAddress() const { return ipAddress_; }
	int port() const { return port_; }

	ModbusCommunication& open();
	void close(); 
	int read(ModbusConfig&);

protected:

private:
	modbus_t *ctx_;
	std::string ipAddress_; // Device ip address
	int port_; 				// Modbus port default 502
	uint16_t *destination_;  // Destination pointer for recieved modbus data
	size_t destinationSize_; // Size of destination for garbage collection

	uint16_t& createDestination(const ModbusConfig&);
	int uncreateDestination();	
};

}

#endif
