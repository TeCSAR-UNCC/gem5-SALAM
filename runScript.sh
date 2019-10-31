#!/bin/bash

BENCHDIR="../../.."
SCRIPTDIR="src/hwacc/Scripts"
MIN=2
TOTALSWEEPS=0
TOTALBENCHMARKS=0
SECONDS=0

if [ "$1" = "all" ]
then
	echo "Performing All Benchmarks Sweeps, This Could Take A While"
	for BENCH in gemm-partial-unroll gemm-no-unroll gemm-full-unroll
	do	
		((TOTALBENCHMARKS+=1))
		RESOURCES="benchmarks/$BENCH/simulation/max_resources.txt"
		echo "Performing First Pass For $BENCH Application "
		# Run with unlimited resources for first pass 
		COUNTER=-1
		INTADDER=-1
		INTMUL=-1
		SHIFTER=-1
		BITWISE=-1
		FLOATADD=-1
		DOUBADD=-1
		FLOATMUL=-1
		DOUBMUL=-1
		ZERO=-1
		GEP=-1
		CONV=-1
		PORTS=16
		TIMING=10  #this one doesn't matter as much, but should be set anyways
		python3 $SCRIPTDIR/createDomain.py \
			-f "benchmarks/$BENCH/config.ini" \
			-m $PORTS \
			-t $TIMING \
			-c $COUNTER \
			-ia $INTADDER \
			-im $INTMUL \
			-s $SHIFTER \
			-b $BITWISE \
			-fa $FLOATADD \
			-da $DOUBADD \
			-fm $FLOATMUL \
			-dm $DOUBMUL \
			-z $ZERO \
			-g $GEP \
			-cv $CONV
		echo "Benchmark set to unlimited resources to define upper bounds"
		./baremetalarm $BENCH;
		echo "First Pass Finished"
		# Store the max functional unit values from previous simulation
		python3 $SCRIPTDIR/extractFunctionUnits.py -f "BM_ARM_OUT/$BENCH/debug_trace.txt" \
										  		   -d "benchmarks/$BENCH/simulation/max_resources.txt"
		# Pull max functional unit values into bash script	
					      
		COUNTER=`head -n+12 $RESOURCES | tail -1`
		COUNTER="${COUNTER:1}"
		echo "Counters = $COUNTER"	
		if [ $((COUNTER%2)) != 0 ]
		then
			COUNTER=$((COUNTER+=1))
		fi
		echo "Counters = $COUNTER"
		INTADDER=`head -n+13 $RESOURCES | tail -1`
		INTADDER="${INTADDER:1}"
		if [ $((INTADDER%2)) != 0 ]
		then
			INTADDER=$((INTADDER+=1))
		fi
		INTMUL=`head -n+14 $RESOURCES | tail -1`
		INTMUL="${INTMUL:1}"
		if [ $((INTMUL%2)) != 0 ]
		then
			INTMUL=$((INTMUL+=1))
		fi
		SHIFTER=`head -n+15 $RESOURCES | tail -1`
		SHIFTER="${SHIFTER:1}"
		if [ $((SHIFTER%2)) != 0 ]
		then
			SHIFTER=$((SHIFTER+=1))
		fi
		BITWISE=`head -n+16 $RESOURCES | tail -1`
		BITWISE="${BITWISE:1}"
		if [ $((BITWISE%2)) != 0 ]
		then
			BITWISE=$((BITWISE+=1))
		fi
		FLOATADD=`head -n+17 $RESOURCES | tail -1`
		FLOATADD="${FLOATADD:1}"
		if [ $((FLOATADD%2)) != 0 ]
		then
			FLOATADD=$((FLOATADD+=1))
		fi
		DOUBADD=`head -n+18 $RESOURCES | tail -1`
		DOUBADD="${DOUBADD:1}"
		if [ $((DOUBADD%2)) != 0 ]
		then
			DOUBADD=$((DOUBADD+=1))
		fi
		FLOATMUL=`head -n+19 $RESOURCES | tail -1`
		FLOATMUL="${FLOATMUL:1}"
		if [ $((FLOATMUL%2)) != 0 ]
		then
			FLOATMUL=$((FLOATMUL+=1))
		fi
		DOUBMUL=`head -n+20 $RESOURCES | tail -1`
		DOUBMUL="${DOUBMUL:1}"
		if [ $((DOUBMUL%2)) != 0 ]
		then
			DOUBMUL=$((DOUBMUL+=1))
		fi
		ZERO=`head -n+21 $RESOURCES | tail -1`
		ZERO="${ZERO:1}"
		if [ $((ZERO%2)) != 0 ]
		then
			ZERO=$((ZERO+=1))
		fi
		GEP=`head -n+22 $RESOURCES | tail -1`
		GEP="${GEP:1}"
		if [ $((GEP%2)) != 0 ]
		then
			GEP=$((GEP+=1))
		fi
		CONV=`head -n+23 $RESOURCES | tail -1`
		CONV="${CONV:1}"
		if [ $((CONV%2)) != 0 ]
		then
			CONV=$((CONV+=1))
		fi	
		echo "Maximum functional units found"			
		
		python3 $SCRIPTDIR/statParser.py -f "BM_ARM_OUT/$BENCH/debug_trace.txt" \
						      			 -d "benchmarks/$BENCH/simulation/design.txt"
		# Clear results array for new simulation profile
		python3 $SCRIPTDIR/resetResults.py -f "benchmarks/$BENCH/simulation/results.txt"
		echo "Previous results cleared, beginning new design sweep"
		#Begin Design Sweep Loops
		echo "Beginning design sweep for $BENCH"
		# Transistor timing
		for TIMING in 10 6 5 4 3 2 1
		do
			echo "Transistor Timing: $TIMING ns"
			# Memory read/write port paths
			for PORTS in 16 8 4 2
			do
				echo "Read/Write Ports: $PORTS"
				# Converstion
				c=$CONV
				#while
					#echo "Conversion instructions limited to $c"
					# GEP
					g=$GEP
					#while
						#echo "GEP instructions limited to $g"
						# Zero
						z=$ZERO
						#while
							#echo "Zero cycle instructions limited to $z"
							# Double Mul/Div
							dm=$DOUBMUL
							while
								echo "Double Mul/Div Limited to $dm"
								# Double Add/Sub
								da=$DOUBADD
								while
									echo "Double Add/Sub Limited to $da"
									# Float Mul/Div
									fm=$FLOATMUL
									while
										echo "Floating Point Mul/Div Limited to $fm"
										# Float Add/Sub
										fa=$FLOATADD
										while
											echo "Floating Point Add/Sub Limited to $fa"
											# Bitwise
											ib=$BITWISE
											#while
												#echo "Bitwise Units Limited to $ib"
												# Shifters
												is=$SHIFTER
												#while
													#echo "Shifters Limited to $is"
													# Integer Mul/Div
													im=$INTMUL
													while
														echo "Integer Mul/Div Limited to $im"
														# Integer Add/Sub
														ia=$INTADDER
														while
															echo "Integer Add/Sub Limited to $ia"
															# Counters
															ic=$COUNTER
															#while
																#echo "Counters Limited to $ic"
																#############
																echo "Minor: Configuration Update"
																((TOTALSWEEPS+=1))
																python3 $SCRIPTDIR/createDomain.py \
																	-f "benchmarks/$BENCH/config.ini" \
																	-m $PORTS \
																	-t $TIMING \
																	-c $ic \
																	-ia $ia \
																	-im $im \
																	-s $is \
																	-b $ib \
																	-fa $fa \
																	-da $da \
																	-fm $fm \
																	-dm $dm \
																	-z $z \
																	-g $g \
																	-cv $c
																echo "Minor: Configuration Update Complete"
																echo "New Simulation: Benchmark #$TOTALBENCHMARKS, Sweep #$TOTALSWEEPS"
																./baremetalarm $BENCH;
																echo "Simulation Complete"
																echo "Storing Results"
																python3 $SCRIPTDIR/storeResults.py \
																	-f "BM_ARM_OUT/$BENCH/debug_trace.txt" \
																	-d "benchmarks/$BENCH/simulation/results.txt"
																echo "Results Stored"
																#############
															#(($ic>$MIN))
															#do
															#((ic/=2))
															#done
														(($ia>$MIN))
														do
														((ia/=2))
														done
													(($im>$MIN))
													do
													((im/=2))
													done
												#(($is>$MIN))
												#do
												#((is/=2))
												#done
											#(($ib>$MIN))
											#do
											#((ib/=2))
											#done
										(($fa>$MIN))
										do
										((fa/=2))
										done
									(($fm>$MIN))
									do
									((fm/=2))
									done
								(($da>$MIN))
								do
								((da/=2))			
								done 
							(($dm>$MIN)) 
							do
							((dm/=2))
							done
						#(($z>$MIN))
						#do
						#((z/=2))
						#done
					#(($g>$MIN))
					#do
					#((g/=2))
					#done
				#(($c>$MIN))
				#do
				#((c/=2))	
				#done
				echo "Pass Completed, Creating New Domain"
				if [ $PORTS != 2 ]
				then
					# Reset functional units
					COUNTER=-1
					INTADDER=-1
					INTMUL=-1
					SHIFTER=-1
					BITWISE=-1
					FLOATADD=-1
					DOUBADD=-1
					FLOATMUL=-1
					DOUBMUL=-1
					ZERO=-1
					GEP=-1
					CONV=-1
					# Get new functional unit max after reducing port sizes
					python3 $SCRIPTDIR/createDomain.py \
						-f "benchmarks/$BENCH/config.ini" \
						-m $(($PORTS/2)) \
						-t $TIMING \
						-c $COUNTER \
						-ia $INTADDER \
						-im $INTMUL \
						-s $SHIFTER \
						-b $BITWISE \
						-fa $FLOATADD \
						-da $DOUBADD \
						-fm $FLOATMUL \
						-dm $DOUBMUL \
						-z $ZERO \
						-g $GEP \
						-cv $CONV
					./baremetalarm $BENCH;
					# Store the max functional unit values from previous simulation
					python3 $SCRIPTDIR/extractFunctionUnits.py -f "BM_ARM_OUT/$BENCH/debug_trace.txt" \
													  		   -d "benchmarks/$BENCH/simulation/max_resources.txt"
					# Pull max functional unit values into bash script							      
					COUNTER=`head -n+12 $RESOURCES | tail -1`
					COUNTER="${COUNTER:1}"
					echo "Counters = $COUNTER"	
					if [ $((COUNTER%2)) != 0 ]
					then
						COUNTER=$((COUNTER+=1))
					fi
					echo "Counters = $COUNTER"
					INTADDER=`head -n+13 $RESOURCES | tail -1`
					INTADDER="${INTADDER:1}"
					if [ $((INTADDER%2)) != 0 ]
					then
						INTADDER=$((INTADDER+=1))
					fi
					INTMUL=`head -n+14 $RESOURCES | tail -1`
					INTMUL="${INTMUL:1}"
					if [ $((INTMUL%2)) != 0 ]
					then
						INTMUL=$((INTMUL+=1))
					fi
					SHIFTER=`head -n+15 $RESOURCES | tail -1`
					SHIFTER="${SHIFTER:1}"
					if [ $((SHIFTER%2)) != 0 ]
					then
						SHIFTER=$((SHIFTER+=1))
					fi
					BITWISE=`head -n+16 $RESOURCES | tail -1`
					BITWISE="${BITWISE:1}"
					if [ $((BITWISE%2)) != 0 ]
					then
						BITWISE=$((BITWISE+=1))
					fi
					FLOATADD=`head -n+17 $RESOURCES | tail -1`
					FLOATADD="${FLOATADD:1}"
					if [ $((FLOATADD%2)) != 0 ]
					then
						FLOATADD=$((FLOATADD+=1))
					fi
					DOUBADD=`head -n+18 $RESOURCES | tail -1`
					DOUBADD="${DOUBADD:1}"
					if [ $((DOUBADD%2)) != 0 ]
					then
						DOUBADD=$((DOUBADD+=1))
					fi
					FLOATMUL=`head -n+19 $RESOURCES | tail -1`
					FLOATMUL="${FLOATMUL:1}"
					if [ $((FLOATMUL%2)) != 0 ]
					then
						FLOATMUL=$((FLOATMUL+=1))
					fi
					DOUBMUL=`head -n+20 $RESOURCES | tail -1`
					DOUBMUL="${DOUBMUL:1}"
					if [ $((DOUBMUL%2)) != 0 ]
					then
						DOUBMUL=$((DOUBMUL+=1))
					fi
					ZERO=`head -n+21 $RESOURCES | tail -1`
					ZERO="${ZERO:1}"
					if [ $((ZERO%2)) != 0 ]
					then
						ZERO=$((ZERO+=1))
					fi
					GEP=`head -n+22 $RESOURCES | tail -1`
					GEP="${GEP:1}"
					if [ $((GEP%2)) != 0 ]
					then
						GEP=$((GEP+=1))
					fi
					CONV=`head -n+23 $RESOURCES | tail -1`
					CONV="${CONV:1}"
					if [ $((CONV%2)) != 0 ]
					then
						CONV=$((CONV+=1))
					fi	
					echo "New maximum functional units found"	
					# Set new max variables
					python3 $SCRIPTDIR/createDomain.py \
						-f "benchmarks/$BENCH/config.ini" \
						-m $(($PORTS/2)) \
						-t $TIMING \
						-c $COUNTER \
						-ia $INTADDER \
						-im $INTMUL \
						-s $SHIFTER \
						-b $BITWISE \
						-fa $FLOATADD \
						-da $DOUBADD \
						-fm $FLOATMUL \
						-dm $DOUBMUL \
						-z $ZERO \
						-g $GEP \
						-cv $CONV
					echo "Major: Configuration Update Complete"
				fi
			done
		done
	done
	echo "All Simulations Complete!"
	ENDTIME=$SECONDS
	ENDMINUTES=$(($ENDTIME/60))
	ENDHOURS=$(($ENDMINUTES/60))
	ENDSECONDS=$(($ENDTIME%60))
	
	echo "Explored $TOTALBENCHMARKS Total Benchmarks"
	echo "By running $TOTALSWEEPS Total Sweeps!"
	echo "Which took $ENDHOURS hrs, $ENDMINUTES min, $ENDSECONDS sec!"
