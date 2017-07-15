#include "ModbusLib.hh"

#include <iomanip>
#include <iostream>

#include "ModbusPkg.hh"
#include "ModbusServer.hh"

namespace graComm {

ModbusLib::ModbusLib()
: pkgs_(),
  server_(),
  updateMutex_()
{ }

ModbusLib& ModbusLib::addServer(std::shared_ptr<ModbusServer> spMS) {
  server_ = spMS;
  print_("server updated");
  return *this;
}

ModbusLib& ModbusLib::addPkg(std::shared_ptr<ModbusPkg> pkg) {
  pkgs_.push_back(pkg);
  print_("package list updated");
  return *this;
}

/* enque tags owned by this library into the server's consumer queue; */
ModbusLib& ModbusLib::updateLibTags() {
  std::lock_guard<std::mutex> lock(this->updateMutex_);
  if(auto qh = server_->getQueue().lock()) {
    for (auto pkg : pkgs_) {
      qh->push_back(pkg);
    }
  server_->run(); 
  }
  return *this;
}

void ModbusLib::print_(const std::string& s) const {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t); 
  std::cout << "[" << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "] " 
	    << "Lib: " << s  <<  std::endl;
}

void ModbusLib::whois() const {
  std::cout << this << std::endl;
}

}
