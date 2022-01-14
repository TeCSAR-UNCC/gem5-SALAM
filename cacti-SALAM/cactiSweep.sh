#!/bin/bash

input="benchmarks.list"
while IFS= read -r line;
do
    [[ $line =~ ^#.* ]] && continue
        python3 cactiWrapper.py $line
        sleep 1
    
done < "$input"

