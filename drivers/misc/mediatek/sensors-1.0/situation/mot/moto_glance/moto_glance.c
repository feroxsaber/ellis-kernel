/*============================================================================
  @file moto_glance.c

  @brief
  Implementation of Motorola glance interface to scp

  Copyright (c) 2019 by Motorola Mobility, LLC.  All Rights Reserved
  ===========================================================================*/

/* moto_glance sensor driver
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/kobject.h>
#include <linux/platform_device.h>
#include <linux/atomic.h>
#include <linux/pm_wakeup.h>

#include <hwmsensor.h>
#include <sensors_io.h>
#include "moto_glance.h"
#include "situation.h"

#include <hwmsen_helper.h>

#include <SCP_sensorHub.h>
#include <linux/notifier.h>
#include "scp_helper.h"

#define MOTO_GLANCE_TAG                  "[moto_glance] "
#define MOTO_GLANCE_FUN(f)               pr_debug(MOTO_GLANCE_TAG"%s\n", __func__)
#define MOTO_GLANCE_PR_ERR(fmt, args...)    pr_err(MOTO_GLANCE_TAG"%s %d : "fmt, __func__, __LINE__, ##args)
#define MOTO_GLANCE_LOG(fmt, args...)    pr_debug(MOTO_GLANCE_TAG fmt, ##args)

static struct wakeup_source *moto_glance_wake_lock;

static int  moto_glance_get_data(int *probability, int *status)
{
	int err = 0;
	struct data_unit_t data;
	uint64_t time_stamp = 0;

	err = sensor_get_data_from_hub(ID_MOT_GLANCE, &data);
	if (err < 0) {
		MOTO_GLANCE_PR_ERR("sensor_get_data_from_hub fail!!\n");
		return -1;
	}
	time_stamp = data.time_stamp;
	*probability = data.gesture_data_t.probability;
	MOTO_GLANCE_LOG("recv ipi: timestamp: %lld, probability: %d!\n", time_stamp,
		*probability);
	return 0;
}

static int moto_glance_open_report_data(int open)
{
	int ret = 0;

	MOTO_GLANCE_LOG("%s : enable=%d\n", __func__, open);
#if defined CONFIG_MTK_SCP_SENSORHUB_V1
	if (open == 1)
		ret = sensor_set_delay_to_hub(ID_MOT_GLANCE, 120);
#elif defined CONFIG_NANOHUB

#else

#endif

	ret = sensor_enable_to_hub(ID_MOT_GLANCE, open);
	return ret;
}

static int moto_glance_batch(int flag, int64_t samplingPeriodNs, int64_t maxBatchReportLatencyNs)
{
	MOTO_GLANCE_LOG("%s : flag=%d\n", __func__, flag);

	return sensor_batch_to_hub(ID_MOT_GLANCE, flag, samplingPeriodNs, maxBatchReportLatencyNs);
}

static int moto_glance_flush(void)
{
	return sensor_flush_to_hub(ID_MOT_GLANCE);
}

static int moto_glance_recv_data(struct data_unit_t *event, void *reserved)
{
	if (event->flush_action == FLUSH_ACTION)
		situation_flush_report(ID_MOT_GLANCE);
	else if (event->flush_action == DATA_ACTION) {
		__pm_wakeup_event(moto_glance_wake_lock, msecs_to_jiffies(100));
		//situation_notify(ID_MOT_GLANCE);
		MOTO_GLANCE_PR_ERR("event = %x \n",event->gesture_data_t.probability);
		situation_data_report(ID_MOT_GLANCE, event->gesture_data_t.probability);
	}
	return 0;
}

static int moto_glance_local_init(void)
{
	struct situation_control_path ctl = {0};
	struct situation_data_path data = {0};
	int err = 0;

	ctl.open_report_data = moto_glance_open_report_data;
	ctl.batch = moto_glance_batch;
	ctl.flush = moto_glance_flush;
	ctl.is_support_wake_lock = true;
	err = situation_register_control_path(&ctl, ID_MOT_GLANCE);
	if (err) {
		MOTO_GLANCE_PR_ERR("register moto glance control path err\n");
		goto exit;
	}

	data.get_data = moto_glance_get_data;
	err = situation_register_data_path(&data, ID_MOT_GLANCE);
	if (err) {
		MOTO_GLANCE_PR_ERR("register moto_glance data path err\n");
		goto exit;
	}
	err = scp_sensorHub_data_registration(ID_MOT_GLANCE, moto_glance_recv_data);
	if (err) {
		MOTO_GLANCE_PR_ERR("SCP_sensorHub_data_registration fail!!\n");
		goto exit_create_attr_failed;
	}
	//wakeup_source_init(&moto_glance_wake_lock, "moto_glance_wake_lock");
        moto_glance_wake_lock = wakeup_source_register(NULL, "moto_glance_wake_lock");
	return 0;
exit:
exit_create_attr_failed:
	return -1;
}
static int moto_glance_local_uninit(void)
{
	return 0;
}

static struct situation_init_info moto_glance_init_info = {
	.name = "moto_glance_hub",
	.init = moto_glance_local_init,
	.uninit = moto_glance_local_uninit,
};

static int __init moto_glance_init(void)
{
	situation_driver_add(&moto_glance_init_info, ID_MOT_GLANCE);
	return 0;
}

static void __exit moto_glance_exit(void)
{
        if (moto_glance_wake_lock)
            wakeup_source_unregister(moto_glance_wake_lock);

	MOTO_GLANCE_LOG();
}

module_init(moto_glance_init);
module_exit(moto_glance_exit);
MODULE_LICENSE("GPL");
//MODULE_DESCRIPTION("GLANCE_GESTURE_HUB driver");
MODULE_AUTHOR("oscar@motorola.com");

