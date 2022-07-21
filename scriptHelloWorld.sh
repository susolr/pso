#!/bin/bash
#------- SLURM OPTIONS
#SBATCH -J jobname
#SBATCH -p guest
#SBATCH -n 1
#SBATCH -c 16
#SBATCH -o jobname_%j.out
#SBATCH -e jobname_%j.err
#SBATCH --mail-user=user@example.com
#SBATCH --mail-type=END,FAIL
#------- USER COMMANDS
for i in {1..10}
do
echo "Iteration %i"
srun ./Hello_world
done