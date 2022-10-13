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

#ifndef ND_HASCONVEXDECOMP_TRACER
 #define ND_HASCONVEXDECOMP_TRACER
#endif

class ndConvexDecompositionTracer
{
public:
	enum ETraceLevel
	{
		eNone = 0,
		eTraceFunctions = 0x1,
		eTraceData = 0x2,
	};
	
	virtual ~ndConvexDecompositionTracer()
	{ }

	virtual void trace( char const *a_strMsg ) = 0;

	virtual void startTraceData( char const *a_strWhat) = 0;
	virtual void traceData( char const *a_strData ) = 0;
	virtual void endTraceData() = 0;

	virtual int getLevel() = 0;

	virtual void addref() = 0;
	virtual void release() = 0;
};

class ndConvexDecompositionTracable
{
public:
	virtual void setTracer( ndConvexDecompositionTracer *) = 0;
};
