#ifndef GUARD_GRAAI_COMMIMPL_HPP_
#define GUARD_GRAAI_COMMIMPL_HPP_

#include <string>
#include <memory>
#include <queue>
#include <modbus.h>

#include "ModbusPkg.hh"

namespace graComm {

class ModbusServer {
public:
  typedef std::shared_ptr<std::deque<std::shared_ptr<ModbusPkg> > > sDequeHandle;
  typedef std::weak_ptr<std::deque<std::shared_ptr<ModbusPkg> > > wDequeHandle;

  ModbusServer(const std::string& ip = "127.0.0.1", int port = 1504);
  
  std::string ipAddress() const { return ipAddress_; }
  int port() const { return port_; }
  wDequeHandle getQueue() { return wDequeHandle(commQueue_); };

  int open();
  void close();
  int run();
  
  int read(std::shared_ptr<ModbusPkg>&);

private:
  modbus_t *ctx_;
  std::string ipAddress_;  // Device ip address
  int port_;               // Modbus port default 502
  sDequeHandle commQueue_; // update queue handle
  
  void print_(const std::string&) const;
};

}

#endif
