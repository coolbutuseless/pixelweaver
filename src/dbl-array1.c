

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "aaa.h"
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
#define array1_pack_func(name, byte0, byte1, byte2, byte3)                      \
void (name) (double *src, unsigned char *dst, int w, int h, double maxval) {  \
                                                                              \
  double *src_grey = src;                                                     \
                                                                              \
  double *g;                                                                  \
                                                                              \
  double factor = 255/maxval;                                                 \
                                                                              \
  for (int j = 0; j < h; j++) {                                               \
    g = src_grey + j;                                                         \
    for (int i = 0; i < w; i++) {                                             \
      (byte0);                                                                \
      (byte1);                                                                \
      (byte2);                                                                \
      (byte3);                                                                \
      g += h;                                                                 \
    }                                                                         \
  }                                                                           \
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
#define array1_unpack_func(name, red, green, blue)                                  \
void (name) (unsigned char *src, double *dst, int w, int h) {                               \
                                                                                            \
  double *dst_grey = dst;                                                                   \
                                                                                            \
  double *g;                                                                                \
                                                                                            \
  for (int j = 0; j < h; j++) {                                                             \
    g = dst_grey + j;                                                                       \
    for (int i = 0; i < w; i++) {                                                           \
      *g = (0.3 * (double)(red) + 0.59 * (double)(green) + 0.11 * (double)(blue)) / 255.0;  \
      g += h;                                                                               \
      src += 4;                                                                             \
    }                                                                                       \
  }                                                                                         \
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ARGB32
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
array1_pack_func(
  dbl_array1_to_packed_argb32,                         \
  *dst++ = (unsigned char)(*g * factor),                     \
  *dst++ = (unsigned char)(*g * factor),                     \
  *dst++ = (unsigned char)(*g * factor),                     \
  *dst++ = 255                                               \
);


array1_unpack_func(packed_argb32_to_dbl_array1, src[2], src[1], src[0]);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// RGBA32
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
array1_pack_func(
  dbl_array1_to_packed_rgba32,                          \
  *dst++ = 255,                                               \
  *dst++ = (unsigned char)(*g * factor),                      \
  *dst++ = (unsigned char)(*g * factor),                      \
  *dst++ = (unsigned char)(*g * factor)                       \
);


array1_unpack_func(packed_rgba32_to_dbl_array1, src[3], src[2], src[1]);



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ABGR32
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
array1_pack_func(
  dbl_array1_to_packed_abgr32,                           \
  *dst++ = (unsigned char)(*g * factor),                       \
  *dst++ = (unsigned char)(*g * factor),                       \
  *dst++ = (unsigned char)(*g * factor),                       \
  *dst++ = 255                                                 \
);


array1_unpack_func(packed_abgr32_to_dbl_array1, src[0], src[1], src[2]);





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Pack an RGBA array to an external pointer
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP dbl_array1_to_packed_(SEXP src_, SEXP maxval_, SEXP format_) {

  int format = INTEGER(format_)[0];

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Sanity check the input array is
  //  (a) an array
  //  (b) the right depth
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (!isMatrix(src_)) {
    error("dbl_array1_to_packed_(): src_ must be an matrix");
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

  switch(format) {
  case ARGB32:
    dbl_array1_to_packed_argb32(src, dst, w, h, maxval);
    break;
  case RGBA32:
    dbl_array1_to_packed_rgba32(src, dst, w, h, maxval);
    break;
  case ABGR32:
    dbl_array1_to_packed_abgr32(src, dst, w, h, maxval);
    break;
  default:
    error("dbl_array1_to_packed_(): Format not understood: ", format);
  }

  result_ = PROTECT(R_MakeExternalPtr(dst, R_NilValue, outdim));
  SET_CLASS(result_, mkString("unsigned char"));
  R_RegisterCFinalizer(result_, generic_finalizer);


  UNPROTECT(2);
  return result_;
}













//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Unpack packed ARGB32 data into matrix of grey pixel values
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP packed_to_dbl_array1_(SEXP src_, SEXP format_) {

  int format = INTEGER(format_)[0];

  unsigned char *src;

  src = isNull(src_) ? NULL : (unsigned char *)R_ExternalPtrAddr(src_);
  if (src == NULL) error("packed_to_dbl_array1_: 'unsigned char * src' pointer is invalid/NULL");

  SEXP prot_ = R_ExternalPtrProtected(src_);
  int h = INTEGER(prot_)[0];
  int w = INTEGER(prot_)[1];

  SEXP result_ = PROTECT(allocVector(REALSXP, w*h));
  SEXP dim_ = PROTECT(allocVector(INTSXP, 2));
  INTEGER(dim_)[0] = h;
  INTEGER(dim_)[1] = w;
  SET_DIM(result_, dim_);

  double *dst = REAL(result_);

  switch(format) {
  case ARGB32:
    packed_argb32_to_dbl_array1(src, dst, w, h);
    break;
  case RGBA32:
    packed_rgba32_to_dbl_array1(src, dst, w, h);
    break;
  case ABGR32:
    packed_abgr32_to_dbl_array1(src, dst, w, h);
    break;
  default:
    error("packed_to_dbl_array_rgb_(): Format not understood: ", format);
  }

  UNPROTECT(2);
  return result_;
}

