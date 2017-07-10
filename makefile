CXX= g++
CXXSOURCES= ModbusServer.cc TagEngine.cc ModbusPkg.cc Lib.cc
CXXOBJECTS= $(patsubst %.cc, %.o, $(CXXSOURCES))
OPTIMFLAGS= -g -O
PACKAGES= libmodbus 
PKGCONFIG= pkg-config
CXXFLAGS= -std=c++1y -Wall -pthread $(OPTIMFLAGS)
CPPFLAGS:=  $(shell $(PKGCONFIG) --cflags $(PACKAGES))
LIBES:= $(shell $(PKGCONFIG) --libs $(PACKAGES)) -ldl
.PHONY: all clean

test_ModbusLib: test_ModbusLib.o $(CXXOBJECTS)
	$(LINK.cc) -rdynamic $^ $(LIBES) -o $@

test_ModbusServer: test_ModbusServer.o $(CXXOBJECTS)
	$(LINK.cc) -rdynamic $^ $(LIBES) -o $@

test_TagEngine: test_TagEngine.o $(CXXOBJECTS)
	$(LINK.cc) -rdynamic $^ $(LIBES) -o $@

test_ModbusPkg: test_ModbusPkg.o $(CXXOBJECTS)
	$(LINK.cc) -rdynamic $^ $(LIBES) -o $@

clean:
	    $(RM) *.o *.so *.orig *~ base core* *.hh.gch
