/** 
  TagEngine.cpp
  Implements a scan engine for ModbusPkg, updating through a ModbusComm class.

  @author Owen Edgerton
  @version 1.0 7/3/17 */

#include "TagEngine.hh"

#include <queue>
#include <iostream>
#include <iomanip>
#include <thread>
#include <unistd.h>
#include <algorithm>
#include <future>

#include "ModbusPlugin.hh"

namespace graComm {

typedef std::shared_ptr<ServerBase> spServer;
typedef std::shared_ptr<PkgBase> spPkg;
typedef std::shared_ptr<LibraryBase> spLib;

/* Library
   ^^^^^^^ */
Library::Library()
: pkgs_(),
  server_(),
  pkgListMutex_()
{ };

Library& Library::addServer(spServer server) {
  server_ = server;
  print_("server updated");
  return *this;
}

Library& Library::addPkg(spPkg pkg) {
  pkgs_.push_back(pkg);
  print_("package list updated");
  return *this;
}

spLib Library::update(spLib lib) {
  std::lock_guard<std::mutex> lock_here(this->pkgListMutex_);
  if(auto qh = server_->getQueue().lock()) {
    for (auto pkg : pkgs_) {
      std::lock_guard<std::mutex> lock_there(server_->queueMutex());
      qh->push_back(pkg);
    }
  server_->run();
  }
  return lib;
}

void Library::print_(const std::string& s) const {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t); 
  std::cout << "[" << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "] " 
	    << "Lib: " << s  <<  std::endl;
}

	
/* TAG ENGINE
   ^^^^^^^^^^ */
TagEngine::TagEngine(const int updateRate_ms) 
: libs_(),
  updateLibs_(),
  updateLibs_mutex_(),
  updateRate_ms_(updateRate_ms)
{ }

TagEngine& TagEngine::addLibrary(std::shared_ptr<LibraryBase> lib) {
  libs_.push_back(lib);
  print_("lib list updated");
  return *this;
}

/** enqueLib
  enqueLib waits on a future LibraryBase reference, and enques the Library when it is returned.
  The work of when a Library package should be re-enqued in the main updateList is done here. */
int TagEngine::enqueLib(std::future<std::shared_ptr<LibraryBase> > futureLib) {
  auto spml = futureLib.get();                          // spml: shared pointer to ModbusLib
  std::lock_guard<std::mutex> lock(updateLibs_mutex_);  // lock ulibs_
  updateLibs_.push_back(spml);                          // reinsert ModbusLib 
  return 0;
}

/** TagEngine::update
  Main update loop in the TagEngine. Any Library reference in the updateLibs_ list will have a packaged_task
  created for it's update() member function. A future created from the packaged_task will be created as well.
  The packaged_task spins a daemon thread for update member function. The future is passed to a second daemon thread
  which calls TagEngine::enqueLib */
int TagEngine::update(bool* running) {
  updateLibs_.clear();
  for(auto& lib : libs_) {
    updateLibs_.push_back(lib); 
  }

  while(*running) {
    updateLibs_mutex_.lock();
    for(auto it = updateLibs_.begin(); it != updateLibs_.end(); ++it) {
      // package a ModbusLib::updateLibTags task for separate thread execution. 
      std::packaged_task<std::shared_ptr<LibraryBase>(LibraryBase*, std::shared_ptr<LibraryBase>) > 
                        updateLib_task(&LibraryBase::update);
      
      // create a future promised a shared_ptr<ModbusLib> that has been updated
      std::future<std::shared_ptr<LibraryBase> > updateLib_future = updateLib_task.get_future();

      // Modbuslib::updateLibTags thread:
      std::thread(std::move(updateLib_task), it->get(), *it).detach();

      // Enque returned shared_ptr<ModbusLib> to the updateLibs list (for continued update)
      std::thread(&TagEngine::enqueLib, this, std::move(updateLib_future)).detach();
  
      updateLibs_.pop_front();
    }
    updateLibs_mutex_.unlock();
    sleep(3);
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
