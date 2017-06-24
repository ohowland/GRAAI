/**
	config.cpp
	Purpose: Implements the ModbusConfig class. Used to translate filestream
	data into a standard form for use with the ModbusCommunication class.

	@author Owen Edgerton
	@version 1.0 6/17/17
 */

#include "config.hpp"
#include <iostream>
#include <algorithm> 

namespace graComm {

bool compareAddress(const ModbusTag& lhs, const ModbusTag& rhs)
{
	return lhs.address() < rhs.address();
}

/* ***ModbusTag implementation*** */
ModbusTag::ModbusTag()
 : name_(),
   address_(0),
   datatype_(U16),
   access_(R)
{
	std::cout << "ModbusTag: Constructor" << std::endl;
}

ModbusTag::ModbusTag(std::istream& is)
 : name_(),
   address_(0),
   datatype_(U16),
   access_(R)
{
	std::cout << "ModbusTag: Constructor(istream&)" << std::endl;
	int dtype; // for conversion to datatype
	int atype; // for conversion to access

	is >> name_ >> address_ >> dtype >> atype;

	datatype_ = static_cast<requestDataType>(dtype);
	access_ = static_cast<requestAccessType>(atype);
}


/* ***ModbusConfig Implementation*** */
ModbusConfig::ModbusConfig()
  : size_(0) // TODO initalize all parameters
{
	std::cout << "ModbusConfig: Constructor" << std::endl;
}

/**
	Read configuration data into the vector<ModbusData>
	from a istream. Data should be entered as:
	Name, Register Address, Data Type, Access Type.

	@param 'is' reference to istream
	@return reference to istream.
 */
std::istream& ModbusConfig::readConfigStream(std::istream& is)
{
	std::cout << "ModbusConfig: Constructor(istream&)" << std::endl;
	
	tags_.clear();
	char c;
	while (is >> c) {
		is.putback(c);
		tags_.push_back(ModbusTag(is));
	}
	
	size_ = nRegisters();
	return is;
}

/**
	Read configuration data into the vector<ModbusData>
	from a filestream. Data should be entered as:
	Name, Register Address, Data Type, Access Type.

	@param 'fs' reference to filestream
	@param 'filepath' filename as string
	@return reference to filestream.
 */
std::fstream& ModbusConfig::readConfigFile(std::fstream& fs, 
										   const std::string& filepath)
{
	fs.open(filepath.c_str());
	tags_.clear();

	char c;	
	while (fs >> c) {
		fs.putback(c);
		tags_.push_back(ModbusTag(fs));
	}
	fs.close(); // how to write as exception safe?
				// try statement around while loop?
	
	size_ = nRegisters();
	return fs;
}

/**
	Displays the Modbus configuration values held
   	in the ModbusConfiguration vector<ModbusData>.
 */ 
void ModbusConfig::print() const
{
	std::cout << "----------------------------------" 
			  << std::endl
			  << "MODBUS CONFIGURATION FILE:"
			  << std::endl
		      << "NAME | Address | Datatype | Access" << std::endl;	

	// std::sort(tags_.begin(), tags_.end(), compareAddress);

	for (std::vector< std::tr1::shared_ptr<ModbusTag> >::const_iterator it = tags_.begin();
	     it < tags_.end(); it++)
   	{
		std::cout << it->name() << " "
				  << it->address() << " ";

		switch(it->datatype()) {
		case U16:
			std::cout << "U16 ";
			break;
		case U32:
			std::cout << "U32 ";
			break;
		case FLOAT:
			std::cout << "SINGLE FLOAT ";
			break;
		case DOUBLE:
			std::cout << "DOUBLE FLOAT ";
			break;
		default:
			std::cout << "UNDEFINED ";
			break;
		}

		switch(it->access()) {
		case R:
			std::cout << "READ";
			break;
		case W:
			std::cout << "WRITE";
			break;
		case RW:
			std::cout << "READ/WRITE";
			break;
		default:
			std::cout << "UNDEFINED";
			break;
		}
		std::cout << std::endl;

	}
	std::cout << "----------------------------------"
			  << std::endl;
}

size_t ModbusConfig::nRegisters() const
{
	size_t nRegisters = 0;
	for (std::vector< std::tr1::shared_ptr<ModbusTag> >::const_iterator it = tags_.begin(); 
		 it != tags_.end(); it++)
	{
		switch(it->datatype()) {
		case U16:
			nRegisters += (sizeof(uint16_t)/2);
			std::cout << "FOUND: U16, destinationSize: " << nRegisters;
			break;
		case U32:
			nRegisters += (sizeof(uint32_t)/2);
			std::cout << "FOUND: U32, destinationSize: " << nRegisters;
			break;
		case FLOAT:
			nRegisters += (sizeof(float)/2);
			std::cout << "FOUND: float, destinationSize: " << nRegisters;
			break;
		case DOUBLE:
			nRegisters += (sizeof(double)/2);
			std::cout << "FOUND: double, destinationSize: " << nRegisters;
			break;
		default:
			std::cout << "WARNING! FOUND: undefined type. behavior undefined!";
			break;
		}
		std::cout << std::endl;
	}
	return nRegisters;
}
}
