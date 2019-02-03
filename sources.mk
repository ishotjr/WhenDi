OBJ_SRCS := 
S_SRCS := 
ASM_SRCS := 
C_SRCS := 
S_UPPER_SRCS := 
O_SRCS := 
EXECUTABLES := 
OBJS := 
C_DEPS := 

C_SRCS += RapidIot_Base/CMSIS_driver/fsl_dspi_cmsis.c
OBJS += RapidIot_Base/CMSIS_driver/fsl_dspi_cmsis.o
C_DEPS += RapidIot_Base/CMSIS_driver/fsl_dspi_cmsis.d

C_SRCS += RapidIot_Base/CMSIS_driver/fsl_i2c_cmsis.c
OBJS += RapidIot_Base/CMSIS_driver/fsl_i2c_cmsis.o
C_DEPS += RapidIot_Base/CMSIS_driver/fsl_i2c_cmsis.d

C_SRCS += RapidIot_Base/app_update/app_program_ext.c
OBJS += RapidIot_Base/app_update/app_program_ext.o
C_DEPS += RapidIot_Base/app_update/app_program_ext.d

C_SRCS += RapidIot_Base/app_update/flash_ica_driver.c
OBJS += RapidIot_Base/app_update/flash_ica_driver.o
C_DEPS += RapidIot_Base/app_update/flash_ica_driver.d

C_SRCS += RapidIot_Base/app_update/img_program_ext.c
OBJS += RapidIot_Base/app_update/img_program_ext.o
C_DEPS += RapidIot_Base/app_update/img_program_ext.d

C_SRCS += RapidIot_Base/app_update/rpk_led.c
OBJS += RapidIot_Base/app_update/rpk_led.o
C_DEPS += RapidIot_Base/app_update/rpk_led.d

C_SRCS += RapidIot_Base/board/backlight.c
OBJS += RapidIot_Base/board/backlight.o
C_DEPS += RapidIot_Base/board/backlight.d

C_SRCS += RapidIot_Base/board/battery.c
OBJS += RapidIot_Base/board/battery.o
C_DEPS += RapidIot_Base/board/battery.d

C_SRCS += RapidIot_Base/board/board.c
OBJS += RapidIot_Base/board/board.o
C_DEPS += RapidIot_Base/board/board.d

C_SRCS += RapidIot_Base/board/clock_config.c
OBJS += RapidIot_Base/board/clock_config.o
C_DEPS += RapidIot_Base/board/clock_config.d

C_SRCS += RapidIot_Base/board/hardware_init_MK64F12.c
OBJS += RapidIot_Base/board/hardware_init_MK64F12.o
C_DEPS += RapidIot_Base/board/hardware_init_MK64F12.d

C_SRCS += RapidIot_Base/board/peripherals.c
OBJS += RapidIot_Base/board/peripherals.o
C_DEPS += RapidIot_Base/board/peripherals.d

C_SRCS += RapidIot_Base/board/pin_mux.c
OBJS += RapidIot_Base/board/pin_mux.o
C_DEPS += RapidIot_Base/board/pin_mux.d

C_SRCS += RapidIot_Base/board/pin_mux_rpk.c
OBJS += RapidIot_Base/board/pin_mux_rpk.o
C_DEPS += RapidIot_Base/board/pin_mux_rpk.d

C_SRCS += RapidIot_Base/board/rgb_led.c
OBJS += RapidIot_Base/board/rgb_led.o
C_DEPS += RapidIot_Base/board/rgb_led.d

C_SRCS += RapidIot_Base/crc/src/crc16.c
OBJS += RapidIot_Base/crc/src/crc16.o
C_DEPS += RapidIot_Base/crc/src/crc16.d

C_SRCS += RapidIot_Base/crc/src/crc32.c
OBJS += RapidIot_Base/crc/src/crc32.o
C_DEPS += RapidIot_Base/crc/src/crc32.d

C_SRCS += RapidIot_Base/devices/MK64F12/system_MK64F12.c
OBJS += RapidIot_Base/devices/MK64F12/system_MK64F12.o
C_DEPS += RapidIot_Base/devices/MK64F12/system_MK64F12.d

C_SRCS += RapidIot_Base/drivers/fsl_adc16.c
OBJS += RapidIot_Base/drivers/fsl_adc16.o
C_DEPS += RapidIot_Base/drivers/fsl_adc16.d

C_SRCS += RapidIot_Base/drivers/fsl_clock.c
OBJS += RapidIot_Base/drivers/fsl_clock.o
C_DEPS += RapidIot_Base/drivers/fsl_clock.d

C_SRCS += RapidIot_Base/drivers/fsl_cmp.c
OBJS += RapidIot_Base/drivers/fsl_cmp.o
C_DEPS += RapidIot_Base/drivers/fsl_cmp.d

C_SRCS += RapidIot_Base/drivers/fsl_cmt.c
OBJS += RapidIot_Base/drivers/fsl_cmt.o
C_DEPS += RapidIot_Base/drivers/fsl_cmt.d

C_SRCS += RapidIot_Base/drivers/fsl_common.c
OBJS += RapidIot_Base/drivers/fsl_common.o
C_DEPS += RapidIot_Base/drivers/fsl_common.d

C_SRCS += RapidIot_Base/drivers/fsl_crc.c
OBJS += RapidIot_Base/drivers/fsl_crc.o
C_DEPS += RapidIot_Base/drivers/fsl_crc.d

C_SRCS += RapidIot_Base/drivers/fsl_dac.c
OBJS += RapidIot_Base/drivers/fsl_dac.o
C_DEPS += RapidIot_Base/drivers/fsl_dac.d

C_SRCS += RapidIot_Base/drivers/fsl_dmamux.c
OBJS += RapidIot_Base/drivers/fsl_dmamux.o
C_DEPS += RapidIot_Base/drivers/fsl_dmamux.d

C_SRCS += RapidIot_Base/drivers/fsl_dspi.c
OBJS += RapidIot_Base/drivers/fsl_dspi.o
C_DEPS += RapidIot_Base/drivers/fsl_dspi.d

C_SRCS += RapidIot_Base/drivers/fsl_dspi_edma.c
OBJS += RapidIot_Base/drivers/fsl_dspi_edma.o
C_DEPS += RapidIot_Base/drivers/fsl_dspi_edma.d

C_SRCS += RapidIot_Base/drivers/fsl_dspi_freertos.c
OBJS += RapidIot_Base/drivers/fsl_dspi_freertos.o
C_DEPS += RapidIot_Base/drivers/fsl_dspi_freertos.d

C_SRCS += RapidIot_Base/drivers/fsl_edma.c
OBJS += RapidIot_Base/drivers/fsl_edma.o
C_DEPS += RapidIot_Base/drivers/fsl_edma.d

C_SRCS += RapidIot_Base/drivers/fsl_enet.c
OBJS += RapidIot_Base/drivers/fsl_enet.o
C_DEPS += RapidIot_Base/drivers/fsl_enet.d

C_SRCS += RapidIot_Base/drivers/fsl_ewm.c
OBJS += RapidIot_Base/drivers/fsl_ewm.o
C_DEPS += RapidIot_Base/drivers/fsl_ewm.d

C_SRCS += RapidIot_Base/drivers/fsl_flash.c
OBJS += RapidIot_Base/drivers/fsl_flash.o
C_DEPS += RapidIot_Base/drivers/fsl_flash.d

C_SRCS += RapidIot_Base/drivers/fsl_flexbus.c
OBJS += RapidIot_Base/drivers/fsl_flexbus.o
C_DEPS += RapidIot_Base/drivers/fsl_flexbus.d

C_SRCS += RapidIot_Base/drivers/fsl_flexcan.c
OBJS += RapidIot_Base/drivers/fsl_flexcan.o
C_DEPS += RapidIot_Base/drivers/fsl_flexcan.d

C_SRCS += RapidIot_Base/drivers/fsl_ftm.c
OBJS += RapidIot_Base/drivers/fsl_ftm.o
C_DEPS += RapidIot_Base/drivers/fsl_ftm.d

C_SRCS += RapidIot_Base/drivers/fsl_gpio.c
OBJS += RapidIot_Base/drivers/fsl_gpio.o
C_DEPS += RapidIot_Base/drivers/fsl_gpio.d

