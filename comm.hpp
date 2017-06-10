#ifndef GUARD_GRAAI_COMMIMPL_HPP_
#define GUARD_GRAAI_COMMIMPL_HPP_

#include <string>
#include <modbus.h>

namespace graComm {

class CommunicationCore {
/* Base class for objects handling pysical layer communication */

public:
	CommunicationCore();
	virtual ~CommunicationCore() { }

	virtual CommunicationCore& open() { return *this; }
	virtual void close() = 0; 

	std::string processName() const { return processName_; }

protected:

private:
	std::string processName_; // communication process name

};

class ModbusCommunication: public CommunicationCore {
/* Child class for modbus communication */

public:
	ModbusCommunication();
	std::string ipAddress() const { return ipAddress_; }
	int port() const { return port_; }

	ModbusCommunication& open();
	void close(); 
	int read();

protected:

private:
	modbus_t *ctx_;
	std::string ipAddress_;
	int port_;

};

/* use templating for dynamically allocating space for
 * the modbus dest data

template <class T>
void lst<T>::uncreate() {
	if (data) {
		for (iterator iter = data; iter != avail; ++iter)
			alloc.destroy(iter);
		alloc.deallocate(data, avail - data);
	}

	data = avail = 0;
}

template <class T>
void lst<T>::create(size_type n) {
	data = alloc.allocate(n);
	avail = data + n;
}

*/

}

#endif
