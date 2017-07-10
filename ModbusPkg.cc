/**
	config.cpp
	Purpose: Implements the ModbusPkg class. Used to translate filestream
	data into a standard form for use with the ModbusPkg class.

	@author Owen Edgerton
	@version 1.0 6/17/17
 */

#include "ModbusPkg.hh"
#include <iostream>
#include <algorithm> 
#include <iomanip>

namespace graComm {

bool sortByAddress (const ModbusTag& lhs,
                    const ModbusTag& rhs)
{
  return lhs.address() < rhs.address();
}

/**
ModbusTag implementation
^^^^^^^^^^^^^^^^^^^^^^^^ */
ModbusTag::ModbusTag()
 : name_(),
   address_(0),
   datatype_(U16),
   access_(R)
{ }

ModbusTag::ModbusTag(std::istream& is)
 : name_(),
   address_(0),
   datatype_(U16),
   access_(R)
{
  int dtype; /* for conversion to datatype */
  int atype; /* for conversion to access */

  is >> name_ >> address_ >> dtype >> atype;

  datatype_ = static_cast<requestDataType>(dtype);
  access_ = static_cast<requestAccessType>(atype);
}

/** 
ModbusPkg Implementation
^^^^^^^^^^^^^^^^^^^^^^^^ */
ModbusPkg::ModbusPkg()
: tags_(),
  localDestination_(),
  size_(0)
{ }

ModbusPkg::ModbusPkg(std::fstream& fs, const std::string& filepath)
: tags_(),
  localDestination_(),
  size_(0)
{
  init(fs, filepath);
}

/** 
init() handles getting information into the ModbusPkg object,
and creating the space required to recieve a call made using
the object. */ 
std::fstream& ModbusPkg::init(std::fstream& fs,
                              const std::string& filepath)
{
  readConfigFile(fs, filepath);
  size_ = sizeOfBlock();
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
  fs.open(filepath.c_str());
  tags_.clear();

  char c;  
  while (fs >> c) {
    fs.putback(c);
    tags_.push_back(ModbusTag(fs));
  }
  fs.close(); // how to write as exception safe?
              // try statement around while loop?

  return fs;
}

ModbusPkg&
ModbusPkg::createDestination()
{
  localDestination_ = std::make_unique<uint16_t[]>(this->size());
  return *this;
}

size_t ModbusPkg::sizeOfBlock() const
{
  size_t nRegisters = 0;
  for(ModbusTag it : tags_) {
    switch(it.datatype()) {
    case U16:
      nRegisters += (sizeof(uint16_t)/2);
      print("FOUND U16");
      break;
    case U32:
      nRegisters += (sizeof(uint32_t)/2);
      print("FOUND U32");
      break;
    case FLOAT:
      nRegisters += (sizeof(float)/2);
      print("FOUND float");
      break;
    case DOUBLE:
      nRegisters += (sizeof(double)/2);
      print("FOUND double");
      break;
    default:
      print("WARNING! FOUND undefined type. behavior undefined!");
      break;
    }
  }
  return nRegisters;
}

void ModbusPkg::print(const std::string& s) const {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t); 
  std::cout << "[" << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "] " 
	    << "ModbusPkg: " << s  <<  std::endl;
}

void ModbusPkg::whois() { std::cout << "whois " << this << std::endl; }

}
