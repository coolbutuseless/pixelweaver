

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Set the image dimensions of the data in an packed color object
#'
#' This function should be used if the External Pointer to data was created in
#' another library. In this case there would be no dimension information stored
#' within the object, and it would need to be set explicitly.
#'
#' Warning: Be careful when using this function - if you set dimensions which
#' don't match the data you'll likely get a segfault.
#'
#' @param packed_ptr External pointer to data which contains packed color values
#' @param width,height image dimensions in pixels
#'
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
packed_ptr_set_dim <- function(packed_ptr, width, height) {
  .Call("packed_ptr_set_dim_", packed_ptr, width, height)
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Convert a matrix or array into packed color format
#'
#' @param planar_data R object containing planar color data.
#'        Either a matrix (representing grey values), or
#'        an array with 3 or 4 planes (representing RGB and RGBA color
#'        respectively).  Data must be double precision floating point.
#' @param maxval specify the maximum possible value in the \code{robj} data.
#'        This is used to scale the results into the range required for packed
#'        pixel representations.
#'        For standard R image representations, data is often in the range
#'        [0,1] so use \code{maxval=1}
#' @param format Integer value denoting the packed format. 0L = ARGB32,
#'        1L = RGBA32, 2L = ABGR32.  See also \code{?packed_fmt}.
#' @param as_ptr Logical: Return value as an external pointer to memory in C?
#'        Default: FALSE - return a standard R vector of raw values
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
planar_to_packed <- function(planar_data, format, maxval, as_ptr) {
  .Call("dbl_array_to_packed_",
        planar_data, as.double(maxval), as.integer(format), isTRUE(as_ptr))
}


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Convert an object with packed color data into planar color array in R
#'
#' @inheritParams planar_to_packed
#' @param packed_data An object containing packed color data. This can be a
#'        vector of raw values, or an external pointer to data.
#' @param nchannel Number of color channels in the output i.e. the number of
#'        color planes in the matrix or array. 1 = grey (matrix),
#'        3 = RGB, 4 = RGBA
#' @param width,height dimensions of image. This must be specified when
#'        \code{packed_data} is a vector of raw values, or if the external pointer
#'        to data was created by another library and \code{packed_ptr_set_dim} has
#'        not been used.
#'
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
packed_to_planar <- function(packed_data, format, nchannel, width = -1L, height = -1L) {
  if (typeof(packed_data) == 'externalptr' && inherits(packed_data, 'unsigned char')) {

  } else if (is.raw(packed_data)) {
    stopifnot("Must specify width + height for raw vector input" = width > 0 && height > 0)
    stopifnot("Raw vector must contain data" = length(packed_data) > 0)
    stopifnot("Specified dimensions exceed raw pointer length" = width * height * 4 <= length(packed_data))
  } else {
    stop("Can't unpack type")
  }


  .Call("packed_to_dbl_array_",
        packed_data, as.integer(format), as.integer(nchannel),
        as.integer(width), as.integer(height))

}

