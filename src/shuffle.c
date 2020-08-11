

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "aaa.h"
#include "R-finalizers.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Shuffle
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP shuffle_ARGB32_to_ABGR32_(SEXP raw_vec_) {

  unsigned char *p = RAW(raw_vec_);

  for (int i=0; i<length(raw_vec_); i+=4) {
    unsigned char tmp = p[0];
    p[0] = p[2];
    p[2] = tmp;
    p += 4;
  }

  return R_NilValue;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Shuffle
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP shuffle_ABGR32_to_RGBA32_(SEXP raw_vec_) {

  unsigned char *p = RAW(raw_vec_);

  for (int i=0; i<length(raw_vec_); i+=4) {
    unsigned char tmp = p[0];
    p[0] = p[3];
    p[3] = tmp;

    tmp = p[1];
    p[1] = p[2];
    p[2] = tmp;
    p += 4;
  }

  return R_NilValue;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Shuffle
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP shuffle_RGBA32_to_ARGB32_(SEXP raw_vec_) {

  unsigned char *p = RAW(raw_vec_);

  for (int i=0; i<length(raw_vec_); i+=4) {
    unsigned char tmp = p[0];
    p[0] = p[1];
    p[1] = p[2];
    p[2] = p[3];
    p[3] = tmp;

    p += 4;
  }

  return R_NilValue;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Shuffle
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP shuffle_ARGB32_to_RGBA32_(SEXP raw_vec_) {

  unsigned char *p = RAW(raw_vec_);

  for (int i=0; i<length(raw_vec_); i+=4) {
    unsigned char tmp = p[3];
    p[3] = p[2];
    p[2] = p[1];
    p[1] = p[0];
    p[0] = tmp;

    p += 4;
  }

  return R_NilValue;
}


SEXP shuffle_(SEXP raw_vec_, SEXP src_fmt_, SEXP dst_fmt_) {

  int src_fmt = INTEGER(src_fmt_)[0];
  int dst_fmt = INTEGER(dst_fmt_)[0];

  if (src_fmt == dst_fmt) {

  } else if (src_fmt == ARGB32 && dst_fmt == RGBA32) {
    shuffle_ARGB32_to_RGBA32_(raw_vec_);
  } else if (src_fmt == ARGB32 && dst_fmt == ABGR32) {
    shuffle_ARGB32_to_ABGR32_(raw_vec_);
  } else if (src_fmt == RGBA32 && dst_fmt == ARGB32) {
    shuffle_RGBA32_to_ARGB32_(raw_vec_);
  } else if (src_fmt == RGBA32 && dst_fmt == ABGR32) {
    shuffle_ABGR32_to_RGBA32_(raw_vec_);
  } else if (src_fmt == ABGR32 && dst_fmt == ARGB32) {
    shuffle_ARGB32_to_ABGR32_(raw_vec_);
  } else if (src_fmt == ABGR32 && dst_fmt == RGBA32) {
    shuffle_ABGR32_to_RGBA32_(raw_vec_);
  } else{
    error("shuffle_(): Don't know how to convert those formats");
  }


  return R_NilValue;
}





