/*
 * am335x_evm.h
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __CONFIG_BEAGLEBONE_H
#define __CONFIG_BEAGLEBONE_H

#include <configs/ti_am335x_common.h>
#define CONFIG_SUPPORT_RAW_INITRD

#define MACH_TYPE_BEAGLEBONE		3808	/* Until the next sync */
#define CONFIG_MACH_TYPE		MACH_TYPE_BEAGLEBONE
#define CONFIG_BOARD_LATE_INIT

/* Clock Defines */
#define V_OSCK				24000000  /* Clock output from T2 */
#define V_SCLK				(V_OSCK)

/* Custom script for NOR */
#define CONFIG_SYS_LDSCRIPT		"board/ti/beaglebone/u-boot.lds"

/* Always 128 KiB env size */
#define CONFIG_ENV_SIZE			(128 << 10)

/* Enhance our eMMC support / experience. */
#define CONFIG_CMD_GPT
#define CONFIG_EFI_PARTITION
#define CONFIG_PARTITION_UUIDS
#define CONFIG_CMD_PART

#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

#ifndef CONFIG_SPL_BUILD
#define CONFIG_EXTRA_ENV_SETTINGS \
	"loadaddr=0x82000000\0" \
	"fdtaddr=0x88000000\0" \
	"fdt_high=0xffffffff\0" \
	"boot_fdt=try\0" \
	"rdaddr=0x88080000\0" \
	"addip=setenv bootargs ${bootargs} ip=${ipaddr}:${serverip}:${gatewayip}:${netmask}:${hostname}:${netdev}:off\0" \
	"netdev=eth0\0" \
	"bootpart=0:1\0" \
	"splfile=MLO\0" \
	"uboot_file=u-boot.img\0" \
	"load_spl=tftp ${loadaddr} ${splfile}\0" \
	"update_spl=fatwrite mmc ${bootpart} ${loadaddr} ${splfile} ${filesize}\0" \
	"load_uboot=tftp ${loadaddr} ${uboot_file}\0" \
	"update_uboot=fatwrite mmc ${bootpart} ${loadaddr} ${uboot_file} ${filesize}\0" \
	"update_boot=run load_spl; run update_spl; run load_uboot; run update_uboot\0" \
	"loadk=tftp ${loadaddr} ${bootfile}\0" \
	"updatek=fatwrite mmc ${bootpart} ${loadaddr} ${bootfile} ${filesize}\0" \
	"load_devicetree=tftp ${loadaddr} ${fdtfile}\0" \
	"update_devicetree=fatwrite mmc ${bootpart} ${loadaddr} ${fdtfile} ${filesize}\0" \
	"update_linux=run load_devicetree; run update_devicetree; run loadk; run updatek\0" \
	"bootfile=zImage\0" \
	"ethaddr=d4:94:a1:3b:5d:14\0" \
	"ipaddr=192.168.85.129\0" \
	"serverip=192.168.85.128\0" \
	"netmask=255.255.255.0\0" \
	"gatewayip=192.168.85.1\0" \
	"kern_mem=252M\0" \
	"splashimage=0x8F000000\0" \
	"splashfile=splash.bmp\0" \
	"update_splash=tftp ${loadaddr} ${splashfile};fatwrite mmc ${bootpart} ${loadaddr} ${splashfile} ${filesize}\0" \
	"load_splash=mmc rescan; load mmc ${bootpart} ${splashimage} ${splashfile}\0" \
	"fdtfile=bone_printer.dtb\0" \
	"console=serial\0" \
	"console_linux=ttyO0,115200n8\0" \
	"partitions=" \
		"uuid_disk=${uuid_gpt_disk};" \
		"name=rootfs,start=2MiB,size=-,uuid=${uuid_gpt_rootfs}\0" \
	"optargs=\0" \
	"mmcdev=0\0" \
	"mmcpart=1\0" \
	"mmcroot=/dev/mmcblk0p2 rw fixrtc\0" \
	"mmcrootfstype=ext4 rootwait\0" \
	"rootpath=/export/rootfs\0" \
	"nfsopts=nolock\0" \
	"static_ip=${ipaddr}:${serverip}:${gatewayip}:${netmask}:${hostname}::off\0" \
	"mmcargs=setenv bootargs console=${console_linux} mem=${kern_mem} " \
		"${optargs} " \
		"root=${mmcroot} " \
		"rootfstype=${mmcrootfstype}\0" \
	"netargs=setenv bootargs console=${console_linux} " \
		"${optargs} " \
		"root=/dev/nfs " \
		"nfsroot=${serverip}:${rootpath},${nfsopts} rw " \
		"ip=dhcp\0" \
	"bootenv=uEnv.txt\0" \
	"loadbootenv=load mmc ${bootpart} ${loadaddr} ${bootenv}\0" \
	"importbootenv=echo Importing environment from mmc ...; " \
		"env import -t $loadaddr $filesize\0" \
	"loadramdisk=load mmc ${mmcdev} ${rdaddr} ramdisk.gz\0" \
	"loadimage=load mmc ${bootpart} ${loadaddr} ${bootfile}\0" \
	"loadfdt=load mmc ${bootpart} ${fdtaddr} ${fdtfile}\0" \
	"mmcloados=run mmcargs addip; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if run loadfdt; then " \
				"bootz ${loadaddr} - ${fdtaddr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0" \
	"mmcboot=mmc dev ${mmcdev}; " \
		"if mmc rescan; then " \
			"gpio set 54;" \
			"echo SD/MMC found on device ${mmcdev};" \
			"if run loadbootenv; then " \
				"gpio set 55;" \
				"echo Loaded environment from ${bootenv};" \
				"run importbootenv;" \
			"fi;" \
			"if test -n $cape; then " \
				"if test -e mmc ${bootpart} $fdtbase-$cape.dtb; then " \
					"setenv fdtfile $fdtbase-$cape.dtb; " \
				"fi; " \
				"echo using: $fdtfile...; " \
			"fi; " \
			"echo Checking if uenvcmd is set ...;" \
			"if test -n $uenvcmd; then " \
				"gpio set 56; " \
				"echo Running uenvcmd ...;" \
				"run uenvcmd;" \
			"fi;" \
			"echo; echo uenvcmd was not defined in uEnv.txt ...;"\
 		"fi;\0" \
	"netboot=echo Booting from network ...; " \
		"setenv autoload no; " \
		"dhcp; " \
		"tftp ${loadaddr} ${bootfile}; " \
		"tftp ${fdtaddr} ${fdtfile}; " \
		"run netargs; " \
		"bootz ${loadaddr} - ${fdtaddr}\0" \
	DFUARGS
