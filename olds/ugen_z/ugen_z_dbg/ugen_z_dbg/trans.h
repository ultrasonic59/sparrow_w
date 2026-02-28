/***********************************************************
* Smithsonian Astrophysical Observatory
* Submillimeter Receiver Laboratory
* 
*
* transform.c                  S. Paine rev. 2010 February 2
*
*
* Discrete Fourier, Hartley, and Hilbert transforms.
************************************************************/

/*
 * This file contains functions for fast Fourier, Hartley, and Hilbert
 * transforms, written with the aid of Joerg Arndt's excellent text,
 * "Matters Computational," online at http://www.jjj.de/fxt.  The FFT's
 * and FHT's here omit the usual reverse-binary permutations, since the
 * transform-domain operations in am (convolution and Hilbert transformation)
 * can be done on the permuted data.
 *
 * For transforms, the top level external functions are:
 *  fft_dif(z, n) - forward complex fft, bit reversed output
 *  ifft_dit(z, n) - inverse complex fft, bit reversed input
 *  hilbert(z, n) - computes Im(z) as the Hilbert transform of Re(z)
 * 
 * For testing, there are two reverse binary permutation functions here,
 * for complex and real arrays:
 *  bitrev_permute(z, n) - reverse binary permutation of complex array z
 *  bitrev_permute_real(x, n) - reverse binary permutation of real array x
 *
 * All of these functions assume n is a power of 2, without checking.
 *
 * The FFT and FHT functions use a combined recursive-iterative strategy,
 * described in (Singleton, R. C. 1967. "On Computing the Fast Fourier
 * Transform," Communications of the ACM 10:647.  See also M. Frigo
 * S. G. Johnson 2005. "The Design and Implementation of FFTW3," Proc.
 * IEEE 93:216.)
 *
 * Taking fft_dif() as an example, fft_dif() calls the recursive FFT
 * fft_dif_rec(), setting an initial branch count of 1 for the recursion.
 * fft_dif_rec() calls itself recursively (twice) to perform two half-
 * sized transforms, until the transform is small enough to be done entirely
 * in L1 cache, at which point it calls an iterative FFT, fft_dif_iter(),
 * instead.  The branch count is doubled at each recursion stage.  Under
 * OpenMP, if the environment variable OMP_SET_NESTED=true, each recursive
 * call starts two new threads until the branch count exceeds the maximum
 * number of threads for a parallel region.
 *
 * There are two versions of each of the iterative FFT's and FHT's: a
 * conventional one (e.g. fft_dif_iter()) which minimizes trigonometric
 * computations at the expense of non-local memory access, and another
 * (e.g. fft_dif_iter_seq()) which accesses memory with unit stride.  As
 * Arndt points out in the text referenced above, on modern hardware the
 * unit-stride version can be significantly faster, despite the larger
 * number of arithmetic operations.  Tests on AMD Opteron and Intel
 * Pentium 4 showed this to be true down to transforms fitting in L2
 * cache, but for transforms fitting in L1 cache the non-local transforms
 * were faster.  Overall, the fastest strategy is to do recursive computation
 * down to the L1 cache size, then switch over to the non-local iterative
 * transform.
 */

#include <math.h>


#define TWOPI          6.283185307179586


#define L1_CACHE_BYTES  0x8000
#define FFT_UNIT_STRIDE 0

static void fft_dif_iter(double*, unsigned long);
static void fft_dif_iter_seq(double*, unsigned long);
static void fft_dif_rec(double*, unsigned long, int);

static void ifft_dit_iter(double*, unsigned long);
static void ifft_dit_iter_seq(double*, unsigned long);
static void ifft_dit_rec(double*, unsigned long, int);





/***********************************************************
* void fft_dif(double *z, unsigned long n)
*
* Purpose:
*   Computes the forward discrete Fourier transform of a complex
*   sequence z, using a radix 2 decimation-in-frequency FFT.
*   z[0..2n-1] is an array of n complex numbers, stored in the
*   usual way with real elements in z[0,2,..2n-2] and imaginary
*   elements in z[1,3,..2n-1].
*
*   Entering this function, z[] should be in normal order.  On
*   return, the FT is stored in bit-reversed order.
*
*   n must be a power of 2.
*
* Arguments:
*   double *z - array of 2n doubles, representing n complex numbers
*   unsigned long n - dimension of z, must be a power of 2
*
* Return:
*   none
************************************************************/

