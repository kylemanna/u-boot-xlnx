/*
 * (C) Copyright 2013 Xilinx, Inc.
 *
 * Configuration settings for the Avnet ZC702x PicoZed
 * See zynq-common.h for Zynq common configs
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_ZYNQ_PICOZED_H
#define __CONFIG_ZYNQ_PICOZED_H

/* This is the default address for things like dhcp & tftpboot */
#define CONFIG_LOADADDR		0x2080000

/* Enable GPT partition table */
#define CONFIG_PARTITION_UUIDS
#define CONFIG_CMD_GPT
#define CONFIG_EFI_PARTITION

/* Override ENV settings for board specific settings */
#define CONFIG_EXTRA_ENV_SETTINGS	\
	"kernel_image=fitImage\0"	\
	"kernel_load_address=0x2080000\0" \
	"ramdisk_image=uramdisk.image.gz\0"	\
	"ramdisk_load_address=0x4000000\0"	\
	"devicetree_image=devicetree.dtb\0"	\
	"devicetree_load_address=0x2000000\0"	\
	"bitstream_image=system.bit.bin\0"	\
	"boot_image=BOOT.bin\0"	\
	"loadbit_addr=0x100000\0"	\
	"loadbootenv_addr=0x2000000\0" \
	"kernel_size=0x500000\0"	\
	"devicetree_size=0x20000\0"	\
	"ramdisk_size=0x5E0000\0"	\
	"boot_size=0xF00000\0"	\
	"fdt_high=0x20000000\0"	\
	"initrd_high=0x20000000\0"	\
	"bootenv=uEnv.txt\0" \
	"tftptimeout=1000\0" \
	"tftptimeoutcountmax=20\0" \
	"loadbootenv=load mmc 0 ${loadbootenv_addr} ${bootenv}\0" \
	"importbootenv=echo Importing environment from SD ...; " \
		"env import -t ${loadbootenv_addr} $filesize\0" \
	"sd_uEnvtxt_existence_test=test -e mmc 0 /uEnv.txt\0" \
	"preboot=if test $modeboot = sdboot && env run sd_uEnvtxt_existence_test; " \
			"then if env run loadbootenv; " \
				"then env run importbootenv; " \
			"fi; " \
		"fi; \0" \
	"mmc_loadbit=echo Loading bitstream from SD/MMC/eMMC to RAM.. && " \
		"mmcinfo && " \
		"load mmc 0 ${loadbit_addr} ${bitstream_image} && " \
		"fpga load 0 ${loadbit_addr} ${filesize}\0" \
	"qspiboot=run emmcboot ; echo Copying Linux from QSPI flash to RAM... && " \
		"sf probe 0 0 0 && " \
		"sf read ${loadaddr} 0x100000 ${kernel_size} && " \
		"sf read ${devicetree_load_address} 0x600000 ${devicetree_size} && " \
		"echo Copying ramdisk... && " \
		"sf read ${ramdisk_load_address} 0x620000 ${ramdisk_size} && " \
		"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}\0" \
	"uenvboot=" \
		"if run loadbootenv; then " \
			"echo Loaded environment from ${bootenv}; " \
			"run importbootenv; " \
		"fi; " \
		"if test -n $uenvcmd; then " \
			"echo Running uenvcmd ...; " \
			"run uenvcmd; " \
		"fi\0" \
	"sdboot=if mmc info; then " \
			"run uenvboot; " \
			"echo Copying Linux from SD to RAM... && " \
			"load mmc 0 ${kernel_load_address} ${kernel_image} && " \
			"load mmc 0 ${devicetree_load_address} ${devicetree_image} && " \
			"load mmc 0 ${ramdisk_load_address} ${ramdisk_image} && " \
			"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}; " \
		"fi\0" \
	"usbboot=if usb start; then " \
			"run uenvboot; " \
			"echo Copying Linux from USB to RAM... && " \
			"load usb 0 ${kernel_load_address} ${kernel_image} && " \
			"load usb 0 ${devicetree_load_address} ${devicetree_image} && " \
			"load usb 0 ${ramdisk_load_address} ${ramdisk_image} && " \
			"bootm ${kernel_load_address} ${ramdisk_load_address} ${devicetree_load_address}; " \
		"fi\0" \
	"jtagboot=echo TFTPing Linux FIT image to RAM... && " \
		"tftpboot ${loadaddr} ${kernel_image} && " \
		"bootm ${loadaddr}\0" \
	"netboot=echo TFTPing Linux FIT image to RAM and auto starting... && " \
		"set autostart yes && " \
		"dhcp ${loadaddr}\0" \
	"emmcboot=if mmc info; then " \
			"echo Copying Linux FIT image from eMMC to RAM... && " \
			"load mmc 0 ${loadaddr} ${kernel_image} && " \
			"bootm ${loadaddr}; " \
		"fi; " \
		"run netboot\0" \


#include <configs/zynq-common.h>

/* Override ENV settings from zynq-common, use minimum sector size of 64kB */
#undef CONFIG_ENV_SIZE
#undef CONFIG_ENV_SECT_SIZE

#define CONFIG_ENV_SIZE         (64 << 10)
#define CONFIG_ENV_SECT_SIZE    CONFIG_ENV_SIZE

#endif /* __CONFIG_ZYNQ_PICOZED_H */
