/*
 * FILE: simgalib.cpp, v.1.7.1, 4/28/2014
 * Author: Tomas V. Arredondo
 *
 * SimGALib: A simple yet flexible GA implementation in C++.
 *
 * This is NOT a tutorial on GA as there are many tutorials and books on this
 * subject see http://en.wikipedia.org/wiki/Genetic_algorithm for more information.
 *
 * To change the function and range being optimized go to ga.h and change
 * FUNCTION_LOWER_RANGE, FUNCTION_UPPER_RANGE and FUNCTION definitions.
 *
 * DISCLAIMER: No liability is assumed by the author for any use made
 * of this program.
 * DISTRIBUTION: Any use may be made of this program, as long as the
 * clear acknowledgment is made to the author in code and runtime executables
 */
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <stdarg.h>
#include <stdlib.h>
#include "simgalib.h"

using namespace std;
using namespace simgalib;
using namespace simtstlib;

extern int function_cnt;  // function eval counter

void simgalib::Population::evaluate()
{

    // update the index
    for (int i = 0; i < population_size; i++)
    {
        pool[i]->index=i;
    }

    // evaluate fitness
    for (std::vector<Organism*>::iterator it_pool = pool.begin(); it_pool != pool.end(); ++it_pool)
    {
        // This is the function for which the population is trying to find the max value.
        (*it_pool)->fitness = evaluator.evaluate((*it_pool)->genes);
        function_cnt++;
    }

    if (selection_method==Ranking)
    {
        // Select population for crossover using ranking.
        float max=rank_parms.getMax(); // Ranking: max expected num of descendents for worst individual
        float inc=rank_parms.getInc(); // increment
        float tmp_fitness;

        // Sort population by fitness: in greater to lesser order
        std::sort(pool.begin(), pool.end(), greater_mag());

        // Set the rank fitness in high to low order according to ranking
        tmp_fitness=max;
        for (int i = 0; i < population_size; i++)
        {
            pool[i]->rank_fitness=tmp_fitness;
            tmp_fitness-=inc;
        }
    }
}

void simgalib::Population::crossover(const Population_data& pop_info)
{
    int selected_pop[population_size];
    double r,fitsum;
    int i,j,k,site1,site2,temp;
    std::vector<Organism> new_pool(population_size);
    int temp_gene;

    if (getSelectionMethod()==FitnessProp)
    {
        // Select population for crossover
        for (i=0; i<population_size; i++)
        {
            fitsum=0;
            r=(double)ran2(100);

            for (j=0; (j < population_size && fitsum <= r) ; j++)
                fitsum += ((pool[j]->fitness/pop_info.sum_fitness)*100);

            j--;  // since the for always post increments
            selected_pop[i]=j;
        }
    }
    else if (getSelectionMethod()==Ranking)
    {
        // Select population for crossover
        for (i=0; i<population_size; i++)
        {
            fitsum=0;
            r=(double)ran2(100);

            for (j=0; (j < population_size && fitsum <= r) ; j++)
                fitsum += ((pool[j]->rank_fitness/pop_info.sum_rank_fitness)*100);

            j--;  // since the for always post increments
            selected_pop[i]=j;
        }
    }
    else if (selection_method==Tournament)
    {
        for (i=0; i < population_size; i++)
        {
            j=(int)ran2(population_size);
            k=(int)ran2(population_size);

            if (flip(ptournament))
            {
                // if random number is less than ptournament take greater fitness
                if (pool[j]->fitness>=pool[k]->fitness)
                {
                    selected_pop[i]=j;
                }
                else
                {
                    selected_pop[i]=k;
                }
            }
            else
            {
                // else choose member with smaller fitness
                if (pool[j]->fitness>=pool[k]->fitness)
                {
                    selected_pop[i]=k;
                }
                else
                {
                    selected_pop[i]=j;
                }
            }
        }
    }

    // Copy selected ones into new_pool
    for (i=0; i<population_size; i++)
        new_pool[i]=*(pool[selected_pop[i]]);

    if (elite_flag)
    {
        // Use ELITE selection
        elite_organism=*(pool[pop_info.max_index]);
    }

    for (i=0; i < population_size; i+=2)
    {
        if (flip(pcross))
        {
            site1=(int) ran2(pool[0]->genes.size());
            site2=(int) ran2(pool[0]->genes.size());

            if (site1 > site2)
            {
                temp=site2;
                site2=site1;
                site1=temp;
            }

            for (j=site1; j<=site2; j++)
            {
                temp_gene=new_pool[i].genes[j];
                new_pool[i].genes[j]=new_pool[i+1].genes[j];
                new_pool[i+1].genes[j]=temp_gene;
            }
        }
    }

    // Copy back selected ones into new_pool
    for (i=0; i<population_size; i++)
        *(pool[i])=new_pool[i];

}

