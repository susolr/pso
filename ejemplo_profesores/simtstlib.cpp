/*
 * FILE: simtstlib_test_fns.cpp, v.1.7.1, 4/28/2014
 * Author: Tomas V. Arredondo
 *
 * SimTstLib: A simple yet flexible implementation of some test functions in C++.
 *
 * Some basic test functions are defined in this file.
 *
 * DISCLAIMER: No liability is assumed by the author for any use made
 * of this program.
 * DISTRIBUTION: Any use may be made of this program, as long as the
 * clear acknowledgment is made to the author in code and runtime executables
 */
#include <iostream>
#include <vector>
#include <cmath>
#include "simtstlib.h"

using namespace simtstlib;

// A simple test function with one variable of 32 bits, a max val of 13.8526 in the range -3 to +3
double simtstlib::simple_test_fn_real(int num_vars, double x[])
{
    double rslt=0.0;
    rslt=(pow(x[0],3)*sin(x[0])) + x[0] + 2;
    return rslt;
}
double simtstlib::simple_test_fn(int num_vars, int bits_per_var, double range_low, double range_high,vector<int> genes)
{
    unsigned int uint_genes;
    int one_to_shift;
    double a_range=range_low;
    double b_range=range_high;
    double result=0;
    double x[1];
    int num_genes=genes.size();

    x[0]=0.0;
    uint_genes=0;
    one_to_shift=1;
    for (int i=0; i<num_genes; i++)
    {
        uint_genes += (genes[i] * one_to_shift );
        one_to_shift=one_to_shift<<1;
    }

    // Convert the unsigned integer sum of the genes into floating point number x1
    x[0] = a_range + (uint_genes * ((b_range-a_range)/(pow((double) 2.0, num_genes)-1)));

    result=simple_test_fn_real(num_vars, x);

    return result;
}

double simtstlib::dejong1_test_fn_real(int num_vars, double x[])
{
    double rslt=0.0;

    for (int j=0; j<num_vars; j++)
    {
        rslt+=(pow(x[j],2));
    }
    return rslt;
}
double simtstlib::dejong1_test_fn(int num_vars, int bits_per_var, double range_low, double range_high, vector<int> genes)
{
    unsigned int uint_genes;
    int one_to_shift;
    double a_range=range_low;
    double b_range=range_high;
    double result=0.0;
    double x[num_vars];

    for (int j=0; j<num_vars; j++)
    {
        x[j]=0.0;

        uint_genes=0;
        one_to_shift=1;
        for (int i=0; i<bits_per_var; i++)
        {
            uint_genes += (genes[(j*bits_per_var)+i] * one_to_shift );
            one_to_shift=one_to_shift<<1;
        }

        // Convert the unsigned integer sum of the genes into floating point number x
        x[j] = a_range + (uint_genes * ((b_range-a_range)/(pow((double) 2.0, bits_per_var)-1)));
    }

    result=dejong1_test_fn_real(num_vars, x);

    return result;
}

double simtstlib::dejong2_test_fn_real(int num_vars, double x[])
{
    double rslt=0.0;

    for (int j=0; j<num_vars-1; j++)
    {
        rslt+=(100*pow(pow(x[j],2)-x[j+1],2)) + pow((1-x[j]),2);
    }
    return rslt;
}
double simtstlib::dejong2_test_fn(int num_vars, int bits_per_var, double range_low, double range_high, vector<int> genes)
{
    unsigned int uint_genes;
    int one_to_shift;
    double a_range=range_low;
    double b_range=range_high;
    double result=0.0;
    double x[num_vars];

    for (int j=0; j<num_vars; j++)
    {
        x[j]=0.0;

        uint_genes=0;
        one_to_shift=1;
        for (int i=0; i<bits_per_var; i++)
        {
            uint_genes += (genes[(j*bits_per_var)+i] * one_to_shift );
            one_to_shift=one_to_shift<<1;
        }

        // Convert the unsigned integer sum of the genes into floating point number x
        x[j] = a_range + (uint_genes * ((b_range-a_range)/(pow((double) 2.0, bits_per_var)-1)));
    }

    //result=(100*pow(pow(x[0],2)-x[1],2)) + pow((1-x[0]),2);
    result=dejong2_test_fn_real(num_vars, x);

    return result;
}

