
#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "aaa.h"
#include "R-finalizers.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Extern function prototypes
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void dbl_array1_to_packed_argb32(double *src, unsigned char *dst, int w, int h, double maxval);
void dbl_array1_to_packed_rgba32(double *src, unsigned char *dst, int w, int h, double maxval);
void dbl_array1_to_packed_abgr32(double *src, unsigned char *dst, int w, int h, double maxval);

void dbl_array3_to_packed_argb32(double *src, unsigned char *dst, int w, int h, double maxval);
void dbl_array3_to_packed_rgba32(double *src, unsigned char *dst, int w, int h, double maxval);
void dbl_array3_to_packed_abgr32(double *src, unsigned char *dst, int w, int h, double maxval);

void dbl_array4_to_packed_argb32(double *src, unsigned char *dst, int w, int h, double maxval);
void dbl_array4_to_packed_rgba32(double *src, unsigned char *dst, int w, int h, double maxval);
void dbl_array4_to_packed_abgr32(double *src, unsigned char *dst, int w, int h, double maxval);



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Pack an RGBA array to an external pointer
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP dbl_array_to_packed_(SEXP src_, SEXP maxval_, SEXP format_, SEXP as_ptr_) {


  int format = INTEGER(format_)[0];
  int as_ptr = INTEGER(as_ptr_)[0];

  int h, w;
  int nchannel = 1;

  if (!isArray(src_) && !isMatrix(src_)) {
    error("dbl_array_to_packed_(): src must be an array or matrix");
  }

  int *dim = INTEGER(GET_DIM(src_));
  h = dim[0];
  w = dim[1];

  if (isMatrix(src_)) {
    nchannel = 1;
  } else if (isArray(src_)) {
    nchannel = dim[2];

    if (nchannel != 3 && nchannel != 4) {
      error("dbl_array_to_packed_(): Array must have 3 or 4 planes");
    }
  }


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Sanitcy check  format
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (format != 0 && format != 1 && format != 2) {
    error("packed_to_dbl_array_(): format must be 0, 1, 2");
  }


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Prepare to convert
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  double *src = REAL(src_);
  double maxval = REAL(maxval_)[0];
  SEXP result_;

  SEXP outdim = PROTECT(allocVector(INTSXP, 2));
  INTEGER(outdim)[0] = h;
  INTEGER(outdim)[1] = w;


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Allocate memory to use as the storage for packed color
  // Either:
  //   as_ptr == 1: store the data in C memory
  //   as_ptr == 0: create a standard R vector of R values
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  unsigned char *dst;
  if (as_ptr == 1) {
    dst = (unsigned char *)calloc(w*h*4, sizeof(unsigned char));
    result_ = PROTECT(R_MakeExternalPtr(dst, outdim, R_NilValue));
    SET_CLASS(result_, mkString("unsigned char"));
    R_RegisterCFinalizer(result_, generic_finalizer);
  } else {
    result_ = PROTECT(allocVector(RAWSXP, w*h*4));
    dst = RAW(result_);
  }


  if (nchannel == 1) {
    switch(format) {
    case ARGB32:
      dbl_array1_to_packed_argb32(src, dst, w, h, maxval); break;
    case RGBA32:
      dbl_array1_to_packed_rgba32(src, dst, w, h, maxval); break;
    case ABGR32:
      dbl_array1_to_packed_abgr32(src, dst, w, h, maxval); break;
    }
  } else if (nchannel == 3) {
    switch(format) {
    case ARGB32:
      dbl_array3_to_packed_argb32(src, dst, w, h, maxval); break;
    case RGBA32:
      dbl_array3_to_packed_rgba32(src, dst, w, h, maxval); break;
    case ABGR32:
      dbl_array3_to_packed_abgr32(src, dst, w, h, maxval); break;
    }
  } else if (nchannel == 4) {
    switch(format) {
    case ARGB32:
      dbl_array4_to_packed_argb32(src, dst, w, h, maxval); break;
    case RGBA32:
      dbl_array4_to_packed_rgba32(src, dst, w, h, maxval); break;
    case ABGR32:
      dbl_array4_to_packed_abgr32(src, dst, w, h, maxval); break;
    }
  } else {
    error("dbl_array_to_packed_(): Should never get here");
  }


  UNPROTECT(2);
  return result_;
}

