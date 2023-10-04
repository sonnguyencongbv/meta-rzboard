
UBOOT_URL = "${RZBOARD_GIT_HOST_MIRROR}/renesas-u-boot.git"
BRANCH = "rzboard_v2l_v2021.10_r2"
SRCREV = "c8b760169eb697084607452ba4c5b5ab6413a0e6"

SRC_URI = "${UBOOT_URL};branch=${BRANCH};${RZBOARD_GIT_PROTOCOL};${RZBOARD_GIT_USER}"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI_append = " \
    file://0001-apply-fastboot-ums-usb-dev-emmc.patch \
"
