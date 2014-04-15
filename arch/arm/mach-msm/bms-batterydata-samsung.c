/* Copyright (c) 2012-2013, Xiaomi corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/mfd/pm8xxx/pm8921-bms.h>

static struct single_row_lut fcc_temp = {
	.x	= {-20, 0, 5, 10, 15, 20, 25, 40, 60},
	.y	= {1962, 1962, 1966, 1962, 1957, 1959, 1964, 1961, 1960},
	.cols	= 9
};

static struct single_row_lut fcc_sf = {
	.x	= {0},
	.y	= {100},
	.cols	= 1
};

static struct sf_lut rbatt_sf = {
        .rows           = 28,
        .cols           = 9,
        /* row_entries are temperature */
        .row_entries	= {-20, 0, 5, 10, 15, 20, 25, 40, 60},
        .percent        = {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
        .sf		= {
{442,180,153,131,117,106,100,90,89},
{444,183,157,135,120,110,103,93,91},
{441,183,159,138,123,113,106,95,93},
{437,181,158,138,124,114,107,97,94},
{437,181,157,140,126,116,109,99,96},
{439,180,155,140,127,117,110,100,98},
{445,181,154,134,127,118,111,101,99},
{452,182,154,132,119,114,107,101,99},
{461,182,155,132,117,108,101,92,92},
{473,183,156,133,117,108,101,92,90},
{491,184,156,134,119,109,102,93,93},
{533,184,157,135,121,111,104,95,95},
{588,185,160,138,123,113,107,96,97},
{658,189,165,142,126,117,110,99,97},
{744,192,170,147,131,120,113,101,95},
{854,198,175,149,133,122,113,99,95},
{1009,218,173,144,127,116,108,97,95},
{1279,252,192,150,127,114,105,95,93},
{478,197,171,138,120,109,101,93,92},
{505,199,173,140,121,110,102,94,93},
{535,204,177,143,123,112,104,95,94},
{582,208,181,145,125,114,105,97,95},
{649,215,187,149,128,116,107,98,97},
{739,223,195,154,131,118,109,99,97},
{876,234,203,159,133,119,108,96,94},
{1073,252,210,160,132,117,106,96,95},
{1407,284,227,170,138,120,108,97,96},
{1983,371,264,189,148,126,114,102,99},
			}
};


static struct pc_temp_ocv_lut pc_temp_ocv = {
	.rows		= 29,
	.cols		= 9,
	.temp		= {-20, 0, 5, 10, 15, 20, 25, 40, 60},
	.percent	= {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
	.ocv		= {
				{4180, 4179, 4180, 4179, 4178, 4178, 4176, 4173, 4167},
				{4109, 4120, 4122, 4122, 4122, 4122, 4120, 4118, 4114},
				{4058, 4072, 4077, 4077, 4077, 4077, 4074, 4073, 4069},
				{4003, 4022, 4032, 4034, 4035, 4035, 4033, 4031, 4028},
				{3953, 3976, 3988, 3994, 3996, 3996, 3994, 3993, 3990},
				{3916, 3940, 3950, 3961, 3963, 3963, 3961, 3959, 3955},
				{3883, 3907, 3914, 3928, 3932, 3933, 3931, 3929, 3925},
				{3852, 3878, 3882, 3886, 3902, 3904, 3902, 3901, 3897},
				{3824, 3853, 3855, 3856, 3858, 3867, 3866, 3873, 3870},
				{3800, 3830, 3832, 3832, 3832, 3832, 3831, 3831, 3831},
				{3781, 3811, 3812, 3812, 3811, 3811, 3809, 3808, 3806},
				{3767, 3793, 3795, 3795, 3795, 3795, 3794, 3792, 3790},
				{3755, 3778, 3781, 3781, 3782, 3782, 3781, 3778, 3778},
				{3745, 3767, 3772, 3773, 3773, 3773, 3772, 3769, 3767},
				{3733, 3757, 3766, 3767, 3766, 3767, 3766, 3761, 3754},
				{3721, 3740, 3756, 3759, 3759, 3758, 3757, 3751, 3736},
				{3705, 3722, 3735, 3740, 3742, 3741, 3738, 3728, 3715},
				{3683, 3708, 3706, 3708, 3709, 3707, 3705, 3696, 3684},
				{3655, 3695, 3688, 3685, 3683, 3680, 3678, 3669, 3659},
				{3646, 3689, 3684, 3682, 3680, 3678, 3676, 3668, 3657},
				{3635, 3684, 3680, 3679, 3678, 3676, 3673, 3666, 3655},
				{3622, 3679, 3676, 3676, 3675, 3673, 3671, 3663, 3652},
				{3607, 3672, 3671, 3671, 3671, 3669, 3667, 3658, 3647},
				{3588, 3659, 3660, 3661, 3661, 3658, 3656, 3646, 3631},
				{3562, 3632, 3630, 3633, 3634, 3630, 3625, 3610, 3595},
				{3525, 3587, 3579, 3582, 3585, 3579, 3571, 3556, 3544},
				{3469, 3517, 3505, 3510, 3514, 3508, 3499, 3484, 3474},
				{3371, 3412, 3397, 3407, 3414, 3408, 3393, 3379, 3373},
				{3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200}
	}
};


struct pm8921_bms_battery_data samsung_1900_data = {
	.fcc			= 1930,
	.fcc_temp_lut		= &fcc_temp,
	.fcc_sf_lut		= &fcc_sf,
	.pc_temp_ocv_lut	= &pc_temp_ocv,
	.rbatt_sf_lut		= &rbatt_sf,
	.default_rbatt_mohm	= 139,

};
