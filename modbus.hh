#ifndef GUARD_GRAAI_MODBUS_HH_
#define GUARD_GRAAI_MODBUS_HH_

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <stdint.h> 

namespace graComm {

enum requestDataType { U16, U32, FLOAT, DOUBLE };
enum requestAccessType { R, W, RW };

class ModbusTag {
public:
	ModbusTag();
	ModbusTag(std::istream& is); 

	~ModbusTag() { };

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

class ModbusPkg {
public:
  typedef std::vector<ModbusTag>::iterator iterator;
  typedef std::vector<ModbusTag>::const_iterator const_iterator;
  typedef std::vector<ModbusTag>::reference reference;
  typedef std::vector<ModbusTag>::const_reference const_reference;
  typedef std::vector<ModbusTag>::value_type value_type;

  ModbusPkg();
  ModbusPkg(std::fstream&, const std::string&);
  ~ModbusPkg() { } 
	
  size_t size() const { return size_; }

  value_type operator[](size_t i) { return tags_[i]; }
  const value_type operator[](size_t i) const { return tags_[i]; }
	
  iterator begin() { return tags_.begin(); }
  const_iterator begin() const { return tags_.begin(); }
  iterator end() { return tags_.end(); }
  const_iterator end() const { return tags_.end(); }
  value_type front() { return tags_.front(); }

  uint16_t* localDestination() { return localDestination_.get(); }  // returns a pointer, evading encapsulation, bad.
  void whois();

private:
  std::vector<ModbusTag> tags_;
  std::unique_ptr<uint16_t[]> localDestination_;
  size_t size_;                 

  std::fstream& init(std::fstream&, const std::string&);
  std::fstream& readConfigFile(std::fstream&, const std::string&);
  ModbusPkg& createDestination();
  size_t sizeOfBlock() const;

  void print(const std::string&) const;
};

bool sortByAddress(const ModbusTag&, const ModbusTag&);
bool sortByAccess(const ModbusTag&, const ModbusTag&);



}
#endif