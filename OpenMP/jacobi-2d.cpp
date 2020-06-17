/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/* jacobi-2d.c: this file is part of PolyBench/C */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

/* Include polybench common header. */
#include <polybench.h>

/* Include benchmark-specific header. */
#include "jacobi-2d.h"


/* Array initialization. */
static
void init_array (int n,
         DATA_TYPE POLYBENCH_2D(A,N,N,n,n),
         DATA_TYPE POLYBENCH_2D(B,N,N,n,n))
{
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      {
    A[i][j] = ((DATA_TYPE) i*(j+2) + 2) / n;
    B[i][j] = ((DATA_TYPE) i*(j+3) + 3) / n;
      }
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int n,
         DATA_TYPE POLYBENCH_2D(A,N,N,n,n))

{
  int i, j;

  POLYBENCH_DUMP_START;
  POLYBENCH_DUMP_BEGIN("A");
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++) {
      if ((i * n + j) % 20 == 0) fprintf(POLYBENCH_DUMP_TARGET, "\n");
      fprintf(POLYBENCH_DUMP_TARGET, DATA_PRINTF_MODIFIER, A[i][j]);
    }
  POLYBENCH_DUMP_END("A");
  POLYBENCH_DUMP_FINISH;
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_jacobi_2d(int tsteps,
                int n,
                DATA_TYPE POLYBENCH_2D(A,N,N,n,n),
                DATA_TYPE POLYBENCH_2D(B,N,N,n,n))
{
  int t, i, j;

#pragma scop
  for (t = 0; t < _PB_TSTEPS; t++)
  {
  // #pragma omp parallel num_threads(THREAD_NUM)
  // {
  //   #pragma omp for private(j) schedule(static, CHUNK_SIZE)
  //   for (i = 1; i < _PB_N - 1; i++)
  //     for (j = 1; j < _PB_N - 1; j++)
  //       B[i][j] = SCALAR_VAL(0.2) * (A[i][j] + A[i][j-1] + A[i][1+j] + A[1+i][j] + A[i-1][j]);
  //   #pragma omp for private(j) schedule(static, CHUNK_SIZE)
  //   for (i = 1; i < _PB_N - 1; i++)
  //     for (j = 1; j < _PB_N - 1; j++)
  //       A[i][j] = SCALAR_VAL(0.2) * (B[i][j] + B[i][j-1] + B[i][1+j] + B[1+i][j] + B[i-1][j]);
  // }
# ifdef _OPENMP
  #pragma omp parallel num_threads(THREAD_NUM)
  {
    polybench_start_per_thread_instruments(omp_get_thread_num());
    #pragma omp for private(j) schedule(static, CHUNK_SIZE)
# endif
    for (i = 0; i < _PB_N - 2; i++)
    {
      for (j = 0; j < _PB_N - 2; j++)
      {
        B[i+1][j+1] = SCALAR_VAL(0.2) * (A[i+1][j+1] + A[i+1][j] + A[i+1][j+2] + A[i+2][j+1] + A[i][j+1]);
      }
    }
# ifdef _OPENMP
    #pragma omp for private(j) schedule(static, CHUNK_SIZE)
# endif
    for (i = 0; i < _PB_N - 2; i++)
    {
      for (j = 0; j < _PB_N - 2; j++)
      {
        A[i+1][j+1] = SCALAR_VAL(0.2) * (B[i+1][j+1] + B[i+1][j] + B[i+1][j+2] + B[i+2][j+1] + B[i][j+1]);
      }
    }
# ifdef _OPENMP
    polybench_stop_per_thread_instruments(omp_get_thread_num());
  }
# endif
  }
#pragma endscop

}


int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int n = N;
  int tsteps = TSTEPS;

  /* Variable declaration/allocation. */
  POLYBENCH_2D_ARRAY_DECL(A, DATA_TYPE, N, N, n, n);
  POLYBENCH_2D_ARRAY_DECL(B, DATA_TYPE, N, N, n, n);


  /* Initialize array(s). */
  init_array (n, POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(B));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_jacobi_2d(tsteps, n, POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(B));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(n, POLYBENCH_ARRAY(A)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(B);

  return 0;
}
