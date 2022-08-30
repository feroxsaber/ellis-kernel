// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 MediaTek Inc.
 */

#include "kd_imgsensor.h"
#include "imgsensor_sensor_list.h"

/* Add Sensor Init function here
 * Note:
 * 1. Add by the resolution from ""large to small"", due to large sensor
 *    will be possible to be main sensor.
 *    This can avoid I2C error during searching sensor.
 * 2. This should be the same as
 *     mediatek\custom\common\hal\imgsensor\src\sensorlist.cpp
 */
struct IMGSENSOR_INIT_FUNC_LIST kdSensorList[MAX_NUM_OF_SUPPORT_SENSOR] = {
#if defined(MOT_ELLIS_HI1336_MIPI_RAW)
	{MOT_ELLIS_HI1336_SENSOR_ID,
	SENSOR_DRVNAME_MOT_ELLIS_HI1336_MIPI_RAW,
	MOT_ELLIS_HI1336_MIPI_RAW_SensorInit},
#endif
#if defined(MOT_ELLIS_SC500CS_MIPI_RAW)
   {MOT_ELLIS_SC500CS_SENSOR_ID,
   SENSOR_DRVNAME_MOT_ELLIS_SC500CS_MIPI_RAW,
   MOT_ELLIS_SC500CS_MIPI_RAW_SensorInit},
#endif
#if defined(MOT_ELLIS_OV02B1B_MIPI_RAW)
	{MOT_ELLIS_OV02B1B_SENSOR_ID,
	SENSOR_DRVNAME_MOT_ELLIS_OV02B1B_MIPI_RAW,
	MOT_ELLIS_OV02B1B_MIPI_RAW_SensorInit},
#endif
#if defined(MOT_ELLIS_HI556D_MIPI_RAW)
	{MOT_ELLIS_HI556D_SENSOR_ID,
	SENSOR_DRVNAME_MOT_ELLIS_HI556D_MIPI_RAW,
	MOT_ELLIS_HI556D_MIPI_RAW_SensorInit},
#endif
#if defined(MOT_ELLIS_GC02M1B_MIPI_RAW)
	{MOT_ELLIS_GC02M1B_SENSOR_ID,
	SENSOR_DRVNAME_MOT_ELLIS_GC02M1B_MIPI_RAW,
	MOT_ELLIS_GC02M1B_MIPI_RAW_SensorInit},
#endif
#if defined(MOT_TONGA_S5KJN1SQ_MIPI_RAW)
	{MOT_TONGA_S5KJN1SQ_SENSOR_ID,
	SENSOR_DRVNAME_MOT_TONGA_S5KJN1SQ_MIPI_RAW,
	MOT_TONGA_S5KJN1SQ_MIPI_RAW_SensorInit},
#endif
#if defined(MOT_TONGA_S5K4H7_MIPI_RAW)
	{MOT_TONGA_S5K4H7_SENSOR_ID,
	SENSOR_DRVNAME_MOT_TONGA_S5K4H7_MIPI_RAW,
	MOT_TONGA_S5K4H7_MIPI_RAW_SensorInit},
#endif
#if defined(MOT_TONGA_OV02B1B_MIPI_RAW)
	{MOT_TONGA_OV02B1B_SENSOR_ID,
	SENSOR_DRVNAME_MOT_TONGA_OV02B1B_MIPI_RAW,
	MOT_TONGA_OV02B1B_MIPI_RAW_SensorInit},
#endif
#if defined(MOT_TONGA_GC02M1B_MIPI_RAW)
	{MOT_TONGA_GC02M1B_SENSOR_ID,
	SENSOR_DRVNAME_MOT_TONGA_GC02M1B_MIPI_RAW,
	MOT_TONGA_GC02M1B_MIPI_RAW_SensorInit},
#endif
#if defined(MOT_TONGA_GC02M1_MIPI_RAW)
	{MOT_TONGA_GC02M1_SENSOR_ID,
	SENSOR_DRVNAME_MOT_TONGA_GC02M1_MIPI_RAW,
	MOT_TONGA_GC02M1_MIPI_RAW_SensorInit},
#endif
	/*  ADD sensor driver before this line */
	{0, {0}, NULL}, /* end of list */
};
/* e_add new sensor driver here */

