#ifndef GUARD_GRAAI_TAGENGINE_HPP_
#define GUARD_GRAAI_TAGENGINE_HPP_

#include <list>
#include <queue>
#include <memory>

#include "ModbusPkg.hpp"

namespace graComm {

class TagEngine {
/* TagEngine for asyncronous modbus communication */

public:
  typedef std::list<std::shared_ptr<ModbusPkg> >::iterator iterator;
  typedef const std::list<std::shared_ptr<ModbusPkg> >::const_iterator const_iterator;

  TagEngine();
  TagEngine(const std::shared_ptr<std::deque<std::shared_ptr<ModbusPkg> > >&);
  ~TagEngine() { }	

  int run(bool*);
  int print();

  TagEngine& addPkg(std::shared_ptr<ModbusPkg>);

private:
/**
Owns a list of ModbusPkg
ModbusPkgs should be associated with a Process Name, used
to determine which communicating object they should be sent to. */
  std::list<std::shared_ptr<ModbusPkg> > pkgs_;
	
/** Pointer to the communicating object's deque. */
  std::weak_ptr<std::deque<std::shared_ptr<ModbusPkg> > > commQueue_;

  bool runUpdateLoop_;
};

}

#endif
