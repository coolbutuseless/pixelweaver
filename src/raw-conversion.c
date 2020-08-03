

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "R-finalizers.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copy the data in a raw vector to newly allocated memory for ARGB32 data
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP raw_to_argb32_(SEXP raw_, SEXP w_, SEXP h_) {

  int w = INTEGER(w_)[0];
  int h = INTEGER(h_)[0];
  int l = length(raw_);

  if (l != w * h * 4) {
    error("raw_to_argb32_(): Length of raw vector (%i) does not match w*h*4 = %i", l, w*h*4);
  }

  unsigned char *dst = (unsigned char *)calloc(w*h*4, sizeof(unsigned char));

  memcpy(dst, RAW(raw_), w*h*4);

  SEXP outdim = PROTECT(allocVector(INTSXP, 2));
  INTEGER(outdim)[0] = h;
  INTEGER(outdim)[1] = w;

  SEXP result_ = PROTECT(R_MakeExternalPtr(dst, R_NilValue, outdim));
  SET_CLASS(result_, mkString("unsigned char"));
  R_RegisterCFinalizer(result_, generic_finalizer);

  UNPROTECT(2);
  return result_;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copy ARGB32 data into a raw vector
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP argb32_to_raw_(SEXP argb32_) {

  unsigned char *argb32;

  argb32 = isNull(argb32_) ? NULL : (unsigned char *)R_ExternalPtrAddr(argb32_);
  if (argb32 == NULL) error("argb32_to_raw_: 'unsigned char * src' pointer is invalid/NULL");

  SEXP prot_ = R_ExternalPtrProtected(argb32_);
  if (isNull(prot_)) {
    error("Need 'prot' set on external pointer");
  }
  int h = INTEGER(prot_)[0];
  int w = INTEGER(prot_)[1];


  SEXP result_ = PROTECT(allocVector(RAWSXP, w*h*4));
  memcpy(RAW(result_), argb32, w*h*4);

  UNPROTECT(1);
  return result_;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Set dimensions on argb32
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP argb32_set_dim_(SEXP argb32_, SEXP w_, SEXP h_) {
  SEXP outdim = PROTECT(allocVector(INTSXP, 2));
  INTEGER(outdim)[0] = INTEGER(h_)[0];
  INTEGER(outdim)[1] = INTEGER(w_)[0];

  R_SetExternalPtrProtected(argb32_, outdim);
  UNPROTECT(1);
  return argb32_;
}









