/*----------------------------------------------------------------
 * Author : Yuvraj Saxena (frap130@github.com)
 * Contact : thefrap130@gmail.com
 * Note : (Don't remove this block of code from here!)
 * Neither you and nor your any friends or family members will
 * change credits from here! you know the consequences of removing
 *---------------------------------------------------------------*/

#ifndef BUILD_LK
#include <linux/string.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
	#include <mt_gpio.h>
#elif defined(BUILD_UBOOT)
	#include <asm/arch/mt_gpio.h>
#else
	#include <mach/mt_gpio.h>
#endif

#ifdef BUILD_LK
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#include <linux/kernel.h>
#endif

#define _LCM_DEBUG_

/* ---------------------------------------------------------------------------
   Local Constants
   --------------------------------------------------------------------------- */

#define FRAME_WIDTH  										(720)
#define FRAME_HEIGHT 										(1280)

#define REGFLAG_DELAY             							0XFE
#define REGFLAG_END_OF_TABLE      							0xFF

#define LCM_ID_OTM1287A 0x8B


/* ---------------------------------------------------------------------------
   Local Variables
   --------------------------------------------------------------------------- */

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))
#define SET_GPIO_OUT(gpio_num,val)    						(lcm_util.set_gpio_out((gpio_num),(val)))


#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))

#ifdef _LCM_DEBUG_
  #ifdef BUILD_LK
  #define LCM_PRINT printf
  #else
  #define LCM_PRINT printk
  #endif
#else
	#define LCM_PRINT
#endif

/* ---------------------------------------------------------------------------
   Local Functions
   --------------------------------------------------------------------------- */

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg											lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)   			lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)    
       

struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[72];
};


