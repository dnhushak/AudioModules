#include "Parameter.h"

namespace device {
	
	Parameter::Parameter() {
		p1.i = 0;
		p2.i = 0;
	}
	
	Parameter::param Parameter::getParam1() {
		return p1;
	}
	Parameter::param Parameter::getParam2() {
		return p2;
	}

	void Parameter::setParam1(int newParam) {
		p1.i = newParam;
	}
	void Parameter::setParam1(long newParam) {
		p1.l = newParam;
	}
	void Parameter::setParam1(float newParam) {
		p1.f = newParam;
	}
	void Parameter::setParam1(double newParam) {
		p1.d = newParam;
	}

	void Parameter::setParam2(int newParam) {
		p2.i = newParam;
	}
	void Parameter::setParam2(long newParam) {
		p2.l = newParam;
	}
	void Parameter::setParam2(float newParam) {
		p2.f = newParam;
	}
	void Parameter::setParam2(double newParam) {
		p2.d = newParam;
	}

	Parameter::~Parameter() {
	}

}
