#!/bin/bash
################# Global variables
NREPETITIONS=5
NP=(100 200 300 400)
NMAXHEBRAS=12
NMAXNODOS=7
NODES="compute-0-2,compute-0-3,compute-0-4"
DIR="mediciones_tiempo_multi"
rm -rf $DIR && mkdir $DIR
date && echo "Start the execution"
echo "-------------------"
make -j N_FEATURES=3600
for P in "${NP[@]}";
do
    echo -e "\t`date`" && echo -e "\tStarts with" $P "particulas"
    DIR+="/$P particulas"
    mkdir "$DIR"
    for R in $( eval echo {1..$NREPETITIONS} );
    do
        echo -e "\t`date`" && echo -e "\tStarts " $R "repetition"
        echo "Repetition: $R" >> "$DIR/raw.txt"
        echo -e "\t" $R
        salloc -p guest -N7 -n7 -w compute-0-4,compute-0-1,compute-0-2,compute-0-3,compute-0-6,compute-0-7,compute-0-8 mpiexec --map-by node --bind-to none -x OMP_NUM_THREADS=12 ./pso -nP $P > last.txt
        #mpiexec ./pso -nH $H > last.txt
        #echo "`sed -n 1p last.txt`" >> tmp_time.txt
        paste last* > "$DIR/clasificacion_$R.txt"
        rm last.txt
    done
    DIR="$(dirname "$DIR")"
done
find ./mediciones_tiempo -name \*.txt -exec sed -i 's/\./,/g' {} \;