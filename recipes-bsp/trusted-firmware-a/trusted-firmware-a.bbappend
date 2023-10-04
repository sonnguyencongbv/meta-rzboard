TFA_URL = "${RZBOARD_GIT_HOST_MIRROR}/trusted-firmware-a.git"
BRANCH = "rzboard_v2.7_rz"
SRCREV = "5157586ad59de928c32eecedd2a0507a968213c4"

SRC_URI_remove ="git://github.com/renesas-rz/rzg_trusted-firmware-a.git;branch=${BRANCH};protocol=https"
SRC_URI_prepend = "${TFA_URL};branch=${BRANCH};${RZBOARD_GIT_PROTOCOL};${RZBOARD_GIT_USER}"

COMPATIBLE_MACHINE_rzboard = "(smarc-rzg2l|smarc-rzv2l|rzv2l-dev|rzboard)"

PLATFORM_rzboard = "v2l"
EXTRA_FLAGS_rzboard = "BOARD=rzboard"
FLASH_ADDRESS_BL2_BP_rzboard = "00000"
FLASH_ADDRESS_FIP_rzboard = "1D200"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI_append = " \
       file://0001-remove-U-boot-from-system-boot.patch \
       file://0002-support-loading-FreeRTOS-binaries.patch \
       file://0003-Kick-CM33-smarc-rzv2l.patch \
       file://0004-init-I2C-clock-in-ATF.patch \
       file://0005-kick-cm33-before-loading-kernel.patch \
       file://0006-pull-up-pin-P48_3-at-ATF-entry.patch \
       file://0007-BL2-init-essential-clocks-for-avnet-board.patch \
       file://0008-bl2-W-A-to-bring-CM33-up.patch \
"
