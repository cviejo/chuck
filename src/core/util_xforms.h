/*----------------------------------------------------------------------------
  ChucK Strongly-timed Audio Programming Language
    Compiler, Virtual Machine, and Synthesis Engine

  Copyright (c) 2003 Ge Wang and Perry R. Cook. All rights reserved.
    http://chuck.stanford.edu/
    http://chuck.cs.princeton.edu/

  This program is free software; you can redistribute it and/or modify
  it under the dual-license terms of EITHER the MIT License OR the GNU
  General Public License (the latter as published by the Free Software
  Foundation; either version 2 of the License or, at your option, any
  later version).

  This program is distributed in the hope that it will be useful and/or
  interesting, but WITHOUT ANY WARRANTY; without even the implied warranty
  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  MIT Licence and/or the GNU General Public License for details.

  You should have received a copy of the MIT License and the GNU General
  Public License (GPL) along with this program; a copy of the GPL can also
  be obtained by writing to the Free Software Foundation, Inc., 59 Temple
  Place, Suite 330, Boston, MA 02111-1307 U.S.A.
-----------------------------------------------------------------------------*/

 //-----------------------------------------------------------------------------
// name: util_xform.h
// desc: transform utilities
//
// authors: Ge Wang (ge@ccrma.stanford.edu | gewang@cs.princeton.edu)
//          Perry R. Cook (prc@cs.princeton.edu)
//          FFT - from CARL distribution
// date: 11.27.2003
//-----------------------------------------------------------------------------
#ifndef __UTIL_XFORM_H__
#define __UTIL_XFORM_H__

#include <math.h>
#include "chuck_def.h"

// our float type
#define FLOAT_XFORM       SAMPLE

// complex absolute value
#define cmp_abs(x) ( sqrt( (x).re * (x).re + (x).im * (x).im ) )
#define __modulus(x) cmp_abs(x)
#define __phase(x) ( atan2((double)(x).im, (double)(x).re) )

#define FFT_FORWARD 1
#define FFT_INVERSE 0
#define make_window hanning

// c linkage
#if defined(_cplusplus) || defined(__cplusplus)
extern "C" {
#endif

// make the window
void hanning( FLOAT_XFORM * window, unsigned long length );
void hamming( FLOAT_XFORM * window, unsigned long length );
void blackman( FLOAT_XFORM * window, unsigned long length );
void bartlett( FLOAT_XFORM * window, unsigned long length );
// apply the window
void apply_window( FLOAT_XFORM * data, FLOAT_XFORM * window, unsigned long length );

// real fft, N must be power of 2
void rfft( FLOAT_XFORM * x, long N, unsigned int forward );
// complex fft, NC must be power of 2
void cfft( FLOAT_XFORM * x, long NC, unsigned int forward );

// type II dct, often referred to as "the dct"
void the_dct( FLOAT_XFORM * x, unsigned long N, FLOAT_XFORM * out, unsigned long Nout );
// generates NxN type II dct matrix
void the_dct_matrix( FLOAT_XFORM ** out, unsigned long N );
// type III dct, often referred to as "the inverse_dct"
void the_inverse_dct( FLOAT_XFORM * x, unsigned long N, FLOAT_XFORM * out, unsigned long Nout );
// generates NxN type II dct matrix
void the_inverse_dct_matrix( FLOAT_XFORM ** out, unsigned long N );

// apply dct from matrix
void the_dct_now( FLOAT_XFORM * x, FLOAT_XFORM ** matrix, unsigned long N, FLOAT_XFORM * out, unsigned long Nout );
// apply inverse dct from matrix
void the_inverse_dct_now( FLOAT_XFORM * x, FLOAT_XFORM ** matrix, unsigned long N, FLOAT_XFORM * out, unsigned long Nout );

// c linkage
#if ( defined( __cplusplus ) || defined( _cplusplus ) )
}
#endif

#endif
