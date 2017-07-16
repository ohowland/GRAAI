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
#include <algorithm>
#include <future>

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

// producer: TagEngine calls for Tags to be updated (through ModbusLibs)
// worker: ModbusServer async communication.
// consumer: Confirms ModbusLibs are updated.

int TagEngine::enqueLib(std::future<std::shared_ptr<ModbusLib> > futureLib, std::vector<std::shared_ptr<ModbusLib> >* updateLibs) {
  // Why can't I pass a reference to update libs?
  // pointer works fine.
  // will need to mutex updateLibs so that two calls to enqueLib do not create a race condition.
  updateLibs->push_back(futureLib.get());
  return 0;
}

int TagEngine::updateTags(bool* running) {
  std::vector<std::shared_ptr<ModbusLib> > updateLibs;
  
  for(auto& lib : libs_)
    updateLibs.push_back(lib); 

  while(*running) {
    //for(auto it = updateLibs.begin(); it != updateLibs.end(); ++it)
    for(auto lib : updateLibs) {
      std::packaged_task<std::shared_ptr<ModbusLib>(ModbusLib*, std::shared_ptr<ModbusLib>)> updateLib_task(&ModbusLib::updateLibTags);
      std::future<std::shared_ptr<ModbusLib> > updateLib_future = updateLib_task.get_future();

      std::thread(std::move(updateLib_task), lib.get(), lib).detach();
      std::thread(&TagEngine::enqueLib, this, std::move(updateLib_future), &updateLibs).detach(); //why can't i pass a updateLibs ref to this thread?
      
    }
    // libs should be erased as they are returned.
    updateLibs.erase(updateLibs.begin(), updateLibs.end());
    sleep(1);
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
