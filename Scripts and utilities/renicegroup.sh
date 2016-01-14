#!/bin/bash

# this script effectively locks a process at a given priority by
# repeatedly setting it's priority to a given value
# -20 is ultra high priority (not nice), 0 is default, 
# 19 is low priority (really nice)

#first param is the nice value
#second param is the process group for all the threads (PGID)

echo "Running renice loop on PGID $2 with nice value of $1"

while :
do
	sleep 2
	date
	renice -n $1 -g $2			#just run command
done