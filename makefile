CXX= g++
CXXSOURCES= comm.cpp TagEngine.cpp ModbusPkg.cpp
CXXOBJECTS= $(patsubst %.cpp, %.o, $(CXXSOURCES))
OPTIMFLAGS= -g -O
PACKAGES= libmodbus 
PKGCONFIG= pkg-config
CXXFLAGS= -std=c++1y -Wall $(OPTIMFLAGS)
CPPFLAGS:=  $(shell $(PKGCONFIG) --cflags $(PACKAGES))
LIBES:= $(shell $(PKGCONFIG) --libs $(PACKAGES)) -ldl
.PHONY: all clean

test_comm: test_comm.o $(CXXOBJECTS)
	$(LINK.cc) -rdynamic $^ $(LIBES) -o $@

test_TagEngine: test_TagEngine.o $(CXXOBJECTS)
	$(LINK.cc) -rdynamic $^ $(LIBES) -o $@

test_ModbusPkg: test_ModbusPkg.o $(CXXOBJECTS)
	$(LINK.cc) -rdynamic $^ $(LIBES) -o $@

clean:
	    $(RM) *.o *.so *.orig *~ base core* *.hh.gch