void simgalib::Population::mutate()
{
    for (int i=0; i < population_size; i++)
    {
        for (unsigned int j=0; j < pool[0]->genes.size(); j++)
        {
            if (flip(pmut))
            {
                if (pool[i]->genes[j]==0)
                    pool[i]->genes[j]=1;
                else
                    pool[i]->genes[j]=0;
            }
        }
    }

    if (elite_flag)
    {
        *(pool[(int)ran2(population_size)])=elite_organism;
    }

}

void simgalib::Population_data::evaluate_population_info(const Population *pop)
{
    int pop_size=pop->getSize();
    bool ranking_flag=false;

    clear_pop_data();
    min_organism=*(pop->pool[0]);
    min_fitness=(*(pop->pool[0])).fitness;
    min_index=0;

    if (pop->getSelectionMethod()==Ranking)
        ranking_flag=true;

    for (int i = 0; i < pop_size; i++)
    {
        sum_fitness+=(*(pop->pool[i])).fitness;

        if (ranking_flag)
            sum_rank_fitness+=(*(pop->pool[i])).rank_fitness;

        if ((*(pop->pool[i])).fitness > max_fitness)
        {
            max_fitness=(*(pop->pool[i])).fitness;
            // max_organism=(*(pop->pool[i]));
            max_index=i;
        }
        if ((*(pop->pool[i])).fitness < min_fitness)
        {
            min_fitness=(*(pop->pool[i])).fitness;
            // min_organism=(*(pop->pool[i]));
            min_index=i;
        }
    }
    if (sum_fitness)
        avg_fitness=sum_fitness/pop_size;
    else
        avg_fitness=0;
}

void simgalib::Population_data::display_population_stats()
{
    vector<int>::const_iterator it;
#if 0
    for (it = max_organism.genes.begin(); it != max_organism.genes.end(); it++)
    {
        std::cout << "fittest member ["<< max_index << "]";
        std::cout << *it << " ";
        std::cout << std::endl;
    }
#endif

    std::cout << "fittest member ["<< max_index << "]" << endl;
    std::cout << "fittest member fitness: "<< max_fitness << endl;

#if 0
    for (it = min_organism.genes.begin(); it != min_organism.genes.end(); it++)
    {
        std::cout << "least fit member ["<< min_index << "]:";
        std::cout << *it << " ";
        std::cout << std::endl;
    }

    std::cout << "least fit member fitness: "<< min_fitness;
    std::cout << std::endl;
#endif

    std::cout << "pop avg fitness: "<< avg_fitness;
    std::cout << std::endl;
}

void simgalib::Population::destroy()
{

    // destroy population
    for ( vector<Organism*>::iterator i = pool.begin(); i != pool.end(); ++i )
    {
        delete *i;
    }
    // empty the container
    pool.clear();
}

void simgalib::Population::create()
{
    // create population
    for (int i=0; i < population_size; i++)
        pool.push_back( new Organism(num_genes) );  // 1.7.0

    // initialize population
    for (int i = 0; i < population_size; i++)
    {
        for (int j = 0; j < num_genes; j++)
        {
            (*pool[i]).genes.at(j)=flip(.5);
        }
    }
}

void simgalib::Population::display()
{
    for (int i = 0; i < population_size; i++)
    {
        std::cout << "member ["<< i << "]";
        // std::cout << pool[i]; TODO
        std::cout << std::endl;
        std::cout << "fitness: "<< (*pool[i]).fitness << endl;
        // std::cout << "double_genes: "<< pool[i].double_genes << endl;
    }
}

