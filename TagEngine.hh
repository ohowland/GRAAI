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

  TagEngine(const int updateRate_ms = 1000);
  ~TagEngine() { }	

  iterator begin() { return libs_.begin(); }
  const_iterator begin() const { return libs_.begin(); }
  iterator end() { return libs_.end(); }
  const_iterator end() const { return libs_.end(); }

  size_t size() { return libs_.size(); }
  int updateTags(bool*);

  TagEngine& addLibrary(std::shared_ptr<ModbusLib>);

private:
  std::list<std::shared_ptr<ModbusLib> > libs_; // Contains Server and Pkgs
  int const updateRate_ms_; 
  void print_(const std::string&) const;

  int startServers(bool*);
};

}

#endif