else
	echo "Feature turned off"
#	BENCH=$1
#    echo "Performing First Pass: Application = $BENCH"
#	./baremetalarm $BENCH;
#	echo "First Pass Finished"
#	echo "Creating Design Space"

#	# Store initial parameters with unlimited resources
#	python3 $SCRIPTDIR/statParser.py \
#		-f "$BENCHDIR/BM_ARM_OUT/$BENCH/debug_trace.txt" \
#		-d "$BENCHDIR/benchmarks/$BENCH/simulation/design.txt"

#	python3 $SCRIPTDIR/resetResults.py \
#		-f "$DIR/benchmarks/$BENCH/simulation/results.txt"

#	for TIMING in 1 2 3 4 5 6 10
#	do
#		for PORTS in 2 4 8 16
#		do
#			echo "Update Configuration: Ports=$PORTS, Timing=$TIMING"
#			python3 $SCRIPTDIR/createDomain.py \
#				-f "$BENCHDIR/benchmarks/$BENCH/config.ini" \
#				-m $PORTS \
#				-t $TIMING \

#			echo "Configuration Update Complete"
#			echo "New Simulation"
#			./baremetalarm $BENCH;
#			echo "Simulation Complete"
#			echo "Storing Results"
#			python3 $SCRIPTDIR/storeResults.py \
#				-f "$BENCHDIR/BM_ARM_OUT/$BENCH/debug_trace.txt" \
#				-d "benchmarks/$BENCH/simulation/results.txt" 
#			echo "Results Stored"
#		done
#	done
fi
