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
#include <thread>
#include <unistd.h>

namespace graComm {

/**
Constructor */
TagEngine::TagEngine(const int updateRate_ms) 
: libs_(),
  updateRate_ms_(updateRate_ms)
{ }

TagEngine& TagEngine::addLibrary(std::shared_ptr<ModbusLib> lib) {
  libs_.push_back(lib);
  print_("lib list updated");
  return *this;
}

int TagEngine::updateTags(bool* running) {
  while(*running) {
    std::vector<std::thread> libThreads;
    for(auto lib : libs_) {
      libThreads.push_back(std::thread(&ModbusLib::updateLibTags, lib));
    }

    for(auto it = libThreads.begin(); it != libThreads.end(); ++it) {
      it->join();
    }    
    sleep(5);
  }
  return 0;
}

void TagEngine::print_(const std::string& s) const {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t); 
  std::cout << "[" << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "] " 
	    << "TagEngine: " << s  <<  std::endl;
}

}
