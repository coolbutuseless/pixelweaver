
// #define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>

extern SEXP dbl_array_rgba_to_packed_argb32_();
extern SEXP dbl_array_rgb_to_packed_argb32_();
extern SEXP dbl_matrix_grey_to_packed_argb32_();

extern SEXP packed_argb32_to_dbl_array_rgba_();
extern SEXP packed_argb32_to_dbl_array_rgb_();
extern SEXP packed_argb32_to_dbl_matrix_grey_();

extern SEXP raw_to_argb32_();
extern SEXP argb32_to_raw_();
extern SEXP argb32_set_dim_();

static const R_CallMethodDef CEntries[] = {

  // name                       pointer                           Num args
  {"dbl_array_rgba_to_packed_argb32_", (DL_FUNC) &dbl_array_rgba_to_packed_argb32_, 2},
  {"dbl_array_rgb_to_packed_argb32_", (DL_FUNC) &dbl_array_rgb_to_packed_argb32_, 2},
  {"dbl_matrix_grey_to_packed_argb32_", (DL_FUNC) &dbl_matrix_grey_to_packed_argb32_, 2},

  {"packed_argb32_to_dbl_array_rgba_", (DL_FUNC) &packed_argb32_to_dbl_array_rgba_, 1},
  {"packed_argb32_to_dbl_array_rgb_", (DL_FUNC) &packed_argb32_to_dbl_array_rgb_, 1},
  {"packed_argb32_to_dbl_matrix_grey_", (DL_FUNC) &packed_argb32_to_dbl_matrix_grey_, 1},

  {"raw_to_argb32_", (DL_FUNC) &raw_to_argb32_, 3},
  {"argb32_to_raw_", (DL_FUNC) &argb32_to_raw_, 1},
  {"argb32_set_dim_", (DL_FUNC) &argb32_set_dim_, 3},

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



