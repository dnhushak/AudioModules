float * Envelope::advance() {

	// This is the ADSR "state machine"
	// Attack goes from 0 volume to 1
	// Decay goes from 1 to the Sustain volume
	// Release goes from the Sustain volume to 0
	// Below is a graph of a notes volume going through the states of ADSR
	//
	//     /\
	//    /  \__________
	//   /              \
	//  /                \
	// /                  \
	//
	// | A |D|    S    | R |

	switch (state) {
		case ATTACK:
			envmult += Aslope;
			// When the evelope location has hit the number of samples, do a state transition
			if (envloc >= AsampCount) {
				state = DECAY;
				envloc = 0;
			}
			break;

		case DECAY:
			envmult += Dslope;
			// When the evelope location has hit the number of samples, do a state transition
			if (envloc >= DsampCount) {
				state = SUSTAIN;
				envloc = 0;
			}
			break;

		case SUSTAIN:
			// Sustain won't automatically transition the state. The state will change on note release.
			envmult = sustain;
			break;

		case RELEASE:
			envmult += Rslope;
			// When the evelope location has hit the number of samples, do a state transition
			if (envloc >= RsampCount) {
				state = CLEANUP;
				break;
			}
	}
	envloc++;
}