void fft_dif(double *z, unsigned long n)
{
    fft_dif_rec(z, n, 1);
    return;
}   /* fft_dif() */


/***********************************************************
* void ifft_dit(double *z, unsigned long n)
*
* Purpose:
*   Computes the inverse discrete Fourier transform of a complex
*   sequence z, using a radix 2 decimation-in-time FFT.
*   z[0..2n-1] is an array of n complex numbers, stored in the
*   usual way with real elements in z[0,2,..2n-2] and imaginary
*   elements in z[1,3,..2n-1].
*
*   Entering this function, z[] should be in bit-reversed order.
*   The returned inverse FT is restored to normal order.
*
*   n must be a power of 2.
*
* Arguments:
*   double *z - array of 2n doubles, representing n complex numbers
*   unsigned long n - dimension of z, must be a power of 2
*
* Return:
*   none
************************************************************/

void ifft_dit(double *z, unsigned long n)
{
    ifft_dit_rec(z, n, 1);
    return;
}   /* ifft_dit() */



/***********************************************************
* void hilbert(double *z, unsigned long n)
*
* Purpose:
*   Given a real sequence, initially stored in the complex array z,
*   this function computes the corresponding analytic sequence.  The
*   imaginary part is the Hilbert transform of the real part.
*
* Arguments:
*   double *z - array of 2n doubles, representing n complex numbers
*   int n - dimension of z, must be a power of 2
*
* Return:
*   none
************************************************************/

void hilbert(double *z, unsigned long n)
{
    double x;
    unsigned long i, n2;

    n2 = n << 1;
    /*
     * Compute the (bit-reversed) Fourier transform of z.
     */
    fft_dif(z, n);
    /*
     * Form the transform of the analytic sequence by zeroing
     * the transform for negative time, except for the (N/2)th.
     * element.  Since z is now in bit-reversed order, this means
     * zeroing every other complex element.  The array indices of
     * the elements to be zeroed are 6,7,10,11...etc. (The real
     * and imaginary parts of the (N/2)th element are in z[2] and
     * z[3], respectively.)
     */
    for (i = 6; i < n2; i += 4) {
        z[i] = 0.;
        z[i+1] = 0.;
    }
    /*
     * The 0th and (N/2)th elements get multiplied by 0.5.  Test
     * for the trivial 1-point transform, just in case.
     */
    z[0] *= 0.5;
    z[1] *= 0.5;
    if (n > 1) {
        z[2] *= 0.5;
        z[3] *= 0.5;
    }
    /*
     * Compute the inverse transform.
     */
    ifft_dit(z, n);
    /*
     * Normalize the array.  The factor of 2 is left over from
     * forming the transform in the time domain.
     */
    x = 2. / (double)n;
    for (i = 0; i < n2; ++i)
        z[i] *= x;
    return;
}   /* hilbert() */


/***********************************************************
* static void fft_dif_iter(double *z, unsigned long n)
*
* Purpose:
*   Computes the forward discrete Fourier transform of a complex
*   sequence z, using an iterative radix 2 decimation-in-frequency
*   FFT.  z[0..2n-1] is an array of n complex numbers, stored in the
*   usual way with real elements in z[0,2,..2n-2] and imaginary
*   elements in z[1,3,..2n-1].
*
*   Entering this function, z[] should be in normal order.  On
*   return, the FT is stored in bit-reversed order.
*
*   n must be a power of 2.
*
* Arguments:
*   double *z - array of 2n doubles, representing n complex numbers
*   unsigned long n - dimension of z, must be a power of 2
*
* Return:
*   none
************************************************************/

