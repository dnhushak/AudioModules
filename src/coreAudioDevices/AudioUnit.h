#ifndef AUDIOUNIT_H_
#define AUDIOUNIT_H_

namespace audio {
	using namespace device;
	
	class AudioUnit: public Connectable<Device, AudioUnit> {
		public:
			AudioUnit();
			void setAPU(AudioProcessor * newAPU);
			void process();
			void setInputBuffer(int bufferNum);
			void setOutputBuffer(int bufferNum);
			virtual ~AudioUnit();
		private:
			AudioProcessor APU;
			int numInputs;
			int numOutputs;
			int numChannels;
			sample_t * * inputBuffers;
			sample_t * * outputBuffers;
	};

}

#endif /* AUDIOUNIT_H_ */