#endif

#define CONFIG_BOOTCOMMAND \
	"run loadimage; run mmcloados;"

/* NS16550 Configuration */
#define CONFIG_SYS_NS16550_COM1		0x44e09000	/* Base EVM has UART0 */
#define CONFIG_SYS_NS16550_COM2		0x48022000	/* UART1 */
#define CONFIG_SYS_NS16550_COM3		0x48024000	/* UART2 */
#define CONFIG_SYS_NS16550_COM4		0x481a6000	/* UART3 */
#define CONFIG_SYS_NS16550_COM5		0x481a8000	/* UART4 */
#define CONFIG_SYS_NS16550_COM6		0x481aa000	/* UART5 */
#define CONFIG_BAUDRATE			115200

#define CONFIG_CMD_EEPROM
#define CONFIG_ENV_EEPROM_IS_ON_I2C
#define CONFIG_SYS_I2C_EEPROM_ADDR	0x50	/* Main EEPROM */
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN	2
#define CONFIG_SYS_I2C_MULTI_EEPROMS

/* PMIC support */
#define CONFIG_POWER_TPS65217
#define CONFIG_POWER_TPS65910

/* SPL */
#ifndef CONFIG_NOR_BOOT
#define CONFIG_SPL_POWER_SUPPORT
#define CONFIG_SPL_YMODEM_SUPPORT

/* Bootcount using the RTC block */
#define CONFIG_BOOTCOUNT_LIMIT
#define CONFIG_BOOTCOUNT_AM33XX

/* CPSW support */
#define CONFIG_SPL_ETH_SUPPORT

