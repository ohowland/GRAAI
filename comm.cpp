#include "comm.hpp"

#include <string>
#include <iostream>
#include <errno.h>

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

int ModbusCommunication::read() {
	int rc = -1;
	int n = 5;
	
	/* this is for testing, and should be replaced with a class
	 * handing the data storage in the models class. 
	 * */
	uint16_t *dest = new uint16_t(n);

	rc = modbus_read_registers(ctx_, 0, n, dest);
	std::cout << "MODBUS COMMUNICATION: read..." << std::endl;
	if (rc == -1) {
		std::cout << "Read failed: " << modbus_strerror(errno)
				  << std::endl;
		return -1;
	}
	
	for (int i = 0; i < n; i++) {
		std::cout << "--> " << *(dest+i) << std::endl;
	}

	delete dest;
	return rc;
}

}
