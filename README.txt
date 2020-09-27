# GRAAI

In its current state GRAAI is a communications TagEngine. The engine subscribes to policy-driven libraries, meant to provide an interface to implement different communication protocols by defining a "Protocol Server" to handle communication and "Protocol Pkg" to handle the configuration and datastorage. 

Currently only Modicon MODBUS is implemented, see ModbusServer.cc, ModbusPkg.cc, and ModbusPlug.hh.

Dependencies: libmodbus