/* USB gadget RNDIS */
#define CONFIG_SPL_MUSB_NEW_SUPPORT

/* General network SPL, both CPSW and USB gadget RNDIS */
#define CONFIG_SPL_NET_SUPPORT
#define CONFIG_SPL_ENV_SUPPORT
#define CONFIG_SPL_NET_VCI_STRING	"AM335x U-Boot SPL"

/* SPI flash. */
/*#define CONFIG_SPL_SPI_SUPPORT
#define CONFIG_SPL_SPI_FLASH_SUPPORT
#define CONFIG_SPL_SPI_LOAD
#define CONFIG_SPL_SPI_BUS		0
#define CONFIG_SPL_SPI_CS		0
#define CONFIG_SYS_SPI_U_BOOT_OFFS	0x20000*/

#define CONFIG_SPL_LDSCRIPT		"$(CPUDIR)/am33xx/u-boot-spl.lds"

#ifdef CONFIG_NAND
#define CONFIG_NAND_OMAP_GPMC
#define CONFIG_NAND_OMAP_ELM
#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_PAGE_COUNT	(CONFIG_SYS_NAND_BLOCK_SIZE / \
					 CONFIG_SYS_NAND_PAGE_SIZE)
#define CONFIG_SYS_NAND_PAGE_SIZE	2048
#define CONFIG_SYS_NAND_OOBSIZE		64
#define CONFIG_SYS_NAND_BLOCK_SIZE	(128*1024)
#define CONFIG_SYS_NAND_BAD_BLOCK_POS	NAND_LARGE_BADBLOCK_POS
#define CONFIG_SYS_NAND_ECCPOS		{ 2, 3, 4, 5, 6, 7, 8, 9, \
					 10, 11, 12, 13, 14, 15, 16, 17, \
					 18, 19, 20, 21, 22, 23, 24, 25, \
					 26, 27, 28, 29, 30, 31, 32, 33, \
					 34, 35, 36, 37, 38, 39, 40, 41, \
					 42, 43, 44, 45, 46, 47, 48, 49, \
					 50, 51, 52, 53, 54, 55, 56, 57, }

#define CONFIG_SYS_NAND_ECCSIZE		512
#define CONFIG_SYS_NAND_ECCBYTES	14
#define CONFIG_SYS_NAND_ONFI_DETECTION
#define CONFIG_NAND_OMAP_ECCSCHEME	OMAP_ECC_BCH8_CODE_HW
#define CONFIG_SYS_NAND_U_BOOT_START	CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_NAND_U_BOOT_OFFS	0x80000
#endif
#endif

/*
 * For NOR boot, we must set this to the start of where NOR is mapped
 * in memory.
 */
#ifdef CONFIG_NOR_BOOT
#define CONFIG_SYS_TEXT_BASE		0x08000000
#endif

/*
 * USB configuration.  We enable MUSB support, both for host and for
 * gadget.  We set USB0 as peripheral and USB1 as host, based on the
 * board schematic and physical port wired to each.  Then for host we
 * add mass storage support and for gadget we add both RNDIS ethernet
 * and DFU.
 */
#define CONFIG_USB_MUSB_DSPS
#define CONFIG_ARCH_MISC_INIT
#define CONFIG_MUSB_GADGET
#define CONFIG_MUSB_PIO_ONLY
#define CONFIG_MUSB_DISABLE_BULK_COMBINE_SPLIT
#define CONFIG_USB_GADGET
#define CONFIG_USBDOWNLOAD_GADGET
#define CONFIG_USB_GADGET_DUALSPEED
#define CONFIG_USB_GADGET_VBUS_DRAW	2
#define CONFIG_MUSB_HOST
#define CONFIG_AM335X_USB0
#define CONFIG_AM335X_USB0_MODE	MUSB_PERIPHERAL
#define CONFIG_AM335X_USB1
#define CONFIG_AM335X_USB1_MODE MUSB_HOST

#ifdef CONFIG_MUSB_HOST
#define CONFIG_CMD_USB
#define CONFIG_USB_STORAGE
#endif

