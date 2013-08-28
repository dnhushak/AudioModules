//Audio Object Interface
using namespace chip;

class IAudio
{
	//no member variables
	public:
		//Will be different for each IAudio implementation
		std::vector<float> advance(int);
};