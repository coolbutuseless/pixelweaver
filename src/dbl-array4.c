

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "aaa.h"
#include "R-finalizers.h"



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Convert a planar array of colors to packed RGBA format
//
// @param src pointer to RGBA planar array data (stored in doubles) - in R's standard
//        row-major format
// @param dst Packed 4-byte per pixel data
// @param w,h width and height of image data
// @param maxval the maximum value of the src data. used to scale into a byte
//        range. No check of this maximum is performed - data will be truncated,
//        rounded or wrapped according to the compiler's casting to unsigned char after
//        scaling.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define rgba_pack_func(name, byte0, byte1, byte2, byte3)       \
void (name) (double *src, unsigned char *dst, int w, int h, double maxval) { \
                                                               \
double *src_r = src + w * h * 0;                               \
double *src_g = src + w * h * 1;                               \
double *src_b = src + w * h * 2;                               \
double *src_a = src + w * h * 3;                               \
                                                               \
double *a, *r, *g, *b;                                         \
                                                               \
double factor = 255/maxval;                                    \
                                                               \
for (int j = 0; j < h; j++) {                                  \
  a = src_a + j;                                               \
  r = src_r + j;                                               \
  g = src_g + j;                                               \
  b = src_b + j;                                               \
  for (int i = 0; i < w; i++) {                                \
    *dst++ = (unsigned char)( (byte0) * factor);               \
    *dst++ = (unsigned char)( (byte1) * factor);               \
    *dst++ = (unsigned char)( (byte2) * factor);               \
    *dst++ = (unsigned char)( (byte3) * factor);               \
    a += h;                                                    \
    r += h;                                                    \
    g += h;                                                    \
    b += h;                                                    \
  }                                                            \
}                                                              \
}                                                              \





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Convert packed format into a planar array with RGBA color
//
// @param src Packed 4-byte per pixel data
// @param dst pointer to RGBA planar array data (stored in doubles) - in R's standard
//        row-major format
// @param w,h width and height of image data
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define rgba_unpack_func(name, byte0, byte1, byte2, byte3)     \
void (name) (unsigned char *src, double *dst, int w, int h) { \
                                                               \
  double *dst_r = dst + w * h * 0;                             \
  double *dst_g = dst + w * h * 1;                             \
  double *dst_b = dst + w * h * 2;                             \
  double *dst_a = dst + w * h * 3;                             \
                                                               \
  double *a, *r, *g, *b;                                       \
                                                               \
  for (int j = 0; j < h; j++) {                                \
    a = dst_a + j;                                             \
    r = dst_r + j;                                             \
    g = dst_g + j;                                             \
    b = dst_b + j;                                             \
    for (int i = 0; i < w; i++) {                              \
      (byte0) = (double)(*src++ / 255.0);                      \
      (byte1) = (double)(*src++ / 255.0);                      \
      (byte2) = (double)(*src++ / 255.0);                      \
      (byte3) = (double)(*src++ / 255.0);                      \
      a += h;                                                  \
      r += h;                                                  \
      g += h;                                                  \
      b += h;                                                  \
    }                                                          \
  }                                                            \
}



rgba_pack_func(  dbl_array4_to_packed_argb32, *b, *g, *r, *a);
rgba_unpack_func(packed_argb32_to_dbl_array4, *b, *g, *r, *a);

rgba_pack_func(  dbl_array4_to_packed_rgba32, *a, *b, *g, *r);
rgba_unpack_func(packed_rgba32_to_dbl_array4, *a, *b, *g, *r);

rgba_pack_func(  dbl_array4_to_packed_abgr32, *r, *g, *b, *a);
rgba_unpack_func(packed_abgr32_to_dbl_array4, *r, *g, *b, *a);



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Pack an RGBA array to an external pointer
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP dbl_array4_to_packed_(SEXP src_, SEXP maxval_, SEXP format_) {

  int format = INTEGER(format_)[0];

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Sanity check the input array is
  //  (a) an array
  //  (b) the right depth
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (!isArray(src_)) {
    error("dbl_array4_to_packed_(): src_ must be an array");
  }

  int *dim = INTEGER(GET_DIM(src_));

  int h = dim[0];
  int w = dim[1];
  int depth = dim[2];

  if (depth != 4) {
    error("dbl_array4_to_packed_(): Array must have 4 planes");
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
  // Create an unsigned char * and use that as the packed destination
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  unsigned char *dst = (unsigned char *)calloc(w*h*4, sizeof(unsigned char));

  switch(format) {
  case ARGB32:
    dbl_array4_to_packed_argb32(src, dst, w, h, maxval);
    break;
  case RGBA32:
    dbl_array4_to_packed_rgba32(src, dst, w, h, maxval);
    break;
  case ABGR32:
    dbl_array4_to_packed_abgr32(src, dst, w, h, maxval);
    break;
  default:
    error("dbl_array4_to_packed_(): Format not understood: ", format);
  }

  result_ = PROTECT(R_MakeExternalPtr(dst, R_NilValue, outdim));
  SET_CLASS(result_, mkString("unsigned char"));
  R_RegisterCFinalizer(result_, generic_finalizer);


  UNPROTECT(2);
  return result_;
}






//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Unpack packed data into an RGBA array
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP packed_to_dbl_array4_(SEXP src_, SEXP format_) {

  int format = INTEGER(format_)[0];

  unsigned char *src;

  src = isNull(src_) ? NULL : (unsigned char *)R_ExternalPtrAddr(src_);
  if (src == NULL) error("packed_to_dbl_array4_: 'unsigned char * src' pointer is invalid/NULL");

  SEXP prot_ = R_ExternalPtrProtected(src_);
  int h = INTEGER(prot_)[0];
  int w = INTEGER(prot_)[1];

  SEXP result_ = PROTECT(allocVector(REALSXP, w*h*4));
  SEXP dim_ = PROTECT(allocVector(INTSXP, 3));
  INTEGER(dim_)[0] = h;
  INTEGER(dim_)[1] = w;
  INTEGER(dim_)[2] = 4; // depth 4
  SET_DIM(result_, dim_);

  double *dst = REAL(result_);

  switch(format) {
  case ARGB32:
    packed_argb32_to_dbl_array4(src, dst, w, h);
    break;
  case RGBA32:
    packed_rgba32_to_dbl_array4(src, dst, w, h);
    break;
  case ABGR32:
    packed_abgr32_to_dbl_array4(src, dst, w, h);
    break;
  default:
    error("packed_to_dbl_array4_(): Format not understood: ", format);
  }


  UNPROTECT(2);
  return result_;
}

