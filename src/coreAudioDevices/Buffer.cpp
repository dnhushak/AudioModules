#include "Buffer.h"

namespace audio {
	
	Buffer::Buffer() {

	}

	Buffer * Buffer::clone() {
		return new Buffer();
	}

	sample_t * Buffer::advance() {
		return buffer;
	}
	
	void Buffer::setSample(sample_t sample, int bufferLoc) {
		if (bufferLoc < bufferSize) {
			buffer[bufferLoc] = sample;
		}
	}

	sample_t Buffer::getSample(int bufferLoc) {
		std::cout << "\n BufferLoc is: " << bufferLoc << "\t and BufferSize is: " << bufferSize << "\n";
		if (bufferLoc < bufferSize) {
			return buffer[bufferLoc];
		}
		else return 0;
	}

	Buffer::~Buffer() {

	}

}
