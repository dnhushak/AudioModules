#include "Parameter.h"

namespace device {
	
	Parameter::Parameter() {

		paramList.push_back(createEmptyParam());
	}
	
	Parameter::param Parameter::getParam(int paramNum) {
		if (paramNum < getNumParams()) {
			return paramList[paramNum];
		} else {
			return createEmptyParam();
		}
	}

	Parameter::param Parameter::getParam() {
		return paramList[0];
	}

	void Parameter::setParam(int paramNum, int newParamVal) {
		// If param exists in the vector
		if (paramNum < getNumParams()) {
			// Update it
			paramList[paramNum].i = newParamVal;
		} else {
			// Otherwise add a new parameter
			param newParam = createEmptyParam();
			newParam.i = newParamVal;
			paramList.push_back(newParam);
		}
	}
	void Parameter::setParam(int paramNum, long newParamVal) {
		// If param exists in the vector
		if (paramNum < getNumParams()) {
			// Update it
			paramList[paramNum].l = newParamVal;
		} else {
			// Otherwise add a new parameter
			param newParam = createEmptyParam();
			newParam.l = newParamVal;
			paramList.push_back(newParam);
		}
	}
	void Parameter::setParam(int paramNum, float newParamVal) {
		// If param exists in the vector
		if (paramNum < getNumParams()) {
			// Update it
			paramList[paramNum].f = newParamVal;
		} else {
			// Otherwise add a new parameter
			param newParam = createEmptyParam();
			newParam.f = newParamVal;
			paramList.push_back(newParam);
		}
	}
	void Parameter::setParam(int paramNum, double newParamVal) {
		// If param exists in the vector
		if (paramNum < getNumParams()) {
			// Update it
			paramList[paramNum].d = newParamVal;
		} else {
			// Otherwise add a new parameter
			param newParam = createEmptyParam();
			newParam.d = newParamVal;
			paramList.push_back(newParam);
		}
	}

	int Parameter::getNumParams() {
		return paramList.size();
	}

	Parameter::param Parameter::createEmptyParam() {
		param empty;
		// Zero the  param
		memset(&empty, 0, sizeof(empty));
		return empty;
	}

	Parameter::~Parameter() {
	}

}
