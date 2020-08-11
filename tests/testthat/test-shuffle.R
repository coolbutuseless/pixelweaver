


test_that("shuffle works", {

  (arr <- array(seq(4*3*3)/36, c(4, 3, 3)))
  plot(as.raster(arr), interpolate = FALSE)

  for (src_fmt in packed_fmt) {
    for (dst_fmt in packed_fmt) {
      p1 <- planar_to_packed(arr, format = dst_fmt, maxval = 1, as_ptr = FALSE)
      p2 <- planar_to_packed(arr, format = src_fmt, maxval = 1, as_ptr = FALSE)

      if (src_fmt != dst_fmt) {
        expect_false(identical(p1, p2))
      }

      shuffle(p2, src_fmt, dst_fmt)

      expect_true(identical(p1, p2))

    }
  }

})
