SUMMARY = "RZ/V2L AI Evaluation Software"
SECTION = "app"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
	file://gpio.sh \
	file://gpio.service \
"

FILES_${PN} += "${systemd_unitdir}/system/gpio.service"

do_install_append() {
  install -d ${D}/${systemd_unitdir}/system
  install -m 0644 ${WORKDIR}/gpio.service ${D}/${systemd_unitdir}/system
  
  install -d ${D}/usr/sbin
  install -m 0755 ${WORKDIR}/gpio.sh ${D}/usr/sbin
}


RDEPENDS_gpio-service = "bash"
SYSTEMD_SERVICE_${PN} = "gpio.service"
SYSTEMD_AUTO_ENABLE = "enable"

