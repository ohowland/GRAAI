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

// if If the point of TagEngine is add/remove different Library classes at runtime, then a templated design
// is probably not the right approach.
// see https://stackoverflow.com/questions/602593/template-or-abstract-base-class
// this template type design is called 'policy based'.
// Policies can be grouped together, which was the original plan for the ModbusLib (gets a server and a pkg).

/* LIBRARY CLASS
   ^^^^^^^^^^^^^ */
class LibraryBase {
public:
  LibraryBase() { };
  virtual ~LibraryBase() { };
  
  template<class T, class S> T& addServer(std::shared_ptr<S>);
  template<class T, class P> T& addPkg(std::shared_ptr<P>);
  virtual std::shared_ptr<LibraryBase> update(std::shared_ptr<LibraryBase>) = 0; // using a dynamic cast trick to make this work.
                            // what are other options?
  void whois() const;
};

template<class Server, class Package>
class Library : public LibraryBase {
public:
  Library();
  ~Library() { };

  Library& addServer(std::shared_ptr<Server>);
  Library& addPkg(std::shared_ptr<Package>);
  std::shared_ptr<LibraryBase> update(std::shared_ptr<LibraryBase>);
  void print_(const std::string&) const;

private:
  std::list<std::shared_ptr<Package> > pkgs_;
  std::shared_ptr<Server> server_;
  std::mutex updateMutex_; 
};

template<class T, class S> 
T& LibraryBase::addServer(std::shared_ptr<S> server) { return dynamic_cast<T&>(*this).addServer(server); }

template<class T, class P>
T& LibraryBase::addPkg(std::shared_ptr<P> package) { return dynamic_cast<T&>(*this).addPkg(package); }

template<class Server, class Package>
Library<Server, Package>::Library()
: pkgs_(),
  server_(),
  updateMutex_()
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
  std::lock_guard<std::mutex> lock(this->updateMutex_);
  if(auto qh = server_->getQueue().lock()) {
    for (auto pkg : pkgs_) {
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

// shared pointer objects were created to point to a derrived object type.
// which is causing problems when handing references to functions expecting
// a shared pointer to parent.
// It seems like the child object should be created and stored in a shared pointer to parent.

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
