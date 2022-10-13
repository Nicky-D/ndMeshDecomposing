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


#include "nd_hacdStructs.h"
#include "LLConvexDecomposition.h"

void DecompHull::clear()
{
	mVertices.clear();
	mTriangles.clear();
	mLLVertices.clear();
	mLLTriangles.clear();
}

void DecompHull::computeLLVertices()
{
    if( mLLVertices.size() )
        return;

    for (size_t i = 0; i < mVertices.size(); ++i)
    {
        mLLVertices.push_back(static_cast<float>( mVertices[i].mX ));
        mLLVertices.push_back(static_cast<float>( mVertices[i].mY ));
        mLLVertices.push_back(static_cast<float>( mVertices[i].mZ ));
    }
}

void DecompHull::computeLLTriangles()
{
    if( mLLTriangles.size() )
        return;
    for (size_t i = 0; i < mTriangles.size(); ++i)
    {
        mLLTriangles.push_back(mTriangles[i].mI0);
        mLLTriangles.push_back(mTriangles[i].mI1);
        mLLTriangles.push_back(mTriangles[i].mI2);
    }
}

void DecompHull::toLLHull( LLCDHull *aHull )
{
	computeLLVertices();

	aHull->mVertexBase = &mLLVertices[0];
	aHull->mVertexStrideBytes = sizeof( float ) * 3;
	aHull->mNumVertices = mLLVertices.size()/3;
}

void DecompHull::toLLMesh( LLCDMeshData *aMesh )
{
	computeLLVertices();
	computeLLTriangles();

	aMesh->mIndexType = LLCDMeshData::INT_32;
	aMesh->mVertexBase = &mLLVertices[0];
	aMesh->mNumVertices = mLLVertices.size()/3;
	aMesh->mVertexStrideBytes = sizeof( float )*3;

	aMesh->mIndexBase = &mLLTriangles[0];
	aMesh->mIndexStrideBytes = sizeof( uint32_t ) * 3;
	aMesh->mNumTriangles = mLLTriangles.size()/3;
}

void DecompData::clear()
{
	mHulls.clear();
}

HACDDecoder::HACDDecoder()
{
	mStages.resize( NUM_STAGES );
	mCallback = 0;
}

void HACDDecoder::clear()
{
	mVertices.clear();
	mTriangles.clear();

	for ( size_t i = 0;  i < mStages.size(); ++i )
		mStages[i].clear();

	mSingleHull.clear();

	mCallback = 0;
}




