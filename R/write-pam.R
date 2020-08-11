


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Write array to a NETPBM PAM file
#'
#' PAM file is written with depth 4 with \code{TUPLTYPE = RGB_ALPHA}
#'
#' @param arr matrix (grey) or array (with rgb or rgba data)
#' @param filename output filename e.g. \code{"output.pam"}
#' @param maxval maximum value possible in the array representation. For R
#'        arrays this is often 1.
#'
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
write_pam <- function(arr, filename, maxval = 1) {

  packed_raw <- planar_to_packed(arr, format = packed_fmt$ABGR32, maxval = maxval, as_ptr = FALSE)

  write_pam_raw(packed_raw, filename, width = ncol(arr), height = nrow(arr))
}




#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Write packed color in ABGR32 format to a NETPBM PAM file
#'
#' PAM file is written with depth 4 with \code{TUPLTYPE = RGB_ALPHA}
#'
#' @param packed_raw Packed color data in ABGR32 format
#' @param filename output filename e.g. \code{"output.pam"}
#' @param width,height image dimensions
#'
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
write_pam_raw <- function(packed_raw, filename, width, height) {

  header <- sprintf("P7
WIDTH %i
HEIGHT %i
DEPTH 4
MAXVAL 255
TUPLTYPE RGB_ALPHA
ENDHDR", width, height)

  con <- file(description = filename, open = "wb")
  on.exit(close(con))
  writeLines(header, con)
  writeBin(packed_raw, con, size = 1, useBytes = TRUE)

  invisible()
}




if (FALSE) {

  library(foist)

  width  <- 500
  height <- 500

  arr <- array(seq(height * width * 3)/(height * width * 3), c(height, width, 3))
  plot(as.raster(arr), interpolate = FALSE)

  bench::mark(
    write_pam(arr, "working/out.pam"),
    write_pnm(arr, "working/out.png"),
    check = FALSE
  )


  system("convert working/out.pam working/out_pam.png")
  system("convert working/out.pam working/out_pnm.png")


}





