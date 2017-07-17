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
  updateLibs_(),
  updateLibs_mutex_(),
  updateRate_ms_(updateRate_ms)
{ }

TagEngine& TagEngine::addLibrary(std::shared_ptr<ModbusLib> lib) {
  libs_.push_back(lib);
  print_("lib list updated");
  return *this;
}

int TagEngine::enqueLib(std::future<std::shared_ptr<ModbusLib> > futureLib) {
  auto spml = futureLib.get();                          // spml: shared pointer to ModbusLib
  std::lock_guard<std::mutex> lock(updateLibs_mutex_);  // lock ulibs_
  updateLibs_.push_back(spml);                          // reinsert ModbusLib 
  return 0;
}

int TagEngine::updateTags(bool* running) {
  updateLibs_.clear();
  for(auto& lib : libs_) {
    updateLibs_.push_back(lib); 
  }

  while(*running) {
    updateLibs_mutex_.lock();
    for(auto it = updateLibs_.begin(); it != updateLibs_.end(); ++it) {
      /* package a ModbusLib::updateLibTags task for separate thread execution. */
      std::packaged_task<std::shared_ptr<ModbusLib>(ModbusLib*, std::shared_ptr<ModbusLib>)> updateLib_task(&ModbusLib::updateLibTags);
      /* create a future promised a shared_ptr<ModbusLib> that has been updated */
      std::future<std::shared_ptr<ModbusLib> > updateLib_future = updateLib_task.get_future();

      /* Modbuslib::updateLibTags thread: */
      std::thread(std::move(updateLib_task), it->get(), *it).detach();

      /* Enque returned shared_ptr<ModbusLib> to the updateLibs list (for continued update) */
      std::thread(&TagEngine::enqueLib, this, std::move(updateLib_future)).detach();
      
      updateLibs_.pop_front();
    }
    updateLibs_mutex_.unlock();
    usleep(updateRate_ms_*1000);
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