static void fft_dif_iter(double *z, unsigned long n)
{
    unsigned long i, n2;

    n2 = n << 1;
    for (i = n; i > 1; i >>= 1) {
        double a, b, c, s, t;
        unsigned long i2, j;
        i2 = i << 1;
        t = TWOPI / (double)i;
        a = sin(0.5 * t);
        a *= 2.0 * a;
        b = sin(t);
        c = 1.0;
        s = 0.0;
        for (j = 0; j < i; j += 2) {
            double tmp;
            unsigned long kr, kmax;
            kmax = n2 + j;
            for (kr = j; kr < kmax; kr += i2) {
                double ur, ui;
                unsigned long ki, mr, mi;
                ki = kr + 1;
                mr = kr + i;
                mi = mr + 1;
                ur = z[kr];
                ui = z[ki];
                z[kr] = ur + z[mr];
                z[ki] = ui + z[mi];
                ur -= z[mr];
                ui -= z[mi];
                z[mr] = ur * c - ui * s;
                z[mi] = ur * s + ui * c;
            }
            tmp = c;
            c -= a * c + b * s;
            s -= a * s - b * tmp;
        }
    }
    return;
}   /* fft_dif_iter() */


/***********************************************************
* static void fft_dif_iter_seq(double *z, unsigned long n)
*
* Purpose:
*   Computes the forward discrete Fourier transform of a complex
*   sequence z, using an iterative radix 2 decimation-in-frequency
*   FFT.  Compared with fft_dif_iter(), above, the inner loops
*   have been swapped to obtain sequential memory access
*   at the expense of more trig overhead.
*
*   z[0..2n-1] is an array of n complex numbers, stored in the
*   usual way with real elements in z[0,2,..2n-2] and imaginary
*   elements in z[1,3,..2n-1].
*
*   Entering this function, z[] should be in normal order.  On
*   return, the FT is stored in bit-reversed order.
*
*   n must be a power of 2.
*
* Arguments:
*   double *z - array of 2n doubles, representing n complex numbers
*   unsigned long n - dimension of z, must be a power of 2
*
* Return:
*   none
************************************************************/

static void fft_dif_iter_seq(double *z, unsigned long n)
{
    unsigned long i, n2;

    n2 = n << 1;
    for (i = n; i > 1; i >>= 1) {
        double a, b, t;
        unsigned long i2, k;
        i2 = i << 1;
        t = TWOPI / (double)i;
        a = sin(0.5 * t);
        a *= 2.0 * a;
        b = sin(t);
        for (k = 0; k < n2; k += i2) {
            double c, s;
            unsigned long j;
            c = 1.0;
            s = 0.0;
            for (j = 0; j < i; j += 2) {
                double ur, ui, tmp;
                unsigned long kr, ki, mr, mi;
                kr = k + j;
                ki = kr + 1;
                mr = kr + i;
                mi = mr + 1;
                ur = z[kr];
                ui = z[ki];
                z[kr] = ur + z[mr];
                z[ki] = ui + z[mi];
                ur -= z[mr];
                ui -= z[mi];
                z[mr] = ur * c - ui * s;
                z[mi] = ur * s + ui * c;
                tmp = c;
                c -= a * c + b * s;
                s -= a * s - b * tmp;
            }
        }
    }
    return;
}   /* fft_dif_iter_seq() */


/***********************************************************
* static void fft_dif_rec(double *z, unsigned long n, int nbranch)
*
* Purpose:
*   Computes the forward discrete Fourier transform of a complex
*   sequence z, using a radix 2 decimation-in-frequency FFT.
*   If the computation is small enough to fit in cache, it is
*   done iteratively.  Otherwise, it is done recursively until
*   the recursion descends to cache-sized chunks.
*
*   z[0..2n-1] is an array of n complex numbers, stored in the
*   usual way with real elements in z[0,2,..2n-2] and imaginary
*   elements in z[1,3,..2n-1].
*
*   Entering this function, z[] should be in normal order.  On
*   return, the FT is stored in bit-reversed order.
*
*   n must be a power of 2.

*   To support OpenMP parallelism, nbranch keeps track of the
*   number of active transforms at a given recursion level. On
*   the first call to this function, nbranch should be 1.  It
*   is then doubled for each recursion.
*
* Arguments:
*   double *z - array of 2n doubles, representing n complex numbers
*   unsigned long n - dimension of z, must be a power of 2
*   int nbranch - number of transforms at this recursion level
*
* Return:
*   none
************************************************************/