C_SRCS += RapidIot_Base/drivers/fsl_i2c.c
OBJS += RapidIot_Base/drivers/fsl_i2c.o
C_DEPS += RapidIot_Base/drivers/fsl_i2c.d

C_SRCS += RapidIot_Base/drivers/fsl_i2c_edma.c
OBJS += RapidIot_Base/drivers/fsl_i2c_edma.o
C_DEPS += RapidIot_Base/drivers/fsl_i2c_edma.d

C_SRCS += RapidIot_Base/drivers/fsl_i2c_freertos.c
OBJS += RapidIot_Base/drivers/fsl_i2c_freertos.o
C_DEPS += RapidIot_Base/drivers/fsl_i2c_freertos.d

C_SRCS += RapidIot_Base/drivers/fsl_ili9341.c
OBJS += RapidIot_Base/drivers/fsl_ili9341.o
C_DEPS += RapidIot_Base/drivers/fsl_ili9341.d

C_SRCS += RapidIot_Base/drivers/fsl_llwu.c
OBJS += RapidIot_Base/drivers/fsl_llwu.o
C_DEPS += RapidIot_Base/drivers/fsl_llwu.d

C_SRCS += RapidIot_Base/drivers/fsl_lptmr.c
OBJS += RapidIot_Base/drivers/fsl_lptmr.o
C_DEPS += RapidIot_Base/drivers/fsl_lptmr.d

C_SRCS += RapidIot_Base/drivers/fsl_pdb.c
OBJS += RapidIot_Base/drivers/fsl_pdb.o
C_DEPS += RapidIot_Base/drivers/fsl_pdb.d

C_SRCS += RapidIot_Base/drivers/fsl_phy.c
OBJS += RapidIot_Base/drivers/fsl_phy.o
C_DEPS += RapidIot_Base/drivers/fsl_phy.d

C_SRCS += RapidIot_Base/drivers/fsl_pit.c
OBJS += RapidIot_Base/drivers/fsl_pit.o
C_DEPS += RapidIot_Base/drivers/fsl_pit.d

C_SRCS += RapidIot_Base/drivers/fsl_pmc.c
OBJS += RapidIot_Base/drivers/fsl_pmc.o
C_DEPS += RapidIot_Base/drivers/fsl_pmc.d

C_SRCS += RapidIot_Base/drivers/fsl_rcm.c
OBJS += RapidIot_Base/drivers/fsl_rcm.o
C_DEPS += RapidIot_Base/drivers/fsl_rcm.d

C_SRCS += RapidIot_Base/drivers/fsl_rnga.c
OBJS += RapidIot_Base/drivers/fsl_rnga.o
C_DEPS += RapidIot_Base/drivers/fsl_rnga.d

C_SRCS += RapidIot_Base/drivers/fsl_rtc.c
OBJS += RapidIot_Base/drivers/fsl_rtc.o
C_DEPS += RapidIot_Base/drivers/fsl_rtc.d

C_SRCS += RapidIot_Base/drivers/fsl_sai.c
OBJS += RapidIot_Base/drivers/fsl_sai.o
C_DEPS += RapidIot_Base/drivers/fsl_sai.d

C_SRCS += RapidIot_Base/drivers/fsl_sai_edma.c
OBJS += RapidIot_Base/drivers/fsl_sai_edma.o
C_DEPS += RapidIot_Base/drivers/fsl_sai_edma.d

C_SRCS += RapidIot_Base/drivers/fsl_sdhc.c
OBJS += RapidIot_Base/drivers/fsl_sdhc.o
C_DEPS += RapidIot_Base/drivers/fsl_sdhc.d

C_SRCS += RapidIot_Base/drivers/fsl_sim.c
OBJS += RapidIot_Base/drivers/fsl_sim.o
C_DEPS += RapidIot_Base/drivers/fsl_sim.d

C_SRCS += RapidIot_Base/drivers/fsl_smc.c
OBJS += RapidIot_Base/drivers/fsl_smc.o
C_DEPS += RapidIot_Base/drivers/fsl_smc.d

C_SRCS += RapidIot_Base/drivers/fsl_sysmpu.c
OBJS += RapidIot_Base/drivers/fsl_sysmpu.o
C_DEPS += RapidIot_Base/drivers/fsl_sysmpu.d

C_SRCS += RapidIot_Base/drivers/fsl_uart.c
OBJS += RapidIot_Base/drivers/fsl_uart.o
C_DEPS += RapidIot_Base/drivers/fsl_uart.d

C_SRCS += RapidIot_Base/drivers/fsl_uart_edma.c
OBJS += RapidIot_Base/drivers/fsl_uart_edma.o
C_DEPS += RapidIot_Base/drivers/fsl_uart_edma.d

C_SRCS += RapidIot_Base/drivers/fsl_uart_freertos.c
OBJS += RapidIot_Base/drivers/fsl_uart_freertos.o
C_DEPS += RapidIot_Base/drivers/fsl_uart_freertos.d

C_SRCS += RapidIot_Base/drivers/fsl_vref.c
OBJS += RapidIot_Base/drivers/fsl_vref.o
C_DEPS += RapidIot_Base/drivers/fsl_vref.d

C_SRCS += RapidIot_Base/drivers/fsl_wdog.c
OBJS += RapidIot_Base/drivers/fsl_wdog.o
C_DEPS += RapidIot_Base/drivers/fsl_wdog.d

C_SRCS += RapidIot_Base/middleware/authentication/a100x/a100x_interface.c
OBJS += RapidIot_Base/middleware/authentication/a100x/a100x_interface.o
C_DEPS += RapidIot_Base/middleware/authentication/a100x/a100x_interface.d

C_SRCS += RapidIot_Base/middleware/authentication/base64/rpk_base64.c
OBJS += RapidIot_Base/middleware/authentication/base64/rpk_base64.o
C_DEPS += RapidIot_Base/middleware/authentication/base64/rpk_base64.d

C_SRCS += RapidIot_Base/middleware/usb/device/usb_device_dci.c
OBJS += RapidIot_Base/middleware/usb/device/usb_device_dci.o
C_DEPS += RapidIot_Base/middleware/usb/device/usb_device_dci.d

C_SRCS += RapidIot_Base/middleware/usb/device/usb_device_khci.c
OBJS += RapidIot_Base/middleware/usb/device/usb_device_khci.o
C_DEPS += RapidIot_Base/middleware/usb/device/usb_device_khci.d

C_SRCS += RapidIot_Base/middleware/usb/osa/usb_osa_freertos.c
OBJS += RapidIot_Base/middleware/usb/osa/usb_osa_freertos.o
C_DEPS += RapidIot_Base/middleware/usb/osa/usb_osa_freertos.d

C_SRCS += RapidIot_Base/middleware/wireless/bluetooth/ble_shell/ble_otap_client.c
OBJS += RapidIot_Base/middleware/wireless/bluetooth/ble_shell/ble_otap_client.o
C_DEPS += RapidIot_Base/middleware/wireless/bluetooth/ble_shell/ble_otap_client.d

