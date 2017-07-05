/**
	TagEngine.cpp
	Implements a scan engine for ModbusPkg, updating through a ModbusComm class.

	@author Owen Edgerton
	@version 1.0 7/3/17

*/

#include "TagEngine.hpp"

#include <string>
#include <queue>
#include <iostream>

namespace graComm {

TagEngine::TagEngine() : pkgs_(), commQueue_()
{
    std::cout << "TagEngine: Constructor call" << std::endl;
}

/** Create a constructor that can initalize a weak_ptr from a unique_ptr
*/
TagEngine::TagEngine(const std::shared_ptr<std::deque<std::shared_ptr<ModbusPkg> > >& spcq)
: pkgs_(), commQueue_(spcq)
{
	std::cout << "TagEngine: Constructor call w/ queue" << std::endl;
	if( auto spcq = commQueue_.lock() )
		std::cout << "TagEngine: Communication Queue linked to TagEngine" << std::endl;
}

TagEngine& TagEngine::addPkg(std::shared_ptr<ModbusPkg> pkg) {
    pkgs_.push_back(pkg);
    return *this;
}

int TagEngine::run() {
	std::cout << "TagEngine: calling run()" << std::endl;
    for(iterator it = pkgs_.begin(); it != pkgs_.end(); ++it) {
		if(auto spcq = commQueue_.lock()) {
			spcq->push_back(*it);
		}
	}
	return 0;
}

int TagEngine::stop() {
	return 0;
}
}