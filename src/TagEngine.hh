#ifndef GUARD_GRAAI_TAGENGINE_HPP_
#define GUARD_GRAAI_TAGENGINE_HPP_

#include <list>
#include <queue>
#include <memory>
#include <mutex>
#include <future>
#include <iomanip>
#include <iostream>

namespace graComm {

class PkgBase {
public:
  PkgBase() { };
  virtual ~PkgBase() { };
  
  //virtual PkgBase& PkgFactory() = 0;
};

class ServerBase {
public:
  typedef std::weak_ptr<std::deque<std::shared_ptr<PkgBase> > > wDequeHandle ;

  ServerBase() { };
  virtual ~ServerBase() { };
  
  //virtual ServerBase& ServerFactor() = 0;
  
  virtual int run() = 0;
  virtual wDequeHandle getQueue() = 0;
  virtual std::mutex& queueMutex() = 0;
};

/** LibraryBase Class
    Abstract base class for Library. Provides member functions to add a server */
class LibraryBase {
public:
  LibraryBase() { };
  virtual ~LibraryBase() { };
  
  virtual LibraryBase& addServer(std::shared_ptr<ServerBase>) = 0;
  virtual LibraryBase& addPkg(std::shared_ptr<PkgBase>) = 0;
  virtual std::shared_ptr<LibraryBase> update(std::shared_ptr<LibraryBase>) = 0;
};

/** Library Class
*/
class Library : public LibraryBase {
public:
  Library();
  ~Library() { };

  Library& addServer(std::shared_ptr<ServerBase>);
  Library& addPkg(std::shared_ptr<PkgBase>);
  std::shared_ptr<LibraryBase> update(std::shared_ptr<LibraryBase>);
  void print_(const std::string&) const;

private:
  std::list<std::shared_ptr<PkgBase> > pkgs_;
  std::shared_ptr<ServerBase> server_;
  std::mutex pkgListMutex_;
};

/* TAG ENGINE CLASS
   ^^^^^^^^^^^^^^^^ */
class TagEngine {
/* TagEngine for asyncronous modbus communication */

public:
  typedef std::list<std::shared_ptr<LibraryBase> >::iterator iterator;
  typedef const std::list<std::shared_ptr<LibraryBase> >::const_iterator const_iterator;

  TagEngine(const int updateRate_ms = 1000);
  ~TagEngine() { }	

  iterator begin() { return libs_.begin(); }
  const_iterator begin() const { return libs_.begin(); }
  iterator end() { return libs_.end(); }
  const_iterator end() const { return libs_.end(); }

  size_t size() { return libs_.size(); }
  
  int update(bool*); // Start the main TagEngine update loop.
  TagEngine& addLibrary(std::shared_ptr<LibraryBase>);      // Add a new Lib to the TagEngine Lib repository.
  int enqueLib(std::future<std::shared_ptr<LibraryBase> >); // Enque a Lib into the TagEngine update queue.

private:
  std::list<std::shared_ptr<LibraryBase> > libs_;        // Contains Server and Pkgs
  std::deque<std::shared_ptr<LibraryBase> > updateLibs_; // change to weak_ptr?
  std::mutex updateLibs_mutex_;                         // Mutex for the updateLibs_ queue
  int const updateRate_ms_;                             // Update rate for TagEngine.
  
  void print_(const std::string&) const;
  
};

}

#endif