C_SRCS += RapidIot_Base/middleware/wireless/bluetooth/ble_shell/otap_service.c
OBJS += RapidIot_Base/middleware/wireless/bluetooth/ble_shell/otap_service.o
C_DEPS += RapidIot_Base/middleware/wireless/bluetooth/ble_shell/otap_service.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/Common/FwkInit.c
OBJS += RapidIot_Base/middleware/wireless/framework/Common/FwkInit.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/Common/FwkInit.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/FSCI/Source/FsciCommands.c
OBJS += RapidIot_Base/middleware/wireless/framework/FSCI/Source/FsciCommands.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/FSCI/Source/FsciCommands.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/FSCI/Source/FsciCommunication.c
OBJS += RapidIot_Base/middleware/wireless/framework/FSCI/Source/FsciCommunication.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/FSCI/Source/FsciCommunication.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/FSCI/Source/FsciLogging.c
OBJS += RapidIot_Base/middleware/wireless/framework/FSCI/Source/FsciLogging.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/FSCI/Source/FsciLogging.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/FSCI/Source/FsciMain.c
OBJS += RapidIot_Base/middleware/wireless/framework/FSCI/Source/FsciMain.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/FSCI/Source/FsciMain.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/FunctionLib/FunctionLib.c
OBJS += RapidIot_Base/middleware/wireless/framework/FunctionLib/FunctionLib.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/FunctionLib/FunctionLib.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/GPIO/GPIO_Adapter.c
OBJS += RapidIot_Base/middleware/wireless/framework/GPIO/GPIO_Adapter.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/GPIO/GPIO_Adapter.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/Keyboard/Source/Keyboard.c
OBJS += RapidIot_Base/middleware/wireless/framework/Keyboard/Source/Keyboard.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/Keyboard/Source/Keyboard.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/LED/Source/LED.c
OBJS += RapidIot_Base/middleware/wireless/framework/LED/Source/LED.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/LED/Source/LED.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/Lists/GenericList.c
OBJS += RapidIot_Base/middleware/wireless/framework/Lists/GenericList.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/Lists/GenericList.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/MemManager/Source/MemManager.c
OBJS += RapidIot_Base/middleware/wireless/framework/MemManager/Source/MemManager.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/MemManager/Source/MemManager.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/Messaging/Source/Messaging.c
OBJS += RapidIot_Base/middleware/wireless/framework/Messaging/Source/Messaging.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/Messaging/Source/Messaging.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/OSAbstraction/Source/fsl_os_abstraction_free_rtos.c
OBJS += RapidIot_Base/middleware/wireless/framework/OSAbstraction/Source/fsl_os_abstraction_free_rtos.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/OSAbstraction/Source/fsl_os_abstraction_free_rtos.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/Panic/Source/Panic.c
OBJS += RapidIot_Base/middleware/wireless/framework/Panic/Source/Panic.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/Panic/Source/Panic.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/Reset/Reset.c
OBJS += RapidIot_Base/middleware/wireless/framework/Reset/Reset.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/Reset/Reset.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/SPI_Adapter/SPI_Adapter.c
OBJS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/SPI_Adapter/SPI_Adapter.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/SPI_Adapter/SPI_Adapter.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/SerialManager.c
OBJS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/SerialManager.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/SerialManager.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/UART_Adapter.c
OBJS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/UART_Adapter.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/UART_Adapter.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/usb_device_cdc_acm.c
OBJS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/usb_device_cdc_acm.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/usb_device_cdc_acm.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/usb_device_ch9.c
OBJS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/usb_device_ch9.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/usb_device_ch9.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/usb_device_class.c
OBJS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/usb_device_class.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/usb_device_class.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/usb_device_descriptor.c
OBJS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/usb_device_descriptor.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/usb_device_descriptor.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/virtual_com.c
OBJS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/virtual_com.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/virtual_com.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/Shell/Source/shell.c
OBJS += RapidIot_Base/middleware/wireless/framework/Shell/Source/shell.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/Shell/Source/shell.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/Shell/Source/shell_autocomplete.c
OBJS += RapidIot_Base/middleware/wireless/framework/Shell/Source/shell_autocomplete.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/Shell/Source/shell_autocomplete.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/Shell/Source/shell_cmdhelp.c
OBJS += RapidIot_Base/middleware/wireless/framework/Shell/Source/shell_cmdhelp.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/Shell/Source/shell_cmdhelp.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/Shell/Source/shell_cmdhist.c
OBJS += RapidIot_Base/middleware/wireless/framework/Shell/Source/shell_cmdhist.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/Shell/Source/shell_cmdhist.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/TimersManager/Source/TMR_Adapter.c
OBJS += RapidIot_Base/middleware/wireless/framework/TimersManager/Source/TMR_Adapter.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/TimersManager/Source/TMR_Adapter.d

C_SRCS += RapidIot_Base/middleware/wireless/framework/TimersManager/Source/TimersManager.c
OBJS += RapidIot_Base/middleware/wireless/framework/TimersManager/Source/TimersManager.o
C_DEPS += RapidIot_Base/middleware/wireless/framework/TimersManager/Source/TimersManager.d

C_SRCS += RapidIot_Base/middleware/wireless/nwk_ip/base/thci/thci.c
OBJS += RapidIot_Base/middleware/wireless/nwk_ip/base/thci/thci.o
C_DEPS += RapidIot_Base/middleware/wireless/nwk_ip/base/thci/thci.d

C_SRCS += RapidIot_Base/middleware/wireless/nwk_ip/base/thread_config/thread_config.c
OBJS += RapidIot_Base/middleware/wireless/nwk_ip/base/thread_config/thread_config.o
C_DEPS += RapidIot_Base/middleware/wireless/nwk_ip/base/thread_config/thread_config.d

C_SRCS += RapidIot_Base/middleware/wireless/nwk_ip/base/utils/debug_log.c
OBJS += RapidIot_Base/middleware/wireless/nwk_ip/base/utils/debug_log.o
C_DEPS += RapidIot_Base/middleware/wireless/nwk_ip/base/utils/debug_log.d

C_SRCS += RapidIot_Base/middleware/wireless/nwk_ip/base/utils/mac_filtering.c
OBJS += RapidIot_Base/middleware/wireless/nwk_ip/base/utils/mac_filtering.o
C_DEPS += RapidIot_Base/middleware/wireless/nwk_ip/base/utils/mac_filtering.d

C_SRCS += RapidIot_Base/middleware/wireless/nwk_ip/core/util/event_manager.c
OBJS += RapidIot_Base/middleware/wireless/nwk_ip/core/util/event_manager.o
C_DEPS += RapidIot_Base/middleware/wireless/nwk_ip/core/util/event_manager.d

C_SRCS += RapidIot_Base/middleware/wireless/nwk_ip/core/util/network_utils.c
OBJS += RapidIot_Base/middleware/wireless/nwk_ip/core/util/network_utils.o
C_DEPS += RapidIot_Base/middleware/wireless/nwk_ip/core/util/network_utils.d

C_SRCS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_coap_observe.c
OBJS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_coap_observe.o
C_DEPS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_coap_observe.d

C_SRCS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_echo_udp.c
OBJS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_echo_udp.o
C_DEPS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_echo_udp.d

C_SRCS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_ethernet.c
OBJS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_ethernet.o
C_DEPS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_ethernet.d

C_SRCS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_ethernet_usb.c
OBJS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_ethernet_usb.o
C_DEPS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_ethernet_usb.d

C_SRCS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_event_monitoring.c
OBJS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_event_monitoring.o
C_DEPS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_event_monitoring.d

C_SRCS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_led.c
OBJS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_led.o
C_DEPS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_led.d

C_SRCS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_observe_demo.c
OBJS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_observe_demo.o
C_DEPS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_observe_demo.d

C_SRCS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_serial_tun.c
OBJS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_serial_tun.o
C_DEPS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_serial_tun.d

C_SRCS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_socket_utils.c
OBJS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_socket_utils.o
C_DEPS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_socket_utils.d

C_SRCS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_thread_callbacks.c
OBJS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_thread_callbacks.o
C_DEPS += RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_thread_callbacks.d

C_SRCS += RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/src/cmd_ble.c
OBJS += RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/src/cmd_ble.o
C_DEPS += RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/src/cmd_ble.d

C_SRCS += RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/src/cmd_threadip.c
OBJS += RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/src/cmd_threadip.o
C_DEPS += RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/src/cmd_threadip.d

C_SRCS += RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/src/evt_ble.c
OBJS += RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/src/evt_ble.o
C_DEPS += RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/src/evt_ble.d

C_SRCS += RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/src/evt_threadip.c
OBJS += RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/src/evt_threadip.o
C_DEPS += RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/src/evt_threadip.d

C_SRCS += RapidIot_Base/rtos/freertos/Source/croutine.c
OBJS += RapidIot_Base/rtos/freertos/Source/croutine.o
C_DEPS += RapidIot_Base/rtos/freertos/Source/croutine.d

C_SRCS += RapidIot_Base/rtos/freertos/Source/event_groups.c
OBJS += RapidIot_Base/rtos/freertos/Source/event_groups.o
C_DEPS += RapidIot_Base/rtos/freertos/Source/event_groups.d

C_SRCS += RapidIot_Base/rtos/freertos/Source/list.c
OBJS += RapidIot_Base/rtos/freertos/Source/list.o
C_DEPS += RapidIot_Base/rtos/freertos/Source/list.d

C_SRCS += RapidIot_Base/rtos/freertos/Source/portable/GCC/ARM_CM4F/port.c
OBJS += RapidIot_Base/rtos/freertos/Source/portable/GCC/ARM_CM4F/port.o
C_DEPS += RapidIot_Base/rtos/freertos/Source/portable/GCC/ARM_CM4F/port.d

