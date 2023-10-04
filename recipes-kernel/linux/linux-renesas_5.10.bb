DESCRIPTION = "Linux kernel for RzBoard"

require recipes-kernel/linux/linux-yocto.inc
#require include/docker-control.inc

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}/:"
COMPATIBLE_MACHINE = "(smarc-rzg2l|smarc-rzv2l|rzv2l-dev|rzboard)"

KERNEL_URL = " \
    git://github.com/renesas-rz/rz_linux-cip.git"
BRANCH = "${@oe.utils.conditional("IS_RT_BSP", "1", "rz-5.10-cip29-rt12", "rz-5.10-cip29",d)}"
SRCREV = "${@oe.utils.conditional("IS_RT_BSP", "1", "5de4d17d289dab05a92b718de0ea056c9dbe4c67", "6d2215071fe0ab3d4ddd65dfa70cb8c91545bd9d",d)}"

SRC_URI = "${KERNEL_URL};protocol=https;nocheckout=1;branch=${BRANCH}"
LIC_FILES_CHKSUM = "file://COPYING;md5=6bc538ed5bd9a7fc9398086aedcd7e46"
LINUX_VERSION ?= "${@oe.utils.conditional("IS_RT_BSP", "1", "5.10.175-cip29-rt12", "5.10.175-cip29",d)}"

PV = "${LINUX_VERSION}+git${SRCPV}"
PR = "r1"

SRC_URI_append = "\
    file://0001-Fixed-an-issue-that-caused-flicker-when-outputting-t.patch \
    file://0002-arm64-dts-renesas-disable-OSTM2-I2C0-and-CRU.patch \
    file://0003-arm64-dts-renesas-change-default-bootargs-to-boot-fr.patch \
    file://0004-clk-renesas-r9a07g044-Set-SCIF2-OSTM2-CRU-I2C0-as-cr.patch \
    file://0005-driver-DRP-AI-add-DPR-AI-support-for-RZ-V2L-board.patch \
    file://0006-Workaround-GPU-driver-remove-power-domains-v2l.patch \
    file://0001-bootup-DEFCONFIG-RzBoard-add-Avnet-RzBoard-support.patch \
    file://0002-Mcore-DEFCONFIG-RzBoard-Add-UIO-support.patch \
    file://0003-DTBO-CODE-RzBoard-Add-dtbo-support-for-devicetree-ov.patch \
    file://0004-MIPI-Audio-CODE-RzBoard-Ported-the-driver-from-branc.patch \
    file://0005-KEY-CODE-RzBoard-update-linux-kernel-common-GPIO-key.patch \
    file://0006-WLAN-CODE-RzBoard-Add-wifi-driver-for-NXP-88W8987-ch.patch \
    file://0007-bootup-DEFCONFIG-RzBoard-Apply-the-changes-of-Avnet-.patch \
    file://0008-bootup-DTS-RzBoard-Upgrade-devicetree-of-Avnet-RzBoa.patch \
    file://0009-MODIFIED-HDMI-CODE-RzBoard-Change-the-mipi-clock-to-.patch \
    file://0010-MODIFIED-MIPI-CODE-DTS-RzBoard-Change-the-clock-and-.patch \
    file://0011-MIPI-DSI-CODE-DTS-RzBoard-Fixed-the-bug-that-the-new.patch \
    file://0012-arm64-dts-renesas-avnet-board-merge-hdmi-and-cm33-ov.patch \
    file://0013-arm64-dts-renesas-disable-I2C0.patch \
    file://0014-arm64-dts-renesas-change-default-bootargs-for-rzboar.patch \
    file://0015-arm64-defconfig-avnet-board-Update-defconfig-for-opt.patch \
    file://0016-fix-flicker-HDMI.patch \
"

SRC_URI_append = "\
  ${@oe.utils.conditional("USE_DOCKER", "1", " file://docker.cfg ", "", d)} \
"

USE_SYSTEMD = "1"
SRC_URI_append = "\
   ${@oe.utils.conditional("USE_SYSTEMD", "1", " file://systemd.cfg ", "", d)} \
"

B = "${WORKDIR}/build"

SHORT_SRCREV = "${@d.getVar('PV').split('+')[2]}"
LINUX_VERSION_EXTENSION = "-g${SHORT_SRCREV}"

KBUILD_DEFCONFIG = "defconfig"
KCONFIG_MODE = "alldefconfig"

do_kernel_metadata_af_patch() {
    # need to recall do_kernel_metadata after do_patch for some patches applied to defconfig
    rm -f ${WORKDIR}/defconfig
    do_kernel_metadata
}

addtask do_kernel_metadata_af_patch after do_patch before do_kernel_configme

# Fix race condition, which can causes configs in defconfig file be ignored
do_kernel_configme[depends] += "virtual/${TARGET_PREFIX}binutils:do_populate_sysroot"
do_kernel_configme[depends] += "virtual/${TARGET_PREFIX}gcc:do_populate_sysroot"
do_kernel_configme[depends] += "bc-native:do_populate_sysroot bison-native:do_populate_sysroot"

# Fix error: openssl/bio.h: No such file or directory
DEPENDS += "openssl-native"

# Auto load Wi-Fi driver (chipset NXP 88W8987)
KERNEL_MODULE_AUTOLOAD += "moal"
KERNEL_MODULE_PROBECONF += "moal"
module_conf_moal = "options moal mod_para=nxp/wifi_mod_para.conf"

# support to build dtbo
KERNEL_DTC_FLAGS = "-@"
KERNEL_DEVICETREE_OVERLAY ?= ""

do_compile_prepend() {
    if [ -n "${KERNEL_DTC_FLAGS}" ]; then
       export DTC_FLAGS="${KERNEL_DTC_FLAGS}"
    fi
}

do_compile_append() {
    for dtbf in ${KERNEL_DEVICETREE_OVERLAY}; do
        dtb=`normalize_dtb "$dtbf"`
        oe_runmake $dtb CC="${KERNEL_CC} $cc_extra " LD="${KERNEL_LD}" ${KERNEL_EXTRA_ARGS}
    done
}

do_deploy_append(){
   install -d ${DEPLOYDIR}/overlays
   cp ${B}/arch/arm64/boot/dts/renesas/overlays/* ${DEPLOYDIR}/overlays
}

do_populate_sysroot () {
    install -d ${WORKDIR}/sysroot-destdir/boot
    install -m 0644 ${D}/boot/Image ${WORKDIR}/sysroot-destdir/boot
    install -m 0644 ${D}/boot/rzboard.dtb ${WORKDIR}/sysroot-destdir/boot
}
