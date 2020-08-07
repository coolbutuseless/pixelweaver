
// #define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>

extern SEXP dbl_array_to_packed_();
extern SEXP packed_to_dbl_array_();

extern SEXP raw_to_packed_();
extern SEXP packed_to_raw_();

extern SEXP packed_ptr_set_dim_();

static const R_CallMethodDef CEntries[] = {

  // name                       pointer                       Num args
  {"dbl_array_to_packed_" , (DL_FUNC) &dbl_array_to_packed_ , 4},
  {"packed_to_dbl_array_" , (DL_FUNC) &packed_to_dbl_array_ , 5},

  {"raw_to_packed_" , (DL_FUNC) &raw_to_packed_ , 3},
  {"packed_to_raw_" , (DL_FUNC) &packed_to_raw_ , 1},

  {"packed_ptr_set_dim_", (DL_FUNC) &packed_ptr_set_dim_, 3},

  {NULL , NULL, 0}
};


void R_init_pixelweaver(DllInfo *info) {
  R_registerRoutines(
    info,      // DllInfo
    NULL,      // .C
    CEntries,  // .Call
    NULL,      // Fortran
    NULL       // External
  );
  R_useDynamicSymbols(info, FALSE);
}



