#ifndef PARAMETER_H_
#define PARAMETER_H_

#include <vector>

namespace device {
	
	/**
	 * A container class for storing multiple values of multiple types.
	 * Heavily utilized in the ```Alterable``` line of thinking, by exposing
	 * several functions to a generic style of parameterization.
	 */
	class Parameter {
			union param {
					int i;
					long l;
					float f;
					double d;
			};
		public:
			Parameter();

			/**
			 * Get a desired parameter.
			 *
			 * Note! After calling, must use the union qualifier to actually extract a value
			 * (for instance, ```param.i``` to get the int value)
			 *
			 * @param paramNum The parameter number to extract, for multi-valued parameters
			 * @return The desired parameter
			 */
			param getParam(int paramNum);

			/**
			 * Get the first parameter
			 *
			 * Note! After calling, must use the union qualifier to actually extract a value
			 * (for instance, ```param.i``` to get the int value)
			 *
			 * @return The first parameter
			 */
			param getParam();


			void setParam(int paramNum, int newParam);
			void setParam(int paramNum, long newParam);
			void setParam(int paramNum, float newParam);
			void setParam(int paramNum, double newParam);

			int getNumParams();


			virtual ~Parameter();

		private:
			param createEmptyParam();

			std::vector<param> paramList;

	};

}

#endif /* PARAMETER_H_ */
