//Audio Object Interface
using namespace chip;

class IAudio:
{
	//no member variables
	public:
		//function that advances..things! Will be different for each IAudio implementation
		float[] advance(int);
}