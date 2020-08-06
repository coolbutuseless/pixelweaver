

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Convert a raw vector into a packed color data pointer
#'
#' This function copies the original data into a new object and returns a
#' pointer to this data.
#'
#' @param raw_vec raw vector
#' @param width,height image dimensions
#'
#' @return An external pointer to packed pixel data
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
raw_to_packed <- function(raw_vec, width, height) {
  .Call(raw_to_packed_, raw_vec, width, height)
}


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Convert a packed data pointer to a raw vector
#'
#' This function copies the data in the packed pointer to a new raw vector and
#' returns the new raw vector
#'
#' @param packed_ptr pointer to data containing packed colors
#'
#' @return raw vector
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
packed_to_raw <- function(packed_ptr) {
  .Call(packed_to_raw_, packed_ptr)
}
