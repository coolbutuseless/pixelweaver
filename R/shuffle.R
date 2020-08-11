

 #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 #' Shuffle packed colors in-place
 #'
 #' @param packed_raw Packed color stored in a vector of raw values
 #' @param src_fmt,dst_fmt Format specification. See \code{pixelweaver::packed_fmt}
 #'
 #' @return Nothing.  The vector is shuffled in-place.
 #' @export
 #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 shuffle <- function(packed_raw, src_fmt, dst_fmt) {
   invisible(.Call('shuffle_', packed_raw, as.integer(src_fmt), as.integer(dst_fmt)))
 }


