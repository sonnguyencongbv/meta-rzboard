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
