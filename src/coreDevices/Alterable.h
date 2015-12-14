#ifndef ALTERABLE_H_
#define ALTERABLE_H_

#include "Parameter.h"

namespace device {
	using namespace device;
	using namespace std;

	/**
	 * ```Alterable``` is a mixin style class enhancer, adding functions to whatever class is defined as its inherit type
	 * This class provides a generalized public function intended to alter a class's member variables.
	 *
	 * For example: a ```Circle``` and ```Square``` class exist, each with their own member variables (```radius``` for the circle,
	 * and ```width``` and ```height``` for the square), as well as set functions (```setRadius(int rad)``` for example) for said
	 * member variables. If there is a ```ShapeModifier``` class, the ```alter``` function would make it much easier to alter the
	 * shapes, without the ```ShapeModifier``` needing to know the specific public function names to alter the shapes' member variables.
	 */
	template<class InheritType>
	class Alterable: public InheritType {
		public:
			Alterable() {
				numParameters = 0;
			}

			virtual void alter(int paramNum, Parameter p) = 0;

			virtual ~Alterable() {

			}
		protected:
			int numParameters;
	};

}

#endif /* ALTERABLE_H_ */
