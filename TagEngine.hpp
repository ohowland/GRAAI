#ifndef GUARD_GRAAI_TAGENGINE_HPP_
#define GUARD_GRAAI_TAGENGINE_HPP_

#include <list>
#include <queue>
#include <memory>
#include <unordered_map>

#include "ModbusPkg.hpp"

namespace graComm {

class TagEngine {
/* TagEngine for asyncronous modbus communication */

public:
  typedef std::list<std::shared_ptr<ModbusPkg> >::iterator iterator;
  typedef const std::list<std::shared_ptr<ModbusPkg> >::const_iterator const_iterator;
  typedef std::shared_ptr<std::deque<std::shared_ptr<ModbusPkg> > > sDequeHandle;
  typedef std::weak_ptr<std::deque<std::shared_ptr<ModbusPkg> > > wDequeHandle;

  TagEngine();
  TagEngine(const sDequeHandle&);
  ~TagEngine() { }	

  iterator begin() { return pkgs_.begin(); }
  const_iterator begin() const { return pkgs_.begin(); }
  iterator end() { return pkgs_.end(); }
  const_iterator end() const { return pkgs_.end(); }

  size_t size() { return pkgs_.size(); }
  int run(bool*);

  TagEngine& addServer(int, const sDequeHandle&);
  TagEngine& addPkg(std::shared_ptr<ModbusPkg>);

private:
/**
Owns a list of ModbusPkg
ModbusPkgs should be associated with a Process Name, used
to determine which communicating object they should be sent to. */
  std::list<std::shared_ptr<ModbusPkg> > pkgs_;
	
/** Pointer to the communicating object's deque. */
  wDequeHandle commQueue_;
  std::unordered_map<int, wDequeHandle> commQueueMap_;
  
  void print(const std::string&) const;
};

}

#endif
