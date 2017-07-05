#include "comm.hpp"
#include "TagEngine.hpp"
#include "ModbusPkg.hpp"

#include <vector>
#include <iostream>
#include <fstream>

int main() {

	graComm::ModbusCommunication comm1;
	comm1.open();

	graComm::TagEngine engine1(comm1.commQueue());
	
	std::fstream fs;
	std::shared_ptr<graComm::ModbusPkg> pkg1(new graComm::ModbusPkg(fs, "ModbusConfig.txt"));

	engine1.addPkg(pkg1);
	engine1.run();

	comm1.waitOnQueue();
    comm1.close();
	return 0;
}