static struct LCM_setting_table lcm_initialization_setting[] = {
{0x00, 1, {0x00}},
{0xff, 3, {0x12,0x87,0x01}},
{0x00, 1, {0x80}},
{0xff, 2, {0x12,0x87}},
{0x00, 1, {0x92}},
{0xff, 2, {0x20,0x02}},
{0x00, 1, {0x80}},
{0xc0, 9, {0x00,0x64,0x00,0x10,0x10,0x00,0x64,0x10,0x10}},
{0x00, 1, {0x90}},
{0xc0, 6, {0x00,0x5c,0x00,0x01,0x00,0x04}},
{0x00, 1, {0xa2}},
{0xc0, 3, {0x01,0x00,0x00}},
{0x00, 1, {0xb3}},
{0xc0, 2, {0x00,0x55}},
{0x00, 1, {0x81}},
{0xc1, 1, {0x66}},
{0x00, 1, {0xa2}},
{0xc1, 1, {0x0a}},
{0x00, 1, {0xa4}},
{0xc1, 1, {0xf0}},
{0x00, 1, {0x80}},
{0xc4, 1, {0x01}},
{0x00, 1, {0x88}},
{0xc4, 1, {0x80}},
{0x00, 1, {0xc2}},
{0xf5, 1, {0x40}},
{0x00, 1, {0x92}},
{0xb3, 1, {0x02}},
{0x00, 1, {0xc8}},
{0xb0, 1, {0x42}},
{0x00, 1, {0x97}},
{0xc5, 1, {0x99}},
{0x00, 1, {0xa0}},
{0xc4, 14, {0x05,0x10,0x04,0x02,0x05,0x15,0x11,0x05,0x10,0x07,0x02,0x05,0x15,0x11}},
{0x00, 1, {0xb0}},
{0xc4, 2, {0x00,0x00}},
{0x00, 1, {0x91}},
{0xc5, 2, {0x29,0x52}},
{0x00, 1, {0x00}},
{0xd8, 2, {0xb4,0xb4}},
{0x00, 1, {0x00}},
{0xd9, 1, {0x52}},
{0x00, 1, {0xb3}},
{0xc5, 1, {0x84}},
{0x00, 1, {0xbb}},
{0xc5, 1, {0x8a}},
{0x00, 1, {0x82}},
{0xc4, 1, {0x0a}},
{0x00, 1, {0xc6}},
{0xb0, 1, {0x03}},
{0x00, 1, {0x00}},
{0xd0, 1, {0x40}},
{0x00, 1, {0x00}},
{0xd1, 2, {0x00,0x00}},
{0x00, 1, {0x80}},
{0xc4, 1, {0x00}},
{0x00, 1, {0x98}},
{0xc5, 1, {0x10}},
{0x00, 1, {0x81}},
{0xf5, 1, {0x15}},
{0x00, 1, {0x83}},
{0xf5, 1, {0x15}},
{0x00, 1, {0x85}},
{0xf5, 1, {0x15}},
{0x00, 1, {0x87}},
{0xf5, 1, {0x15}},
{0x00, 1, {0x89}},
{0xf5, 1, {0x15}},
{0x00, 1, {0x8b}},
{0xf5, 1, {0x15}},
{0x00, 1, {0x95}},
{0xf5, 1, {0x15}},
{0x00, 1, {0x97}},
{0xf5, 1, {0x15}},
{0x00, 1, {0x99}},
{0xf5, 1, {0x15}},
{0x00, 1, {0xa1}},
{0xf5, 1, {0x15}},
{0x00, 1, {0xa3}},
{0xf5, 1, {0x15}},
{0x00, 1, {0xa5}},
{0xf5, 1, {0x15}},
{0x00, 1, {0xa7}},
{0xf5, 1, {0x15}},
{0x00, 1, {0xab}},
{0xf5, 1, {0x18}},
{0x00, 1, {0x94}},
{0xf5, 2, {0x00,0x00}},
{0x00, 1, {0xd2}},
{0xf5, 2, {0x06,0x15}},
{0x00, 1, {0xb2}},
{0xf5, 2, {0x00,0x00}},
{0x00, 1, {0xb6}},
{0xf5, 2, {0x00,0x00}},
{0x00, 1, {0xb4}},
{0xc5, 1, {0xcc}},
{0x00, 1, {0x90}},
{0xf5, 4, {0x02,0x11,0x02,0x15}},
{0x00, 1, {0x90}},
{0xc5, 1, {0x50}},
{0x00, 1, {0x94}},
{0xc5, 1, {0x77}},
{0x00, 1, {0x80}},
{0xcb, 11, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00, 1, {0x90}},
{0xcb, 15, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00}},
{0x00, 1, {0xa0}},
{0xcb, 15, {0xff,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x00, 1, {0xb0}},
{0xcb, 15, {0x00,0x00,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,0x00}},
{0x00, 1, {0xc0}},
{0xcb, 15, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x05,0x00,0x05,0x05,0x05,0x05,0x05}},
{0x00, 1, {0xd0}},
{0xcb, 15, {0x05,0x05,0x05,0x05,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05}},
{0x00, 1, {0xe0}},
{0xcb, 14, {0x05,0x00,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x00,0x00}},
{0x00, 1, {0xf0}},
{0xcb, 11, {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}},
{0x00, 1, {0x80}},
{0xcc, 15, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x07,0x00,0x11,0x15,0x13,0x17,0x0d}},
{0x00, 1, {0x90}},
{0xcc, 15, {0x09,0x0f,0x0b,0x01,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06}},
{0x00, 1, {0xa0}},
{0xcc, 14, {0x08,0x00,0x12,0x16,0x14,0x18,0x0e,0x0a,0x10,0x0c,0x02,0x04,0x00,0x00}},
{0x00, 1, {0xb0}},
{0xcc, 15, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x02,0x00,0x14,0x18,0x12,0x16,0x0c}},
{0x00, 1, {0xc0}},
{0xcc, 15, {0x10,0x0a,0x0e,0x08,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03}},
{0x00, 1, {0xd0}},
{0xcc, 14, {0x01,0x00,0x13,0x17,0x11,0x15,0x0b,0x0f,0x09,0x0d,0x07,0x05,0x00,0x00}},
{0x00, 1, {0x80}},
{0xce, 12, {0x87,0x03,0x28,0x86,0x03,0x28,0x85,0x03,0x28,0x84,0x03,0x28}},
{0x00, 1, {0x90}},
{0xce, 14, {0x34,0xfc,0x28,0x34,0xfd,0x28,0x34,0xfe,0x28,0x34,0xff,0x28,0x00,0x00}},
{0x00, 1, {0xa0}},
{0xce, 14, {0x38,0x07,0x05,0x00,0x00,0x28,0x00,0x38,0x06,0x05,0x01,0x00,0x28,0x00}},
{0x00, 1, {0xb0}},
{0xce, 14, {0x38,0x05,0x05,0x02,0x00,0x28,0x00,0x38,0x04,0x05,0x03,0x00,0x28,0x00}},
{0x00, 1, {0xc0}},
{0xce, 14, {0x38,0x03,0x05,0x04,0x00,0x28,0x00,0x38,0x02,0x05,0x05,0x00,0x28,0x00}},
{0x00, 1, {0xd0}},
{0xce, 14, {0x38,0x01,0x05,0x06,0x00,0x28,0x00,0x38,0x00,0x05,0x07,0x00,0x28,0x00}},
{0x00, 1, {0x80}},
{0xcf, 14, {0x38,0x07,0x05,0x00,0x00,0x18,0x25,0x38,0x06,0x05,0x01,0x00,0x18,0x25}},
{0x00, 1, {0x90}},
{0xcf, 14, {0x38,0x05,0x05,0x02,0x00,0x18,0x25,0x38,0x04,0x05,0x03,0x00,0x18,0x25}},
{0x00, 1, {0xa0}},
{0xcf, 14, {0x38,0x03,0x05,0x04,0x00,0x18,0x25,0x38,0x02,0x05,0x05,0x00,0x18,0x25}},
{0x00, 1, {0xb0}},
{0xcf, 14, {0x38,0x01,0x05,0x06,0x00,0x18,0x25,0x38,0x00,0x05,0x07,0x00,0x18,0x25}},
{0x00, 1, {0xc0}},
{0xcf, 11, {0x01,0x01,0x20,0x20,0x00,0x00,0x01,0x81,0x00,0x03,0x08}},
{0x00, 1, {0x00}},
{0xe1, 20, {0x2e,0x4a,0x57,0x65,0x74,0x82,0x82,0xa8,0x96,0xac,0x59,0x45,0x5a,0x3a,0x3a,0x2f,0x23,0x1a,0x18,0x17}},
{0x00, 1, {0x00}},
{0xe2, 20, {0x1d,0x4a,0x57,0x65,0x74,0x82,0x82,0xa8,0x96,0xac,0x59,0x45,0x5a,0x3a,0x3a,0x2f,0x23,0x1a,0x18,0x06}},
{0x00, 1, {0x00}},
{0xff, 3, {0xff,0xff,0xff}},
{0x35, 1, {0x00}},
{0x11, 1, {0x00}},
{REGFLAG_DELAY, 120, {}},
{0x29, 1, {0x00}},
{REGFLAG_DELAY, 20, {}},
{REGFLAG_END_OF_TABLE, 0x00, {}},
};