static void fft_dif_rec(double *z, unsigned long n, int nbranch)
{
    double a, b, c, s, t;
    unsigned long nh, kr;

    if (n == 1)
        return;
    if (n < (unsigned long)(L1_CACHE_BYTES / (2 * sizeof(double)))) {
        if (FFT_UNIT_STRIDE)
            fft_dif_iter_seq(z, n);
        else
            fft_dif_iter(z, n);
        return;
    }
    t = TWOPI / (double)n;
    a = sin(0.5 * t);
    a *= 2.0 * a;
    b = sin(t);
    c = 1.0;
    s = 0.0;
    for (kr = 0; kr < n; kr += 2) {
        double ur, ui, tmp;
        unsigned long ki, mr, mi;
        ki = kr + 1;
        mr = kr + n;
        mi = mr + 1;
        ur = z[kr];
        ui = z[ki];
        z[kr] = ur + z[mr];
        z[ki] = ui + z[mi];
        ur -= z[mr];
        ui -= z[mi];
        z[mr] = ur * c - ui * s;
        z[mi] = ur * s + ui * c;
        tmp = c;
        c -= a * c + b * s;
        s -= a * s - b * tmp;
    }
    nh = n >> 1;
    nbranch <<= 1;
    
	fft_dif_rec(z, nh, nbranch);
    fft_dif_rec(z + n, nh, nbranch);

    return;
}   /* fft_dif_rec() */


/***********************************************************
* static void ifft_dit_iter(double *z, unsigned long n)
*
* Purpose:
*   Computes the inverse discrete Fourier transform of a complex
*   sequence z, using an iterative radix 2 decimation-in-time FFT.
*   z[0..2n-1] is an array of n complex numbers, stored in the
*   usual way with real elements in z[0,2,..2n-2] and imaginary
*   elements in z[1,3,..2n-1].
*
*   Entering this function, z[] should be in bit-reversed order.
*   The returned inverse FT is restored to normal order.
*
*   n must be a power of 2.
*
* Arguments:
*   double *z - array of 2n doubles, representing n complex numbers
*   unsigned long n - dimension of z, must be a power of 2
*
* Return:
*   none
************************************************************/

static void ifft_dit_iter(double *z, unsigned long n)
{
    unsigned long i, n2;

    n2 = n << 1;
    for (i = 2; i <= n; i <<= 1) {
        double a, b, c, s, t;
        unsigned long i2, j;
        i2 = i << 1;
        t = -TWOPI / (double)i;
        a = sin(0.5 * t);
        a *= 2.0 * a;
        b = sin(t);
        c = 1.0;
        s = 0.0;
        for (j = 0; j < i; j += 2) {
            double tmp;
            unsigned long kr, kmax;
            kmax = n2 + j;
            for (kr = j; kr < kmax; kr += i2) {
                double vr, vi;
                unsigned long ki, mr, mi;
                ki = kr + 1;
                mr = kr + i;
                mi = mr + 1;
                vr = z[mr] * c - z[mi] * s;
                vi = z[mr] * s + z[mi] * c;
                z[mr] = z[kr] - vr;
                z[mi] = z[ki] - vi;
                z[kr] += vr;
                z[ki] += vi;
            }
            tmp = c;
            c -= a * c + b * s;
            s -= a * s - b * tmp;
        }
    }
    return;
}   /* ifft_dit_iter() */


/***********************************************************
* static void ifft_dit_iter_seq(double *z, unsigned long n)
*
* Purpose:
*   Computes the inverse discrete Fourier transform of a complex
*   sequence z, using an iterative radix 2 decimation-in-time FFT.
*   Compared with ifft_dit_iter(), above, the inner loops
*   have been swapped to obtain sequential memory access
*   at the expense of more trig overhead.
*
*   z[0..2n-1] is an array of n complex numbers, stored in the
*   usual way with real elements in z[0,2,..2n-2] and imaginary
*   elements in z[1,3,..2n-1].
*
*   Entering this function, z[] should be in bit-reversed order.
*   The returned inverse FT is restored to normal order.
*
*   n must be a power of 2.
*
* Arguments:
*   double *z - array of 2n doubles, representing n complex numbers
*   int n - dimension of z, must be a power of 2
*
* Return:
*   none
************************************************************/

