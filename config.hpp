#ifndef GUARD_GRAAI_CONFIG_HPP_
#define GUARD_GRAAI_CONFIG_HPP_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cctype>
#include <stdint.h> 

namespace graComm {

enum requestDataType {
	U16,
	U32,
	FLOAT,
	DOUBLE	
};

enum requestAccessType {
	R,
	W,
	RW
};

struct modbusData {
	std::string name;
	uint16_t registerAddr;
	requestDataType dataType;
	requestAccessType accessType; 
};

class ModbusConfig {
public:
	
	// pass through the underlying data structure iterator?
	typedef std::vector<modbusData>::iterator iterator;
	typedef std::vector<modbusData>::const_iterator const_iterator;
	typedef std::vector<modbusData>::reference reference;
	typedef std::vector<modbusData>::const_reference const_reference;
	
	ModbusConfig() { }
	ModbusConfig(std::istream& is) { readConfigStream(is); }
	// define copy constructor?
	// define copy assignment constructor?
	~ModbusConfig() { }
	
	std::istream& readConfigStream(std::istream&);
	std::fstream& readConfigFile(std::fstream&, const std::string&);
	void print();

	iterator begin() { return data.begin(); }
	const_iterator begin() const { return data.begin(); }
	iterator end() { return data.end(); }
	const_iterator end() const { return data.end(); }

private:
	std::vector<modbusData> data;

};

}

#endif
