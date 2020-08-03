

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Convert a raw vector into a ARGB32 pointer
#'
#' This function copies the original data into a new object and returns a
#' pointer to this data.
#'
#' @param raw_vec raw vector
#' @param width,height image dimensions
#'
#' @return An external poitner to ARGB32 data
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
raw_to_argb32 <- function(raw_vec, width, height) {
  .Call(raw_to_argb32_, raw_vec, width, height)
}


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Convert an ARGB32 pointer to a raw vector
#'
#' This function copies the data in the ARGB32 pointer to a new raw vector and
#' returns the new raw vector
#'
#' @param argb32_ptr ARGB32 pointer
#'
#' @return raw vector
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
argb32_to_raw <- function(argb32_ptr) {
  .Call(argb32_to_raw_, argb32_ptr)
}
