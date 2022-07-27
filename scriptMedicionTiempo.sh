#!/bin/bash
################# Global variables
NREPETITIONS=5
NMAXHEBRAS=12
NMAXNODOS=4
DIR="mediciones_tiempo_multi"
rm -rf $DIR && mkdir $DIR
date && echo "Start the execution"
echo "-------------------"
make -j N_FEATURES=3600
for N in $( eval echo {1..$NMAXNODOS} );
do
    echo -e "\t`date`" && echo -e "\tStarts with" $H "hebras"
    DIR+="/$N nodos"
    mkdir "$DIR"
    for R in $( eval echo {1..$NREPETITIONS} );
    do
        echo -e "\t`date`" && echo -e "\tStarts " $R "repetition"
        echo "Repetition: $R" >> "$DIR/raw.txt"
        echo -e "\t" $R
        salloc -N$N -n1 -p guest mpiexec --bind-to none -x OMP_NUM_THREADS=12 ./pso > last.txt
        #mpiexec ./pso -nH $H > last.txt
        echo "`sed -n 1p last.txt`" >> tmp_time.txt
    done
    rm last.txt
    paste tmp_time* > "$DIR/time.txt"
    rm -rf tmp*
    DIR="$(dirname "$DIR")"
done
find ./mediciones_tiempo -name \*.txt -exec sed -i 's/\./,/g' {} \;