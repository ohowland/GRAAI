#include "TagEngine.hpp"
#include "ModbusPkg.hpp"

#include <vector>
#include <iostream>
#include <fstream>

int main() {

	std::shared_ptr<std::deque<std::shared_ptr<graComm::ModbusPkg> > >
	spcq(new std::deque<std::shared_ptr<graComm::ModbusPkg> >);

	graComm::TagEngine engine1(spcq);
	
	std::fstream fs;
	std::shared_ptr<graComm::ModbusPkg> pkg1(new graComm::ModbusPkg(fs, "ModbusConfig.txt"));

	engine1.addPkg(pkg1);
	engine1.run();
	
	return 0;
}
