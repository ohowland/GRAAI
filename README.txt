Class ModbusTag
-ModbusTag defines Modbus TCP configuration data.
{name, address, type, access}

@name ModbusPkg
- ModbusPkg composed of vector<ModbusTag> containing contigious register modbus data for a single read or write request.
- Allocates space for returned message

@name TagEngine
vector<ModbusPkg> 
- Facilitates scan read by other system resources.

@name ModbusComm
- A queue takes ModbusConfig's and puts them through the ModbusComm object
- this object owns the ctx_ = modbus_new_tcp()




