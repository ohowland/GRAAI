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

/** LibraryBase Class
    Abstract base class for Library. Provides member functions to add a server */
class LibraryBase {
public:
  LibraryBase() { };
  virtual ~LibraryBase() { };
  
  template<class T, class S> T& addServer(std::shared_ptr<S>);
  template<class T, class P> T& addPkg(std::shared_ptr<P>);
  virtual std::shared_ptr<LibraryBase> update(std::shared_ptr<LibraryBase>) = 0;
};

template<class T, class S> 
T& LibraryBase::addServer(std::shared_ptr<S> server) { return dynamic_cast<T&>(*this).addServer(server); }

template<class T, class P>
T& LibraryBase::addPkg(std::shared_ptr<P> package) { return dynamic_cast<T&>(*this).addPkg(package); }

/** Library Class
    Template class derived from LibraryBase. */
template<class S, class P>
class Library : public LibraryBase {
public:
  Library();
  ~Library() { };

  Library& addServer(std::shared_ptr<S>);
  Library& addPkg(std::shared_ptr<P>);
  std::shared_ptr<LibraryBase> update(std::shared_ptr<LibraryBase>);
  void print_(const std::string&) const;

private:
  std::list<std::shared_ptr<P> > pkgs_;
  std::shared_ptr<S> server_;
  std::mutex pkgListMutex_;
};

template<class Server, class Package>
Library<Server, Package>::Library()
: pkgs_(),
  server_(),
  pkgListMutex_()
{ };

template<class Server, class Package>
Library<Server, Package>& Library<Server, Package>::addServer(std::shared_ptr<Server> spMS) {
  server_ = spMS;
  print_("server updated");
  return *this;
}

template<class Server, class Package>
Library<Server, Package>& Library<Server, Package>::addPkg(std::shared_ptr<Package> pkg) {
  pkgs_.push_back(pkg);
  print_("package list updated");
  return *this;
}

template<class Server, class Package>
std::shared_ptr<LibraryBase> Library<Server, Package>::update(std::shared_ptr<LibraryBase> splb) {
  std::lock_guard<std::mutex> lock_here(this->pkgListMutex_);
  if(auto qh = server_->getQueue().lock()) {
    for (auto pkg : pkgs_) {
      std::lock_guard<std::mutex> lock_there(server_->pkgQueueMutex());
      qh->push_back(pkg);
    }
  server_->run();
  }
  return splb;
}

template<class Server, class Package>
void Library<Server, Package>::print_(const std::string& s) const {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t); 
  std::cout << "[" << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "] " 
	    << "Lib: " << s  <<  std::endl;
}

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
