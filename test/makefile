CXX= g++
CXXSOURCES= ./src/ModbusServer.cc ./src/TagEngine.cc ./src/ModbusPkg.cc
CXXOBJECTS= $(patsubst %.cc, %.o, $(CXXSOURCES))
OPTIMFLAGS= -g -O
PACKAGES= libmodbus 
PKGCONFIG= pkg-config
CXXFLAGS= -std=c++1y -Wall -pthread $(OPTIMFLAGS)
CPPFLAGS:=  $(shell $(PKGCONFIG) --cflags $(PACKAGES))
LIBES:= $(shell $(PKGCONFIG) --libs $(PACKAGES)) -ldl
.PHONY: all clean

GRAAI: GRAAI.o $(CXXOBJECTS)
	$(LINK.cc) -rdynamic $^ $(LIBES) -o $@

test_server: test_server.o $(CXXOBJECTS)
	$(LINK.cc) -rdynamic $^ $(LIBES) -o $@

clean:
	    $(RM) *.o *.so *.orig *~ base core* *.hh.gch
