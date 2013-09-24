#include <stdio.h>
#include <portmidi.h>
#include <porttime.h>


void readMIDI(int devID) {

	int cnt, i;
	PmError retval;
	const PmDeviceInfo *info;
	PmEvent msg[32];
	PortMidiStream *mstream;
	
	Pm_Initialize();

		
		Pt_Start(devID, NULL, NULL);
		retval = Pm_OpenInput(&mstream, devID, NULL, 512L, NULL, NULL);
		
		if(retval != pmNoError) {
			printf("error: %s \n", Pm_GetErrorText(retval));
		}
		else {
			while(1) {
				if(Pm_Poll(mstream)) {
					cnt = Pm_Read(mstream, msg, 32);
					for(i=0; i<cnt; i++) {
						printf("status:%d, byte1=%d, byte2=%d, time=%.3f\n",
							Pm_MessageStatus(msg[i].message),
							Pm_MessageData1(msg[i].message),
							Pm_MessageData2(msg[i].message),
							msg[i].timestamp/1000.0);
					}
				}
			}
		}
		Pm_Close(mstream);
	
	Pm_Terminate();
	return;
}


int main(int argc, char *argv[]) {
	int devID;
	if (argc==0){
		devID=0;
	}
	else if (argc>2){
		printf("Enter MIDI Device ID\n");
		return -1;
	}
	else{
		devID = atoi(argv[1]);
	}
	
	
	readMIDI(devID);
	return 0;
}
