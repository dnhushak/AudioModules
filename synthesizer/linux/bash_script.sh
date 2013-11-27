#!/bin/bash

clear

while [ "pgrep Chipophone > /dev/null && echo Running" != "Running" ]
do
	echo "Starting Chipophone"
	./Synthesizer 2
	sleep 1
done

echo "Script finished. Should not reach here."
