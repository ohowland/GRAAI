#ifndef GUARD_GRAAI_COMMIMPL_HPP_
#define GUARD_GRAAI_COMMIMPL_HPP_

#include <string>
#include <memory>
#include <queue>
#include <modbus.h>

#include "ModbusPkg.hpp"

namespace graComm {

class ModbusCommunication {
/* Class for modbus communication */

public:
  ModbusCommunication();
  std::string ipAddress() const { return ipAddress_; }
  int port() const { return port_; }

  ModbusCommunication& open();
  void close();

  std::shared_ptr<std::deque<std::shared_ptr<ModbusPkg> > > 
  commQueue() { return commQueue_; }	

  int read(std::shared_ptr<ModbusPkg>&);
  int run(bool* const);

private:
  modbus_t *ctx_;
  std::string ipAddress_; // Device ip address
  int port_;              // Modbus port default 502
  std::shared_ptr<std::deque<std::shared_ptr<ModbusPkg> > > commQueue_;
  
  void print(const std::string&) const;
};

}

#endif
