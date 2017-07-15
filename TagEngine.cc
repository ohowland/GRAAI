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

// set all tags as updating (initial).
// if not updating, build thread to update tags.
// when done set as not updating.
void TagEngine::enqueLib(std::future<std::shared_ptr<ModbusLib> > futureLib) {
  futureLib.get()->whois();
}

int TagEngine::updateTags(bool* running) {
  std::vector<std::shared_ptr<ModbusLib> > updateLibs;
  for(auto& lib : libs_)
    updateLibs.push_back(lib); 

  // we want to pass the reference to the shared_ptr<ModbusLib>, not the reference to the ModbusLib itself. 
  while(*running) {
    for(auto& lib : updateLibs) {
      std::packaged_task<ModbusLib(ModbusLib*)> updateLib_task(&ModbusLib::updateLibTags);
      std::future<ModbusLib> updateLib_future = updateLib_task.get_future();

      std::thread(std::move(updateLib_task), &lib).detach();
      std::thread(&TagEngine::enqueLib, std::move(updateLib_future)).detach();
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
