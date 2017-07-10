/** 
  TagEngine.cpp
  Implements a scan engine for ModbusPkg, updating through a ModbusComm class.

  @author Owen Edgerton
  @version 1.0 7/3/17 */

#include "TagEngine.hpp"

#include <string>
#include <queue>
#include <iostream>
#include <unistd.h>
#include <iomanip>

namespace graComm {

/**
Constructor */
TagEngine::TagEngine() 
: pkgs_(), commQueue_()
{
  std::cout << "TagEngine: Constructor call" << std::endl;
}

/** 
Constructor to initalize a weak_ptr to comm server's queue from a shared_ptr */
TagEngine::TagEngine(const sDequeHandle& sdh)
: pkgs_(), commQueue_(sdh)
{
  std::cout << "TagEngine: Constructor call w/ queue" << std::endl;
  if(auto sdh = commQueue_.lock()) {
    std::cout << "TagEngine: Communication Queue linked to TagEngine" << std::endl;
  } else {
    std::cout << "TagEngine: Failed to link Communication Queue with TagEngine" << std::endl;
  }
}

TagEngine& TagEngine::addServer(int processId, const sDequeHandle& sdh) {
  auto wdh = wDequeHandle(sdh);
  if(auto sdh = wdh.lock()) {
    auto r = commQueueMap_.insert(std::make_pair(processId, wdh));
    if(r.second == true) {
      print("Server added");
    } else {
      print("Failed to add server");
    }
  }
  return *this;
}

/** 
Add a ModbusPkg to the TagEngine list. */
TagEngine& TagEngine::addPkg(std::shared_ptr<ModbusPkg> pkg) {
  pkgs_.push_back(pkg);
  return *this;
}

int TagEngine::run(bool* running) {
  while(*running) {
    for(auto& pkg : pkgs_) {
      if(auto qh = commQueue_.lock()) {
        qh->push_back(pkg);
      }
    }
    sleep(5);
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
