/*
 * Copyright (c) 2011, 2014 Bastien Nocera <hadess@hadess.net>
 *
 * orientation_calc() from the sensorfw package
 * Copyright (C) 2009-2010 Nokia Corporation
 * Authors:
 *   Üstün Ergenoglu <ext-ustun.ergenoglu@nokia.com>
 *   Timo Rongas <ext-timo.2.rongas@nokia.com>
 *   Lihan Guo <lihan.guo@digia.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3 as published by
 * the Free Software Foundation.
 *
 */

#include <cstddef>

#include "orientation.h"

static const char *orientations[] = {
        "undefined",
        "normal",
        "bottom-up",
        "left-up",
        "right-up",
        NULL
};

static const char *tilts[] = {
        "undefined",
        "vertical",
        "tilted-up",
        "tilted-down",
        "face-up",
        "face-down",
        NULL
};

const char *
orientation_to_string (OrientationUp o)
{
        return orientations[o];
}

const char *
tilt_to_string (Tilt t)
{
        return tilts[t];
}
