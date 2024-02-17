/**
 * @file pruebas.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <cmath>

using namespace std;

int main(int argc, char **argv)
{

    double sig;

    sig = 1.0 / (1.0 + exp(-1.7)); // Sigmoidea
    cout << "sig: " << sig << endl;
    cout << rand() << endl;
    double var_rand = (rand() % 10000) * 1.0 / 10000.0;
    cout << var_rand << endl;
    if (0.02 < sig)
    {
        cout << 1 << endl;
    }
    else
    {
        cout << 0 << endl;
    }

    return 0;
}