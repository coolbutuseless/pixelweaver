
# pixelweaver 0.1.3

* `write_pam()` output arrays to NETPBM PAM format
* `write_pam_raw()` - output packed raw color to NETPBM PAM format
* `shuffle()` shuffling of byte order in a vector of packed raw color.  This
  shuffling is done 'in-place' with no new storage allocated
* Internal - move dimension information stored in ExternalPtr from `prot` to `tag`

# pixelweaver 0.1.2

* Direct support for packed color stored in vectors of raw values
* Internal refactor to reduce code duplication
* Nomenclature tidy

# pixelweaver 0.1.1

* Make package a little more generic to support more pixel formats
* Add support for: RGBA32, ABGR32


# pixelweaver 0.1.0

* Initial release
