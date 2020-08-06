
// #define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>

extern SEXP dbl_array4_to_packed_();
extern SEXP dbl_array3_to_packed_();
extern SEXP dbl_array1_to_packed_();

extern SEXP packed_to_dbl_array4_();
extern SEXP packed_to_dbl_array3_();
extern SEXP packed_to_dbl_array1_();

extern SEXP raw_to_packed_();
extern SEXP packed_to_raw_();
extern SEXP packed_set_dim_();

static const R_CallMethodDef CEntries[] = {

  // name                       pointer                           Num args
  {"dbl_array4_to_packed_" , (DL_FUNC) &dbl_array4_to_packed_ , 3},
  {"dbl_array3_to_packed_"  , (DL_FUNC) &dbl_array3_to_packed_  , 3},
  {"dbl_array1_to_packed_", (DL_FUNC) &dbl_array1_to_packed_, 3},

  {"packed_to_dbl_array4_" , (DL_FUNC) &packed_to_dbl_array4_ , 2},
  {"packed_to_dbl_array3_"  , (DL_FUNC) &packed_to_dbl_array3_  , 2},
  {"packed_to_dbl_array1_", (DL_FUNC) &packed_to_dbl_array1_, 2},

  {"raw_to_packed_", (DL_FUNC) &raw_to_packed_, 3},
  {"packed_to_raw_", (DL_FUNC) &packed_to_raw_, 1},
  {"packed_set_dim_", (DL_FUNC) &packed_set_dim_, 3},

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



