

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "R-finalizers.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// generic finalizer for objects which are malloced.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void generic_finalizer(SEXP ptr_) {
  if (!isNull(ptr_)) {
    void *ptr = (void *)R_ExternalPtrAddr(ptr_);
    if (ptr != NULL) {
      free(ptr);
      R_ClearExternalPtr(ptr_);
    }
  }
}
