
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
void packed_argb32_to_dbl_array1 (unsigned char *src, double *dst, int w, int h);
void packed_rgba32_to_dbl_array1 (unsigned char *src, double *dst, int w, int h);
void packed_abgr32_to_dbl_array1 (unsigned char *src, double *dst, int w, int h);

void packed_argb32_to_dbl_array3 (unsigned char *src, double *dst, int w, int h);
void packed_rgba32_to_dbl_array3 (unsigned char *src, double *dst, int w, int h);
void packed_abgr32_to_dbl_array3 (unsigned char *src, double *dst, int w, int h);

void packed_argb32_to_dbl_array4 (unsigned char *src, double *dst, int w, int h);
void packed_rgba32_to_dbl_array4 (unsigned char *src, double *dst, int w, int h);
void packed_abgr32_to_dbl_array4 (unsigned char *src, double *dst, int w, int h);



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Unpack packed data into an RGBA array
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP packed_to_dbl_array_(SEXP src_, SEXP format_, SEXP nchannel_, SEXP width_, SEXP height_) {

  int format   = INTEGER(format_)[0];
  int width    = INTEGER(width_ )[0];
  int height   = INTEGER(height_)[0];
  int nchannel = INTEGER(nchannel_)[0];


  unsigned char *src;
  int h,w;

  if (TYPEOF(src_) == RAWSXP) {
    src = RAW(src_);
    if (height <= 0 || width <= 0) {
      error("packed_to_dbl_array_(): Raw vector requires height and width. Got (%i, %i)", height, width);
    } else if (height * width * 4 != length(src_)) {
      error("packed_to_dbl_array_(): User-specified dims (%i * %i * 4) do not equal raw vector length %i", height, width, length(src_));
    }
    h = height;
    w = width;
  } else {
    src = isNull(src_) ? NULL : (unsigned char *)R_ExternalPtrAddr(src_);
    if (src == NULL) error("packed_to_dbl_array_: 'unsigned char * src' pointer is invalid/NULL");


    SEXP tag_ = R_ExternalPtrTag(src_);

    if (isNull(tag_)) {
      if (height <= 0 || width <= 0) {
        error("packed_to_dbl_array_(): Packed pointer from external sources requires height and width. Got (%i, %i)", height, width);
      }
      h = height;
      w = width;
    } else {
      if (height > 0 || width > 0) {
        warning("packed_to_dbl_array_(): User specified height and width ignored for pointer with dimensions");
      }
      h = INTEGER(tag_)[0];
      w = INTEGER(tag_)[1];
    }
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Sanitcy check nchannel and format
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (nchannel != 1 && nchannel != 3 && nchannel !=4) {
    error("packed_to_dbl_array_(): nchannel must be 1, 3, or 4");
  }

  if (format != 0 && format != 1 && format != 2) {
    error("packed_to_dbl_array_(): format must be 0, 1, 2");
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Allocate destination array
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SEXP result_ = PROTECT(allocVector(REALSXP, w*h*nchannel));


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Allocate dim() structure + attach dim to result
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SEXP dim_;
  if (nchannel == 1) {
    dim_ = PROTECT(allocVector(INTSXP, 2));
    INTEGER(dim_)[0] = h;
    INTEGER(dim_)[1] = w;
  } else {
    dim_ = PROTECT(allocVector(INTSXP, 3));
    INTEGER(dim_)[0] = h;
    INTEGER(dim_)[1] = w;
    INTEGER(dim_)[2] = nchannel;
  }
  SET_DIM(result_, dim_);


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Call correct function to unpack the result to the appropriate format
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  double *dst = REAL(result_);

  if (nchannel == 1) {
    switch(format) {
    case ARGB32:
      packed_argb32_to_dbl_array1(src, dst, w, h); break;
    case RGBA32:
      packed_rgba32_to_dbl_array1(src, dst, w, h); break;
    case ABGR32:
      packed_abgr32_to_dbl_array1(src, dst, w, h); break;
    }
  } else if (nchannel == 3) {
    switch(format) {
    case ARGB32:
      packed_argb32_to_dbl_array3(src, dst, w, h); break;
    case RGBA32:
      packed_rgba32_to_dbl_array3(src, dst, w, h); break;
    case ABGR32:
      packed_abgr32_to_dbl_array3(src, dst, w, h); break;
    }
  } else if (nchannel == 4) {
    switch(format) {
    case ARGB32:
      packed_argb32_to_dbl_array4(src, dst, w, h); break;
    case RGBA32:
      packed_rgba32_to_dbl_array4(src, dst, w, h); break;
    case ABGR32:
      packed_abgr32_to_dbl_array4(src, dst, w, h); break;
    }
  } else {
    error("packed_to_dbl_array_(): Should not get here");
  }


  UNPROTECT(2);
  return result_;
}
