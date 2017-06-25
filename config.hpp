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
#include <tr1/memory>
#include <map>
#include <fstream>
#include <stdint.h> 

namespace graComm {

enum requestDataType { U16, U32, FLOAT, DOUBLE };
enum requestAccessType { R, W, RW };

class ModbusTag {
public:
	ModbusTag();
	ModbusTag(std::istream& is); 
	
	~ModbusTag() { }

	std::string name() const { return name_; }
	uint16_t address() const { return address_; }
	requestDataType datatype() const { return datatype_; }
	requestAccessType access() const { return access_; }

private:
	std::string name_;
	uint16_t address_;
	requestDataType datatype_;
	requestAccessType access_;
};

class ModbusConfig {
public:
	// pass through the underlying data structure iterator?
	typedef std::vector<ModbusTag>::iterator iterator;
	typedef std::vector<ModbusTag>::const_iterator const_iterator;
	typedef std::vector<ModbusTag>::reference reference;
	typedef std::vector<ModbusTag>::const_reference const_reference;
	typedef std::vector<ModbusTag>::value_type value_type;

	ModbusConfig();
	ModbusConfig(std::istream& is) { readConfigStream(is); }

	// ModbusConfig(const ModbusConfig&);
	// ModbusConfig& operator=(const ModbusConfig&); 
	
	~ModbusConfig() { }
	
	std::istream& readConfigStream(std::istream&);
	std::fstream& readConfigFile(std::fstream&, const std::string&);
	void print() const;
	size_t size() const { return size_; }

	value_type operator[](size_t i) { return tags_[i]; }
	const value_type operator[](size_t i) const { return tags_[i]; }

	iterator begin() { return tags_.begin(); }
	const_iterator begin() const { return tags_.begin(); }
	iterator end() { return tags_.end(); }
	const_iterator end() const { return tags_.end(); }

private:
	// container for modbus configuration data
	std::vector<ModbusTag> tags_;
	size_t size_;                 // number of uint16_t's
	
	size_t nRegisters() const;    // initalizes size_	
};
	
bool sortByAddress(const ModbusTag&,
				   const ModbusTag&);

}

#endif