static void ifft_dit_iter_seq(double *z, unsigned long n)
{
    unsigned long i, n2;

    n2 = n << 1;
    for (i = 2; i <= n; i <<= 1) {
        double a, b, t;
        unsigned long i2, k;
        i2 = i << 1;
        t = -TWOPI / (double)i;
        a = sin(0.5 * t);
        a *= 2.0 * a;
        b = sin(t);
        for (k = 0; k < n2; k += i2) {
            double c, s;
            unsigned long j;
            c = 1.0;
            s = 0.0;
            for (j = 0; j < i; j += 2) {
                double vr, vi, tmp;
                unsigned long kr, ki, mr, mi;
                kr = k + j;
                ki = kr + 1;
                mr = kr + i;
                mi = mr + 1;
                vr = z[mr] * c - z[mi] * s;
                vi = z[mr] * s + z[mi] * c;
                z[mr] = z[kr] - vr;
                z[mi] = z[ki] - vi;
                z[kr] += vr;
                z[ki] += vi;
                tmp = c;
                c -= a * c + b * s;
                s -= a * s - b * tmp;
            }
        }
    }
    return;
}   /* ifft_dit_iter_seq() */


/***********************************************************
* static void ifft_dit_rec(double *z, unsigned long n, int nbranch)
*
* Purpose:
*   Computes the inverse discrete Fourier transform of a complex
*   sequence z, using a radix 2 decimation-in-time FFT. If the
*   computation is small enough to fit in cache, it is done
*   iteratively.  Otherwise, it is done recursively until the
*   recursion descends to cache-sized chunks.
*
*   z[0..2n-1] is an array of n complex numbers, stored in the
*   usual way with real elements in z[0,2,..2n-2] and imaginary
*   elements in z[1,3,..2n-1].
*
*   Entering this function, z[] should be in normal order.  On
*   return, the FT is stored in bit-reversed order.
*
*   n must be a power of 2.
*
*   To support OpenMP parallelism, nbranch keeps track of the
*   number of active transforms at a given recursion level. On
*   the first call to this function, nbranch should be 1.  It
*   is then doubled for each recursion.
*
* Arguments:
*   double *z - array of 2n doubles, representing n complex numbers
*   unsigned long n - dimension of z, must be a power of 2
*   int nbranch - number of transforms at this recursion level
*
* Return:
*   none
************************************************************/

static void ifft_dit_rec(double *z, unsigned long n, int nbranch)
{
    double a, b, c, s, t;
    unsigned long  nh, kr;

    if (n == 1)
        return;
    if (n < (unsigned long)(L1_CACHE_BYTES / (2 * sizeof(double)))) {
        if (FFT_UNIT_STRIDE)
            ifft_dit_iter_seq(z, n);
        else
            ifft_dit_iter(z, n);
        return;
    }
    nh = n >> 1;
    nbranch <<= 1;

    ifft_dit_rec(z, nh, nbranch);
    ifft_dit_rec(z + n, nh, nbranch);

    t = -TWOPI / (double)n;
    a = sin(0.5 * t);
    a *= 2.0 * a;
    b = sin(t);
    c = 1.0;
    s = 0.0;
    for (kr = 0; kr < n; kr += 2) {
        double vr, vi, tmp;
        unsigned long ki, mr, mi;
        ki = kr + 1;
        mr = kr + n;
        mi = mr + 1;
        vr = z[mr] * c - z[mi] * s;
        vi = z[mr] * s + z[mi] * c;
        z[mr] = z[kr] - vr;
        z[mi] = z[ki] - vi;
        z[kr] += vr;
        z[ki] += vi;
        tmp = c;
        c -= a * c + b * s;
        s -= a * s - b * tmp;
    }
    return;
}   /* ifft_dit_rec() */





/***********************************************************
* void bitrev_permute(double *z, unsigned long n)
*
* Purpose:
*   Carry out a reverse binary bit-reversal permutation of
*   the elements of a complex array z, with length n, where
*   n is a power of 2.
*   
* Arguments:
*   double *z - array of 2n doubles, representing n complex numbers
*   unsigned long n - dimension of z, must be a power of 2
************************************************************/