#if 0
void simgalib::Population::display_fittest()
{
    for (int i = 0; i < population_size; i++)
    {
        std::cout << "member ["<< i << "]";
        std::cout << pool[i];
        std::cout << std::endl;
        std::cout << "fitness: "<< pool[i].fitness << endl;
        // std::cout << "double_genes: "<< pool[i].double_genes << endl;
    }
}
#endif

double simgalib::EvalFN::evaluate(vector<int> genes)
{
    return ((*eval_fn)(num_parms,num_bits_per_parm,lower_range,upper_range,genes));
}

#if 0
std::cout << "---------- Unsorted GA Population (press enter) ------" << std::endl << flush;
//   std::cin.get(temp);
for (int i = 0; i < population_size; i++)
{
    std::cout << "member ["<< i << "]" << endl;
    std::cout << "member index:" << pool[i].index << endl;
    std::cout << pool[i];
    std::cout << std::endl;
    std::cout << "fitness: "<< pool[i].fitness << endl;
    //std::cout << "double_genes: "<< pool[i].double_genes << endl;
}
std::cout << flush;
char temp2;
std::cin.get(temp2);
#endif

//-----------------------------------------------------------------------------
// Optimization Methods
// WARNING: ALL THESE METHODS ASUME THAT WE ARE MAXIMIZING THE OBJECTIVE FUNCTION.
//-----------------------------------------------------------------------------
int simgalib::run_ga(EvalFN eval, int number_runs, int ga_pop_size, int ga_number_iters,
                     enumSelectionMethod selection_method, bool elite_flag, float Pcross,
                     float Pmut, float tournament_Prob, float ranking_MAX )
{
    clock_t start,end,diff=0;
    start=clock();

    // Run params
    int nRun=0;
    double nRunsAvgFitness=0;
    double nRunsMaxFitness=0;
    double nRunsMinFitness=999999999;
    double nRunFitness[number_runs];
    double nRunsAvgFitnessVariance=0;
    double nRunsAvgFitnessSTD=0;
    function_cnt=0;

    for (int i=0; i < number_runs ; i++)
        nRunFitness[i]=0;

    // Set the population parameters: number of bits per organism and others
    Population pop(eval.num_parms*eval.num_bits_per_parm);

    // Set pop parameters
    pop.setEvalFN(eval);
    Population_data pop_info;
    RankingParms rank_parms;
    rank_parms.setMax(ranking_MAX);
    pop.setRankingParms(rank_parms);
    pop.setSize(ga_pop_size);
    pop.setNumIters(ga_number_iters);
    pop.setSelectionMethod(Tournament);
    pop.setEliteFlag(elite_flag);
    pop.setPcross(Pcross);
    pop.setPmut(Pmut);
    pop.setPtournament(tournament_Prob);

    srand(clock());

    for (nRun=0; nRun < number_runs; nRun++)
    {

        //    std::cout << "---------- RUN NUMBER" << nRun << std::endl;
        //    std::cout << "---------- BEGIN OPTIMIZATION ----------" << std::endl;

        pop.create(); // instantiate (new) the population
        pop.evaluate();
        pop_info.evaluate_population_info(&pop);

        // std::cout << "---------- Initial Population Stats (press enter) -------" << std::endl << flush;
        // std::cin.get(temp);
        // pop_info.display_population_stats();

        for (int i=1; i < pop.getNumIters() ; i++)
        {
            pop.crossover(pop_info);
            pop.mutate();

            pop.evaluate();
            pop_info.evaluate_population_info(&pop);
            // pop_info.display_population_stats();

            // std::cout << "iteration: "<< i << "-- Press enter to continue --" << std::endl << flush;
            //std::cin.get(temp);
        }

        //std::cout << "---------- Final Population (press enter) ------" << std::endl << flush;
        //std::cin.get(temp);
        //pop.display();
        //std::cout << flush;

        // std::cout << "---------- Final Population Stats (press enter) -------" << std::endl << flush;
        // pop_info.display_population_stats();
        // std::cin.get(temp);
        // std::cout << "---------- END OPTIMIZATION ----------" << std::endl;

        // Add up numbers for each Run
        if (pop_info.max_fitness > nRunsMaxFitness)
        {
            nRunsMaxFitness = pop_info.max_fitness;
        }
        if (pop_info.min_fitness < nRunsMinFitness)
        {
            nRunsMinFitness = pop_info.min_fitness;
        }

        nRunFitness[nRun]=pop_info.avg_fitness;
        nRunsAvgFitness+=pop_info.avg_fitness;

        pop.destroy();  // del the population
    }

    end=clock();
    diff=end-start;

    // Calculate RUN results
    nRunsAvgFitness=nRunsAvgFitness/number_runs;

    for (int i=0; i<nRun; i++)
    {
        nRunsAvgFitnessVariance+=pow(nRunsAvgFitness-nRunFitness[i],2);
    }
    nRunsAvgFitnessVariance=nRunsAvgFitnessVariance/number_runs;
    nRunsAvgFitnessSTD=pow(nRunsAvgFitnessVariance, 0.5);

    std::cout << "---------- GA FINAL RESULTS ----------" << std::endl;
    std::cout << "Number of Runs: "  << nRun << std::endl;
    std::cout << "Function: "        << eval.szName << std::endl;
    std::cout << "Runs max fitness: "<< nRunsMaxFitness << std::endl;
    std::cout << "Runs min fitness: "<< nRunsMinFitness << std::endl;
    std::cout << "Runs avg fitness: "<< nRunsAvgFitness << std::endl;
    std::cout << "Runs std dev fitness: "<< nRunsAvgFitnessSTD << std::endl;
    std::cout << "Simulation time in clocks: " << diff << std::endl;
    std::cout << "Simulation clocks per sec: " << CLOCKS_PER_SEC << std::endl;
    std::cout << "----------- PARAMETERS USED: -----------" << std::endl;
    std::cout << "Sel Method: " << pop.getSelectionMethod() << std::endl;
    std::cout << "Pop size: "   << pop.getSize() << std::endl;
    std::cout << "Elite flag: " << pop.getEliteFlag() << std::endl;
    std::cout << "Prob Cross: " << pop.getPcross() << std::endl;
    std::cout << "Prob Mut: "   << pop.getPmut() << std::endl;
    std::cout << "Prob Tourn: " << pop.getPtournament() << std::endl;
    std::cout << "Ranking Max Parm: " << pop.getRankingMaxParm() << std::endl;
    std::cout << "Number of Genes: "     << pop.getNumGenes() << std::endl;
    std::cout << "Number of Iterations: "     << pop.getNumIters() << std::endl;
    std::cout << "Number of Function Evals: "     << function_cnt << std::endl;
    std::cout << "---------- END FINAL RUN RESULTS ------------" << std::endl;
    return 0;
}

