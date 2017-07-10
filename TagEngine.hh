#ifndef GUARD_GRAAI_TAGENGINE_HPP_
#define GUARD_GRAAI_TAGENGINE_HPP_

#include <list>
#include <queue>
#include <memory>

#include "ModbusPkg.hh"
#include "ModbusLib.hh"

namespace graComm {

class TagEngine {
/* TagEngine for asyncronous modbus communication */

public:
  typedef std::list<std::shared_ptr<ModbusLib> >::iterator iterator;
  typedef const std::list<std::shared_ptr<ModbusLib> >::const_iterator const_iterator;
  // typedef std::shared_ptr<std::deque<std::shared_ptr<ModbusLib> > > sDequeHandle;
  // typedef std::weak_ptr<std::deque<std::shared_ptr<ModbusLib> > > wDequeHandle;

  TagEngine();
  // TagEngine(const sDequeHandle&);
  ~TagEngine() { }	

  iterator begin() { return libs_.begin(); }
  const_iterator begin() const { return libs_.begin(); }
  iterator end() { return libs_.end(); }
  const_iterator end() const { return libs_.end(); }

  size_t size() { return libs_.size(); }
  int run(bool*);

  TagEngine& addLibrary(std::shared_ptr<ModbusLib>);

  // TagEngine& addServer(int, const sDequeHandle&);
  // TagEngine& addPkg(std::shared_ptr<ModbusPkg>);

private:
/**
Owns a list of ModbusPkg
ModbusPkgs should be associated with a Process Name, used
to determine which communicating object they should be sent to. */
  //std::list<std::shared_ptr<ModbusPkg> > pkgs_;

/** Owns a list of ModbusLibs */
  std::list<std::shared_ptr<ModbusLib> > libs_;

/** Pointer to the communicating object's deque. */
//  wDequeHandle commQueue_;
//  std::unordered_map<int, wDequeHandle> commQueueMap_;
  
  void print(const std::string&) const;
};

}

#endif