/*******************************************************************************************
float ran2(long *idum)
Numerical Recipes in C: The Art of Scientific Computing. William H. Press - Saul A. Teukolsky - William T. Vetterling - Brian P. Flanner
Long period (> 2 × 10^18) random number generator of L’Ecuyer with Bays-Durham shuffle and added safeguards.
Returns a uniform random deviate between 0.0 and 1.0 (exclusive of the endpoint values).
Call with idum a negative integer to initialize; thereafter, do not alter idum between successive deviates in a sequence.
RNMX should approximate the largest floating value that is less than 1.
*******************************************************************************************/
// seed random number generator for ran2
long semran2s = -1000;

float simtstlib::ran2(long *idum)
{
    int j;
    long k;
    static long idum2=123456789;
    static long iy=0;
    static long iv[NTAB];
    float temp;
    if (*idum <= 0)   //Initialize.
    {

        if (-(*idum) < 1)           //Be sure to prevent idum = 0.
            *idum=1;
        else
            *idum = -(*idum);

        idum2=(*idum);

        for (j=NTAB+7; j>=0; j--)  //Load the shufle table (after 8 warm-ups).
        {
            k=(*idum)/IQ1;
            *idum=IA1*(*idum-k*IQ1)-k*IR1;
            if (*idum < 0)
                *idum += IM1;
            if (j < NTAB)
                iv[j] = *idum;
        }

        iy=iv[0];
    }
    k=(*idum)/IQ1; 					//Start here when not initializing.
    *idum=IA1*(*idum-k*IQ1)-k*IR1;  //Compute idum=(IA1*idum) % IM1 without

    if (*idum < 0)
        *idum += IM1; 				//overflows by Schrage’s method.

    k=idum2/IQ2;
    idum2=IA2*(idum2-k*IQ2)-k*IR2; //Compute idum2=(IA2*idum) % IM2 likewise.

    if (idum2 < 0)
        idum2 += IM2;

    j=iy/NDIV; 						//Will be in the range 0..NTAB-1.
    iy=iv[j]-idum2; 				//Here idum is shuffled, idum and idum2 are
    iv[j] = *idum; 					//combined to generate output.

    if (iy < 1)
        iy += IMM1;

    if ((temp=AM*iy) > RNMX)
        return RNMX; 				//Because users don’t expect endpoint values.
    else
        return temp;
}

/* float ran2(int num): generates an integer random number between [0-num) (num non inclusive)*/
float simtstlib::ran2(int num)
{
    float f = 0;
    f = ran2((float)(num-1), 0.0);
    return f;
}

/* float ran2(float max, float min): uses rand() to generate a float random number */
/* between [min,max] (both inclusive). */
float simtstlib::ran2(float max, float min)
{
    float f = 0;
    f = (min) + ((max)-(min))*ran2(&(semran2s));
    return f;
}

/*******************************************************************/
// The following random2 functions uses rand() which has a period of
// RAND_MAX and could cause problems... user beware.
/*******************************************************************/

/* float random2(): uses rand() to generate a float random number between [0-1] (inclusive).*/
/* Note that rand( ) based functions are not recommended.*/
float simtstlib::random2()
{
    float result=0;

    result=(float) rand() / (float) RAND_MAX;
    return result;
}

/* float random2(int num): generates an integer random number between [0-num) (num non inclusive)*/
float simtstlib::random2(int num)
{
    float result=0;

    if (num)
    {
        result=(rand()%num);
    }
    return result;
}

/* float random2(float max, float min): uses rand() to generate a float random number */
/* between [min,max] (both inclusive). Note that rand( ) based functions are not recommended.*/
float simtstlib::random2(float max, float min)
{
    float f = 0;
    f = (min) + ((max)-(min))*(rand()/((float) RAND_MAX));
    return f;
}

/* flip flips a coin, with probability (prob) returns a 1 else a 0 */
int simtstlib::flip(double prob)
{
    double i;

    i = ran2(&(semran2s));

    if (i<=prob)
        return 1;
    else
        return 0;
}