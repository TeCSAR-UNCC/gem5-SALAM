#!/bin/bash

if [ "$1" = "all" ]
then
	echo "Performing All Benchmarks, This Could Take A While"
	for BENCH in bfs fft gemm hotspot nw spmv stencil2d stencil3d
	do	
		echo "Performing First Pass: Application = $BENCH"
		./baremetalarm $BENCH;
		echo "First Pass Finished"
		echo "Creating Design Space"
		# Store initial parameters with unlimited resources
		python3 statParser.py -f "BM_ARM_OUT/$BENCH/debug_trace.txt" -d "benchmarks/$BENCH/simulation/design.txt"
		# Clear results array for new simulation profile
		python3 resetResults.py -f "benchmarks/$BENCH/simulation/results.txt"
		# Transistor timing
		for TIMING in 1 2 3 4 5 6 10
		do
			# Memory read/write port paths
			for PORTS in 2 4 8 16
			do
				echo "Update Configuration: Ports=$PORTS, Timing=$TIMING"
				python3 createDomain.py -m $PORTS -t $TIMING -f "benchmarks/$BENCH/config.ini"
				echo "Configuration Update Complete"
				echo "New Simulation"
				./baremetalarm $BENCH;
				echo "Simulation Complete"
				echo "Storing Results"
				python3 storeResults.py -f "BM_ARM_OUT/$BENCH/debug_trace.txt" -d "benchmarks/$BENCH/simulation/results.txt" -m $PORTS -t $TIMING
				echo "Results Stored"
			done
		done
	done
	echo "All Simulations Complete!"
else
    echo "Performing First Pass: Application = $BENCH"
	./baremetalarm $BENCH;
	echo "First Pass Finished"
	echo "Creating Design Space"

	# Store initial parameters with unlimited resources
	python3 statParser.py -f "BM_ARM_OUT/$BENCH/debug_trace.txt" -d "benchmarks/$BENCH/simulation/design.txt"

	python3 resetResults.py -f "benchmarks/$BENCH/simulation/results.txt"


	for TIMING in 1 2 3 4 5 6 10
	do
		for PORTS in 2 4 8 16
		do
			echo "Update Configuration: Ports=$PORTS, Timing=$TIMING"
			python3 createDomain.py -m $PORTS -t $TIMING -f "benchmarks/$BENCH/config.ini"
			echo "Configuration Update Complete"
			echo "New Simulation"
			./baremetalarm $BENCH;
			echo "Simulation Complete"
			echo "Storing Results"
			python3 storeResults.py -f "BM_ARM_OUT/$BENCH/debug_trace.txt" -d "benchmarks/$BENCH/simulation/results.txt" -m $PORTS -t $TIMING
			echo "Results Stored"
		done
	done
fi
