/**
	config.cpp
	Purpose: Implements the ModbusConfig class. Used to translate filestream
	data into a standard form for use with the ModbusCommunication class.

	@author Owen Edgerton
	@version 1.0 6/17/17
 */

#include "config.hpp"

namespace graComm {

ModbusConfig::ModbusConfig()
  : size_(0)
{
	std::cout << "MODBUS_CONFIG: Constructor" << std::endl;
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
	data_.clear();
	int dtype; // for conversion to dataType
	int atype; // for conversion to accessType
	modbusData incomingConfigData;
	while (is >> incomingConfigData.name
			  >> incomingConfigData.registerAddr
			  >> dtype
			  >> atype) 
	{	
		incomingConfigData.dataType = 
		static_cast<requestDataType>(dtype);
		
		incomingConfigData.accessType =
		static_cast<requestAccessType>(atype);

		data_.push_back(incomingConfigData);
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
	data_.clear();
	int dtype; // for conversion to dataType
	int atype; // for conversion to accessType
	modbusData incomingConfigData;
	/* TODO: there is a real boner of a bug here.
	 * the while loop runs once more than it should even though it should
	 * be hitting an EOF.
	*/
	while (fs >> incomingConfigData.name
			  >> incomingConfigData.registerAddr
			  >> dtype 
			  >> atype) 
	{		
		incomingConfigData.dataType = 
		static_cast<requestDataType>(dtype);
		
		incomingConfigData.accessType =
		static_cast<requestAccessType>(atype);
		
		data_.push_back(incomingConfigData);
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
	
	for (std::vector<modbusData>::const_iterator it = data_.begin();
	     it < data_.end(); it++)
   	{
		std::cout << it->name << " "
				  << it->registerAddr << " ";

		switch(it->dataType) {
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

		switch(it->accessType) {
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
	for (std::vector<modbusData>::const_iterator it = data_.begin(); 
		 it <= data_.end(); it++)
	{
		switch(it->dataType) {
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
