

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Set the image dimensions of the data in an packed pixel object
#'
#' Warning: if you set dimensions which don't match the data you'll
#' likely get a segfault.
#'
#' @param packed_ptr packed pixel pointer
#' @param width,height image dimensions in pixels
#'
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
packed_set_dim <- function(packed_ptr, width, height) {
  .Call("packed_set_dim_", packed_ptr, width, height)
}


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Convert a matrix or array into packed pixel format
#'
#' @param robj R representation. Either a matrix (representing grey values), or
#'        an array with 3 or 4 planes (representing RGB and RGBA color
#'        respectively).  Data must be double precision floating point.
#' @param maxval specify the maximum possible value in the \code{robj} data.
#'        This is used to scale the results into the range required for packed
#'        pixel representations.
#'        For standard R image representations, data is often in the range
#'        [0,1] so use \code{maxval=1}
#' @param format Integer value denoting the packed format. 0L = ARGB32,
#'        1L = RGBA32, 2L = ABGR32.  See also \code{?pack_fmt}.
#'
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
planar_to_packed <- function(robj, format, maxval) {


  if (is.matrix(robj)) {
    .Call("dbl_array1_to_packed_", robj, maxval, format)
  } else if (is.array(robj)) {
    dims <- dim(robj)
    if (dims[3] == 3L) {
      .Call("dbl_array3_to_packed_", robj, maxval, format)
    } else if (dims[3] == 4L) {
      .Call("dbl_array4_to_packed_", robj, maxval, format)
    } else {
      stop("If 'robj' is array, must have 3 or 4 planes")
    }
  }
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Convert an object with packed pixels into array of color values in R
#'
#' @inheritParams planar_to_packed
#' @param packed_ptr An object containing packed pixel data. This is an external pointer
#'                  to data.
#' @param nchannel Number of color channels in the planar representation. 1 = grey,
#'        3 = RGB, 4 = RGBA
#'
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
packed_to_planar <- function(packed_ptr, format, nchannel) {
  if (typeof(packed_ptr) == 'externalptr' && inherits(packed_ptr, 'unsigned char')) {
  } else {
    stop("Can't unpack type")
  }

  if (nchannel == 4) {
    .Call("packed_to_dbl_array4_", packed_ptr, format)
  } else if (nchannel == 3) {
    .Call("packed_to_dbl_array3_", packed_ptr, format)
  } else if (nchannel == 1) {
    .Call("packed_to_dbl_array1_", packed_ptr, format)
  } else {
    stop("Don't know how to deal with nchannel = ", nchannel)
  }

}


