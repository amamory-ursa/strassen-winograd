//
//  main.c
//  strassen-winograd
//
//  Created by Pavel Kravets on 03.10.13.
//  Copyright (c) 2013 Pavel Kravets. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "naive.h"
#include "winograd.h"
#include "strassen.h"
#ifdef USE_ORCA
#include "orca-hardware-counters.h"
#else
#include <sys/time.h>
#include <sys/resource.h>
#endif


#ifndef USE_ORCA
double calculate(const struct rusage* b, const struct rusage* a)
{
    if (b == NULL || a == NULL)
    {
        return 0.0;
    }
    else
    {
        return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
                  (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
                 ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
                  (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec)))
                / 1000000.0);
    }
}
#endif

void experiment()
{
    #ifdef USE_ORCA
    unsigned int N=16;
    unsigned int LIMIT=64;
    unsigned int before, after;
    #else
    unsigned int N=64;
    unsigned int LIMIT=512;
    struct rusage before, after;
    #endif
    
    while (N<=LIMIT)
    {
        #ifdef USE_ORCA
        unsigned int M = 1;
        unsigned int naive_mult_time = 0;
        unsigned int winograd_mult_time = 0;
        unsigned int strassen_mult_time = 0;
        #else
        unsigned int M = (2048 / N);
        double naive_mult_time = .0;
        double winograd_mult_time = .0;
        double strassen_mult_time = .0;
        #endif
        
        matrix* m1 = alloc_matrix();
        matrix* m2 = alloc_matrix();
        matrix* naive_res = alloc_matrix();
        matrix* winograd_res = alloc_matrix();
        matrix* strassen_res = alloc_matrix();
        
        printf("starting to alloc %dx%d matrix ...\n", N, N);
        random_matrix(N, N, m1);
        random_matrix(N, N, m2);
        printf("allocated!\n");
        for(int i = 0; i<M; i++)
        {

            #ifdef USE_ORCA
            before = *CPU_COUNTER_CYCLES_TOTAL;
            #else
            getrusage(RUSAGE_SELF, &before);
            #endif
            naive_mult(m1, m2, naive_res);
            #ifdef USE_ORCA
            after = *CPU_COUNTER_CYCLES_TOTAL;
            naive_mult_time += after - before;
            #else
            getrusage(RUSAGE_SELF, &after);
            naive_mult_time += calculate(&before, &after);
            #endif
            
        }
        naive_mult_time /= M;
        printf("naive done!\n");
        
        
        for(int i = 0; i<M; i++)
        {
            #ifdef USE_ORCA
            before = *CPU_COUNTER_CYCLES_TOTAL;
            #else
            getrusage(RUSAGE_SELF, &before);
            #endif
            winograd_mult(m1, m2, winograd_res);
            #ifdef USE_ORCA
            after = *CPU_COUNTER_CYCLES_TOTAL;
            winograd_mult_time += after - before;
            #else
            getrusage(RUSAGE_SELF, &after);
            winograd_mult_time += calculate(&before, &after);
            #endif
        }
        winograd_mult_time /= M;
        printf("winograd done!\n");
        
        
        for(int i = 0; i<M; i++)
        {
            #ifdef USE_ORCA
            before = *CPU_COUNTER_CYCLES_TOTAL;
            #else
            getrusage(RUSAGE_SELF, &before);
            #endif
            strassen_mult(m1, m2, strassen_res, 100, 32);
            #ifdef USE_ORCA
            after = *CPU_COUNTER_CYCLES_TOTAL;
            strassen_mult_time += after - before;
            #else
            getrusage(RUSAGE_SELF, &after);
            strassen_mult_time += calculate(&before, &after);
            #endif
        }
        strassen_mult_time /= M;
        printf("strassen done!\n");
        
        #ifdef USE_ORCA
        printf("{\"N\" : %d, \"naive\" : %d, \"winograd\" : %d, \"strassen\" : %d}\n\n", N, naive_mult_time, winograd_mult_time, strassen_mult_time);
        #else
        printf("{\"N\" : %d, \"naive\" : %f, \"winograd\" : %f, \"strassen\" : %f}\n\n", N, naive_mult_time, winograd_mult_time, strassen_mult_time);
        #endif
        
        free_matrix(m1);
        free_matrix(m2);
        free_matrix(naive_res);
        free_matrix(winograd_res);
        free_matrix(strassen_res);
        
        N *= 2;
    }
}

int main(int argc, const char * argv[])
{
    experiment();
    return 0;
}

