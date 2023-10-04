SECTION = "fip srec"
SUMMARY = "Firmware Packaging"
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/BSD-3-Clause;md5=550794465ba0ec5312d6919e203a55f9"

inherit deploy

DEPENDS = "trusted-firmware-a u-boot bootparameter-native fiptool-native"
DEPENDS += "linux-renesas cm33"

S = "${WORKDIR}"

do_compile () {
	# Create bl2_bp.bin
	bootparameter ${WORKDIR}/recipe-sysroot/boot/bl2-${MACHINE}.bin bl2_bp.bin
	cat ${WORKDIR}/recipe-sysroot/boot/bl2-${MACHINE}.bin >> bl2_bp.bin
    
    # Convert to srec
	objcopy -O srec --adjust-vma=0x00011E00 --srec-forceS3 -I binary bl2_bp.bin bl2_bp_rzboard.srec

	# Create fip-rzboard.bin
	fiptool create --align 16 \
        --soc-fw ${WORKDIR}/recipe-sysroot/boot/bl31-${MACHINE}.bin \
        --nt-fw-config ${S}/recipe-sysroot/boot/rzboard.dtb \
        --nt-fw ${S}/recipe-sysroot/boot/Image \
        --fw-config ${S}/recipe-sysroot/boot/cm33/rzv2l_cm33_rpmsg_demo_secure_code.bin \
        --hw-config ${S}/recipe-sysroot/boot/cm33/rzv2l_cm33_rpmsg_demo_non_secure_vector.bin \
        --soc-fw-config ${S}/recipe-sysroot/boot/cm33/rzv2l_cm33_rpmsg_demo_secure_vector.bin \
        --rmm-fw ${S}/recipe-sysroot/boot/cm33/rzv2l_cm33_rpmsg_demo_non_secure_code.bin fip_rzboard.bin

	# Convert to srec
	objcopy -I binary -O srec --adjust-vma=0x0000 --srec-forceS3 fip_rzboard.bin fip_rzboard.srec
}

inherit deploy
addtask deploy after do_install

do_deploy () {
	install -d ${DEPLOYDIR}/optimize
    install -m 0644 ${WORKDIR}/fip_rzboard.bin ${DEPLOYDIR}/fip_rzboard.bin
    install -m 0644 ${WORKDIR}/bl2_bp_rzboard.srec ${DEPLOYDIR}/optimize_bl2_bp_rzboard.srec
    install -m 0644 ${WORKDIR}/fip_rzboard.srec ${DEPLOYDIR}/optimize_fip_rzboard.srec
}

COMPATIBLE_MACHINE = "(rzboard)"
PACKAGE_ARCH = "${MACHINE_ARCH}"

