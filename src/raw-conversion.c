

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "R-finalizers.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copy the data in a raw vector to newly allocated memory for packed data
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP raw_to_packed_(SEXP raw_, SEXP w_, SEXP h_) {

  int w = INTEGER(w_)[0];
  int h = INTEGER(h_)[0];
  int l = length(raw_);

  if (l != w * h * 4) {
    error("raw_to_packed_(): Length of raw vector (%i) does not match w*h*4 = %i", l, w*h*4);
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
// Copy packed data into a raw vector
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP packed_to_raw_(SEXP packed_ptr_) {

  unsigned char *packed_ptr;

  packed_ptr = isNull(packed_ptr_) ? NULL : (unsigned char *)R_ExternalPtrAddr(packed_ptr_);
  if (packed_ptr == NULL) error("packed_ptr_to_raw_: 'unsigned char * src' pointer is invalid/NULL");

  SEXP prot_ = R_ExternalPtrProtected(packed_ptr_);
  if (isNull(prot_)) {
    error("Need 'prot' set on external pointer");
  }
  int h = INTEGER(prot_)[0];
  int w = INTEGER(prot_)[1];


  SEXP result_ = PROTECT(allocVector(RAWSXP, w*h*4));
  memcpy(RAW(result_), packed_ptr, w*h*4);

  UNPROTECT(1);
  return result_;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Set dimensions on packed pointer
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP packed_set_dim_(SEXP packed_ptr_, SEXP w_, SEXP h_) {
  SEXP outdim = PROTECT(allocVector(INTSXP, 2));
  INTEGER(outdim)[0] = INTEGER(h_)[0];
  INTEGER(outdim)[1] = INTEGER(w_)[0];

  R_SetExternalPtrProtected(packed_ptr_, outdim);
  UNPROTECT(1);
  return packed_ptr_;
}









