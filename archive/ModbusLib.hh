#ifndef GUARD_GRAAI_MODBUSLIB_HH_
#define GUARD_GRAAI_MODBUSLIB_HH_

#include <list>
#include <mutex>
#include <future>

#include "ModbusPkg.hh"
#include "ModbusServer.hh"

namespace graComm {

class ModbusLib { 
public:
  ModbusLib();
  ~ModbusLib() { };

  ModbusLib& addServer(std::shared_ptr<ModbusServer>);
  ModbusLib& addPkg(std::shared_ptr<ModbusPkg>);

  std::shared_ptr<ModbusLib> updateLibTags(std::shared_ptr<ModbusLib>);

  void whois() const;

private:
  std::list<std::shared_ptr<ModbusPkg> > pkgs_;
  std::shared_ptr<ModbusServer> server_;
  std::mutex updateMutex_; 
  void print_(const std::string&) const;
};


}

#endif
