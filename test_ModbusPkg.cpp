#include "config.hpp"

#include <vector>
#include <iostream>
#include <fstream>

int main() {
	
	std::fstream fs;
	
	std::unique_ptr<graComm::ModbusPkg> 
	pkg1(new graComm::ModbusPkg(fs, "ModbusConfig.txt"));

	return 0;
}
