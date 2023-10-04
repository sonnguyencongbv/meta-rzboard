SUMMARY = "Yocto Linux System Image for Avnet RzBoard"
LICENSE = "MIT"

inherit core-image features_check

REQUIRED_DISTRO_FEATURES = "wayland"

CORE_IMAGE_BASE_INSTALL += "weston weston-init"
CORE_IMAGE_BASE_INSTALL += "${@bb.utils.contains('DISTRO_FEATURES', 'x11', 'weston-xwayland matchbox-terminal', '', d)}"

# Basic packages
require include/core-image-renesas-base.inc
require include/core-image-renesas-mmp.inc
require include/core-image-bsp.inc

# Add DRP-AI packages
require include/core-image-sdk.inc
require include/extend_packages.inc

IMAGE_INSTALL_append = " opencv opencv-dev "
# ##TOOLCHAIN_TARGET_TASK_append = " drpai "

# Add CM33 example
IMAGE_INSTALL_append = " cm33"

# Add fip example
DEPENDS += "fip"

# Add gpio service
IMAGE_INSTALL_append = " gpio-service"

# Add yolo service
IMAGE_INSTALL_append = " yolo-service"

# Add app-run
IMAGE_INSTALL_append = " app-run"

IMAGE_INSTALL_append = "\
    gstreamer1.0 \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gst-examples \
    gstreamer1.0-plugins-ugly \
    gst-player \
    gstreamer1.0-meta-base \
    gstreamer1.0-rtsp-server \
    gdk-pixbuf \
    gdk-pixbuf-loader-png \
    gdk-pixbuf-loader-jpeg \
    gdk-pixbuf-loader-bmp \
"

# Basic packages
IMAGE_INSTALL_remove = " \
    bonnie++ \
    util-linux \
    busybox \
    libusb1 \
    pciutils \
    iproute2 \
    i2c-tools \
    can-utils \
    ethtool \
    iperf3 \
    usbutils \
    mtd-utils \
    dosfstools \
    e2fsprogs-badblocks \
    e2fsprogs-dumpe2fs \
    e2fsprogs-e2fsck \
    e2fsprogs-e2scrub \
    e2fsprogs-mke2fs \
    e2fsprogs-resize2fs \
    e2fsprogs-tune2fs \
    minicom \
    memtester \
    alsa-utils \
    libdrm \
    libdrm-tests \
    yavta \
    kernel-module-uvcvideo \
    watchdog \
"

# Additional tools for support Tool develop
IMAGE_INSTALL_remove = " \
    ckermit \
    connman \
    connman-client \
    connman-tools \
    connman-tests \
    connman-wait-online \
    lttng-modules \
    lttng-tools \
    lttng-ust \
    tcf-agent \
"

# Support Docker Engine
IMAGE_INSTALL_remove = " \
    packagegroup-docker \
"

# IMAGE_FEATURES_remove = " ssh-server-dropbear"
# IMAGE_FEATURES_append = " ssh-server-openssh"

# IMAGE_INSTALL_remove = " \
#     optee-client \
#     v4l2-init \
# "

# ROOTFS_POSTPROCESS_COMMAND_remove= "update_issue;"

# Additional packages
# IMAGE_INSTALL_append = " \
#     curl \
#     openssh \
#     alsa-state \
#     gnupg \
#     evtest \
#     spitools \
#     xz \
#     udev-extraconf \
#     vim \
#     nano \
#     cmake \
# "

# Wifi/Bluetooth packages
# IMAGE_INSTALL_append = " \
#     wireless-tools \
#     wpa-supplicant \
#     hostapd \
#     obexftp \
#     bluez5 \
#     bluez5-dev \
#     rzboard-firmware \
# "

# CORE_IMAGE_EXTRA_INSTALL += " packagegroup-tools-bluetooth "

# Add app demos
# IMAGE_INSTALL_append = " \
# 	drpai-app-demos \
# 	usb-camera-http-demo \
# 	rzboard-app-script \
# 	rzboard-demo-launcher \
# "

# unzip_drpai_demo() {
#     DEMO_FILE=app_demos.tar.bz2

#     if [ -e ${IMAGE_ROOTFS}/home/root/${DEMO_FILE} ] ; then
#         tar -x -j -f ${IMAGE_ROOTFS}/home/root/${DEMO_FILE} -C ${IMAGE_ROOTFS}/home/root/
#         rm ${IMAGE_ROOTFS}/home/root/${DEMO_FILE}
#     fi
# }
# ROOTFS_POSTPROCESS_COMMAND += "unzip_drpai_demo; "

# Add extra packages for RzBoard
inherit extrausers
EXTRA_USERS_PARAMS = "\
	usermod -P avnet root; \
"
# rootfs partition space created in Kbytes
IMAGE_ROOTFS_SIZE = "1596000"

