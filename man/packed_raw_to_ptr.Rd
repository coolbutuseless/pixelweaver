% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/raw-conversion.R
\name{packed_raw_to_ptr}
\alias{packed_raw_to_ptr}
\title{Convert a raw vector into a packed color data pointer}
\usage{
packed_raw_to_ptr(raw_vec, width, height)
}
\arguments{
\item{raw_vec}{raw vector}

\item{width, height}{image dimensions}
}
\value{
An external pointer to packed color data
}
\description{
This function copies the original data into a new object and returns a
pointer to this data.
}
