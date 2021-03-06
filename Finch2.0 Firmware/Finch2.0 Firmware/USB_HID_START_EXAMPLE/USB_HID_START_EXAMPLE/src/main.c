/**
 * \file
 *
 * \brief Main functions for Generic example
 *
 * Copyright (c) 2011-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <delay.h>
#include <system.h>
#include "conf_usb.h"
#include "ui.h"
#include "finch_adc.h"
#include "orb.h"
#include "sound.h"
#include "ir.h"
#include "accel.h"
//#include "ultrasound.h"
#include "ultrasound_new.h"
#include "power_enable.h"
#include "led_left_right.h"

static volatile bool main_b_generic_enable = false;

/*! \brief Main function. Execution starts here.*/

void config_test_pins()
{
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(PIN_PA08, &config_port_pin);
}

int main(void)
{
	static uint16_t i =0;
	
	configure_power_enable_pins();
	enable_power_ic();
	
	board_init(); //Predefined
	system_init(); //Predefined
	
	//irq_initialize_vectors();
	//cpu_irq_enable();
	//Initialize the sleep manager
	//sleepmgr_init();

	delay_init();
	
	config_test_pins();

	ui_init();
	ui_powerdown();
	
	configure_rgb_led();
	configure_tc_callbacks();
	
	configure_adc();
	config_motors();

//config_led();
//configure_dac();
//configure_dac_channel();
//config_ir();
//configure_tcc_callbacks();

	configure_buzzer();
	config_accel(); // Need to do this before USB to get I2C working while allowing USB to use the other two pins of SERCOM3

	configure_ultrasound();
	setup_ultrasound();
//configure_tc3();
	
	
	
	//calc_baud = (uint8_t)(system_gclk_chan_get_hz(SERCOM0_GCLK_ID_CORE + 1)/10000000);
	// Start USB stack to authorize VBus monitoring
	udc_start();
	
	system_interrupt_enable_global();
	
	
	
	
	// The main loop manages only the power mode
	// because the USB management is done by interrupt
	while (true) {
		
	
	//! [main_loop]
	//! [main_write]
	//dac_chan_write(&dac_instance, DAC_CHANNEL_0, i);
	//! [main_write]

	//! [main_inc_val]
	//if (++i == 0x3FF) {
		//i = 0;
	//}
		
		
	//	port_pin_toggle_output_level(PIN_PB02);
	//	cpu_delay_s(1);
		/*
#ifdef   USB_DEVICE_LOW_SPEED
		// No USB "Keep a live" interrupt available in low speed
		// to scan generic interface then use main loop
		if (main_b_generic_enable) {
			static volatile uint16_t virtual_sof_sub = 0;
			static uint16_t virtual_sof = 0;
			if (sysclk_get_cpu_hz()/50000 ==
				virtual_sof_sub++) {
				virtual_sof_sub = 0;
				static uint16_t virtual_sof = 0;
				ui_process(virtual_sof++);
			}
		}
#else*/
	//	sleepmgr_enter_sleep();
//#endif
	/*for(int i = 0; i < 0xFF; i+=20)
	{
		write_sound(i);
		delay_ms(150);
	}
	for(int i = 0xFF; i > 0; i-=20)
	{
		write_sound(i);
		delay_ms(150);
	}*/
	
	}
}

void main_suspend_action(void)
{
	ui_powerdown();
}

void main_resume_action(void)
{
	ui_wakeup();
}

void main_sof_action(void)
{
	if (!main_b_generic_enable)
		return;
	ui_process(udd_get_frame_number());
}

void main_remotewakeup_enable(void)
{
	ui_wakeup_enable();
}

void main_remotewakeup_disable(void)
{
	ui_wakeup_disable();
}

bool main_generic_enable(void)
{
	main_b_generic_enable = true;
	return true;
}

void main_generic_disable(void)
{
	main_b_generic_enable = false;
}

void main_hid_set_feature(uint8_t* report)
{
	/*if (report[0] == 0xAA && report[1] == 0x55
			&& report[2] == 0xAA && report[3] == 0x55) {
		// Disconnect USB Device
		udc_stop();
		ui_powerdown();
	}*/
}




/**
 * \mainpage ASF USB Device HID Generic
 *
 * \section intro Introduction
 * This example shows how to implement a USB Device HID Generic
 * on Atmel MCU with USB module.
 * The application note AVR4905 provides information about this implementation.
 *
 * \section startup Startup
 * The example uses the buttons or sensors available on the board
 * to simulate a standard generic.
 * After loading firmware, connect the board (EVKxx,Xplain,...) to the USB Host.
 * When connected to a USB host system this application provides a HID generic
 * application in the Unix/Mac/Windows operating systems.
 * This example uses the native HID driver for these operating systems.
 *
 * A Atmel PC tool allows to communicate with the HID generic device.
 * This document gives information on integrating the Atmel USB HID DLL functions.
 * Simple code examples that demonstrate different types of implementation are given.
 * http://www.atmel.com/dyn/resources/prod_documents/doc7645.pdf
 * The PC tool is available here:
 * http://www.atmel.com/dyn/resources/prod_documents/AVR153.zip
 * Note: Use the PID 0x2402 in tool graphical interface,
 * and the button Firmware Upgrade only disconnects the USB device.
 *
 * \copydoc UI
 *
 * \section example About example
 *
 * The example uses the following module groups:
 * - Basic modules:
 *   Startup, board, clock, interrupt, power management
 * - USB Device stack and HID modules:
 *   <br>services/usb/
 *   <br>services/usb/udc/
 *   <br>services/usb/class/hid/
 *   <br>services/usb/class/hid/generic/
 * - Specific implementation:
 *    - main.c,
 *      <br>initializes clock
 *      <br>initializes interrupt
 *      <br>manages UI
 *    - specific implementation for each target "./examples/product_board/":
 *       - conf_foo.h   configuration of each module
 *       - ui.c         implement of user's interface (buttons, leds)
 */