C_SRCS += RapidIot_Base/rtos/freertos/Source/portable/MemMang/heap_4.c
OBJS += RapidIot_Base/rtos/freertos/Source/portable/MemMang/heap_4.o
C_DEPS += RapidIot_Base/rtos/freertos/Source/portable/MemMang/heap_4.d

C_SRCS += RapidIot_Base/rtos/freertos/Source/queue.c
OBJS += RapidIot_Base/rtos/freertos/Source/queue.o
C_DEPS += RapidIot_Base/rtos/freertos/Source/queue.d

C_SRCS += RapidIot_Base/rtos/freertos/Source/tasks.c
OBJS += RapidIot_Base/rtos/freertos/Source/tasks.o
C_DEPS += RapidIot_Base/rtos/freertos/Source/tasks.d

C_SRCS += RapidIot_Base/rtos/freertos/Source/timers.c
OBJS += RapidIot_Base/rtos/freertos/Source/timers.o
C_DEPS += RapidIot_Base/rtos/freertos/Source/timers.d

C_SRCS += RapidIot_Base/smc/smc.c
OBJS += RapidIot_Base/smc/smc.o
C_DEPS += RapidIot_Base/smc/smc.d

C_SRCS += RapidIot_Base/spi/spi_bus_share.c
OBJS += RapidIot_Base/spi/spi_bus_share.o
C_DEPS += RapidIot_Base/spi/spi_bus_share.d

C_SRCS += RapidIot_Base/spi/spi_flash_driver.c
OBJS += RapidIot_Base/spi/spi_flash_driver.o
C_DEPS += RapidIot_Base/spi/spi_flash_driver.d

C_SRCS += adc/adc.c
OBJS += adc/adc.o
C_DEPS += adc/adc.d

C_SRCS += adc/adc_mk64f.c
OBJS += adc/adc_mk64f.o
C_DEPS += adc/adc_mk64f.d

C_SRCS += app_src/app_init.c
OBJS += app_src/app_init.o
C_DEPS += app_src/app_init.d

C_SRCS += app_src/atmosphere_abilityHandler.c
OBJS += app_src/atmosphere_abilityHandler.o
C_DEPS += app_src/atmosphere_abilityHandler.d

C_SRCS += app_src/atmosphere_callbacks.c
OBJS += app_src/atmosphere_callbacks.o
C_DEPS += app_src/atmosphere_callbacks.d

C_SRCS += app_src/atmosphere_elementSetup.c
OBJS += app_src/atmosphere_elementSetup.o
C_DEPS += app_src/atmosphere_elementSetup.d

C_SRCS += app_src/atmosphere_interruptsHandler.c
OBJS += app_src/atmosphere_interruptsHandler.o
C_DEPS += app_src/atmosphere_interruptsHandler.d

C_SRCS += app_src/atmosphere_platform.c
OBJS += app_src/atmosphere_platform.o
C_DEPS += app_src/atmosphere_platform.d

C_SRCS += app_src/atmosphere_triggerHandler.c
OBJS += app_src/atmosphere_triggerHandler.o
C_DEPS += app_src/atmosphere_triggerHandler.d

C_SRCS += app_src/atmosphere_variantSetup.c
OBJS += app_src/atmosphere_variantSetup.o
C_DEPS += app_src/atmosphere_variantSetup.d

C_SRCS += atmo/core.c
OBJS += atmo/core.o
C_DEPS += atmo/core.d

C_SRCS += auth/auth_mk64f.c
OBJS += auth/auth_mk64f.o
C_DEPS += auth/auth_mk64f.d

C_SRCS += battery/battery_mk64f.c
OBJS += battery/battery_mk64f.o
C_DEPS += battery/battery_mk64f.d

C_SRCS += ble/ble.c
OBJS += ble/ble.o
C_DEPS += ble/ble.d

C_SRCS += ble/ble_db_mk64f.c
OBJS += ble/ble_db_mk64f.o
C_DEPS += ble/ble_db_mk64f.d

C_SRCS += ble/ble_mk64f.c
OBJS += ble/ble_mk64f.o
C_DEPS += ble/ble_mk64f.d

C_SRCS += ble/ble_mk64f_helpers.c
OBJS += ble/ble_mk64f_helpers.o
C_DEPS += ble/ble_mk64f_helpers.d

C_SRCS += block/block.c
OBJS += block/block.o
C_DEPS += block/block.d

C_SRCS += block/block_mk64f.c
OBJS += block/block_mk64f.o
C_DEPS += block/block_mk64f.d

C_SRCS += board/startup_MK64F12.S
OBJS += board/startup_MK64F12.o
C_DEPS += board/startup_MK64F12.d

C_SRCS += buzzer/buzzer.c
OBJS += buzzer/buzzer.o
C_DEPS += buzzer/buzzer.d

C_SRCS += ccs811/ccs811.c
OBJS += ccs811/ccs811.o
C_DEPS += ccs811/ccs811.d

C_SRCS += ccs811/ccs811_internal.c
OBJS += ccs811/ccs811_internal.o
C_DEPS += ccs811/ccs811_internal.d

C_SRCS += cloud/cloud.c
OBJS += cloud/cloud.o
C_DEPS += cloud/cloud.d

C_SRCS += cloud/cloud_ble.c
OBJS += cloud/cloud_ble.o
C_DEPS += cloud/cloud_ble.d

C_SRCS += cloud/cloud_http.c
OBJS += cloud/cloud_http.o
C_DEPS += cloud/cloud_http.d

C_SRCS += datetime/datetime.c
OBJS += datetime/datetime.o
C_DEPS += datetime/datetime.d

C_SRCS += datetime/datetime_mk64f.c
OBJS += datetime/datetime_mk64f.o
C_DEPS += datetime/datetime_mk64f.d

C_SRCS += datetime/pcf2123.c
OBJS += datetime/pcf2123.o
C_DEPS += datetime/pcf2123.d

C_SRCS += ds1307/ds1307.c
OBJS += ds1307/ds1307.o
C_DEPS += ds1307/ds1307.d

C_SRCS += emWin/emwin_support.c
OBJS += emWin/emwin_support.o
C_DEPS += emWin/emwin_support.d

C_SRCS += ens210/ens210.c
OBJS += ens210/ens210.o
C_DEPS += ens210/ens210.d

C_SRCS += ens210/ens210_internal.c
OBJS += ens210/ens210_internal.o
C_DEPS += ens210/ens210_internal.d

C_SRCS += filesystem/filesystem.c
OBJS += filesystem/filesystem.o
C_DEPS += filesystem/filesystem.d

C_SRCS += filesystem/filesystem_crastfs.c
OBJS += filesystem/filesystem_crastfs.o
C_DEPS += filesystem/filesystem_crastfs.d

C_SRCS += filesystem/filesystem_lfs.c
OBJS += filesystem/filesystem_lfs.o
C_DEPS += filesystem/filesystem_lfs.d

C_SRCS += filesystem/lfs.c
OBJS += filesystem/lfs.o
C_DEPS += filesystem/lfs.d

C_SRCS += filesystem/lfs_util.c
OBJS += filesystem/lfs_util.o
C_DEPS += filesystem/lfs_util.d

C_SRCS += fxas21002/fxas21002.c
OBJS += fxas21002/fxas21002.o
C_DEPS += fxas21002/fxas21002.d

C_SRCS += fxos8700/fxos8700.c
OBJS += fxos8700/fxos8700.o
C_DEPS += fxos8700/fxos8700.d

C_SRCS += fxos8700/fxos8700_internal.c
OBJS += fxos8700/fxos8700_internal.o
C_DEPS += fxos8700/fxos8700_internal.d

C_SRCS += gpio/gpio.c
OBJS += gpio/gpio.o
C_DEPS += gpio/gpio.d

C_SRCS += gpio/gpio_mk64f.c
OBJS += gpio/gpio_mk64f.o
C_DEPS += gpio/gpio_mk64f.d

C_SRCS += http/http.c
OBJS += http/http.o
C_DEPS += http/http.d

C_SRCS += i2c/i2c.c
OBJS += i2c/i2c.o
C_DEPS += i2c/i2c.d

C_SRCS += i2c/i2c_mk64f.c
OBJS += i2c/i2c_mk64f.o
C_DEPS += i2c/i2c_mk64f.d

