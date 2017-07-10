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

namespace graComm {

typedef  std::shared_ptr<std::deque<std::shared_ptr<ModbusPkg> > > queueHandle;

TagEngine::TagEngine() 
: pkgs_(), commQueue_(), runUpdateLoop_(false)
{
  std::cout << "TagEngine: Constructor call" << std::endl;
}

/** 
  Create a constructor that can initalize a weak_ptr from a shared_ptr */
TagEngine::TagEngine(const queueHandle& qh)
: pkgs_(), commQueue_(qh), runUpdateLoop_(false) 
{
  std::cout << "TagEngine: Constructor call w/ queue" << std::endl;
  if(auto qh = commQueue_.lock()) {
    std::cout << "TagEngine: Communication Queue linked to TagEngine" << std::endl;
  } else {
    std::cout << "TagEngine: Failed to link Communication Queue with TagEngine" << std::endl;
  }
}

/** Add a ModbusPkg to the TagEngine list.
*/
TagEngine& TagEngine::addPkg(std::shared_ptr<ModbusPkg> pkg) {
  pkgs_.push_back(pkg);
  return *this;
}

int TagEngine::run(bool* running) {
  std::cout << "TagEngine: call run()" << std::endl;
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

}
