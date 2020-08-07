

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
#define rgba_pack_func(name, byte0, byte1, byte2, byte3)                     \
void (name) (double *src, unsigned char *dst, int w, int h, double maxval) { \
                                                                             \
  double *src_r = src + w * h * 0;                                           \
  double *src_g = src + w * h * 1;                                           \
  double *src_b = src + w * h * 2;                                           \
  double *src_a = src + w * h * 3;                                           \
                                                                             \
  double *a, *r, *g, *b;                                                     \
                                                                             \
  double factor = 255/maxval;                                                \
                                                                             \
  for (int j = 0; j < h; j++) {                                              \
    a = src_a + j;                                                           \
    r = src_r + j;                                                           \
    g = src_g + j;                                                           \
    b = src_b + j;                                                           \
    for (int i = 0; i < w; i++) {                                            \
      *dst++ = (unsigned char)( (byte0) * factor);                           \
      *dst++ = (unsigned char)( (byte1) * factor);                           \
      *dst++ = (unsigned char)( (byte2) * factor);                           \
      *dst++ = (unsigned char)( (byte3) * factor);                           \
      a += h;                                                                \
      r += h;                                                                \
      g += h;                                                                \
      b += h;                                                                \
    }                                                                        \
  }                                                                          \
}                                                                            \





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Convert packed format into a planar array with RGBA color
//
// @param src Packed 4-byte per pixel data
// @param dst pointer to RGBA planar array data (stored in doubles) - in R's standard
//        row-major format
// @param w,h width and height of image data
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define rgba_unpack_func(name, byte0, byte1, byte2, byte3)     \
void (name) (unsigned char *src, double *dst, int w, int h) {  \
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


