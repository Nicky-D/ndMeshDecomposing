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

#include "nd_hacdConvexDecomposition.h"
#include "llconvexdecomposition.h"

LLConvexDecomposition* LLConvexDecomposition::getInstance()
{
	return nd_hacdConvexDecomposition::getInstance();
}

LLCDResult LLConvexDecomposition::initSystem()
{
	return nd_hacdConvexDecomposition::initSystem();
}

LLCDResult LLConvexDecomposition::initThread()
{
	return nd_hacdConvexDecomposition::initThread();
}

LLCDResult LLConvexDecomposition::quitThread()
{
	return nd_hacdConvexDecomposition::quitThread();
}

LLCDResult LLConvexDecomposition::quitSystem()
{
	return nd_hacdConvexDecomposition::quitSystem();
}


