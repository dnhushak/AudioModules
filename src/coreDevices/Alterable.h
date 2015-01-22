#ifndef ALTERABLE_H_
#define ALTERABLE_H_

#include "Parameter.h"
//#include <string>
#include <map>

namespace device {
	template<class InheritType>
	class Alterable: public InheritType {
		public:
			Alterable() {
				numParameters = 0;
			}

			virtual void alter(std::string paramName, device::Parameter p) = 0;

			virtual ~Alterable() {

			}
		protected:
			int numParameters;

	};

}

#endif /* ALTERABLE_H_ */
