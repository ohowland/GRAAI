#include <iostream>
#include <string>
#include <tr1/memory>
#include <fstream>

#include "comm.hpp"
#include "config.hpp"

int main() {

	/* COMM.o */

	std::tr1::shared_ptr<graComm::ModbusCommunication>
	comm1( new graComm::ModbusCommunication() );
	
	graComm::ModbusConfig config1;	

	std::fstream fs;
	config1.readConfigFile(fs, "ModbusConfig.txt");

	comm1->open();
	comm1->read(config1);
	return 0;
}
