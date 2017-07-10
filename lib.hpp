#ifndef GUARD_GRAAI_LIB_HPP
#define GUARD_GRAAI_LIB_HPP

namespace graComm {

class Lib {
public:
  Lib();
  ~Lib() { };
  virtual Lib& getServer() = 0;
  virtual Lib& getPkgs() = 0;

};

class ModbusLib : public Lib {
public:
  ModbusLib();
  ~ModbusLib() { };

  ModbusLib& getServer();
  ModbusLib& getPkgs();

private:

};


}

#endif
