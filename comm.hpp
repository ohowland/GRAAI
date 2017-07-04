#ifndef GUARD_GRAAI_COMMIMPL_HPP_
#define GUARD_GRAAI_COMMIMPL_HPP_

#include <string>
#include <modbus.h>


namespace graComm {

class ModbusConfig;

class ModbusCommunication {
/* Class for modbus communication */

public:
	ModbusCommunication();
	std::string ipAddress() const { return ipAddress_; }
	int port() const { return port_; }

	ModbusCommunication& open();
	void close(); 
	
	// int read(const ModbusConfig&, std::map<std::string, uint16_t>&);

protected:

private:
	modbus_t *ctx_;
	std::string ipAddress_; // Device ip address
	int port_; 				// Modbus port default 502
	uint16_t *destination_;  // Destination pointer for recieved modbus data

	ModbusCommunication& createDestination(const ModbusConfig&);
	ModbusCommunication& uncreateDestination();	
	
	// std::map<std::string, uint16_t> mapReturnedData(const ModbusConfig&);
};

}

#endif
