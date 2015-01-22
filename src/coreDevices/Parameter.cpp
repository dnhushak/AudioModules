#include "Parameter.h"

namespace device {
	
	Parameter::Parameter() {
		param start;
		// Zero the first param
		memset(&start, 0, sizeof(start));
		paramList.push_back(start);
	}
	
	Parameter::param Parameter::getParam(int paramNum) {
		if (paramNum < getNumParams()) {
			return paramList[paramNum];
		} else {
			param empty;
			empty.i = 0;
			return empty;
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
			param newParam;
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
			param newParam;
			memset(&newParam, 0, sizeof(newParam));
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
			param newParam;
			memset(&newParam, 0, sizeof(newParam));
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
			param newParam;
			memset(&newParam, 0, sizeof(newParam));
			newParam.d = newParamVal;
			paramList.push_back(newParam);
		}
	}

	int Parameter::getNumParams() {
		return paramList.size();
	}

	Parameter::~Parameter() {
	}

}
