#ifndef GUARD_GRAAI_MODBUSLIB_HH_
#define GUARD_GRAAI_MODBUSLIB_HH_

#include <list>
#include "ModbusPkg.hh"
#include "ModbusServer.hh"

namespace graComm {

class ModbusLib { 
public:
  ModbusLib();
  ~ModbusLib() { };

  ModbusLib& addServer(std::shared_ptr<ModbusServer>);
  ModbusLib& addPkg(std::shared_ptr<ModbusPkg>);

private:
  std::list<std::shared_ptr<ModbusPkg> > pkgs_;
  std::shared_ptr<ModbusServer> server_;
  
  void print(const std::string&) const;
};


}

#endif
