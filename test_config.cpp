#include "config.hpp"

#include <vector>
#include <iostream>
#include <fstream>

int main() {

	graComm::ModbusConfig config1;
	
	std::fstream fs;
	config1.readConfigFile(fs, "ModbusConfig.txt");
	config1.print();

	return 0;
}
