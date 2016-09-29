#include <stdio.h>
#include <immintrin.h>
#include <omp.h>
#include <math.h>
#include "computepi.h"

double compute_pi_baseline(size_t N)
{
    double pi = 0.0;
    double x;              
    for (size_t i = 0; i < N; i++) {
        int t = (i%2) ?-1:1;
        x = (double)t / (2*i + 1);      
        pi += x;       
    }
    return pi * 4.0;
}

double compute_pi_openmp(size_t N, int threads)
{
    double pi = 0.0;
    double x;       
    #pragma omp parallel num_threads(threads)
    {
        #pragma omp for private(x) reduction(+:pi)
        for (size_t i = 0; i < N; i++) {
            int t = (i%2) ?-1:1;
            x = (double)t / (2*i + 1);     
            pi += x; 
        }
    }
    return pi * 4.0;
}

double compute_pi_avx(size_t N)
{
    double pi = 0.0;
    double dt = 1.0 / N;
    register __m256d ymm0, ymm1, ymm2, ymm3, ymm4;
    ymm0 = _mm256_set_pd(1.0,-1.0,1.0,-1.0);
    ymm1 = _mm256_set1_pd(2.0);
    ymm2 = _mm256_set1_pd(1.0);
    ymm4 = _mm256_setzero_pd();             // sum of pi

    for (int i = 0; i <= N - 4; i += 4) {
        ymm3 = _mm256_set_pd(i ,i+1.0,i+2.0,i+3.0);     //
        ymm3 = _mm256_mul_pd(ymm1, ymm3);   //  
        ymm3 = _mm256_add_pd(ymm3, ymm2);   // 
        ymm3 = _mm256_div_pd(ymm0, ymm3);   // 
        ymm4 = _mm256_add_pd(ymm4, ymm3);   //
    }
    double tmp[4] __attribute__((aligned(32)));
    _mm256_store_pd(tmp, ymm4);             // move packed float64 values to  256-bit aligned memory location
    pi += tmp[0] + tmp[1] + tmp[2] + tmp[3];
    return pi * 4.0;
}

double compute_pi_avx_unroll(size_t N)
{
    double pi = 0.0;
    
    register __m256d ymm0, ymm1, ymm2, ymm3, ymm4,
             ymm5, ymm6, ymm7, ymm8, ymm9,
             ymm10;
    ymm0 = _mm256_set_pd(-1.0,1.0,-1.0,1.0);
    ymm1 = _mm256_set1_pd(2.0);
    ymm2 = _mm256_set1_pd(1.0);
                
    ymm7 = _mm256_setzero_pd();             // second sum of pi
    ymm8 = _mm256_setzero_pd();             // third sum of pi
    ymm9 = _mm256_setzero_pd();             // fourth sum of pi
    ymm10 = _mm256_setzero_pd();

    for (int i = 0; i <= N - 16; i += 16) {
    
        ymm3 = _mm256_set_pd(i+3.0 ,i+2.0,i+1.0,i);
        ymm4 = _mm256_set_pd(i+7.0 ,i+6.0,i+5.0,i+4.0);
        ymm5 = _mm256_set_pd(i+11.0 ,i+10.0,i+9.0,i+8.0);
        ymm6 = _mm256_set_pd(i+15.0 ,i+14.0,i+13.0,i+12.0);

        ymm3 = _mm256_mul_pd(ymm1, ymm3); 
        ymm4 = _mm256_mul_pd(ymm1, ymm4);
        ymm5 = _mm256_mul_pd(ymm1, ymm5);
        ymm6 = _mm256_mul_pd(ymm1, ymm6);

        ymm3 = _mm256_add_pd(ymm3, ymm2);
        ymm4 = _mm256_add_pd(ymm4, ymm2);
        ymm5 = _mm256_add_pd(ymm5, ymm2);
        ymm6 = _mm256_add_pd(ymm6, ymm2);

        ymm3 = _mm256_div_pd(ymm0, ymm3);
        ymm4 = _mm256_div_pd(ymm0, ymm4);
        ymm5 = _mm256_div_pd(ymm0, ymm5);
        ymm6 = _mm256_div_pd(ymm0, ymm6);
       
        ymm7 = _mm256_add_pd(ymm7, ymm3);
        ymm8 = _mm256_add_pd(ymm8, ymm4);
        ymm9 = _mm256_add_pd(ymm9, ymm5);
        ymm10 = _mm256_add_pd(ymm10, ymm6);
    }

    double tmp1[4] __attribute__((aligned(32)));
    double tmp2[4] __attribute__((aligned(32)));
    double tmp3[4] __attribute__((aligned(32)));
    double tmp4[4] __attribute__((aligned(32)));

    _mm256_store_pd(tmp1, ymm7);
    _mm256_store_pd(tmp2, ymm8);
    _mm256_store_pd(tmp3, ymm9);
    _mm256_store_pd(tmp4, ymm10);

    pi += tmp1[0] + tmp1[1] + tmp1[2] + tmp1[3] +
          tmp2[0] + tmp2[1] + tmp2[2] + tmp2[3] +
          tmp3[0] + tmp3[1] + tmp3[2] + tmp3[3] +
          tmp4[0] + tmp4[1] + tmp4[2] + tmp4[3];
    return pi * 4.0;
}
