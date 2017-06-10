#include "models.hpp"

namespace graModel {

/* BASE ASSET MODEL */
BaseAssetModel::BaseAssetModel(): processName_("UNINITIALIZED"),
	   							  active_(0) {
	std::cout << "BASE_ASSET_MODEL: initializing members DEFAULT" 
			  << std::endl;
}

BaseAssetModel::BaseAssetModel(std::string init) {
	std::cout << "BASE_ASSET_MODEL: initializing members " 
			  << init << std::endl;
	
	BaseAssetModel::processName_ = init;

}

/* DIESEL MODEL */
DieselModel::DieselModel(): ratedKW_(-1),
							ratedKVA_(-1),
							outputKW_(-1),
							outputKVAR_(-1), 
							state_("INITALIZE") {
	
	std::cout << "DIESEL_MODEL: initializing members DEFAULT" 
			  << std::endl;
}

DieselModel::DieselModel(std::string init): BaseAssetModel(init) {
	std::cout << "DIESEL_MODEL: initailizing members "
			  << init << std::endl;
}
}
