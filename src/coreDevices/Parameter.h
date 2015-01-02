#ifndef PARAMETER_H_
#define PARAMETER_H_

namespace device {
	
	class Parameter {
			union param {
					int i;
					long l;
					float f;
					double d;
			};
		public:
			Parameter();

			param getParam1();
			param getParam2();

			void setParam1(int newParam);
			void setParam1(long newParam);
			void setParam1(float newParam);
			void setParam1(double newParam);

			void setParam2(int newParam);
			void setParam2(long newParam);
			void setParam2(float newParam);
			void setParam2(double newParam);

			virtual ~Parameter();

		private:
			param p1;
			param p2;

	};

}

#endif /* PARAMETER_H_ */