# the alignment of the root filesystem image in kilobytes.
IMAGE_ROOTFS_ALIGNMENT = "16"


optimize_service_sytemd(){
	#run
	rm -rf ${IMAGE_ROOTFS}/run/user/0/systemd
	rm -rf ${IMAGE_ROOTFS}/run/udev/tags/systemd
	rm -rf ${IMAGE_ROOTFS}/run/systemd
	#usr
	rm -rf ${IMAGE_ROOTFS}/usr/share/systemd
	rm -rf ${IMAGE_ROOTFS}/usr/lib/systemd
	rm -rf ${IMAGE_ROOTFS}/usr/include/systemd
	#var
	rm -rf ${IMAGE_ROOTFS}/var/lib/systemd
	#etc
	rm -rf ${IMAGE_ROOTFS}/etc/xdg/systemd

	# /etc/systemd
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/journald.conf
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/networkd.conf
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/sleep.conf
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/timesyncd.conf
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/logind.conf
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/pstore.conf
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/user
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/network
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/resolved.conf
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system.conf
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/user.conf

	# /etc/systemd/system
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/banner.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/bluetooth.target.wants
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/bootmisc.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/checkfs.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/checkroot.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/ctrl-alt-del.target
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/dbus-1.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/dbus-org.bluez.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/dbus-org.freedesktop.network1.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/dbus-org.freedesktop.resolve1.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/dbus-org.freedesktop.timesync1.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/dbus-org.freedesktop.Avahi.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/devpts.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/dmesg.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/getty.target.wants
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/hostname.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/hwclock.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/local-fs.target.wants
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/modutils.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/mountall.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/mountnfs.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/multi-user.target.wants
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/networking.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/network-online.target.wants
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/populate-volatile.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/read-only-rootfs-hook.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/rmnologin.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/sockets.target.wants
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/sysfs.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/sysinit.target.wants
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/syslog.service
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/systemd-random-seed.service.wants
	rm -rf ${IMAGE_ROOTFS}/etc/systemd/system/urandom.service

	# /lib/systemd/
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-ac-power
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-backlight
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-boot-check-no-failures
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-cgroups-agent
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-dissect
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-fsck
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-growfs
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-hibernate-resume
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-hostnamed
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-initctl
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-journald
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-localed
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-network-generator
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-networkd
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-networkd-wait-online
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-pstore
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-quotacheck
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-random-seed
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-reply-password
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-resolved
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-rfkill
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-shutdown
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-sleep
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-socket-proxyd
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-update-done
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-update-utmp
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-user-runtime-dir
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-user-sessions
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-vconsole-setup
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-volatile-root
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-makefs
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-sysctl
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/systemd-sulogin-shell
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/journald.conf.d
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/logind.conf.d
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/resolv.conf
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system.conf.d
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/ntp-units.d
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system-shutdown
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/network
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system-preset
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system-sleep

	# /systemd/system-generators/
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system-generators/systemd-debug-generator
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system-generators/systemd-fstab-generator
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system-generators/systemd-gpt-auto-generator
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system-generators/systemd-hibernate-resume-generator
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system-generators/systemd-rc-local-generator
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system-generators/systemd-run-generator
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system-generators/systemd-system-update-generator
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system-generators/systemd-sysv-generator

	# /lib/systemd/system/multi-user.target.wants/
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/multi-user.target.wants/dbus.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/multi-user.target.wants/systemd-update-utmp-runlevel.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/multi-user.target.wants/systemd-ask-password-wall.path
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/multi-user.target.wants/systemd-logind.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/multi-user.target.wants/systemd-user-sessions.service

	# /lib/systemd/system/
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/a*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/c*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/dbus*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/debug-shell.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/dev*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/dropbearkey.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/dropbear@.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/dropbear.socket
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/e*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/f*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/h*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/i*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/j*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/k*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/l*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/n*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/p*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/r*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/t*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/u*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/v*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-a*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-b*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-e*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-f*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-h*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-i*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-j*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-k*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-n*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-p*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-q*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-r*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-s*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-t*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/ssh*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/so*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/md*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/quotaon.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sh*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/si*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sl*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sm*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/su*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sw*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-us*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-v*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-ma*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-loc*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/syslog.socket
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sys-*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/system-update*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/bluetooth.target
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/boot-complete.target
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/busybox-klogd.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/busybox-syslog.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/getty-pre.target
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/getty@.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-udevd-control.socket
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-udevd-kernel.socket
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/bluetooth.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/bluetooth.target
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/getty-pre.target
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/ofono.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/optee.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/quotaon.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/shutdown.target
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sigpwr.target
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sleep.target
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/slices.target
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/smartcard.target
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sockets.targetls
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sockets.target.wants
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sound.target
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sound.target.wants
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/suspend.target
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/suspend-then-hibernate.target
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/swap.target
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sys-fs-fuse-connections.mount
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sys-kernel-config.mount
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sys-kernel-debug.mount
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/syslog.socket
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/system-update-cleanup.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-udevd-control.socket
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-udevd-kernel.socket
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/system-update-pre.target
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/system-update.target
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-update-done.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-update-utmp-runlevel.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-update-utmp.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/watchdog.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/wpa_supplicant-nl80211@.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/wpa_supplicant.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/wpa_supplicant@.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/wpa_supplicant-wired@.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/graphical.target.wants
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/dnf-*

	# /lib/systemd/system/sysinit.target.wants/
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sysinit.target.wants/systemd-a*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sysinit.target.wants/systemd-h*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sysinit.target.wants/systemd-j*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sysinit.target.wants/dev-*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sysinit.target.wants/k*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sysinit.target.wants/l*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sysinit.target.wants/sys-*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sysinit.target.wants/systemd-ma*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sysinit.target.wants/systemd-ra*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sysinit.target.wants/systemd-tmp*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sysinit.target.wants/systemd-upda*
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/sysinit.target.wants/systemd-sysusers.service

	# 13 services is to 12 services
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/systemd-udev-settle.service
	rm -rf ${IMAGE_ROOTFS}/lib/systemd/system/weston@.service

	# serial-getty@.service  : auto login
	sed -i 's/^/#/' ${IMAGE_ROOTFS}/lib/systemd/system/serial-getty@.service
	echo "[Service]" >> ${IMAGE_ROOTFS}/lib/systemd/system/serial-getty@.service
	echo "StandardOutput=null" >> ${IMAGE_ROOTFS}/lib/systemd/system/serial-getty@.service
	echo "StandardError=null"  >> ${IMAGE_ROOTFS}/lib/systemd/system/serial-getty@.service
	echo "ExecStartPre=mount -o remount,rw /" >> ${IMAGE_ROOTFS}/lib/systemd/system/serial-getty@.service
	echo "ExecStart=-/sbin/agetty --autologin root -8 -L %I 115200 \$TERM" >> ${IMAGE_ROOTFS}/lib/systemd/system/serial-getty@.service
	echo "Type=idle" >> ${IMAGE_ROOTFS}/lib/systemd/system/serial-getty@.service

	# reduce boot time with 12 services
	sed -i 's/^/#/' ${IMAGE_ROOTFS}/lib/systemd/system/basic.target
	sed -i 's/^/#/' ${IMAGE_ROOTFS}/lib/systemd/system/default.target
	sed -i 's/^/#/' ${IMAGE_ROOTFS}/lib/systemd/system/getty.target
	sed -i 's/^/#/' ${IMAGE_ROOTFS}/lib/systemd/system/graphical.target
	sed -i 's/^/#/' ${IMAGE_ROOTFS}/lib/systemd/system/multi-user.target
	sed -i 's/^/#/' ${IMAGE_ROOTFS}/lib/systemd/system/sysinit.target
	sed -i 's/^/#/' ${IMAGE_ROOTFS}/lib/systemd/system/systemd-logind.service
	sed -i 's/^/#/' ${IMAGE_ROOTFS}/lib/systemd/system/systemd-udevd.service
	sed -i 's/^/#/' ${IMAGE_ROOTFS}/lib/systemd/system/systemd-udev-trigger.service

	#Error "Failed To"
	echo "[Service]"			>> ${IMAGE_ROOTFS}/lib/systemd/system/systemd-modules-load.service
	echo "StandardOutput=null"	>> ${IMAGE_ROOTFS}/lib/systemd/system/systemd-modules-load.service
	echo "StandardError=null"	>> ${IMAGE_ROOTFS}/lib/systemd/system/systemd-modules-load.service

	echo "[Service]"			>> ${IMAGE_ROOTFS}/lib/systemd/system/weston@.service
	echo "StandardOutput=null"	>> ${IMAGE_ROOTFS}/lib/systemd/system/weston@.service
	echo "StandardError=null"	>> ${IMAGE_ROOTFS}/lib/systemd/system/weston@.service

	# Remove demo ai default app
	rm -rf ${IMAGE_ROOTFS}/home/root/RZV_AI_Eva_SW

	# Remove demo v4l2-init.sh
	rm -rf ${IMAGE_ROOTFS}/home/root/v4l2-init.sh
}

ROOTFS_POSTPROCESS_COMMAND += "optimize_service_sytemd; "

MACHINE_ESSENTIAL_EXTRA_RDEPENDS += " kernel-module-gpio-driver"

