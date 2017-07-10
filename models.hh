#ifndef GUARD_GRAAI_MODELS_H_
#define GUARD_GRAAI_MODELS_H_

#include <string>
#include <iostream>

namespace graModel {

class BaseAssetModel {
public:
	BaseAssetModel();
	explicit BaseAssetModel(std::string init);
	virtual ~BaseAssetModel() { }

	std::string processName() const { return processName_; }
	bool active() const { return active_; }

	virtual BaseAssetModel* clone() const 
		{ return new BaseAssetModel(*this); }

private:
	std::string processName_;
	bool active_;
};

class DieselModel: public BaseAssetModel {
public:
	DieselModel();
	explicit DieselModel(std::string init);
	~DieselModel() { }

	int ratedKW() const { return ratedKW_; }
	int ratedKVA() const { return ratedKVA_; }
	int outputKW() const { return outputKW_; }
	int outputKVAR() const { return outputKVAR_; }
	std::string state() const { return state_; }

	DieselModel* clone() const 
		{ return new DieselModel(*this); }

private:
	int ratedKW_;
	int ratedKVA_;
	int outputKW_;
	int outputKVAR_;
	std::string state_;
};


};

#endif
