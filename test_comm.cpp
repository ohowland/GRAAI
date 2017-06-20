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
	std::map<std::string, uint16_t> map1;
	comm1->read(config1, map1);

	for(std::map<std::string, uint16_t>::const_iterator it = map1.begin();
	    it != map1.end(); it++) {
		std::cout << it->first << " " << it->second << std::endl;
	}
	return 0;
}
