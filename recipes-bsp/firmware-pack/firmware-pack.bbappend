DEPENDS = "trusted-firmware-a u-boot bootparameter-native fiptool-native"
DEPENDS += "linux-renesas cm33"

do_compile_append () {

	# Create bl2_bp.bin
	bootparameter ${WORKDIR}/recipe-sysroot/boot/bl2-${MACHINE}.bin bl2_bp.bin
	cat ${WORKDIR}/recipe-sysroot/boot/bl2-${MACHINE}.bin >> bl2_bp.bin

	# Create fip.bin
	fiptool create --align 16 \
        --soc-fw ${WORKDIR}/recipe-sysroot/boot/bl31-${MACHINE}.bin \
        --nt-fw-config ${S}/recipe-sysroot/boot/rzboard.dtb \
        --nt-fw ${S}/recipe-sysroot/boot/u-boot.bin \
        --fw-config ${S}/recipe-sysroot/boot/cm33/rzv2l_cm33_rpmsg_demo_secure_code.bin \
        --hw-config ${S}/recipe-sysroot/boot/cm33/rzv2l_cm33_rpmsg_demo_non_secure_vector.bin \
        --soc-fw-config ${S}/recipe-sysroot/boot/cm33/rzv2l_cm33_rpmsg_demo_secure_vector.bin \
        --rmm-fw ${S}/recipe-sysroot/boot/cm33/rzv2l_cm33_rpmsg_demo_non_secure_code.bin fip.bin

	# Convert to srec
	objcopy -O srec --adjust-vma=0x00011E00 --srec-forceS3 -I binary bl2_bp.bin bl2_bp.srec
	objcopy -I binary -O srec --adjust-vma=0x0000 --srec-forceS3 fip.bin fip.srec

        if [ "${PMIC_SUPPORT}" = "1" ]; then
		bootparameter ${WORKDIR}/recipe-sysroot/boot/bl2-${MACHINE}_pmic.bin bl2_bp_pmic.bin
		cat ${WORKDIR}/recipe-sysroot/boot/bl2-${MACHINE}_pmic.bin >> bl2_bp_pmic.bin
		fiptool create --align 16 --soc-fw ${WORKDIR}/recipe-sysroot/boot/bl31-${MACHINE}_pmic.bin --nt-fw ${WORKDIR}/recipe-sysroot/boot/u-boot.bin fip_pmic.bin
		objcopy -O srec --adjust-vma=0x00011E00 --srec-forceS3 -I binary bl2_bp_pmic.bin bl2_bp_pmic.srec
		objcopy -I binary -O srec --adjust-vma=0x0000 --srec-forceS3 fip_pmic.bin fip_pmic.srec
	fi

}