C_SRCS += interval/interval.c
OBJS += interval/interval.o
C_DEPS += interval/interval.d

C_SRCS += interval/interval_mk64f.c
OBJS += interval/interval_mk64f.o
C_DEPS += interval/interval_mk64f.d

C_SRCS += mpl3115/mpl3115.c
OBJS += mpl3115/mpl3115.o
C_DEPS += mpl3115/mpl3115.d

C_SRCS += mpl3115/mpl3115_internal.c
OBJS += mpl3115/mpl3115_internal.o
C_DEPS += mpl3115/mpl3115_internal.d

C_SRCS += nfc/i2c_kinetis_fsl.c
OBJS += nfc/i2c_kinetis_fsl.o
C_DEPS += nfc/i2c_kinetis_fsl.d

C_SRCS += nfc/isr_common.c
OBJS += nfc/isr_common.o
C_DEPS += nfc/isr_common.d

C_SRCS += nfc/jsmn.c
OBJS += nfc/jsmn.o
C_DEPS += nfc/jsmn.d

C_SRCS += nfc/nfc.c
OBJS += nfc/nfc.o
C_DEPS += nfc/nfc.d

C_SRCS += nfc/nfc_mk64f.c
OBJS += nfc/nfc_mk64f.o
C_DEPS += nfc/nfc_mk64f.d

C_SRCS += nfc/ntag_bridge.c
OBJS += nfc/ntag_bridge.o
C_DEPS += nfc/ntag_bridge.d

C_SRCS += nfc/ntag_driver.c
OBJS += nfc/ntag_driver.o
C_DEPS += nfc/ntag_driver.d

C_SRCS += nfc/timer_kinetis.c
OBJS += nfc/timer_kinetis.o
C_DEPS += nfc/timer_kinetis.d

C_SRCS += pwm/pwm.c
OBJS += pwm/pwm.o
C_DEPS += pwm/pwm.d

C_SRCS += pwm/pwm_mk64f.c
OBJS += pwm/pwm_mk64f.o
C_DEPS += pwm/pwm_mk64f.d

C_SRCS += ringbuffer/atmosphere_ringbuffer.c
OBJS += ringbuffer/atmosphere_ringbuffer.o
C_DEPS += ringbuffer/atmosphere_ringbuffer.d

C_SRCS += spi/spi.c
OBJS += spi/spi.o
C_DEPS += spi/spi.d

C_SRCS += spi/spi_mk64f.c
OBJS += spi/spi_mk64f.o
C_DEPS += spi/spi_mk64f.d

C_SRCS += sx9500/sx9500.c
OBJS += sx9500/sx9500.o
C_DEPS += sx9500/sx9500.d

C_SRCS += sx9500/sx9500_internal.c
OBJS += sx9500/sx9500_internal.o
C_DEPS += sx9500/sx9500_internal.d

C_SRCS += thread/thread_volansys.c
OBJS += thread/thread_volansys.o
C_DEPS += thread/thread_volansys.d

C_SRCS += tsl2572/tsl2572.c
OBJS += tsl2572/tsl2572.o
C_DEPS += tsl2572/tsl2572.d

C_SRCS += tsl2572/tsl2572_internal.c
OBJS += tsl2572/tsl2572_internal.o
C_DEPS += tsl2572/tsl2572_internal.d

C_SRCS += uart/regex.c
OBJS += uart/regex.o
C_DEPS += uart/regex.d

C_SRCS += uart/uart.c
OBJS += uart/uart.o
C_DEPS += uart/uart.d

C_SRCS += uart/uart_mk64f.c
OBJS += uart/uart_mk64f.o
C_DEPS += uart/uart_mk64f.d

C_SRCS += ui_template/AtmosphereIoTSplashBlack.c
OBJS += ui_template/AtmosphereIoTSplashBlack.o
C_DEPS += ui_template/AtmosphereIoTSplashBlack.d

C_SRCS += ui_template/ui_pagecontrol.c
OBJS += ui_template/ui_pagecontrol.o
C_DEPS += ui_template/ui_pagecontrol.d

C_SRCS += ui_template/ui_template_iconlines.c
OBJS += ui_template/ui_template_iconlines.o
C_DEPS += ui_template/ui_template_iconlines.d

C_SRCS += ui_template/ui_template_selecticon.c
OBJS += ui_template/ui_template_selecticon.o
C_DEPS += ui_template/ui_template_selecticon.d

C_SRCS += ui_template/ui_template_singleicontext.c
OBJS += ui_template/ui_template_singleicontext.o
C_DEPS += ui_template/ui_template_singleicontext.d

C_SRCS += ui_template/ui_template_statictext.c
OBJS += ui_template/ui_template_statictext.o
C_DEPS += ui_template/ui_template_statictext.d

C_SRCS += ui_template/ui_template_systemstatus.c
OBJS += ui_template/ui_template_systemstatus.o
C_DEPS += ui_template/ui_template_systemstatus.d

C_SRCS += user_buttons/user_buttons.c
OBJS += user_buttons/user_buttons.o
C_DEPS += user_buttons/user_buttons.d

C_SRCS += wifi/wifi.c
OBJS += wifi/wifi.o
C_DEPS += wifi/wifi.d

