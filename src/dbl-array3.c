

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "aaa.h"
#include "R-finalizers.h"




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Convert a planar array of RGB color to packed color format
//
// @param src pointer to RGB planar array data (stored in doubles) - in R's standard
//        row-major format
// @param dst Packed 4-byte per pixel
// @param w,h width and height of image data
// @param maxval the maximum value of the src data. used to scale into a byte
//        range. No check of this maximum is performed - data will be truncated,
//        rounded or wrapped according to the compiler's casting to unsigned char after
//        scaling.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define rgb_pack_func(name, byte0, byte1, byte2, byte3)                      \
void (name)(double *src, unsigned char *dst, int w, int h, double maxval) {  \
                                                                             \
  double *src_r = src + w * h * 0;                                           \
  double *src_g = src + w * h * 1;                                           \
  double *src_b = src + w * h * 2;                                           \
                                                                             \
  double *r, *g, *b;                                                         \
                                                                             \
  double factor = 255/maxval;                                                \
                                                                             \
  for (int j = 0; j < h; j++) {                                              \
    r = src_r + j;                                                           \
    g = src_g + j;                                                           \
    b = src_b + j;                                                           \
    for (int i = 0; i < w; i++) {                                            \
      (byte0);                                                               \
      (byte1);                                                               \
      (byte2);                                                               \
      (byte3);                                                               \
      r += h;                                                                \
      g += h;                                                                \
      b += h;                                                                \
    }                                                                        \
  }                                                                          \
}                                                                            \



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Convert packed color data into a planar array with RGB color
//
// @param src Packed 4-byte per pixel
// @param dst pointer to RGB planar array data (stored in doubles) - in R's standard
//        row-major format
// @param w,h width and height of image data
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define rgb_unpack_func(name, byte0, byte1, byte2, byte3)        \
void (name) (unsigned char *src, double *dst, int w, int h) {    \
                                                                 \
  double *dst_r = dst + w * h * 0;                               \
  double *dst_g = dst + w * h * 1;                               \
  double *dst_b = dst + w * h * 2;                               \
                                                                 \
  double *r, *g, *b;                                             \
                                                                 \
  for (int j = 0; j < h; j++) {                                  \
    r = dst_r + j;                                               \
    g = dst_g + j;                                               \
    b = dst_b + j;                                               \
    for (int i = 0; i < w; i++) {                                \
      (byte0);                                                   \
      (byte1);                                                   \
      (byte2);                                                   \
      (byte3);                                                   \
      r += h;                                                    \
      g += h;                                                    \
      b += h;                                                    \
    }                                                            \
  }                                                              \
}                                                                \



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ARGB32
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
rgb_pack_func(
  dbl_array3_to_packed_argb32,                   \
  *dst++ = (unsigned char)(*b * factor),         \
  *dst++ = (unsigned char)(*g * factor),         \
  *dst++ = (unsigned char)(*r * factor),         \
  *dst++ = 255                                   \
);


rgb_unpack_func(                        \
  packed_argb32_to_dbl_array3,          \
  *b = (double)(*src++ / 255.0),        \
  *g = (double)(*src++ / 255.0),        \
  *r = (double)(*src++ / 255.0),        \
  src++                                 \
);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// RGBA32
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
rgb_pack_func(
  dbl_array3_to_packed_rgba32,                    \
  *dst++ = 255,                                   \
  *dst++ = (unsigned char)(*b * factor),          \
  *dst++ = (unsigned char)(*g * factor),          \
  *dst++ = (unsigned char)(*r * factor)           \
);


rgb_unpack_func(                         \
  packed_rgba32_to_dbl_array3,           \
  src++,                                 \
  *b = (double)(*src++ / 255.0),         \
  *g = (double)(*src++ / 255.0),         \
  *r = (double)(*src++ / 255.0)          \
);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ABGR32
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
rgb_pack_func(
  dbl_array3_to_packed_abgr32,                     \
  *dst++ = (unsigned char)(*r * factor),           \
  *dst++ = (unsigned char)(*g * factor),           \
  *dst++ = (unsigned char)(*b * factor),           \
  *dst++ = 255                                     \
);


rgb_unpack_func(                          \
  packed_abgr32_to_dbl_array3,            \
  *r = (double)(*src++ / 255.0),          \
  *g = (double)(*src++ / 255.0),          \
  *b = (double)(*src++ / 255.0),          \
  src++                                   \
);



