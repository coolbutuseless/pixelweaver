

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "R-finalizers.h"




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Convert a matrix of grey pixel values to packed RGBA format
//
// @param src pointer to matrix data (stored in doubles) - in R's standard
//        row-major format
// @param dst Packed 4-byte per pixel ARGB32 format - in C's standard
//        column-major format
// @param w,h width and height of image data
// @param maxval the maximum value of the src data. used to scale into a byte
//        range. No check of this maximum is performed - data will be truncated,
//        rounded or wrapped according to the compiler's casting to unsigned char after
//        scaling.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void dbl_matrix_grey_to_packed_argb32_inner(double *src, unsigned char *dst, int w, int h, double maxval) {

  double *src_grey = src;

  double *g;

  double factor = 255/maxval;

  for (int j = 0; j < h; j++) {
    g = src_grey + j;
    for (int i = 0; i < w; i++) {
      *dst++ = (unsigned char)(*g * factor);
      *dst++ = (unsigned char)(*g * factor);
      *dst++ = (unsigned char)(*g * factor);
      *dst++ = 255; // alpha
      g += h;
    }
  }
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Pack an RGBA array to an external pointer
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP dbl_matrix_grey_to_packed_argb32_(SEXP src_, SEXP maxval_) {


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Sanity check the input array is
  //  (a) an array
  //  (b) the right depth
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (!isMatrix(src_)) {
    error("dbl_matrix_grey_to_packed_argb32_(): src_ must be an matrix");
  }

  int *dim = INTEGER(GET_DIM(src_));

  int h = dim[0];
  int w = dim[1];

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
  dbl_matrix_grey_to_packed_argb32_inner(src, dst, w, h, maxval);

  result_ = PROTECT(R_MakeExternalPtr(dst, R_NilValue, outdim));
  SET_CLASS(result_, mkString("unsigned char"));
  R_RegisterCFinalizer(result_, generic_finalizer);


  UNPROTECT(2);
  return result_;
}







//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Convert packed ARGB32 format into a matrix of grey pixel values
//
// @param src Packed 4-byte per pixel ARGB32 format - in C's standard
//        column-major format
// @param dst pointer to a matrix (stored in doubles) - in R's standard
//        row-major format
// @param w,h width and height of image data
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void packed_argb32_to_dbl_matrix_grey_inner(unsigned char *src, double *dst, int w, int h) {

  double *dst_grey = dst;

  double *g;

  for (int j = 0; j < h; j++) {
    g = dst_grey + j;
    for (int i = 0; i < w; i++) {
      *g = (0.3 * (double)src[2] + 0.59 * (double)src[1] + 0.11 * (double)src[0]) / 255.0;
      g += h;
      src += 4;
    }
  }
}





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Unpack packed ARGB32 data into matrix of grey pixel values
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP packed_argb32_to_dbl_matrix_grey_(SEXP src_) {

  unsigned char *src;

  src = isNull(src_) ? NULL : (unsigned char *)R_ExternalPtrAddr(src_);
  if (src == NULL) error("packed_argb32_to_dbl_matrix_grey_: 'unsigned char * src' pointer is invalid/NULL");

  SEXP prot_ = R_ExternalPtrProtected(src_);
  int h = INTEGER(prot_)[0];
  int w = INTEGER(prot_)[1];

  SEXP result_ = PROTECT(allocVector(REALSXP, w*h));
  SEXP dim_ = PROTECT(allocVector(INTSXP, 2));
  INTEGER(dim_)[0] = h;
  INTEGER(dim_)[1] = w;
  SET_DIM(result_, dim_);

  double *dst = REAL(result_);

  packed_argb32_to_dbl_matrix_grey_inner(src, dst, w, h);


  UNPROTECT(2);
  return result_;
}