ATMO_INCLUDES = -I"RapidIot_Base/middleware/lwip/port" -I"RapidIot_Base/smc" -I"RapidIot_Base/microseconds" -I"RapidIot_Base/spi" -I"RapidIot_Base/microseconds" -I"RapidIot_Base/utilities" -I"RapidIot_Base/crc" -I"RapidIot_Base/app_update" -I"RapidIot_Base/middleware/authentication/a100x" -I"RapidIot_Base/middleware/authentication/base64" -I"RapidIot_Base/middleware/wireless/nwk_ip/core/interface" -I"RapidIot_Base/middleware/wireless/nwk_ip/core/util" -I"RapidIot_Base/middleware/wireless/nwk_ip/core/interface/thread" -I"RapidIot_Base/middleware/wireless/nwk_ip/core/interface/modules" -I"RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/inc" -I"RapidIot_Base/middleware/wireless/bluetooth/ble_shell" -I"RapidIot_Base/middleware/lwip/src/include" -I"RapidIot_Base/middleware/wireless/framework/Common" -I"RapidIot_Base/middleware/wireless/nwk_ip/examples/border_router/config" -I"RapidIot_Base/middleware/wireless/nwk_ip/examples/common" -I"RapidIot_Base/middleware/wireless/framework/Flash/Internal" -I"RapidIot_Base/middleware/usb/device" -I"RapidIot_Base/middleware/usb/include" -I"RapidIot_Base/middleware/usb/osa" -I"RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom" -I"RapidIot_Base/middleware/wireless/framework/FSCI/Interface" -I"RapidIot_Base/middleware/wireless/framework/FunctionLib" -I"RapidIot_Base/middleware/wireless/framework/GPIO" -I"RapidIot_Base/middleware/wireless/framework/Keyboard/Interface" -I"RapidIot_Base/middleware/wireless/framework/Keyboard/Source" -I"RapidIot_Base/middleware/wireless/framework/LED/Interface" -I"RapidIot_Base/middleware/wireless/framework/LED/Source" -I"RapidIot_Base/middleware/wireless/framework/Lists" -I"RapidIot_Base/middleware/wireless/framework/MemManager/Interface" -I"RapidIot_Base/middleware/wireless/framework/MemManager/Source" -I"RapidIot_Base/middleware/wireless/framework/Messaging/Interface" -I"RapidIot_Base/middleware/wireless/framework/Messaging/Source" -I"RapidIot_Base/middleware/wireless/framework/ModuleInfo" -I"RapidIot_Base/middleware/wireless/framework/OSAbstraction/Interface" -I"RapidIot_Base/middleware/wireless/framework/OSAbstraction/Source" -I"RapidIot_Base/middleware/wireless/framework/Panic/Interface" -I"RapidIot_Base/middleware/wireless/framework/Panic/Source" -I"RapidIot_Base/middleware/wireless/framework/Reset" -I"RapidIot_Base/middleware/wireless/framework/SerialManager/Interface" -I"RapidIot_Base/middleware/wireless/framework/SerialManager/Source" -I"RapidIot_Base/middleware/wireless/framework/Shell/Interface" -I"RapidIot_Base/middleware/wireless/framework/Shell/Source" -I"RapidIot_Base/middleware/wireless/framework/TimersManager/Interface" -I"RapidIot_Base/middleware/wireless/framework/TimersManager/Source" -I"RapidIot_Base/middleware/wireless/framework/Common/rtos/FreeRTOS/config" -I"RapidIot_Base/devices/MK64F12" -I"RapidIot_Base/CMSIS_driver" -I"RapidIot_Base/board" -I"RapidIot_Base/rtos/freertos/Source/portable/GCC/ARM_CM4F" -I"RapidIot_Base/rtos/freertos/Source/include" -I"RapidIot_Base/drivers" -I"RapidIot_Base" -I"RapidIot_Base/CMSIS" -I"RapidIot_Base/devices/MK64F12" -I"RapidIot_Base/middleware/authentication/a100x" -I"RapidIot_Base/middleware/wireless/nwk_ip/base/interface" -I"RapidIot_Base/middleware/wireless/nwk_ip/core/interface/thread" -I"RapidIot_Base/middleware/lwip/port" -I"RapidIot_Base/spi" -I"RapidIot_Base/middleware/lwip/src/include" -I"RapidIot_Base/middleware/wireless/nwk_ip/core/interface/modules" -I"RapidIot_Base/middleware/wireless/nwk_ip/core/interface" -I"RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/inc" -I"RapidIot_Base/middleware/wireless/bluetooth/ble_shell" -I"RapidIot_Base/middleware/wireless/framework/TimersManager/Interface" -I"RapidIot_Base/middleware/wireless/framework/TimersManager/Source" -I"RapidIot_Base/middleware/wireless/framework/Shell/Interface" -I"RapidIot_Base/middleware/wireless/framework/Shell/Source" -I"RapidIot_Base/middleware/wireless/framework/SerialManager/Interface" -I"RapidIot_Base/middleware/wireless/framework/SerialManager/Source" -I"RapidIot_Base/middleware/wireless/framework/SerialManager/Source/SPI_Adapter" -I"RapidIot_Base/middleware/wireless/framework/Reset" -I"RapidIot_Base/middleware/wireless/framework/Panic/Interface" -I"RapidIot_Base/middleware/wireless/framework/Panic/Source" -I"RapidIot_Base/middleware/wireless/framework/OSAbstraction/Source" -I"RapidIot_Base/middleware/wireless/framework/ModuleInfo" -I"RapidIot_Base/middleware/wireless/framework/Messaging/Interface" -I"RapidIot_Base/middleware/wireless/framework/MemManager/Interface" -I"RapidIot_Base/middleware/wireless/framework/Lists" -I"RapidIot_Base/middleware/wireless/framework/LED/Interface" -I"RapidIot_Base/middleware/wireless/framework/Keyboard/Interface" -I"RapidIot_Base/middleware/wireless/framework/GPIO" -I"RapidIot_Base/middleware/wireless/framework/FunctionLib" -I"RapidIot_Base/middleware/wireless/framework/FSCI/Interface" -I"RapidIot_Base/middleware/wireless/framework/Common/rtos/FreeRTOS/config" -I"RapidIot_Base/middleware/wireless/framework/Common" -I"RapidIot_Base/middleware/wireless/framework/OSAbstraction/Interface" -I"RapidIot_Base/middleware/wireless/nwk_ip/examples/border_router/config" -I"RapidIot_Base/middleware/wireless/nwk_ip/examples/common" -I"RapidIot_Base/CMSIS" -I"emWin/Config" -I"emWin/GUI/Core" -I"emWin/GUI/Widget" -I"emWin/GUI/DisplayDriver" -I"emWin/GUI/WM" -I"RapidIot_Base/CMSIS_driver" -I"board" -I"RapidIot_Base/rtos/freertos/Source/include" -I"RapidIot_Base/rtos/freertos/Source/portable/GCC/ARM_CM4F" -I"RapidIot_Base/drivers" -I"RapidIot_Base/board" -I"adc" -I"app_src" -I"atmo" -I"battery" -I"ble" -I"block" -I"board" -I"buzzer" -I"ccs811" -I"cloud" -I"datetime" -I"emWin" -I"ens210" -I"filesystem" -I"fxas21002" -I"fxos8700" -I"gpio" -I"http" -I"i2c" -I"interval" -I"lib" -I"mpl3115" -I"pwm" -I"ringbuffer" -I"spi" -I"sx9500" -I"thread" -I"tsl2572" -I"uart" -I"ui_template" -I"user_buttons" -I"wifi" -I"." -I"pictures"
ATMO_DEFINES = -DRAPID_IOT -DI2C_FSL -DHAVE_STDINT_H -DHAVE_STDBOOL_H -DATMO_STATIC_CORE -DTHREAD_BORDER_ROUTER_CONFIG -DSDK_DEBUGCONSOLE=1 -D__TARGET_FPU_VFP -DDEBUG -DNDEBUG -DUSE_RTOS -DFSL_RTOS_FREE_RTOS -DENABLE_RAM_VECTOR_TABLE=1 -DFRDM_K64F_KW41Z=1 -DFREEDOM -D__MCUXPRESSO -D__USE_CMSIS -DSDK_OS_FREE_RTOS -DCPU_MK64FN1M0VMD12 -D__REDLIB__
ATMO_FLAGS = -O0 -fno-common -Wall -Wno-unused -Wno-missing-braces  -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -imacros "RapidIot_Base/middleware/wireless/nwk_ip/examples/border_router/config/config.h" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP

