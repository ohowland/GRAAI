/** 
  TagEngine.cpp
  Implements a scan engine for ModbusPkg, updating through a ModbusComm class.

  @author Owen Edgerton
  @version 1.0 7/3/17 */

#include "TagEngine.hh"

#include <string>
#include <queue>
#include <iostream>
#include <iomanip>

namespace graComm {

/**
Constructor */
TagEngine::TagEngine() 
: libs_()
{ }

TagEngine& TagEngine::addLibrary(std::shared_ptr<ModbusLib> lib) {
  libs_.push_back(lib);
  print("lib list updated");
  return *this;
}

int TagEngine::run(bool* running) {
  while(*running) {
    // TODO: call update on libs contained in libs_
  }
  return 0;
}

void TagEngine::print(const std::string& s) const {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t); 
  std::cout << "[" << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "] " 
	    << "TagEngine: " << s  <<  std::endl;
}

}