void bitrev_permute(double *z, unsigned long n)
{
    unsigned long i;
    unsigned int ldn = 0;
    unsigned int rshift;
    
    i = n;
    while (i >>= 1)
        ++ldn;
    rshift = 8 * (unsigned int)sizeof(unsigned long) - ldn;
    for (i = 0; i < n; ++i) {
        unsigned long r = 0;
#if (ULONG_MAX == 0xffffffff) 
        r = ((i & 0x55555555) << 1) | ((i & ~0x55555555) >> 1);
        r = ((r & 0x33333333) << 2) | ((r & ~0x33333333) >> 2);
        r = ((r & 0x0f0f0f0f) << 4) | ((r & ~0x0f0f0f0f) >> 4);
        r = ((r & 0x00ff00ff) << 8) | ((r & ~0x00ff00ff) >> 8);
        r = (r << 16) | (r >> 16);
#elif (ULONG_MAX == 0xffffffffffffffff) 
        r = ((i & 0x5555555555555555) << 1) | ((i & ~0x5555555555555555) >> 1);
        r = ((r & 0x3333333333333333) << 2) | ((r & ~0x3333333333333333) >> 2);
        r = ((r & 0x0f0f0f0f0f0f0f0f) << 4) | ((r & ~0x0f0f0f0f0f0f0f0f) >> 4);
        r = ((r & 0x00ff00ff00ff00ff) << 8) | ((r & ~0x00ff00ff00ff00ff) >> 8);
        r = ((r & 0x0000ffff0000ffff) << 16) |
            ((r & ~0x0000ffff0000ffff) >> 16);
        r = (r << 32) | (r >> 32);
#endif
        r >>= rshift;
        if (r > i) {
            double tmp;
            unsigned long i2;
            i2 = i << 1;
            r <<= 1;
            tmp = z[i2]; z[i2] = z[r]; z[r] = tmp;
            tmp = z[i2+1]; z[i2+1] = z[r+1]; z[r+1] = tmp;
        }
    }
    return;
}   /* bitrev_permute() */


/***********************************************************
* void bitrev_permute_real(double *x, unsigned long n)
*
* Purpose:
*   Carry out a reverse binary bit-reversal permutation of
*   the elements of a real array x, with length n, where
*   n is a power of 2.
*   
* Arguments:
*   double *x - array of 2n doubles, representing n complex numbers
*   unsigned long n - dimension of x, must be a power of 2
************************************************************/
/*
void bitrev_permute_real(double *x, unsigned long n)
{
    unsigned long i;
    unsigned int ldn = 0;
    unsigned int rshift;
    
    i = n;
    while (i >>= 1)
        ++ldn;
    rshift = 8 * (unsigned int)sizeof(unsigned long) - ldn;
    for (i = 0; i < n; ++i) {
        unsigned long r = 0;
#if (ULONG_MAX == 0xffffffff) 
        r = ((i & 0x55555555) << 1) | ((i & ~0x55555555) >> 1);
        r = ((r & 0x33333333) << 2) | ((r & ~0x33333333) >> 2);
        r = ((r & 0x0f0f0f0f) << 4) | ((r & ~0x0f0f0f0f) >> 4);
        r = ((r & 0x00ff00ff) << 8) | ((r & ~0x00ff00ff) >> 8);
        r = (r << 16) | (r >> 16);
#elif (ULONG_MAX == 0xffffffffffffffff) 
        r = ((i & 0x5555555555555555) << 1) | ((i & ~0x5555555555555555) >> 1);
        r = ((r & 0x3333333333333333) << 2) | ((r & ~0x3333333333333333) >> 2);
        r = ((r & 0x0f0f0f0f0f0f0f0f) << 4) | ((r & ~0x0f0f0f0f0f0f0f0f) >> 4);
        r = ((r & 0x00ff00ff00ff00ff) << 8) | ((r & ~0x00ff00ff00ff00ff) >> 8);
        r = ((r & 0x0000ffff0000ffff) << 16) |
            ((r & ~0x0000ffff0000ffff) >> 16);
        r = (r << 32) | (r >> 32);
#endif
        r >>= rshift;
        if (r > i) {
            double tmp;
            tmp = x[i]; x[i] = x[r]; x[r] = tmp;
        }
    }
    return;
} */  /* bitrev_permute_real() */

