/**
	config.hpp
	Purpose: Class definitions for ModbusConfig. This class is used
	as a container for Modbus data read from configuration files.
	It provides an API to access the Modbus data.

	@author Owen Edgerton
	@version 1.0 6/20/17
*/


#ifndef GUARD_GRAAI_CONFIG_HPP_
#define GUARD_GRAAI_CONFIG_HPP_

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <cctype>
#include <stdint.h> 

namespace graComm {

class ModbusConfig {
public:

enum requestDataType { U16, U32, FLOAT, DOUBLE };
enum requestAccessType { R, W, RW };

struct modbusData {
	std::string name;
	uint16_t registerAddr;
	requestDataType dataType;
	requestAccessType accessType;
};

	// pass through the underlying data structure iterator?
	typedef std::vector<modbusData>::iterator iterator;
	typedef std::vector<modbusData>::const_iterator const_iterator;
	typedef std::vector<modbusData>::reference reference;
	typedef std::vector<modbusData>::const_reference const_reference;
	
	typedef std::vector<modbusData>::value_type value_type;

	ModbusConfig();
	ModbusConfig(std::istream& is) { readConfigStream(is); }

	// ModbusConfig(const ModbusConfig&);
	//ModbusConfig& operator=(const ModbusConfig&); 
	
	~ModbusConfig() { }
	
	std::istream& readConfigStream(std::istream&);
	std::fstream& readConfigFile(std::fstream&, const std::string&);
	void print() const;
	size_t size() const { return size_; }

	value_type operator[](size_t i) { return data_[i]; }
	const value_type operator[](size_t i) const { return data_[i]; }

	iterator begin() { return data_.begin(); }
	const_iterator begin() const { return data_.begin(); }
	iterator end() { return data_.end(); }
	const_iterator end() const { return data_.end(); }

private:
	std::vector<modbusData> data_; // container for modbus configuration data
	size_t size_;	 			   // number of uint16_t's
	
	size_t nRegisters() const; // initalizes size_	
};

}

#endif
