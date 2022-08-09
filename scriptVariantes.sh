#!/bin/bash

################# Global variables

NPARTICULAS=(200)
CSOCIAL=(1.0)
CCOGNITIVO=(1.0)
NREPETITIONS=5
NITERACIONES=20
NMAXHEBRAS=8
PROJECT=${PWD##*/}

DIR="variante_actualizacion_posicion"
rm -rf $DIR && mkdir $DIR
date && echo "Start the execution"
echo "-------------------"
echo -e "\t`date`" && echo -e "\tStarts with DB 104, O2 and" $NREPETITIONS "repetitions"
make -j N_FEATURES=3600

for R in $( eval echo {1..$NREPETITIONS} ); do
	echo -e "\t\t" $R
	salloc -p guest -N3 -n3 -w compute-0-1,compute-0-2,compute-0-7 mpiexec --map-by node --bind-to none -x OMP_NUM_THREADS=12 ./pso > last.txt
	paste last* > "$DIR/clasificacion_$R.txt"
	rm last.txt
done

find ./caso_base -name \*.txt -exec sed -i 's/\./,/g' {} \;
