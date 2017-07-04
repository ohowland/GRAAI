/**
	config.cpp
	Purpose: Implements the ModbusPkg class. Used to translate filestream
	data into a standard form for use with the ModbusPkg class.

	@author Owen Edgerton
	@version 1.0 6/17/17
 */

#include "ModbusPkg.hpp"
#include <iostream>
#include <algorithm> 

namespace graComm {

bool sortByAddress(const ModbusTag& lhs,
	               const ModbusTag& rhs)
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

/* ***ModbusPkg Implementation*** */

ModbusPkg::ModbusPkg()
: tags_(),
  destination_(),
  size_(0)
{
	std::cout << "ModbusPkg: Constructor" << std::endl;
}

ModbusPkg::ModbusPkg(std::fstream& fs, 
				     const std::string& filepath)
: tags_(),
  destination_(),
  size_(0)
{
	std::cout << "ModbusPkg: Constructor(fstream, string)" << std::endl;
	init(fs, filepath);
}

/** init() handles getting information into the ModbusPkg object,
    and creating the space required to recieve a call made using
	the object.
*/ 
std::fstream& ModbusPkg::init(std::fstream& fs,
                              const std::string& filepath)
{
	readConfigFile(fs, filepath);
	createDestination();
	return fs;
}

/**
	Read configuration data into the vector<ModbusData>
	from a filestream. Data should be entered as:
	Name, Register Address, Data Type, Access Type.

	@param 'fs' reference to filestream
	@param 'filepath' filename as string
	@return reference to filestream.
*/
std::fstream& ModbusPkg::readConfigFile(std::fstream& fs, 
                                        const std::string& filepath)
{
	std::cout << "Reading configuration file..." << std::endl;
	fs.open(filepath.c_str());
	tags_.clear();

	char c;	
	while (fs >> c) {
		fs.putback(c);
		tags_.push_back(ModbusTag(fs));
	}
	fs.close(); // how to write as exception safe?
				// try statement around while loop?
	std::cout << "Finished reading configuration file" << std::endl;

	return fs;
}

ModbusPkg&
ModbusPkg::createDestination()
{
    std::cout << "CALL: createDestination()" << std::endl;
	
	sizeOfBlock();
	
	auto destination_ = std::make_unique<uint16_t[]>(this->size());
	return *this;
}

size_t ModbusPkg::sizeOfBlock() const
{
	size_t nRegisters = 0;
	for (std::vector<ModbusTag>::const_iterator it = tags_.begin(); 
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