#ifdef CONFIG_MUSB_GADGET
#define CONFIG_USB_ETHER
#define CONFIG_USB_ETH_RNDIS
#define CONFIG_USBNET_HOST_ADDR	"de:ad:be:af:00:00"

/* USB TI's IDs */
#define CONFIG_G_DNL_VENDOR_NUM 0x0403
#define CONFIG_G_DNL_PRODUCT_NUM 0xBD00
#define CONFIG_G_DNL_MANUFACTURER "Texas Instruments"
#endif /* CONFIG_MUSB_GADGET */

#if defined(CONFIG_SPL_BUILD) && defined(CONFIG_SPL_USBETH_SUPPORT)
/* disable host part of MUSB in SPL */
#undef CONFIG_MUSB_HOST
/* disable EFI partitions and partition UUID support */
#undef CONFIG_PARTITION_UUIDS
#undef CONFIG_EFI_PARTITION
/*
 * Disable CPSW SPL support so we fit within the 101KiB limit.
 */
#undef CONFIG_SPL_ETH_SUPPORT
#endif

/* USB Device Firmware Update support */
#define CONFIG_DFU_FUNCTION
#define CONFIG_DFU_MMC
#define CONFIG_CMD_DFU
#define DFU_ALT_INFO_MMC \
	"dfu_alt_info_mmc=" \
	"boot part 0 1;" \
	"rootfs part 0 2;" \
	"MLO fat 0 1;" \
	"MLO.raw mmc 100 100;" \
	"u-boot.img.raw mmc 300 400;" \
	"spl-os-args.raw mmc 80 80;" \
	"spl-os-image.raw mmc 900 2000;" \
	"spl-os-args fat 0 1;" \
	"spl-os-image fat 0 1;" \
	"u-boot.img fat 0 1;" \
	"uEnv.txt fat 0 1\0"
#define CONFIG_DFU_RAM
#define DFU_ALT_INFO_RAM \
	"dfu_alt_info_ram=" \
	"kernel ram 0x80200000 0xD80000;" \
	"fdt ram 0x80F80000 0x80000;" \
	"ramdisk ram 0x81000000 0x4000000\0"
#define DFUARGS \
	"dfu_alt_info_emmc=rawemmc mmc 0 3751936\0" \
	DFU_ALT_INFO_MMC \
	DFU_ALT_INFO_RAM

#define CONFIG_DOS_PARTITION
#define CONFIG_CMD_FAT
#define CONFIG_FAT_WRITE
#define CONFIG_CMD_FS_GENERIC

#undef CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_IS_IN_FAT
#define FAT_ENV_DEVICE_AND_PART "0:1"
#define FAT_ENV_INTERFACE	"mmc"
#define FAT_ENV_FILE		"uEnv.txt"
#define FAT_ENV_DEVICE		0
#define FAT_ENV_PART		1

/* SPLASH IMAGE */
#define CONFIG_VIDEO
#if defined(CONFIG_VIDEO)
#define CONFIG_CFB_CONSOLE
#define CONFIG_VGA_AS_SINGLE_DEVICE
#define CONFIG_CMD_BMP
#define CONFIG_VIDEO_DA8XX
#define CONFIG_VIDEO_DA8XX_FB_MEMSIZE	(4*1024*1024)	/* 4 MB */
#define CONFIG_VIDEO_DA8XX_FB_MEMADDR	(((256*1024*1024)-CONFIG_VIDEO_DA8XX_FB_MEMSIZE)+0x80000000)
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_VIDEO_LOGO
#define CONFIG_SYS_CONSOLE_INFO_QUIET
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_VIDEO_BMP_LOGO
#define PWM_TICKS	0x1388
#define PWM_DUTY	0x1000
#define DA8XX_LCD_CNTL_BASE LCD_CNTL_BASE
#endif

/* SPI flash. */
/*#define CONFIG_CMD_SF
#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_WINBOND
#define CONFIG_SF_DEFAULT_SPEED		24000000*/

/* Network. */
#define CONFIG_PHY_GIGE
#define CONFIG_PHYLIB
#define CONFIG_PHY_SMSC

/* LCD Support */
#define CONFIG_LCD_TCM

#endif	/* ! __CONFIG_BEAGLEBONE_H */