ATMO_DEFINES += -I"RapidIot_Base"
ATMO_DEFINES += -I"adc"
ATMO_DEFINES += -I"app_src"
ATMO_DEFINES += -I"atmo"
ATMO_DEFINES += -I"auth"
ATMO_DEFINES += -I"battery"
ATMO_DEFINES += -I"ble"
ATMO_DEFINES += -I"block"
ATMO_DEFINES += -I"board"
ATMO_DEFINES += -I"boards"
ATMO_DEFINES += -I"buzzer"
ATMO_DEFINES += -I"ccs811"
ATMO_DEFINES += -I"cloud"
ATMO_DEFINES += -I"datetime"
ATMO_DEFINES += -I"ds1307"
ATMO_DEFINES += -I"emWin"
ATMO_DEFINES += -I"ens210"
ATMO_DEFINES += -I"filesystem"
ATMO_DEFINES += -I"fxas21002"
ATMO_DEFINES += -I"fxos8700"
ATMO_DEFINES += -I"gpio"
ATMO_DEFINES += -I"http"
ATMO_DEFINES += -I"i2c"
ATMO_DEFINES += -I"interval"
ATMO_DEFINES += -I"lib"
ATMO_DEFINES += -I"makefile"
ATMO_DEFINES += -I"mpl3115"
ATMO_DEFINES += -I"nfc"
ATMO_DEFINES += -I"pwm"
ATMO_DEFINES += -I"ringbuffer"
ATMO_DEFINES += -I"spi"
ATMO_DEFINES += -I"sx9500"
ATMO_DEFINES += -I"thread"
ATMO_DEFINES += -I"tsl2572"
ATMO_DEFINES += -I"uart"
ATMO_DEFINES += -I"ui_template"
ATMO_DEFINES += -I"user_buttons"
ATMO_DEFINES += -I"wifi"
RapidIot_Base/CMSIS_driver/fsl_dspi_cmsis.o: RapidIot_Base/CMSIS_driver/fsl_dspi_cmsis.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/CMSIS_driver/fsl_i2c_cmsis.o: RapidIot_Base/CMSIS_driver/fsl_i2c_cmsis.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/app_update/app_program_ext.o: RapidIot_Base/app_update/app_program_ext.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/app_update/flash_ica_driver.o: RapidIot_Base/app_update/flash_ica_driver.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/app_update/img_program_ext.o: RapidIot_Base/app_update/img_program_ext.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/app_update/rpk_led.o: RapidIot_Base/app_update/rpk_led.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/board/backlight.o: RapidIot_Base/board/backlight.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/board/battery.o: RapidIot_Base/board/battery.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/board/board.o: RapidIot_Base/board/board.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/board/clock_config.o: RapidIot_Base/board/clock_config.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/board/hardware_init_MK64F12.o: RapidIot_Base/board/hardware_init_MK64F12.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/board/peripherals.o: RapidIot_Base/board/peripherals.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/board/pin_mux.o: RapidIot_Base/board/pin_mux.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/board/pin_mux_rpk.o: RapidIot_Base/board/pin_mux_rpk.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/board/rgb_led.o: RapidIot_Base/board/rgb_led.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/crc/src/crc16.o: RapidIot_Base/crc/src/crc16.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/crc/src/crc32.o: RapidIot_Base/crc/src/crc32.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/devices/MK64F12/system_MK64F12.o: RapidIot_Base/devices/MK64F12/system_MK64F12.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_adc16.o: RapidIot_Base/drivers/fsl_adc16.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_clock.o: RapidIot_Base/drivers/fsl_clock.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_cmp.o: RapidIot_Base/drivers/fsl_cmp.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_cmt.o: RapidIot_Base/drivers/fsl_cmt.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_common.o: RapidIot_Base/drivers/fsl_common.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_crc.o: RapidIot_Base/drivers/fsl_crc.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_dac.o: RapidIot_Base/drivers/fsl_dac.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_dmamux.o: RapidIot_Base/drivers/fsl_dmamux.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_dspi.o: RapidIot_Base/drivers/fsl_dspi.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_dspi_edma.o: RapidIot_Base/drivers/fsl_dspi_edma.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_dspi_freertos.o: RapidIot_Base/drivers/fsl_dspi_freertos.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_edma.o: RapidIot_Base/drivers/fsl_edma.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_enet.o: RapidIot_Base/drivers/fsl_enet.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_ewm.o: RapidIot_Base/drivers/fsl_ewm.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_flash.o: RapidIot_Base/drivers/fsl_flash.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_flexbus.o: RapidIot_Base/drivers/fsl_flexbus.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_flexcan.o: RapidIot_Base/drivers/fsl_flexcan.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_ftm.o: RapidIot_Base/drivers/fsl_ftm.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_gpio.o: RapidIot_Base/drivers/fsl_gpio.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_i2c.o: RapidIot_Base/drivers/fsl_i2c.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_i2c_edma.o: RapidIot_Base/drivers/fsl_i2c_edma.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_i2c_freertos.o: RapidIot_Base/drivers/fsl_i2c_freertos.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_ili9341.o: RapidIot_Base/drivers/fsl_ili9341.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_llwu.o: RapidIot_Base/drivers/fsl_llwu.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_lptmr.o: RapidIot_Base/drivers/fsl_lptmr.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_pdb.o: RapidIot_Base/drivers/fsl_pdb.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_phy.o: RapidIot_Base/drivers/fsl_phy.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_pit.o: RapidIot_Base/drivers/fsl_pit.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_pmc.o: RapidIot_Base/drivers/fsl_pmc.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_rcm.o: RapidIot_Base/drivers/fsl_rcm.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_rnga.o: RapidIot_Base/drivers/fsl_rnga.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_rtc.o: RapidIot_Base/drivers/fsl_rtc.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_sai.o: RapidIot_Base/drivers/fsl_sai.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_sai_edma.o: RapidIot_Base/drivers/fsl_sai_edma.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_sdhc.o: RapidIot_Base/drivers/fsl_sdhc.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_sim.o: RapidIot_Base/drivers/fsl_sim.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_smc.o: RapidIot_Base/drivers/fsl_smc.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_sysmpu.o: RapidIot_Base/drivers/fsl_sysmpu.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_uart.o: RapidIot_Base/drivers/fsl_uart.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_uart_edma.o: RapidIot_Base/drivers/fsl_uart_edma.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_uart_freertos.o: RapidIot_Base/drivers/fsl_uart_freertos.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_vref.o: RapidIot_Base/drivers/fsl_vref.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/drivers/fsl_wdog.o: RapidIot_Base/drivers/fsl_wdog.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/authentication/a100x/a100x_interface.o: RapidIot_Base/middleware/authentication/a100x/a100x_interface.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/authentication/base64/rpk_base64.o: RapidIot_Base/middleware/authentication/base64/rpk_base64.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/usb/device/usb_device_dci.o: RapidIot_Base/middleware/usb/device/usb_device_dci.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/usb/device/usb_device_khci.o: RapidIot_Base/middleware/usb/device/usb_device_khci.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/usb/osa/usb_osa_freertos.o: RapidIot_Base/middleware/usb/osa/usb_osa_freertos.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/bluetooth/ble_shell/ble_otap_client.o: RapidIot_Base/middleware/wireless/bluetooth/ble_shell/ble_otap_client.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/bluetooth/ble_shell/otap_service.o: RapidIot_Base/middleware/wireless/bluetooth/ble_shell/otap_service.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/Common/FwkInit.o: RapidIot_Base/middleware/wireless/framework/Common/FwkInit.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/FSCI/Source/FsciCommands.o: RapidIot_Base/middleware/wireless/framework/FSCI/Source/FsciCommands.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/FSCI/Source/FsciCommunication.o: RapidIot_Base/middleware/wireless/framework/FSCI/Source/FsciCommunication.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/FSCI/Source/FsciLogging.o: RapidIot_Base/middleware/wireless/framework/FSCI/Source/FsciLogging.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/FSCI/Source/FsciMain.o: RapidIot_Base/middleware/wireless/framework/FSCI/Source/FsciMain.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/FunctionLib/FunctionLib.o: RapidIot_Base/middleware/wireless/framework/FunctionLib/FunctionLib.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/GPIO/GPIO_Adapter.o: RapidIot_Base/middleware/wireless/framework/GPIO/GPIO_Adapter.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/Keyboard/Source/Keyboard.o: RapidIot_Base/middleware/wireless/framework/Keyboard/Source/Keyboard.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/LED/Source/LED.o: RapidIot_Base/middleware/wireless/framework/LED/Source/LED.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/Lists/GenericList.o: RapidIot_Base/middleware/wireless/framework/Lists/GenericList.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/MemManager/Source/MemManager.o: RapidIot_Base/middleware/wireless/framework/MemManager/Source/MemManager.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/Messaging/Source/Messaging.o: RapidIot_Base/middleware/wireless/framework/Messaging/Source/Messaging.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/OSAbstraction/Source/fsl_os_abstraction_free_rtos.o: RapidIot_Base/middleware/wireless/framework/OSAbstraction/Source/fsl_os_abstraction_free_rtos.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/Panic/Source/Panic.o: RapidIot_Base/middleware/wireless/framework/Panic/Source/Panic.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/Reset/Reset.o: RapidIot_Base/middleware/wireless/framework/Reset/Reset.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/SerialManager/Source/SPI_Adapter/SPI_Adapter.o: RapidIot_Base/middleware/wireless/framework/SerialManager/Source/SPI_Adapter/SPI_Adapter.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/SerialManager/Source/SerialManager.o: RapidIot_Base/middleware/wireless/framework/SerialManager/Source/SerialManager.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/SerialManager/Source/UART_Adapter.o: RapidIot_Base/middleware/wireless/framework/SerialManager/Source/UART_Adapter.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/usb_device_cdc_acm.o: RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/usb_device_cdc_acm.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/usb_device_ch9.o: RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/usb_device_ch9.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/usb_device_class.o: RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/usb_device_class.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/usb_device_descriptor.o: RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/usb_device_descriptor.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/virtual_com.o: RapidIot_Base/middleware/wireless/framework/SerialManager/Source/USB_VirtualCom/virtual_com.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/Shell/Source/shell.o: RapidIot_Base/middleware/wireless/framework/Shell/Source/shell.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/Shell/Source/shell_autocomplete.o: RapidIot_Base/middleware/wireless/framework/Shell/Source/shell_autocomplete.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/Shell/Source/shell_cmdhelp.o: RapidIot_Base/middleware/wireless/framework/Shell/Source/shell_cmdhelp.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/Shell/Source/shell_cmdhist.o: RapidIot_Base/middleware/wireless/framework/Shell/Source/shell_cmdhist.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/TimersManager/Source/TMR_Adapter.o: RapidIot_Base/middleware/wireless/framework/TimersManager/Source/TMR_Adapter.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/framework/TimersManager/Source/TimersManager.o: RapidIot_Base/middleware/wireless/framework/TimersManager/Source/TimersManager.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/nwk_ip/base/thci/thci.o: RapidIot_Base/middleware/wireless/nwk_ip/base/thci/thci.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/nwk_ip/base/thread_config/thread_config.o: RapidIot_Base/middleware/wireless/nwk_ip/base/thread_config/thread_config.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/nwk_ip/base/utils/debug_log.o: RapidIot_Base/middleware/wireless/nwk_ip/base/utils/debug_log.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/nwk_ip/base/utils/mac_filtering.o: RapidIot_Base/middleware/wireless/nwk_ip/base/utils/mac_filtering.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/nwk_ip/core/util/event_manager.o: RapidIot_Base/middleware/wireless/nwk_ip/core/util/event_manager.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/nwk_ip/core/util/network_utils.o: RapidIot_Base/middleware/wireless/nwk_ip/core/util/network_utils.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_coap_observe.o: RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_coap_observe.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_echo_udp.o: RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_echo_udp.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_ethernet.o: RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_ethernet.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_ethernet_usb.o: RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_ethernet_usb.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_event_monitoring.o: RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_event_monitoring.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_led.o: RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_led.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_observe_demo.o: RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_observe_demo.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_serial_tun.o: RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_serial_tun.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_socket_utils.o: RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_socket_utils.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_thread_callbacks.o: RapidIot_Base/middleware/wireless/nwk_ip/examples/common/app_thread_callbacks.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/src/cmd_ble.o: RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/src/cmd_ble.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/src/cmd_threadip.o: RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/src/cmd_threadip.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/src/evt_ble.o: RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/src/evt_ble.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/src/evt_threadip.o: RapidIot_Base/middleware/wireless/nwk_ip/examples/kinetis_host/src/evt_threadip.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/rtos/freertos/Source/croutine.o: RapidIot_Base/rtos/freertos/Source/croutine.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/rtos/freertos/Source/event_groups.o: RapidIot_Base/rtos/freertos/Source/event_groups.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/rtos/freertos/Source/list.o: RapidIot_Base/rtos/freertos/Source/list.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/rtos/freertos/Source/portable/GCC/ARM_CM4F/port.o: RapidIot_Base/rtos/freertos/Source/portable/GCC/ARM_CM4F/port.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/rtos/freertos/Source/portable/MemMang/heap_4.o: RapidIot_Base/rtos/freertos/Source/portable/MemMang/heap_4.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/rtos/freertos/Source/queue.o: RapidIot_Base/rtos/freertos/Source/queue.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/rtos/freertos/Source/tasks.o: RapidIot_Base/rtos/freertos/Source/tasks.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/rtos/freertos/Source/timers.o: RapidIot_Base/rtos/freertos/Source/timers.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/smc/smc.o: RapidIot_Base/smc/smc.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/spi/spi_bus_share.o: RapidIot_Base/spi/spi_bus_share.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
RapidIot_Base/spi/spi_flash_driver.o: RapidIot_Base/spi/spi_flash_driver.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
adc/adc.o: adc/adc.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
adc/adc_mk64f.o: adc/adc_mk64f.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
app_src/app_init.o: app_src/app_init.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
app_src/atmosphere_abilityHandler.o: app_src/atmosphere_abilityHandler.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
app_src/atmosphere_callbacks.o: app_src/atmosphere_callbacks.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
app_src/atmosphere_elementSetup.o: app_src/atmosphere_elementSetup.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
app_src/atmosphere_interruptsHandler.o: app_src/atmosphere_interruptsHandler.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
app_src/atmosphere_platform.o: app_src/atmosphere_platform.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
app_src/atmosphere_triggerHandler.o: app_src/atmosphere_triggerHandler.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
app_src/atmosphere_variantSetup.o: app_src/atmosphere_variantSetup.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
atmo/core.o: atmo/core.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
auth/auth_mk64f.o: auth/auth_mk64f.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
battery/battery_mk64f.o: battery/battery_mk64f.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
ble/ble.o: ble/ble.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
ble/ble_db_mk64f.o: ble/ble_db_mk64f.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
ble/ble_mk64f.o: ble/ble_mk64f.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
ble/ble_mk64f_helpers.o: ble/ble_mk64f_helpers.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
block/block.o: block/block.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
block/block_mk64f.o: block/block_mk64f.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
board/startup_MK64F12.o: board/startup_MK64F12.S 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -c -x assembler-with-cpp -std=gnu99 -D__REDLIB__ $(ATMO_INCLUDES) $(ATMO_FLAGS) -o "$@" "$<"
buzzer/buzzer.o: buzzer/buzzer.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
ccs811/ccs811.o: ccs811/ccs811.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
ccs811/ccs811_internal.o: ccs811/ccs811_internal.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
cloud/cloud.o: cloud/cloud.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
cloud/cloud_ble.o: cloud/cloud_ble.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
cloud/cloud_http.o: cloud/cloud_http.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
datetime/datetime.o: datetime/datetime.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
datetime/datetime_mk64f.o: datetime/datetime_mk64f.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
datetime/pcf2123.o: datetime/pcf2123.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
ds1307/ds1307.o: ds1307/ds1307.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
emWin/emwin_support.o: emWin/emwin_support.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
ens210/ens210.o: ens210/ens210.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
ens210/ens210_internal.o: ens210/ens210_internal.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
filesystem/filesystem.o: filesystem/filesystem.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
filesystem/filesystem_crastfs.o: filesystem/filesystem_crastfs.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
filesystem/filesystem_lfs.o: filesystem/filesystem_lfs.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
filesystem/lfs.o: filesystem/lfs.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
filesystem/lfs_util.o: filesystem/lfs_util.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
fxas21002/fxas21002.o: fxas21002/fxas21002.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
fxos8700/fxos8700.o: fxos8700/fxos8700.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
fxos8700/fxos8700_internal.o: fxos8700/fxos8700_internal.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
gpio/gpio.o: gpio/gpio.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
gpio/gpio_mk64f.o: gpio/gpio_mk64f.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
http/http.o: http/http.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
i2c/i2c.o: i2c/i2c.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
i2c/i2c_mk64f.o: i2c/i2c_mk64f.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
interval/interval.o: interval/interval.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
interval/interval_mk64f.o: interval/interval_mk64f.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
mpl3115/mpl3115.o: mpl3115/mpl3115.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
mpl3115/mpl3115_internal.o: mpl3115/mpl3115_internal.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
nfc/i2c_kinetis_fsl.o: nfc/i2c_kinetis_fsl.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
nfc/isr_common.o: nfc/isr_common.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
nfc/jsmn.o: nfc/jsmn.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
nfc/nfc.o: nfc/nfc.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
nfc/nfc_mk64f.o: nfc/nfc_mk64f.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
nfc/ntag_bridge.o: nfc/ntag_bridge.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
nfc/ntag_driver.o: nfc/ntag_driver.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
nfc/timer_kinetis.o: nfc/timer_kinetis.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
pwm/pwm.o: pwm/pwm.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
pwm/pwm_mk64f.o: pwm/pwm_mk64f.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
ringbuffer/atmosphere_ringbuffer.o: ringbuffer/atmosphere_ringbuffer.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
spi/spi.o: spi/spi.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
spi/spi_mk64f.o: spi/spi_mk64f.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
sx9500/sx9500.o: sx9500/sx9500.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
sx9500/sx9500_internal.o: sx9500/sx9500_internal.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
thread/thread_volansys.o: thread/thread_volansys.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
tsl2572/tsl2572.o: tsl2572/tsl2572.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
tsl2572/tsl2572_internal.o: tsl2572/tsl2572_internal.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
uart/regex.o: uart/regex.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
uart/uart.o: uart/uart.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
uart/uart_mk64f.o: uart/uart_mk64f.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
ui_template/AtmosphereIoTSplashBlack.o: ui_template/AtmosphereIoTSplashBlack.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
ui_template/ui_pagecontrol.o: ui_template/ui_pagecontrol.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
ui_template/ui_template_iconlines.o: ui_template/ui_template_iconlines.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
ui_template/ui_template_selecticon.o: ui_template/ui_template_selecticon.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
ui_template/ui_template_singleicontext.o: ui_template/ui_template_singleicontext.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
ui_template/ui_template_statictext.o: ui_template/ui_template_statictext.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
ui_template/ui_template_systemstatus.o: ui_template/ui_template_systemstatus.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
user_buttons/user_buttons.o: user_buttons/user_buttons.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
wifi/wifi.o: wifi/wifi.c 
	@echo 'Building file: $<'
	ccache arm-none-eabi-gcc -std=gnu99 $(ATMO_DEFINES) $(ATMO_INCLUDES) $(ATMO_FLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
