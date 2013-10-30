#include "portmidi.h"
#include "porttime.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "assert.h"


int main(void){
	int i;
	for (i = 0; i < Pm_CountDevices(); i++) {
        const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
        if (info->input){
        	printf("Input:  ");
        }
        else{
        	printf("Output: ");
        }
        printf("%d: %s, %s", i, info->interf, info->name);
        printf("\n");
    }
	return 0;
}
