#ifndef AUDIOPROCESSOR_H_
#define AUDIOPROCESSOR_H_

namespace audio {
	
	class AudioProcessor {
		public:
			AudioProcessor();
			virtual void process(
					const sample_t * * inputBuffers,
					int numInBuffers,
					int numInChannels,
					sample_t * * outputBuffers,
					int numOutBuffers,
					int numOutChannels,
					int numSamples);

			virtual ~AudioProcessor();
	};

}

#endif /* AUDIOPROCESSOR_H_ */