int simgalib::run_sh(EvalFN eval, int number_runs, int sh_iterations, int sh_starts)
{
    clock_t start,end,diff=0;
    start=clock();

    // Run params
    int nRun=0;
    double nRunsAvgFitness=0;
    double nRunsMaxFitness=0;
    double nRunsMinFitness=999999999;
    double nRunFitness[number_runs];
    double nRunsAvgFitnessVariance=0;
    double nRunsAvgFitnessSTD=0;
    function_cnt=0;

    for (int i=0; i < number_runs ; i++)
        nRunFitness[i]=0;

    // Create and set variables for stochastic hillclimbing
    int nBitPos=0;
    double nFitness=0;
    double test_nFitness=0;
    int nRepetition=0;  // Number of times the algorithm repeats in each run
    double nBestFitness=0;

    srand(clock());

    for (nRun=0; nRun < number_runs; nRun++)
    {

        Organism stoch_org(eval.num_parms*eval.num_bits_per_parm);
        Organism best_stoch_org(eval.num_parms*eval.num_bits_per_parm);

        nBestFitness=0;  // must initialize in every run

        for (nRepetition=0; nRepetition < sh_starts; nRepetition++)
        {

            // Initialize the starting point
            for (int j = 0; j < stoch_org.num_genes; j++)
            {
                stoch_org.genes.at(j)=flip(.5);
            }


            //    for (int j = 0; j < stoch_org.num_genes; j++)
            //    std::cout << stoch_org.genes[j] << std::endl;

            //    std::cout << "---------- RUN NUMBER" << nRun << std::endl;
            //    std::cout << "---------- BEGIN OPTIMIZATION ----------" << std::endl;

            nFitness=eval.evaluate(stoch_org.genes);

            for (int i=0; i < sh_iterations / sh_starts; i++)
            {
                // randomly select bit to change
                nBitPos=(int)ran2(stoch_org.genes.size());

                // invert bit to generate test position
                if (stoch_org.genes[nBitPos] == 0)
                    stoch_org.genes[nBitPos]=1;
                else
                    stoch_org.genes[nBitPos]=0;

                test_nFitness=eval.evaluate(stoch_org.genes);
                function_cnt++;

                if (test_nFitness >= nFitness)
                {
                    nFitness = test_nFitness;
                }
                else
                {
                    // since test position is not equal or better revert bit
                    if (stoch_org.genes[nBitPos] == 0)
                        stoch_org.genes[nBitPos]=1;
                    else
                        stoch_org.genes[nBitPos]=0;
                }
                // std::cout << "iteration: "<< i << "-- Press enter to continue --" << std::endl << flush;
                //std::cin.get(temp);
            }

            // Save the best solution from all repetitios
            if (nFitness > nBestFitness)
            {
                nBestFitness = nFitness;
                best_stoch_org=stoch_org;
            }
        }

        // Add up numbers for each Run
        if (nBestFitness > nRunsMaxFitness)
        {
            nRunsMaxFitness = nBestFitness;
        }
        if (nBestFitness < nRunsMinFitness)
        {
            nRunsMinFitness = nBestFitness;
        }

        nRunFitness[nRun]=nBestFitness;
        nRunsAvgFitness+=nBestFitness;
    }

    end=clock();
    diff=end-start;

    // Calculate RUN results
    nRunsAvgFitness=nRunsAvgFitness/number_runs;

    for (int i=0; i<nRun; i++)
    {
        nRunsAvgFitnessVariance+=pow(nRunsAvgFitness-nRunFitness[i],2);
    }
    nRunsAvgFitnessVariance=nRunsAvgFitnessVariance/number_runs;
    nRunsAvgFitnessSTD=pow(nRunsAvgFitnessVariance, 0.5);

    std::cout << "---------- STOCHASTIC HILLCLIMBING FINAL RESULTS ----------" << std::endl;
    std::cout << "Number of Runs: "  << nRun << std::endl;
    std::cout << "Function: "        << eval.szName << std::endl;
    std::cout << "Runs max fitness: "<< nRunsMaxFitness << std::endl;
    std::cout << "Runs min fitness: "<< nRunsMinFitness << std::endl;
    std::cout << "Runs avg fitness: "<< nRunsAvgFitness << std::endl;
    std::cout << "Runs std dev fitness: "<< nRunsAvgFitnessSTD << std::endl;
    std::cout << "Simulation time in clocks: " << diff << std::endl;
    std::cout << "Simulation clocks per sec: " << CLOCKS_PER_SEC << std::endl;
    std::cout << "----------- PARAMETERS USED: -----------" << std::endl;
    std::cout << "Number of Iterations: "     << sh_iterations << std::endl;
    std::cout << "Number of Restarts: "  << sh_starts  << std::endl;
    std::cout << "Number of Parms: "     << eval.num_parms << std::endl;
    std::cout << "Number of Bits per Parm: " << eval.num_bits_per_parm << std::endl;
    std::cout << "Number of Function Evals: "     << function_cnt << std::endl;
    std::cout << "---------- END FINAL RUN RESULTS ------------" << std::endl;

    return 0;
}


