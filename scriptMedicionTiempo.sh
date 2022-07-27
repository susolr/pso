#!/bin/bash
################# Global variables
NREPETITIONS=1
NMAXHEBRAS=12
DIR="mediciones_tiempo_k"
rm -rf $DIR && mkdir $DIR
date && echo "Start the execution"
echo "-------------------"
make -j N_FEATURES=3600
for H in $( eval echo {1..$NMAXHEBRAS} );
do
    echo -e "\t`date`" && echo -e "\tStarts with" $H "hebras"
    DIR+="/$H hebras"
    mkdir "$DIR"
    for R in $( eval echo {1..$NREPETITIONS} );
    do
        echo -e "\t`date`" && echo -e "\tStarts " $R "repetition"
        echo "Repetition: $R" >> "$DIR/raw.txt"
        echo -e "\t" $R
        salloc -N1 -n1 -p guest -w compute-0-4 mpiexec --bind-to none -x OMP_NUM_THREADS=$H ./pso > last.txt
        #mpiexec ./pso -nH $H > last.txt
        echo "`sed -n 1p last.txt`" >> tmp_time.txt
    done
    rm last.txt
    paste tmp_time* > "$DIR/time.txt"
    rm -rf tmp*
    DIR="$(dirname "$DIR")"
done
find ./mediciones_tiempo -name \*.txt -exec sed -i 's/\./,/g' {} \;