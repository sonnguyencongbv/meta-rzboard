SUMMARY = "Example of how to build an external Linux kernel module"
DESCRIPTION = "${SUMMARY}"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=12f884d2ae1ff87c09e5b7ccc2c4ca7e"

inherit module

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI = "file://Makefile \
           file://COPYING \
           file://gpio-driver.c \
          "
          
S = "${WORKDIR}"

# The inherit of module.bbclass will automatically name module packages with
# "kernel-module-" prefix as required by the oe-core build environment.

RPROVIDES_${PN} += "kernel-module-gpio-driver"

COMPATIBLE_MACHINE_rzg2l = "(rzboard)"

# Autoload gpio-driver
#KERNEL_MODULE_AUTOLOAD += "gpio-driver"
