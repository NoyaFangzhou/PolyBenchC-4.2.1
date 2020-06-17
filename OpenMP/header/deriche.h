/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
#ifndef _DERICHE_H
# define _DERICHE_H

/* Default to LARGE_DATASET. */
# if !defined(MINI_DATASET) && !defined(SMALL_DATASET) && !defined(MEDIUM_DATASET) && !defined(LARGE_DATASET) && !defined(EXTRALARGE_DATASET)
#  define LARGE_DATASET
# endif

# if !defined(W) && !defined(H)
/* Define sample dataset sizes. */
#  ifdef MINI_DATASET
#   define W 64 * POLYBENCH_LOOP_ITERATION
#   define H 64 * POLYBENCH_LOOP_ITERATION
#  endif

#  ifdef SMALL_DATASET
#   define W 192 * POLYBENCH_LOOP_ITERATION
#   define H 128 * POLYBENCH_LOOP_ITERATION
#  endif

#  ifdef MEDIUM_DATASET
#   define W 720 * POLYBENCH_LOOP_ITERATION
#   define H 480 * POLYBENCH_LOOP_ITERATION
#  endif

#  ifdef LARGE_DATASET
#   define W 4096 * POLYBENCH_LOOP_ITERATION
#   define H 2160 * POLYBENCH_LOOP_ITERATION
#  endif

#  ifdef EXTRALARGE_DATASET
#   define W 7680 * POLYBENCH_LOOP_ITERATION
#   define H 4320 * POLYBENCH_LOOP_ITERATION
#  endif


#endif /* !(W H) */

# define _PB_W POLYBENCH_LOOP_BOUND(W,w)
# define _PB_H POLYBENCH_LOOP_BOUND(H,h)


/* Default data type */
# if !defined(DATA_TYPE_IS_INT) && !defined(DATA_TYPE_IS_FLOAT) && !defined(DATA_TYPE_IS_DOUBLE)
#  define DATA_TYPE_IS_FLOAT
# endif

#ifdef DATA_TYPE_IS_INT
#  define DATA_TYPE int
#  define DATA_PRINTF_MODIFIER "%d "
#endif

#ifdef DATA_TYPE_IS_FLOAT
#  define DATA_TYPE float
#  define DATA_PRINTF_MODIFIER "%0.2f "
#  define SCALAR_VAL(x) x##f
#  define SQRT_FUN(x) sqrtf(x)
#  define EXP_FUN(x) expf(x)
#  define POW_FUN(x,y) powf(x,y)
# endif

#ifdef DATA_TYPE_IS_DOUBLE
#  define DATA_TYPE double
#  define DATA_PRINTF_MODIFIER "%0.2lf "
#  define SCALAR_VAL(x) x
#  define SQRT_FUN(x) sqrt(x)
#  define EXP_FUN(x) exp(x)
#  define POW_FUN(x,y) pow(x,y)
# endif

#endif /* !_DERICHE_H */
