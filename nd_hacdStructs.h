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

#include "nd_hacdDefines.h"
#include "llconvexdecomposition.h"
#include <vector>

struct LLCDHull;
struct LLCDMeshData;

struct DecompHull
{
	std::vector< VHACD::Vertex > mVertices;
	std::vector< VHACD::Triangle > mTriangles;

	std::vector< float > mLLVertices;
	std::vector< uint32_t > mLLTriangles;

	void clear();

	void computeLLVertices();

	void computeLLTriangles();

	void toLLHull( LLCDHull *aHull );
	void toLLMesh( LLCDMeshData *aMesh );
};

struct DecompData
{
	std::vector< DecompHull > mHulls;

	void clear();
};

struct HACDDecoder: public VHACD::IVHACD::IUserCallback
{
    DecompQuality mQuality { DecompQuality::High };

	std::vector< float > mVertices;
	std::vector< uint32_t > mTriangles;

	std::vector< DecompData > mStages;
	DecompHull mSingleHull;

	llcdCallbackFunc mCallback;

	HACDDecoder();
	void clear();
    void Update(const double overallProgress,
                        const double stageProgress,
                        const char* const stage,
                        const char* operation) override
    {
        if( mCallback )
            (*mCallback)(operation, static_cast<int>(overallProgress), 0 );

    }
};