static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
    unsigned int i;

    for(i = 0; i < count; i++) {
		
        unsigned cmd;
        cmd = table[i].cmd;
		
        switch (cmd) {
			
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
				
            case REGFLAG_END_OF_TABLE :
                break;
				
            default:
				dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
       	}
    }
	
}

/* ---------------------------------------------------------------------------
   LCM Driver Implementations
   --------------------------------------------------------------------------- */

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}

static void lcm_get_params(LCM_PARAMS *params)
{
	memset(params, 0, sizeof(LCM_PARAMS));

params->dsi.packet_size = 256;
params->dsi.vertical_backporch = 14;
params->dsi.vertical_frontporch = 16;
params->dsi.horizontal_backporch = 64;
params->dsi.horizontal_frontporch = 62;
params->dsi.PLL_CLOCK = 260;
params->dsi.lcm_esd_check_table[0].cmd = 10;
params->dsi.lcm_esd_check_table[0].para_list[0] = 0x00;
params->type = 2;
params->dsi.data_format.format = 2;
params->dsi.PS = 2;
params->dsi.vertical_sync_active = 2;
params->dsi.horizontal_sync_active = 2;
params->dsi.noncont_clock_period = 2;
params->width = 720;
params->dsi.horizontal_active_pixel = 720;
params->height = 1280;
params->dsi.vertical_active_line = 1280;
params->dbi.te_mode = 1;
params->dsi.ssc_disable = 1;
params->dsi.noncont_clock = 1;
params->dsi.esd_check_enable = 1;
params->dsi.customization_esd_check_enable = 1;
params->dsi.lcm_esd_check_table[0].count = 1;
params->dbi.te_edge_polarity = 0;
params->dsi.data_format.color_order = 0;
params->dsi.data_format.trans_seq = 0;
params->dsi.data_format.padding = 0;
params->dsi.intermediat_buffer_num = 0;
params->dsi.mode = 3;
params->dsi.LANE_NUM = 3;
}

static unsigned int lcm_compare_id(void)
{
/*
	unsigned int id = 0;
	unsigned char buffer[2];
	unsigned int array[16];
	
	SET_RESET_PIN(1);
	MDELAY(10);
        SET_RESET_PIN(0);
	MDELAY(20);
        SET_RESET_PIN(1);
	MDELAY(150);

	array[0] = 0x00043902;
	array[1] = 0x018712FF;
	dsi_set_cmdq(array, 2, 1);
	MDELAY(10);

	array[0] = 0x00033700;
	dsi_set_cmdq(array, 1, 1);
	MDELAY(10);

	read_reg_v2(0xA1, buffer, 2);


	id = buffer[1];
	LCM_PRINT("%s,  id otm1287A= 0x%08x\n", __func__, id);
	return (LCM_ID_OTM1287A==id)?1:0;
*/
	return 1;
}

static void lcm_init(void)
{
	SET_RESET_PIN(1);
	SET_RESET_PIN(0);
	MDELAY(20);
        SET_RESET_PIN(1);
	MDELAY(150);
}

static void lcm_suspend(void)
{
	unsigned int data_array[16];

	data_array[0] = 0x00280500;
	dsi_set_cmdq(data_array, 1, 1);

	MDELAY(120);

	data_array[0] = 0x00100500;
	dsi_set_cmdq(data_array, 1, 1);

	MDELAY(120);

	SET_RESET_PIN(1);
        SET_RESET_PIN(0);
	MDELAY(1);
}

static void lcm_resume(void)
{
	lcm_init();
}

/* ---------------------------------------------------------------------------
   Get LCM Driver Hooks
   --------------------------------------------------------------------------- */

LCM_DRIVER otm1287a_hd720_dsi_vdo_yukuailai_lcm_drv = 
{
        .name	        = "otm1287a_hd720_dsi_vdo_yukuailai",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.compare_id     = lcm_compare_id,
};
