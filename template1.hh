#ifndef GUARD_TEMPLATE1_HH_
#define GUARD_TEMPLATE1_HH_

#include <iostream>
#include <vector>

class LibraryBase {
public:
  LibraryBase() { };
  virtual ~LibraryBase() { };
  template<class T> const T& getServer() const;
  template<class T, class S> void setServer(const S& rhs);

};

template<class T>
class Library : public LibraryBase {
public:
  Library(const T& rhs) : lib_(rhs) {}
  const T& getLib() const { return lib_; };
  void addLib(const T& rhs) { lib_ = rhs; };
private:
  T lib_; // this can be a ModbusLib, or CanLib

};

template<class T> const T& LibraryBase::getServer() const { 
	return dynamic_cast<const Library<T>&>(*this).getLib(); }

template<class T, class S> void LibraryBase::setServer(const S& rhs) { 
	return dynamic_cast<Library<T>&>(*this).setLib(rhs); }
#endif
