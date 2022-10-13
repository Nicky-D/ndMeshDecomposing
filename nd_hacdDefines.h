// SPDX-License-Identifier: GPL-2.0
/**
 * Copyright (C) 2022 Nicky Dasmijn
 *
 * This program is free software; you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
**/

#pragma once

#include <algorithm>
#include "VHACD.h"

#define NUM_STAGES 1

//typedef unsigned short hacdUINT16;
//typedef unsigned int hacdUINT32;

typedef uint16_t hacdUINT16;
typedef uint32_t hacdUINT32;

struct hacdHolder
{
    VHACD::IVHACD *mPtr;
    hacdHolder( VHACD::IVHACD *aPtr )
    {
        mPtr = aPtr;
    }
    ~hacdHolder()
    {
        if( mPtr )
            mPtr->Release();
    }

    hacdHolder( hacdHolder && aRhs )
    {
        mPtr = aRhs.mPtr;
        aRhs.mPtr = nullptr;
    }
    hacdHolder(hacdHolder const&) =delete;
    hacdHolder& operator=(hacdHolder const&) = delete;
};

typedef hacdHolder hacdPtr;
typedef VHACD::Vector3< double > tVecDbl;
typedef VHACD::Vector3< uint32_t > tVecLong;

typedef tVecLong ( *fFromIXX )( void const *&, int );

enum class DecompQuality: int { Ultra = 0, VeryHigh, High, Medium, Low, VeryLow, MAX_ENUM};

constexpr uint32_t MAX_HULLS = 256;
// Per Quality
constexpr uint32_t HULLS_PER_QUALITY[(int)DecompQuality::MAX_ENUM] =
        { MAX_HULLS,    // Ultra
          MAX_HULLS/2,  // VeryHigh
          MAX_HULLS/4,  // High
          MAX_HULLS/8,  // Medium
          MAX_HULLS/16, // Low
          MAX_HULLS/32, // Verylow
        };

constexpr uint32_t MAX_VERTICES_PER_HULL     = 256;    // see http://wiki.secondlife.com/wiki/Mesh/Mesh_physics
constexpr uint32_t VERTICES_PER_QUALITY[(int)DecompQuality::MAX_ENUM] =
        { MAX_VERTICES_PER_HULL,    // Ultra
          MAX_VERTICES_PER_HULL/2,  // VeryHigh
          MAX_VERTICES_PER_HULL/4,  // High
          MAX_VERTICES_PER_HULL/8,  // Medium
          MAX_VERTICES_PER_HULL/16, // Low
          MAX_VERTICES_PER_HULL/32, // Verylow
        };


int const TO_SINGLE_HULL_TRIES = 10;

// Use a high value so HACD will generate just one hull. For now we use the same concavity for each run.
int const CONCAVITY_FOR_SINGLE_HULL[  TO_SINGLE_HULL_TRIES ] = { 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000 };

// Max distance to connect CC. Increase this each run.
double const CONNECT_DISTS[ TO_SINGLE_HULL_TRIES ] = { 30, 60, 120, 240, 480, 960, 1920, 3840, 7680, 10000 };
