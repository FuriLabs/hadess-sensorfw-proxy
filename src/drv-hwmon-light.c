/*
 * Copyright (c) 2014 Bastien Nocera <hadess@hadess.net>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3 as published by
 * the Free Software Foundation.
 */

#include "drivers.h"

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#define DEFAULT_POLL_TIME 8000
#define MAX_LIGHT_LEVEL   255

typedef struct DrvData {
	ReadingsUpdateFunc  callback_func;
	gpointer            user_data;

	GUdevDevice        *device;
	guint               timeout_id;
} DrvData;

static DrvData *drv_data = NULL;

static gboolean
hwmon_light_discover (GUdevDevice *device)
{
	return drv_check_udev_sensor_type (device, "hwmon-als", "HWMon light");
}

static gboolean
light_changed (gpointer user_data)
{
	LightReadings readings;
	gdouble level;
	const char *contents;
	int light1, light2;

	contents = g_udev_device_get_sysfs_attr_uncached (drv_data->device, "light");
	if (!contents)
		return G_SOURCE_CONTINUE;

	if (sscanf (contents, "(%d,%d)", &light1, &light2) != 2) {
		g_warning ("Failed to parse light level: %s", contents);
		return G_SOURCE_CONTINUE;
	}
	level = (double) (((float) MAX(light1, light2)) / (float) MAX_LIGHT_LEVEL * 100.0f);

	readings.level = level;
	readings.uses_lux = FALSE;
	drv_data->callback_func (&hwmon_light, (gpointer) &readings, drv_data->user_data);

	return G_SOURCE_CONTINUE;
}

static gboolean
hwmon_light_open (GUdevDevice        *device,
		 ReadingsUpdateFunc  callback_func,
		 gpointer            user_data)
{
	drv_data = g_new0 (DrvData, 1);
	drv_data->callback_func = callback_func;
	drv_data->user_data = user_data;

	drv_data->device = g_object_ref (device);

	return TRUE;
}

static void
hwmon_light_set_polling (gboolean state)
{
	if (drv_data->timeout_id > 0 && state)
		return;
	if (drv_data->timeout_id == 0 && !state)
		return;

	if (drv_data->timeout_id) {
		g_source_remove (drv_data->timeout_id);
		drv_data->timeout_id = 0;
	}

	if (state) {
		drv_data->timeout_id = g_timeout_add (DEFAULT_POLL_TIME, (GSourceFunc) light_changed, NULL);
		g_source_set_name_by_id (drv_data->timeout_id, "[hwmon_light_set_polling] light_changed");

		/* And send a reading straight away */
		light_changed (NULL);
	}
}

static void
hwmon_light_close (void)
{
	hwmon_light_set_polling (FALSE);
	g_clear_object (&drv_data->device);
	g_clear_pointer (&drv_data, g_free);
}

SensorDriver hwmon_light = {
	.name = "Platform HWMon Light",
	.type = DRIVER_TYPE_LIGHT,

	.discover = hwmon_light_discover,
	.open = hwmon_light_open,
	.set_polling = hwmon_light_set_polling,
	.close = hwmon_light_close,
};
