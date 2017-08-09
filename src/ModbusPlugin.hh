#ifndef GUARD_GRAAI_MODBUSPLUGIN_HH_
#define GUARD_GRAAI_MODBUSPLUGIN_HH_

#include <list>
#include <string>
#include <vector>
#include <mutex>
#include <future>
#include <memory>
#include <fstream>
#include <stdint.h> 
#include <queue>
#include <modbus.h>

#include "TagEngine.hh"

namespace graComm {

enum requestDataType { U16, U32, FLOAT, DOUBLE };
enum requestAccessType { R, W, RW };

/* MODBUS TAG CLASS
   ^^^^^^^^^^^^^^^^ */
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

/* MODBUS PACKAGE CLASS
   ^^^^^^^^^^^^^^^^^^^^ */
class ModbusPkg: public PkgBase {
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

/* MODBUS SERVER CLASS
   ^^^^^^^^^^^^^^^^^^^ */
class ModbusServer: public ServerBase {
public:
  typedef std::shared_ptr<std::deque<std::shared_ptr<ModbusPkg> > > sDequeHandle;
  typedef std::weak_ptr<std::deque<std::shared_ptr<ModbusPkg> > > wDequeHandle;
  typedef std::shared_ptr<std::deque<std::shared_ptr<PkgBase> > > sDequeHandleP;
  typedef std::weak_ptr<std::deque<std::shared_ptr<PkgBase> > > wDequeHandleP; 

  ModbusServer(const std::string& ip = "127.0.0.1", int port = 1504);
  
  //std::string ipAddress() const { return ipAddress_; }
  //int port() const { return port_; }
  wDequeHandleP getQueue() { return wDequeHandleP(pkgQueue_); };
  std::mutex& getQueueMutex() { return queueMutex_; };

  int open();   // Open client connection
  void close(); // Close client connection
  int run();    // Process requests from request queue.
  
  int read(std::shared_ptr<PkgBase>&);

private:
  modbus_t *ctx_;
  std::string ipAddress_;     // Device ip address
  int port_;                  // Modbus port default 502
  sDequeHandleP pkgQueue_;    // Update queue handle
  std::mutex queueMutex_;     // mutex for update queue handle
  void print_(const std::string&) const;
};

}
#endif
