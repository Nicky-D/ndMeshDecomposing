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

#include "ndConvexDecomposition.h"

class EnterExitTracer
{
	char const *mFunc;
	ndConvexDecompositionTracer *mTracer;

	void doTrace( char const *aPrefix );

public:
	EnterExitTracer( char const *aFunc, ndConvexDecompositionTracer *aTracer );
	~EnterExitTracer( );
	
};

#ifdef __GNUC__
   #define TRACE_FUNC(ATRACER) EnterExitTracer oETTrace( __PRETTY_FUNCTION__, ATRACER );
#else
   #define TRACE_FUNC(ATRACER) EnterExitTracer oETTrace( __FUNCTION__, ATRACER );
#endif

