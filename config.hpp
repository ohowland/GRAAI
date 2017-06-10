#ifndef GUARD_GRAAI_CONFIG_HPP_
#define GUARD_GRAAI_CONFIG_HPP_

#include <vector>
#include <iostream>

namespace graConfig {

enum requestDataType {
	U16,
	U32,
	FLOAT32
};

struct modbusData {
	std::string name,
	uint16_t modbusRegister,
	requestDataType type
};

class modbusConfig {
public:
	read

private:
	std::vector<modbusData> data;

};

}

#endif
