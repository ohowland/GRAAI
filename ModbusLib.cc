#include "ModbusLib.hh"

#include <iomanip>
#include <iostream>

#include "ModbusPkg.hh"
#include "ModbusServer.hh"

namespace graComm {

ModbusLib::ModbusLib()
: pkgs_(),
  server_()
{ }

ModbusLib& ModbusLib::addServer(std::shared_ptr<ModbusServer> spMS) {
  server_ = spMS;
  print("server updated");
  return *this;
}

ModbusLib& ModbusLib::addPkg(std::shared_ptr<ModbusPkg> pkg) {
  pkgs_.push_back(pkg);
  print("package list updated");
  return *this;
}

void ModbusLib::print(const std::string& s) const {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t); 
  std::cout << "[" << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "] " 
	    << "Lib: " << s  <<  std::endl;
}

}
