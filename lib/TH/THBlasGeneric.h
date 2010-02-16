#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "THBlasGeneric.h"
#else

/* Level 1 */
void THBlas_(swap)(long n, real *x, long incx, real *y, long incy);
void THBlas_(scal)(long n, real a, real *x, long incx);
void THBlas_(copy)(long n, real *x, long incx, real *y, long incy);
void THBlas_(axpy)(long n, real a, real *x, long incx, real *y, long incy);
real THBlas_(dot)(long n, real *x, long incx, real *y, long incy);

/* Level 2 */
void THBlas_(gemv)(char trans, long m, long n, real alpha, real *a, long lda, real *x, long incx, real beta, real *y, long incy);
void THBlas_(ger)(long m, long n, real alpha, real *x, long incx, real *y, long incy, real *a, long lda);

/* Level 3 */
void THBlas_(gemm)(char transa, char transb, long m, long n, long k, real alpha, real *a, long lda, real *b, long ldb, real beta, real *c, long ldc);

#endif
