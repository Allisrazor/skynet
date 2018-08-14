
#include "../stdafx.h"
#include "pooling.h"
#include "snAux/auxFunc.h"
#include "SNOperator/src/structurs.h"

using namespace std;
using namespace SN_Base;

/// объединяющий слой

Pooling::Pooling(const string& name, const string& node, std::map<std::string, std::string>& prms) :
	OperatorBase(name, node, prms){
		
	load(prms);
}

void Pooling::load(std::map<std::string, std::string>& prms){
	
	baseOut_ = new Tensor();
	baseGrad_ = new Tensor();
	
	if ((prms.find("kernel") != prms.end()) && SN_Aux::is_number(prms["kernel"])){

		size_t kernel = stoi(prms["kernel"]);
		if (kernel > 0)
			kernel_ = kernel;
		else
			statusMess("Pooling::setInternPrm error: param 'kernel' <= 0");
	}
	else
		statusMess("Pooling::setInternPrm error: not found (or not numder) param 'kernel'");

	if (prms.find("poolType") != prms.end()){

		string atype = prms["poolType"];
		if (atype == "max") poolType_ = poolType::max;
		else if (atype == "avg") poolType_ = poolType::avg;
		else
			statusMess("Pooling::setInternPrm error: param 'poolType' = " + atype + " indefined");
	}
}

std::vector<std::string> Pooling::Do(const learningParam& lernPrm, const std::vector<OperatorBase*>& neighbOpr){

	if (neighbOpr.size() == 1){
		if (lernPrm.action == snAction::forward)
			forward(neighbOpr[0]->getOutput());
		else
			backward(neighbOpr[0]->getGradient(), lernPrm);
	}
	else{
		if (lernPrm.action == snAction::forward){

			inFwTns_ = *neighbOpr[0]->getOutput();

			size_t sz = neighbOpr.size();
			for (size_t i = 1; i < sz; ++i)
				inFwTns_ += *neighbOpr[i]->getOutput();

			forward(&inFwTns_);
		}
		else{

			inBwTns_ = *neighbOpr[0]->getGradient();

			size_t sz = neighbOpr.size();
			for (size_t i = 1; i < sz; ++i)
				inBwTns_ += *neighbOpr[i]->getGradient();

			backward(&inBwTns_, lernPrm);
		}
	}
	
	return std::vector<std::string>();
}

void Pooling::forward(SN_Base::Tensor* inTns){

	snSize insz = inTns->size();

	/// размер вх данных изменился?
	/*if (insz != inSzMem_){
		inSzMem_ = insz;
		updateConfig(insz);
	}*/
			
	/// расчет выходных значений нейронов
	snFloat* out = baseOut_->getData();
	//fwdFullyConnected(kernel_, insz, pDtMem, baseWeight_->getData(), out);

	
}

void Pooling::backward(SN_Base::Tensor* inTns, const learningParam& lernPrm){

	snFloat* gradIn = inTns->getData();
	
	// расчет вых градиента и коррекции весов
	snFloat* gradOut = baseGrad_->getData();
	//bwdFullyConnected(kernel_, weight, inSzMem_, inDataExp_.data(), gradIn, gradOut, dWeight);
}