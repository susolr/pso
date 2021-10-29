/*
 * FILE: simgalib.h, v.1.7.1, 4/28/2014
 * Author: Tomas V. Arredondo
 *
 * SimGALib: A simple yet flexible GA (Holland 1975) implementation in C++.
 * Stochastic Hillclimbing (Juels 1994), Simulated Annealing (Kirkpatrick et al 1983)
 * are also implemented here due to their underlying compatibility.
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
#ifndef SIMGALIB_H
#define SIMGALIB_H

#include <iostream>
#include <vector>
#include <cmath>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "simtstlib.h"

namespace simgalib
{
using namespace std;

// Early declaration
class EvalFN;

// General options:
enum enumEvolutionaryModel {Darwinian=0, Lamarckian=1, Baldwinian=2};  // not implemented yet

//-----------------------------------------------------------------------------
// Optimization Methods
// WARNING: ALL THESE METHODS ASUME THAT WE ARE MAXIMIZING THE OBJECTIVE FUNCTION.
//-----------------------------------------------------------------------------

// 1- Genetic Algorithm (Holland 1975)
enum enumSelectionMethod {FitnessProp=0, Ranking=1, Tournament=2};

int run_ga(EvalFN eval, int number_runs, int ga_pop_size, int ga_number_iters,
           enumSelectionMethod selection_method, bool elite_flag, float Pcross,
           float Pmut, float tournament_Prob, float ranking_MAX );

// 2- Stochastic Hillclimbing (Juels 1994)
int run_sh(EvalFN eval, int number_runs, int sh_iterations, int sh_restarts);

// 3- Simulated Annealing (Kirkpatrick et al 1983)
// Kb should be about 10% of X range (Energy range), TempDelta is the Temp range
int run_sa(EvalFN eval, int number_runs, int sa_iterations, int sa_restarts, double Kb, double nTempDelta);

// **********************************
// Default simulation values here
#define EVOLUTIONARY_MODEL 0         // default is Darwinian
#define SELECTION_METHOD 0           // default is fitness proportional
#define POPULATION_SIZE 100          // value recommended by De Jong (1975)
#define ELITE_FLAG 1
#define ELITE_COUNT 1
#define PCROSS 0.5                  // value recommended by De Jong (1975)
#define PMUT 0.001                  // value recommended by De Jong (1975)
#define NUM_ITERATIONS 100
#define NUM_GENES_UNDEF -1
#define NUM_GENES 32
#define RANK_MIN 1.0
#define RANK_MAX 1.1                // value recommended by Baker (1985)
#define PTOURNAMENT 0.75            // value in Goldberg (1991)=0.5, M. Mitchell (1996)=0.75

// Default simulation values here
// **********************************
class Organism
{
public:

    Organism()
    {
        num_genes=NUM_GENES_UNDEF;   // 1.7.0
        // Note: this constructor can be used by the elite organism
    }
    Organism(int tmp_num_genes)
    {
        num_genes=tmp_num_genes;
        genes = vector<int>(num_genes);
    }
    friend ostream & operator<<(ostream& output, const Organism& p);
    friend class Population;
    vector<int> genes ;
    double fitness;         // Fitness proportional fitness
    double rank_fitness;    // Rank fitness
    double deviation;
    int num_genes;
    int index;
};

// organism sorting comparison functor: greater to lesser order
struct greater_mag : public binary_function<Organism *, Organism *, bool>
{
    bool operator()(Organism *x, Organism *y)
    {
        return x->fitness > y->fitness;
    }
};

// Early declaration
class Population_data;

class RankingParms
{
private:
    float min; // Ranking: min expected num of descendents for worst individual
    float max; // Ranking: max expected num of descendents for worst individual
    float inc; // increment
    int popsize;
public:
    void updateInc(int tmp_popsize)
    {
        popsize=tmp_popsize; // make sure popsize is up to date :-)
        inc = 2.0 * (max - 1.0) / popsize;
    }
    void setMax(float tmp_max)
    {
        // max should be chosen: 1.0 <= max <= 2.0
        max = tmp_max;
        min = 2.0 - max; // min will be: 0.0 <= min <= 1.0
    }
    float getMax() const
    {
        return max;
    }
    void setPopSize(int tmp_popsize)
    {
        popsize = tmp_popsize;
    }
    void setInc(float tmp_inc)
    {
        inc = tmp_inc;
    }
    float getInc()
    {
        return inc;
    }
    RankingParms(int tmp_popsize)
    {
        setMax(1.1);    // value recommended by Baker (1985)
        updateInc(tmp_popsize);
    }
    RankingParms()
    {
        min=RANK_MIN;
        max=RANK_MAX;
        popsize=POPULATION_SIZE;
        inc = 2.0 * (max - 1.0) / popsize;
    }
};

class EvalFN
{
    friend class Population;
public:
    int num_parms;
    int num_bits_per_parm;
    double lower_range;
    double upper_range;
    double (*eval_fn)(int, int, double, double, vector<int>);
    char szName[256];
    EvalFN(): num_parms(0), num_bits_per_parm(0), lower_range(0), upper_range(0), eval_fn(0)
    {
        ;
    }
    EvalFN(char *tmp_name, int tmp_num_parms, int tmp_num_bits_per_parm, double tmp_lower_range, double tmp_upper_range, double (*tmp_eval_fn)(int, int, double, double, vector<int>))
    {
        strcpy(szName, tmp_name);
        num_parms=tmp_num_parms;
        num_bits_per_parm=tmp_num_bits_per_parm;
        lower_range=tmp_lower_range;
        upper_range=tmp_upper_range;
        eval_fn=tmp_eval_fn;
    }
    double evaluate(vector<int> genes);
};

class Population
{
private:
    int population_size;
    int num_iterations;
    EvalFN evaluator;
    enumEvolutionaryModel evol_model;
    enumSelectionMethod selection_method;
    int num_genes;
    float pcross;
    float pmut;
    float ptournament;
    bool elite_flag;
    int elite_count;
    RankingParms rank_parms;
    Organism elite_organism;

public:
    vector<Organism *> pool;
    void create();
    void destroy();
    void display();
    void evaluate();
    void crossover(const Population_data& pop_info);
    void mutate();
    Population()
    {
        // This population constructor uses a default number of genes per Organism
        population_size=POPULATION_SIZE;
        num_iterations=NUM_ITERATIONS;
        evol_model=(enumEvolutionaryModel) EVOLUTIONARY_MODEL;
        selection_method=(enumSelectionMethod) SELECTION_METHOD;
        elite_flag=ELITE_FLAG;
        pcross=PCROSS;
        pmut=PMUT;
        ptournament=PTOURNAMENT;
        num_genes=NUM_GENES;
    }
    Population(int tmp_num_genes)
    {
        // This population constructor is passed the number of genes per Organism
        population_size=POPULATION_SIZE;
        num_iterations=NUM_ITERATIONS;
        evol_model=(enumEvolutionaryModel) EVOLUTIONARY_MODEL;
        selection_method=(enumSelectionMethod) SELECTION_METHOD;
        elite_flag=ELITE_FLAG;
        pcross=PCROSS;
        pmut=PMUT;
        ptournament=PTOURNAMENT;
        num_genes=tmp_num_genes;
    }
    ~Population()
    {
        ;
    }
    void setEvalFN(EvalFN tmp_evaluator)
    {
        evaluator=tmp_evaluator;
    }
    void setNumIters(int tmp_numiters)
    {
        num_iterations=tmp_numiters;
    }
    int getNumIters() const
    {
        return num_iterations;
    }
    void setEvolutionaryModel(enumEvolutionaryModel tmp_evol_model)
    {
        evol_model=tmp_evol_model;
    }
    enumEvolutionaryModel getEvolModel() const
    {
        return evol_model;
    }
    void setSelectionMethod(enumSelectionMethod tmp_selection_method)
    {
        selection_method=tmp_selection_method;
    }
    enumSelectionMethod getSelectionMethod() const
    {
        return selection_method;
    }
    void setSize(int tmp_size)
    {
        population_size=tmp_size;
        rank_parms.updateInc(tmp_size);
    }

    int getSize() const
    {
        return population_size;
    }
    void setEliteFlag(bool tmp_eliteflag)
    {
        elite_flag=tmp_eliteflag;
    }
    bool getEliteFlag() const
    {
        return elite_flag;
    }
    int getElite_count() const
    {
        return elite_count;
    }
    void setPcross(float tmp_pcross)
    {
        pcross=tmp_pcross;
    }
    float getPcross() const
    {
        return pcross;
    }
    void setPmut(float tmp_pmut)
    {
        pmut=tmp_pmut;
    }
    float getPmut() const
    {
        return pmut;
    }
    void setPtournament(float tmp_ptournament)
    {
        ptournament=tmp_ptournament;
    }
    float getPtournament() const
    {
        return ptournament;
    }
    void setRankingParms(RankingParms tmp_rankparms)
    {
        rank_parms=tmp_rankparms;
    }
    float getRankingMaxParm() const
    {
        return rank_parms.getMax();
    }
    int getNumGenes() const
    {
        return num_genes;
    }
};

class Population_data
{
public:
    int           max_index;
    Organism    max_organism;
    double        max_fitness;
    int           min_index;
    Organism    min_organism;
    double        min_fitness;
    double        avg_fitness;
    double        sum_fitness;
    double        sum_rank_fitness;
    void clear_pop_data()
    {
        max_index=0;
        max_fitness=0;
        min_index=0;
        min_fitness=0;
        avg_fitness=0;
        sum_fitness=0;
        sum_rank_fitness=0;
    }
    void evaluate_population_info(const Population * pop);
    void display_population_stats();
};




} // namespace simgalib


#endif // SIMGALIB_H