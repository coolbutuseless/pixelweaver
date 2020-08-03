


test_that("conversion to/from grey matrix works", {
  mat <- matrix(seq(4*3*1), c(4, 3))/255
  argb32_ptr <- planar_to_argb32(mat, maxval = 1)
  res  <- argb32_to_planar(argb32_ptr, 'grey')
  expect_true(isTRUE(all.equal(mat, res)))

  raw_vec <- argb32_to_raw(argb32_ptr)

  raw_ref <- as.raw(c(
    1,  1,  1, 255,
    5,  5,  5, 255,
    9,  9,  9, 255,
    2,  2,  2, 255,
    6,  6,  6, 255,
    10, 10, 10, 255,
    3,  3,  3, 255,
    7,  7,  7, 255,
    11, 11, 11, 255,
    4,  4,  4, 255,
    8,  8,  8, 255,
    12, 12, 12, 255
  ))

  expect_identical(raw_vec, raw_ref)

  a2 <- raw_to_argb32(raw_vec, width = 3L, height = 4L)
  r2 <- argb32_to_raw(a2)

  expect_identical(raw_vec, r2)

})



test_that("conversion to/from rgb array works", {
  arr <- array(seq(4*3*3), c(4, 3, 3))/255
  argb32_ptr <- planar_to_argb32(arr, maxval = 1)
  res  <- argb32_to_planar(argb32_ptr, 'rgb')
  expect_true(isTRUE(all.equal(arr, res)))


  raw_vec <- argb32_to_raw(argb32_ptr)

  raw_ref <- as.raw(c(
    0x19, 0x0d, 0x01, 0xff, 0x1d, 0x11, 0x05, 0xff, 0x21,
    0x15, 0x09, 0xff, 0x1a, 0x0e, 0x02, 0xff, 0x1e, 0x12, 0x06, 0xff,
    0x22, 0x16, 0x0a, 0xff, 0x1b, 0x0f, 0x03, 0xff, 0x1f, 0x13, 0x07,
    0xff, 0x23, 0x17, 0x0b, 0xff, 0x1c, 0x10, 0x04, 0xff, 0x20, 0x14,
    0x08, 0xff, 0x24, 0x18, 0x0c, 0xff
  ))

  expect_identical(raw_vec, raw_ref)

  a2 <- raw_to_argb32(raw_vec, width = 3L, height = 4L)
  r2 <- argb32_to_raw(a2)

  expect_identical(raw_vec, r2)
})



test_that("conversion to/from rgba array works", {
  arr <- array(seq(4*3*4), c(4, 3, 4))/255
  argb32_ptr <- planar_to_argb32(arr, maxval = 1)
  res  <- argb32_to_planar(argb32_ptr, 'rgba')
  expect_true(isTRUE(all.equal(arr, res)))


  raw_vec <- argb32_to_raw(argb32_ptr)

  raw_ref <- as.raw(c(
    0x19, 0x0d, 0x01, 0x25, 0x1d, 0x11, 0x05, 0x29, 0x21,
    0x15, 0x09, 0x2d, 0x1a, 0x0e, 0x02, 0x26, 0x1e, 0x12, 0x06, 0x2a,
    0x22, 0x16, 0x0a, 0x2e, 0x1b, 0x0f, 0x03, 0x27, 0x1f, 0x13, 0x07,
    0x2b, 0x23, 0x17, 0x0b, 0x2f, 0x1c, 0x10, 0x04, 0x28, 0x20, 0x14,
    0x08, 0x2c, 0x24, 0x18, 0x0c, 0x30
  ))

  expect_identical(raw_vec, raw_ref)

  a2 <- raw_to_argb32(raw_vec, width = 3L, height = 4L)
  r2 <- argb32_to_raw(a2)

  expect_identical(raw_vec, r2)



  expect_error(raw_to_argb32(raw_vec, width = 3L, height = 5L), "Length")
})



test_that("set_dim ok", {
  arr <- array(seq(4*3*4), c(4, 3, 4))/255
  argb32_ptr <- planar_to_argb32(arr, maxval = 1)
  argb32_set_dim(argb32_ptr, 10L, 10L)
  expect_true(TRUE)
})



test_that("some error conditions", {
  arr <- array(seq(4*3*5), c(4, 3, 5))/255
  expect_error(planar_to_argb32(arr, maxval = 1), "3 or 4 planes")

  expect_error(argb32_to_planar(mtcars), "Can't unpack type")

  arr <- array(seq(4*3*4), c(4, 3, 4))/255
  argb32_ptr <- planar_to_argb32(arr, maxval = 1)
  expect_error(argb32_to_planar(argb32_ptr, format = "goober"), "format")

})

