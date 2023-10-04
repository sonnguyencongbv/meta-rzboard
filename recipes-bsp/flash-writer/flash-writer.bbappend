SUMMARY = "Flash Writer for RzBoard"

BOARD ?= ""
BOARD_rzboard = "RZV2L_SMARC_PMIC"

BRANCH = "rz_g2l"
SRCREV = "ff167b676547f3997906c82c9be504eb5cff8ef0"

do_compile_append_rzboard() {
	mv ${S}/AArch64_output/Flash_Writer*${BOARD}*.mot ${S}/AArch64_output/Flash_Writer_SCIF_${MACHINE}.mot
}
