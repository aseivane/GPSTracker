/*
 * file.c
 *
 *  Created on: 4 ene. 2021
 *      Author: z0042kvk
 */
#include "tools.h"


// Function to set the kth bit of n
int setBit(int n, int k)
{
    return (n | (1 << (k - 1)));
}

// Function to clear the kth bit of n
int clearBit(int n, int k)
{
    return (n & (~(1 << (k - 1))));
}

// Function to toggle the kth bit of n
int toggleBit(int n, int k)
{
    return (n ^ (1 << (k - 1)));
}

int pow_10(uint8_t n)
{
    if(!n) return 1;
    n--;
    int power = 10;
    while(n)
    {
        power*=10;
        n--;
    }
    return power;
}

void ascii_to_float(uint8_t* ptrstr, float* num)
{
    uint8_t coma;
    uint8_t size;
    uint8_t entero;
    uint8_t* str=ptrstr;

    *num = 0;

    if(str[0] == '-') str++;

    for (size = 0; str[size] != '\0'; size++);
    size--;

    for ( coma = 0; str[size-coma] != '.'; coma++)
        *num += (str[size-coma] - 48)*pow_10(coma);
    coma++;

    *num = (*num)/pow_10(coma-1);

    for ( entero = coma ; (size+1) != entero; entero++)
        *num += (str[size-entero] - 48)*pow_10(entero-coma);

    if(ptrstr[0]=='-') *num = -*num;
}
