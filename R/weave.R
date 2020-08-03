

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Set the image dimensions of the data in an argb32 object
#'
#' Warning: if you set dimensions which don't match the data you'll
#' likely get a segfault.
#'
#' @param argb32_ptr argb32 pointer
#' @param width,height image dimensions in pixels
#'
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
argb32_set_dim <- function(argb32_ptr, width, height) {
  .Call("argb32_set_dim_", argb32_ptr, width, height)
}


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Convert a matrix or array into packed ARGB32 format
#'
#' @param robj R representation. Either a matrix (representing grey values), or
#'        an array with 3 or 4 planes (representing RGB and RGBA colour
#'        respectively).  Data must be double preceision floating point.
#' @param maxval specify the maximum possible value in the \code{robj} data.
#'        This is used to scale the results into the desired range [0, 255].
#'        For standard R image representations, data is often in the range
#'        [0,1] so use \code{maxval=1}
#'
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
planar_to_argb32 <- function(robj, maxval) {

  if (is.matrix(robj)) {
    .Call("dbl_matrix_grey_to_packed_argb32_", robj, maxval)
  } else if (is.array(robj)) {
    dims <- dim(robj)
    if (dims[3] == 3L) {
      .Call("dbl_array_rgb_to_packed_argb32_", robj, maxval)
    } else if (dims[3] == 4L) {
      .Call("dbl_array_rgba_to_packed_argb32_", robj, maxval)
    } else {
      stop("If 'robj' is array, must have 3 or 4 planes")
    }
  }
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Convert an object with packed ARGB32 pixels into array of planar colour or matrix
#'
#' @param argb32_ptr An object containing ARGB32 data. this is an external pointer
#'                  to data.
#' @param format one of 'rgba', 'rgb', or 'grey' to indicate the result should
#'        be an RGBA array, an RGB array or a matrix, respectively
#'
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
argb32_to_planar <- function(argb32_ptr, format = 'rgba') {
  if (typeof(argb32_ptr) == 'externalptr' && inherits(argb32_ptr, 'unsigned char')) {
    # message("Unweaving ARGB32 external ptr")
  } else {
    stop("Can't unpack type")
  }

  format <- tolower(format)

  if (format == 'rgba') {
    .Call("packed_argb32_to_dbl_array_rgba_", argb32_ptr)
  } else if (format == 'rgb') {
    .Call("packed_argb32_to_dbl_array_rgb_", argb32_ptr)
  } else if (format %in% c('grey', 'gray')) {
    .Call("packed_argb32_to_dbl_matrix_grey_", argb32_ptr)
  } else {
    stop("Don't know how to deal with format = ", format)
  }

}


