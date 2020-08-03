

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "R-finalizers.h"




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Convert a planar array of RGB colour to packed RGBA format
//
// @param src pointer to RGB planar array data (stored in doubles) - in R's standard
//        row-major format
// @param dst Packed 4-byte per pixel ARGB32 format - in C's standard
//        column-major format
// @param w,h width and height of image data
// @param maxval the maximum value of the src data. used to scale into a byte
//        range. No check of this maximum is performed - data will be truncated,
//        rounded or wrapped according to the compiler's casting to unsigned char after
//        scaling.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void dbl_array_rgb_to_packed_argb32_inner(double *src, unsigned char *dst, int w, int h, double maxval) {

  double *src_r = src + w * h * 0;
  double *src_g = src + w * h * 1;
  double *src_b = src + w * h * 2;

  double *r, *g, *b;

  double factor = 255/maxval;

  for (int j = 0; j < h; j++) {
    r = src_r + j;
    g = src_g + j;
    b = src_b + j;
    for (int i = 0; i < w; i++) {
      *dst++ = (unsigned char)(*b * factor);
      *dst++ = (unsigned char)(*g * factor);
      *dst++ = (unsigned char)(*r * factor);
      *dst++ = 255; // alpha
      r += h;
      g += h;
      b += h;
    }
  }
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Pack an RGBA array to an external pointer
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP dbl_array_rgb_to_packed_argb32_(SEXP src_, SEXP maxval_) {


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Sanity check the input array is
  //  (a) an array
  //  (b) the right depth
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (!isArray(src_)) {
    error("dbl_array_rgb_to_packed_argb32_(): src_ must be an array");
  }

  int *dim = INTEGER(GET_DIM(src_));

  int h = dim[0];
  int w = dim[1];
  int depth = dim[2];

  if (depth != 3) {
    error("dbl_array_rgb_to_packed_argb32_(): Array must have 3 planes");
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
  // Create an unsigned char * and use that as the ARGB32 destination
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  unsigned char *dst = (unsigned char *)calloc(w*h*4, sizeof(unsigned char));
  dbl_array_rgb_to_packed_argb32_inner(src, dst, w, h, maxval);

  result_ = PROTECT(R_MakeExternalPtr(dst, R_NilValue, outdim));
  SET_CLASS(result_, mkString("unsigned char"));
  R_RegisterCFinalizer(result_, generic_finalizer);


  UNPROTECT(2);
  return result_;
}







//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Convert packed ARGB32 format into a planar array with RGB colour
//
// @param src Packed 4-byte per pixel ARGB32 format - in C's standard
//        column-major format
// @param dst pointer to RGB planar array data (stored in doubles) - in R's standard
//        row-major format
// @param w,h width and height of image data
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void packed_argb32_to_dbl_array_rgb_inner(unsigned char *src, double *dst, int w, int h) {

  double *dst_r = dst + w * h * 0;
  double *dst_g = dst + w * h * 1;
  double *dst_b = dst + w * h * 2;

  double *r, *g, *b;

  for (int j = 0; j < h; j++) {
    r = dst_r + j;
    g = dst_g + j;
    b = dst_b + j;
    for (int i = 0; i < w; i++) {
      *b = (double)(*src++ / 255.0);
      *g = (double)(*src++ / 255.0);
      *r = (double)(*src++ / 255.0);
      src++;
      r += h;
      g += h;
      b += h;
    }
  }
}





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Unpack packed ARGB32 data into an RGBA array
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP packed_argb32_to_dbl_array_rgb_(SEXP src_) {

  unsigned char *src;

  src = isNull(src_) ? NULL : (unsigned char *)R_ExternalPtrAddr(src_);
  if (src == NULL) error("packed_argb32_to_dbl_array_rgb_: 'unsigned char * src' pointer is invalid/NULL");

  SEXP prot_ = R_ExternalPtrProtected(src_);
  int h = INTEGER(prot_)[0];
  int w = INTEGER(prot_)[1];

  SEXP result_ = PROTECT(allocVector(REALSXP, w*h*3));
  SEXP dim_ = PROTECT(allocVector(INTSXP, 3));
  INTEGER(dim_)[0] = h;
  INTEGER(dim_)[1] = w;
  INTEGER(dim_)[2] = 3; // depth 3
  SET_DIM(result_, dim_);

  double *dst = REAL(result_);

  packed_argb32_to_dbl_array_rgb_inner(src, dst, w, h);


  UNPROTECT(2);
  return result_;
}

