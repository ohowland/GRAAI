#include <iostream>
#include <string>
#include <tr1/memory>

#include "models.hpp"
#include "comm.hpp"

int main() {

	/* MODELS.o */
	/* std::string initalizeString("diesel1");
	
	std::tr1::shared_ptr<graModel::DieselModel> 
	diesel1( new graModel::DieselModel(initalizeString) );
	
	std::tr1::shared_ptr<graModel::DieselModel>
	diesel2( new graModel::DieselModel() );
	
	std::cout << std::endl;
	std::cout << diesel1->processName() << ": " 
			  << "Rated kW: " << diesel1->ratedKW() 
			  << std::endl;
		
	std::cout << diesel2->processName() << ": "
			  << "Rated kW: " << diesel2->ratedKW()
			  << std::endl;
	*/

	/* COMM.o */
	std::tr1::shared_ptr<graComm::ModbusCommunication>
	comm1( new graComm::ModbusCommunication() );

	comm1->open();
	comm1->read();
	return 0;
}