int simgalib::run_sa(EvalFN eval, int number_runs, int sa_iterations, int sa_starts, double Kb, double nTempDelta)
{
    clock_t start,end,diff=0;
    start=clock();

    // Run params
    int nRun=0;
    double nRunsAvgFitness=0;
    double nRunsMaxFitness=0;
    double nRunsMinFitness=999999999;
    double nRunFitness[number_runs];
    double nRunsAvgFitnessVariance=0;
    double nRunsAvgFitnessSTD=0;

    for (int i=0; i < number_runs ; i++)
        nRunFitness[i]=0;

    // Create and set variables for simmulated annealing
    int nBitPos=0;
    int nBitPos2=0;
    int nTmpPos=0;
    double nFitness=0;
    double test_nFitness=0;
    double nFitnessDelta=0;
    double nTemp=0;
    double nTempDecrement=nTempDelta/((double) sa_iterations/sa_starts );
    double nProb=0;
    int nRepetition=0;  // Number of times the algorithm repeats in each run
    double nBestFitness=0;
    int nFlip=0;
    int k=0;
    function_cnt=0;


    srand(clock());

    for (nRun=0; nRun < number_runs; nRun++)
    {
        Organism org(eval.num_parms*eval.num_bits_per_parm);
        Organism best_org(eval.num_parms*eval.num_bits_per_parm);
        Organism tmp_org(eval.num_parms*eval.num_bits_per_parm);

        nBestFitness=0;  // must initialize in every run

        for (nRepetition=0; nRepetition < sa_starts; nRepetition++)
        {

            // Initialize the starting point
            for (int j = 0; j < org.num_genes; j++)
            {
                org.genes.at(j)=flip(.5);
            }

            // Init temp to max value
            nTemp=nTempDelta;

            //    for (int j = 0; j < org.num_genes; j++)
            //    std::cout << org.genes[j] << std::endl;

            //    std::cout << "---------- RUN NUMBER" << nRun << std::endl;
            //    std::cout << "---------- BEGIN OPTIMIZATION ----------" << std::endl;

            nFitness=eval.evaluate(org.genes);

            // Gen. function: set of moves adapted from Neuro-Fuzzy and SoftComputing (Jang) p. 184
            for (int i=0; i < sa_iterations/sa_starts ; i++)
            {
                // copy org.genes to tmp_org.genes
                for (int j = 0; j < org.num_genes; j++)
                {
                    tmp_org.genes.at(j)=org.genes.at(j);
                }

                switch ((int) ran2(3))
                {
                case 0:// invert the order of the genes in the range
                    // randomly select bit positions to invert
#if 0
                    // invert up to all bits in organism
                    nBitPos=(int)ran2(org.genes.size());
                    nBitPos2=(int)ran2(org.genes.size());
                    if (nBitPos>nBitPos2)
                    {
                        nTmpPos=nBitPos;
                        nBitPos=nBitPos2;
                        nBitPos2=nTmpPos;
                    }
#endif
                    // invert up to three bits
                    nBitPos=(int)ran2(org.genes.size()-2);
                    nBitPos2=nBitPos+2;

                    k=0;
                    for (int j=nBitPos; j<=nBitPos2; j++)
                    {
                        tmp_org.genes[j] = org.genes[nBitPos2-k];
                        k++;
                    }
                    break;
                case 1:
                    // translate two genes to a different area
                    nTmpPos=(int)ran2(org.genes.size()-1); // where to translate
                    nBitPos=(int)ran2(org.genes.size()-1); // which to translate

                    if (nTmpPos < nBitPos)
                    {
                        for (int j=0; j < nBitPos-nTmpPos; j++) // make room for the genes
                        {
                            tmp_org.genes.at(nBitPos+1-j) = tmp_org.genes.at(nBitPos-1-j);
                        }

                        tmp_org.genes.at(nTmpPos)=org.genes.at(nBitPos);
                        tmp_org.genes.at(nTmpPos+1)=org.genes.at(nBitPos+1);
                    }
                    else if (nTmpPos > nBitPos)
                    {
                        for (int j=0; j < nBitPos-nTmpPos; j++) // make room for the genes
                        {
                            tmp_org.genes.at(nBitPos+j) = tmp_org.genes.at(nBitPos+2+j);
                        }

                        tmp_org.genes.at(nTmpPos)=org.genes.at(nBitPos);
                        tmp_org.genes.at(nTmpPos+1)=org.genes.at(nBitPos+1);
                    }
                    break;

                case 2:
                    // switch two genes
                    // randomly select bit positions
                    nBitPos=(int)ran2(org.genes.size());
                    nBitPos2=(int)ran2(org.genes.size());

                    tmp_org.genes.at(nBitPos)=org.genes.at(nBitPos2);
                    tmp_org.genes.at(nBitPos2)=org.genes.at(nBitPos);
                    break;
                }

                test_nFitness=eval.evaluate(tmp_org.genes);
                function_cnt++;

                // Check using acceptance funcion to see whether to keep test position
                // NOTE: Since we are maximizing we subtract test from current
                nFitnessDelta=nFitness-test_nFitness;

                nProb=exp(-1*nFitnessDelta/(Kb*nTemp));
                if (nProb > 1.0)
                {
                    nProb=1.0;
                }
                else if (nProb < 0.0)
                {
                    nProb=0.0;
                }
                nFlip=flip(nProb);

                // Debug code to see the effect of Kb, Temp, Fitness, FitnessDelta, Prob, and Flip :-)
                // cout << "nFitness: " << nFitness << " test_nFitness: " << test_nFitness << endl;
                // cout << "nFitnessDelta: " << nFitnessDelta << " Kb: " << Kb << " nTemp: " << nTemp << endl;
                // cout << "nProb: " << nProb << "nFlip: " << nFlip << endl;

                if ((test_nFitness >= nFitness)||((test_nFitness < nFitness) && nFlip))
                {
                    // save temp_org.genes and fitness value
                    nFitness = test_nFitness;
                    for (int j = 0; j < org.num_genes; j++)
                    {
                        org.genes.at(j)=tmp_org.genes.at(j);
                    }
                }
                // std::cout << "iteration: "<< i << "-- Press enter to continue --" << std::endl << flush;
                //std::cin.get(temp);

                // reduce temp using linear decrement of TempDelta
                nTemp-=nTempDecrement;

            } // for (int i=1; i < (sa_iterations) ;i++)

            // Save the best solution from all repetitios
            if (nFitness > nBestFitness)
            {
                nBestFitness = nFitness;
                best_org=org;
            }
        } // for (nRepetition=0; nRepetition < sa_restarts;nRepetition++)

        // Add up numbers for each Run
        if (nBestFitness > nRunsMaxFitness)
        {
            nRunsMaxFitness = nBestFitness;
        }
        if (nBestFitness < nRunsMinFitness)
        {
            nRunsMinFitness = nBestFitness;
        }

        nRunFitness[nRun]=nBestFitness;
        nRunsAvgFitness+=nBestFitness;
    } // for (nRun=0; nRun < number_runs; nRun++)

    end=clock();
    diff=end-start;

    // Calculate RUN results
    nRunsAvgFitness=nRunsAvgFitness/number_runs;
    for (int i=0; i<nRun; i++)
    {
        nRunsAvgFitnessVariance+=pow(nRunsAvgFitness-nRunFitness[i],2);
    }

    nRunsAvgFitnessVariance=nRunsAvgFitnessVariance/number_runs;
    nRunsAvgFitnessSTD=pow(nRunsAvgFitnessVariance, 0.5);

    //Debug code to see the fitness value in each run
    //for (int i=0; i < number_runs ; i++)
    //    std::cout << "i:fitness" << i << ":" << nRunFitness[i] << std::endl;

    std::cout << "---------- SIMULATED ANNEALING FINAL RESULTS ----------" << std::endl;
    std::cout << "Number of Runs: "  << nRun << std::endl;
    std::cout << "Function: "        << eval.szName << std::endl;
    std::cout << "Runs max fitness: "<< nRunsMaxFitness << std::endl;
    std::cout << "Runs min fitness: "<< nRunsMinFitness << std::endl;
    std::cout << "Runs avg fitness: "<< nRunsAvgFitness << std::endl;
    std::cout << "Runs std dev fitness: "<< nRunsAvgFitnessSTD << std::endl;
    std::cout << "Simulation time in clocks: " << diff << std::endl;
    std::cout << "Simulation clocks per sec: " << CLOCKS_PER_SEC << std::endl;
    std::cout << "----------- PARAMETERS USED: -----------" << std::endl;
    std::cout << "Number of Iterations: "     << sa_iterations << std::endl;
    std::cout << "Number of Restarts: "  << sa_starts  << std::endl;
    std::cout << "Number of Parms: "     << eval.num_parms << std::endl;
    std::cout << "Number of Bits per Parm: " << eval.num_bits_per_parm << std::endl;
    std::cout << "Kb Energy (i.e. Fitness) scaling value: "     << Kb << std::endl;
    std::cout << "Starting Temperature: " << nTempDelta  << std::endl;
    std::cout << "Number of Function Evals: "     << function_cnt << std::endl;
    std::cout << "---------- END FINAL RUN RESULTS ------------" << std::endl;

    return 0;
}
