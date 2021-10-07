#!/bin/bash
BENCH=""

while getopts ":b:" opt; do
	case $opt in
		b )
			BENCH=${OPTARG}
			;;
	esac
done


if [ "${BENCH}" == "" ]; then
	echo "No benchmark specified."
else
    echo "Generating HW"
    python ${M5_PATH}/HWProfileGenerator.py -b $BENCH
fi