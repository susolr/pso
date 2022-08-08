#!/bin/bash
################# Global variables
NREPETITIONS=5
VI=(0.2 0.4 0.6 0.8 1.0)
NMAXHEBRAS=12
NMAXNODOS=7
NODES="compute-0-2,compute-0-3,compute-0-4"
DIR="coeficiente_inercia"
rm -rf $DIR && mkdir $DIR
date && echo "Start the execution"
echo "-------------------"
make -j N_FEATURES=3600
for I in "${VI[@]}";
do
    echo -e "\t`date`" && echo -e "\tStarts with" $I "inercia"
    DIR+="/$I inercia"
    mkdir "$DIR"
    for R in $( eval echo {1..$NREPETITIONS} );
    do
        echo -e "\t`date`" && echo -e "\tStarts " $R "repetition"
        echo "Repetition: $R" >> "$DIR/raw.txt"
        echo -e "\t" $R
        salloc -p guest -N7 -n7 -w compute-0-4,compute-0-1,compute-0-2,compute-0-3,compute-0-5,compute-0-6,compute-0-7 mpiexec --map-by node --bind-to none -x OMP_NUM_THREADS=12 ./pso -cI $I > last.txt
        #mpiexec ./pso -nH $H > last.txt
        #echo "`sed -n 1p last.txt`" >> tmp_time.txt
        paste last* > "$DIR/clasificacion_$R.txt"
        rm last.txt
    done
    DIR="$(dirname "$DIR")"
done
find ./mediciones_tiempo -name \*.txt -exec sed -i 's/\./,/g' {} \;