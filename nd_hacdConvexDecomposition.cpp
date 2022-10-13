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

#include <string.h>
#include <memory>
#include <iostream>
#include <string>

#include "nd_hacdConvexDecomposition.h"

#include "nd_hacdDefines.h"
#include "nd_hacdStructs.h"
#include "nd_hacdUtils.h"
#include "ndConvexDecomposition.h"
#include "nd_EnterExitTracer.h"
#include "nd_StructTracer.h"

LLCDStageData nd_hacdConvexDecomposition::mStages[1];

LLCDParam nd_hacdConvexDecomposition::mParams[4];
LLCDParam::LLCDEnumItem nd_hacdConvexDecomposition::mMethods[1];
LLCDParam::LLCDEnumItem nd_hacdConvexDecomposition::mQuality[(int)DecompQuality::MAX_ENUM];
LLCDParam::LLCDEnumItem nd_hacdConvexDecomposition::mSimplify[1];

LLConvexDecomposition* nd_hacdConvexDecomposition::getInstance()
{
	static nd_hacdConvexDecomposition sImpl;
	return &sImpl;
}

nd_hacdConvexDecomposition::nd_hacdConvexDecomposition()
{
    mDecoders[ DEFAULT_ENCODER_ID ] = new HACDDecoder();
	mTracer = 0;
}

nd_hacdConvexDecomposition::~nd_hacdConvexDecomposition()
{
	if( mTracer )
		mTracer->release();
}

LLCDResult nd_hacdConvexDecomposition::initSystem()
{
	memset( &mStages[0], 0, sizeof( mStages ) );
	memset( &mParams[0], 0, sizeof( mParams ) );
	memset( &mMethods[0], 0, sizeof( mMethods ) );
	memset( &mQuality[0], 0, sizeof( mQuality ) );
	memset( &mSimplify[0], 0, sizeof( mSimplify ) );

	mStages[0].mName = "Decompose";
	mStages[0].mDescription = NULL;

	mMethods[0].mName = "Default";
	mMethods[0].mValue = 0;

    mQuality[0].mName = "Ultra";
    mQuality[0].mValue = (int)DecompQuality::Ultra;
    mQuality[1].mName = "Very high";
    mQuality[1].mValue = (int)DecompQuality::VeryHigh;
    mQuality[2].mName = "High";
    mQuality[2].mValue = (int)DecompQuality::High;
    mQuality[3].mName = "Medium";
    mQuality[3].mValue = (int)DecompQuality::Medium;
    mQuality[4].mName = "Low";
    mQuality[4].mValue = (int)DecompQuality::Low;
    mQuality[5].mName = "Very low";
    mQuality[5].mValue = (int)DecompQuality::VeryLow;

    mSimplify[0].mName = "None";
	mSimplify[0].mValue = 0;

	mParams[0].mName = "nd_AlwaysNeedTriangles";
	mParams[0].mDescription = 0;
	mParams[0].mType = LLCDParam::LLCD_BOOLEAN;
	mParams[0].mDefault.mBool = true;

	mParams[1].mName = "Method";
	mParams[1].mType = LLCDParam::LLCD_ENUM;
	mParams[1].mDetails.mEnumValues.mNumEnums = sizeof(mMethods)/sizeof(LLCDParam::LLCDEnumItem);
	mParams[1].mDetails.mEnumValues.mEnumsArray = mMethods;
	mParams[1].mDefault.mIntOrEnumValue = 0;

	mParams[2].mName = "Decompose Quality";
	mParams[2].mType = LLCDParam::LLCD_ENUM;
	mParams[2].mDetails.mEnumValues.mNumEnums = (int)DecompQuality::MAX_ENUM;
	mParams[2].mDetails.mEnumValues.mEnumsArray = mQuality;
	mParams[2].mDefault.mIntOrEnumValue = 0;

	mParams[3].mName = "Simplify Method";
	mParams[3].mType = LLCDParam::LLCD_ENUM;
	mParams[3].mDetails.mEnumValues.mNumEnums = sizeof(mSimplify)/sizeof(LLCDParam::LLCDEnumItem);
	mParams[3].mDetails.mEnumValues.mEnumsArray = mSimplify;
	mParams[3].mDefault.mIntOrEnumValue = 0;

	return LLCD_OK;
}

LLCDResult nd_hacdConvexDecomposition::initThread()
{
	return LLCD_OK;
}

LLCDResult nd_hacdConvexDecomposition::quitThread()
{
	return LLCD_OK;
}

LLCDResult nd_hacdConvexDecomposition::quitSystem()
{
	return LLCD_OK;
}

void nd_hacdConvexDecomposition::genDecomposition( int& decomp )
{
	HACDDecoder *pGen = new HACDDecoder();
	decomp = mNextDecoderId;
	++mNextDecoderId;

    if( mNextDecoderId >= std::numeric_limits<int>::max())
        mNextDecoderId = FIRST_DECODER_ID;

	mDecoders[ decomp ] = pGen;
}

void nd_hacdConvexDecomposition::deleteDecomposition( int decomp )
{
	delete mDecoders[ decomp ];
	mDecoders.erase( decomp );
}

void nd_hacdConvexDecomposition::bindDecomposition( int decomp )
{
	TRACE_FUNC( mTracer );
	mCurrentDecoderId = decomp;
}

LLCDResult nd_hacdConvexDecomposition::setParam( const char* name, float val )
{
	TRACE_FUNC( mTracer );
	return LLCD_OK;
}

LLCDResult nd_hacdConvexDecomposition::setParam( const char* name, bool val )
{
	TRACE_FUNC( mTracer );
	return LLCD_OK;
}

LLCDResult nd_hacdConvexDecomposition::setParam( const char* name, int val )
{
	TRACE_FUNC( mTracer );
    auto *pC{ getCurrentDecoder() };
    if( !pC )
        return LLCD_STAGE_NOT_READY;

    if( name && std::string(name) == "Decompose Quality")
        pC->mQuality = (DecompQuality)val;
	return LLCD_OK;
}

LLCDResult nd_hacdConvexDecomposition::setMeshData( const LLCDMeshData* data, bool vertex_based )
{
	TRACE_FUNC( mTracer );
	ndStructTracer::trace( data, vertex_based, mTracer );

    auto *pC{ getCurrentDecoder() };
    if( !pC )
        return LLCD_STAGE_NOT_READY;

	return ::setMeshData( data, vertex_based, pC );
}

LLCDResult nd_hacdConvexDecomposition::registerCallback( int stage, llcdCallbackFunc callback )
{
	TRACE_FUNC( mTracer );
    auto *pC{ getCurrentDecoder() };
    if( !pC )
        return LLCD_STAGE_NOT_READY;

	pC->mCallback = callback;

	return LLCD_OK;
}

LLCDResult nd_hacdConvexDecomposition::buildSingleHull()
{
	TRACE_FUNC( mTracer );
	return LLCD_OK;
}

LLCDResult nd_hacdConvexDecomposition::executeStage( int stage )
{
	TRACE_FUNC( mTracer );
	if ( stage < 0 || stage >= NUM_STAGES )
		return LLCD_INVALID_STAGE;

    auto *pC{ getCurrentDecoder() };
    if( !pC )
        return LLCD_STAGE_NOT_READY;

    hacdPtr pHACD = init( );

    float *pVertices { &pC->mVertices[0]};
    uint32_t  *pTrianges{nullptr};
    if( pC->mTriangles.size())
        pTrianges = &pC->mTriangles[0];

    auto vCount {pC->mVertices.size()/3};
    auto tCount {pC->mTriangles.size()/3};

    uint32_t maxHulls{MAX_HULLS};
    uint32_t maxVertices{MAX_VERTICES_PER_HULL};
    if( (int)pC->mQuality < (int)DecompQuality::MAX_ENUM)
    {
        maxHulls =  HULLS_PER_QUALITY[(int)pC->mQuality];
        maxVertices = VERTICES_PER_QUALITY[(int)pC->mQuality];

    }
    std::cerr << "Decomposing to max " << maxHulls << " hulls" << std::endl;

	DecompData oRes = decompose( pHACD.mPtr,
                                 pVertices, vCount, pTrianges, tCount,
                                 pC, maxHulls, maxVertices );
	ndStructTracer::trace( oRes, mTracer );

	pC->mStages[ stage ] = oRes;

	return LLCD_OK;
}

int nd_hacdConvexDecomposition::getNumHullsFromStage( int stage )
{
	TRACE_FUNC( mTracer );
    auto *pC{ getCurrentDecoder() };

	if ( !pC )
		return 0;

	if ( stage < 0 || static_cast<size_t>(stage) >= pC->mStages.size() )
		return 0;

	return pC->mStages[stage].mHulls.size();
}

DecompData toHull( HACDDecoder *aDecoder, LLCDResult &aRes, ndConvexDecompositionTracer *aTracer )
{
	TRACE_FUNC( aTracer );
	aRes = LLCD_REQUEST_OUT_OF_RANGE;

	for ( int i = 0; i < TO_SINGLE_HULL_TRIES; ++i )
	{
        hacdPtr pHACD = init();

        float *pVertices { &aDecoder->mVertices[0]};
        uint32_t  *pTrianges{nullptr};
        if( aDecoder->mTriangles.size())
            pTrianges = &aDecoder->mTriangles[0];

        auto vCount {aDecoder->mVertices.size()/3};
        auto tCount {aDecoder->mTriangles.size()/3};

		DecompData oRes = decompose( pHACD.mPtr,
                                     pVertices, vCount, pTrianges, tCount,
                                     aDecoder, 1, MAX_VERTICES_PER_HULL );

		ndStructTracer::trace( oRes, aTracer );

		if ( oRes.mHulls.size() == 1 )
		{
			aRes = LLCD_OK;
			return oRes;
		}
	}

	return DecompData();
}

LLCDResult nd_hacdConvexDecomposition::getSingleHull( LLCDHull* hullOut )
{
	TRACE_FUNC( mTracer );
    memset( hullOut, 0, sizeof( LLCDHull ) );

    auto *pC{ getCurrentDecoder() };
    if( !pC )
        return LLCD_STAGE_NOT_READY;

	LLCDResult res;

	// Will already trace oRes
	DecompData oRes = toHull( pC, res, mTracer );

	if ( LLCD_OK != res || oRes.mHulls.size() != 1 )
		return res;

	pC->mSingleHull = oRes.mHulls[0];
	pC->mSingleHull.toLLHull( hullOut );

	return LLCD_OK;
}

LLCDResult nd_hacdConvexDecomposition::getHullFromStage( int stage, int hull, LLCDHull* hullOut )
{
	TRACE_FUNC( mTracer );

    auto *pC{ getCurrentDecoder() };
    if( !pC )
        return LLCD_STAGE_NOT_READY;

	memset( hullOut, 0, sizeof( LLCDHull ) );

	if ( stage < 0 || static_cast<size_t>(stage) >= pC->mStages.size() )
		return LLCD_INVALID_STAGE;

	DecompData &oData = pC->mStages[ stage ];

	if ( hull < 0 ||static_cast<size_t>(hull) >= oData.mHulls.size() )
		return LLCD_REQUEST_OUT_OF_RANGE;

	oData.mHulls[ hull ].toLLHull( hullOut );
	ndStructTracer::trace( hullOut, mTracer );

	return LLCD_OK;
}

LLCDResult nd_hacdConvexDecomposition::getMeshFromStage( int stage, int hull, LLCDMeshData* meshDataOut )
{
	TRACE_FUNC( mTracer );

	memset( meshDataOut, 0, sizeof( LLCDHull ) );

    auto *pC{ getCurrentDecoder() };
    if( !pC )
        return LLCD_STAGE_NOT_READY;

	if ( stage < 0 || static_cast<size_t>(stage) >= pC->mStages.size() )
		return LLCD_INVALID_STAGE;

	DecompData &oData = pC->mStages[ stage ];

	if ( hull < 0 || static_cast<size_t>(hull) >= oData.mHulls.size() )
		return LLCD_REQUEST_OUT_OF_RANGE;

	oData.mHulls[ hull ].toLLMesh( meshDataOut );
	ndStructTracer::trace( meshDataOut, true, mTracer );
	return LLCD_OK;
}

bool compareVertex( float const *aLHS, float const *aRHS )
{
    for( auto i = 0; i < 3; ++ i)
    {
        if( std::abs( aLHS[i] - aRHS[i]) > 0.0000001)
            return false;
    }

    return true;
}

LLCDResult getMeshFromHull( LLCDHull* hullIn, LLCDMeshData* meshOut, HACDDecoder *pDecoder )
{
    if( !pDecoder )
        return LLCD_NULL_PTR;

    for( auto&  oStage: pDecoder->mStages )
    {
        for( auto &hull: oStage.mHulls )
        {
            int nVertexCount = hullIn->mNumVertices;
            if( nVertexCount != hull.mVertices.size())
                continue;

            float const *pLHS = hullIn->mVertexBase;
            float const *pRHS = &hull.mLLVertices[0];
            int nStride = hullIn->mVertexStrideBytes / sizeof( float );
            bool bMatch{true};
            for ( int i = 0; bMatch && i < nVertexCount; ++i )
            {
                if( !compareVertex( pLHS, pRHS))
                    bMatch = false;
                pLHS += nStride;
                pRHS += 3;
            }

            if( !bMatch )
                continue;

            meshOut->mVertexStrideBytes = sizeof( float ) * 3;
            meshOut->mNumVertices = hull.mLLVertices.size()/3;
            meshOut->mVertexBase = &hull.mLLVertices[0];

            meshOut->mIndexType = LLCDMeshData::INT_32;
            meshOut->mIndexStrideBytes = sizeof( hacdUINT32 ) * 3;
            meshOut->mNumTriangles = hull.mLLTriangles.size()/3;
            meshOut->mIndexBase = &hull.mLLTriangles[0];

            return LLCD_OK;
        }
    }
    return LLCD_INVALID_HULL_DATA;
}

LLCDResult nd_hacdConvexDecomposition::getMeshFromHull( LLCDHull* hullIn, LLCDMeshData* meshOut )
{
	TRACE_FUNC( mTracer );
	memset( meshOut, 0, sizeof( LLCDMeshData ) );

	if( !hullIn || !hullIn->mVertexBase || !meshOut )
		return LLCD_NULL_PTR;
	if( hullIn->mVertexStrideBytes < 3*sizeof(float) || hullIn->mNumVertices < 3 )
		return LLCD_INVALID_HULL_DATA;

    if(::getMeshFromHull(hullIn, meshOut, getCurrentDecoder()) == LLCD_OK )
        return LLCD_OK;

    if(::getMeshFromHull(hullIn, meshOut, mDecoders[DEFAULT_ENCODER_ID]) == LLCD_OK )
        return LLCD_OK;

    return LLCD_INVALID_HULL_DATA;
}

LLCDResult nd_hacdConvexDecomposition::generateSingleHullMeshFromMesh( LLCDMeshData* meshIn, LLCDMeshData* meshOut )
{
	TRACE_FUNC( mTracer );
	memset( meshOut, 0, sizeof( LLCDMeshData ) );
    auto *pDefaultEncoder = mDecoders[DEFAULT_ENCODER_ID];
    pDefaultEncoder->clear();
	LLCDResult res = ::setMeshData( meshIn, meshIn->mNumVertices > 3, pDefaultEncoder );

	if ( LLCD_OK != res )
		return res;

	// Will already trace oRes
	DecompData oRes = toHull( pDefaultEncoder, res, mTracer );

	if ( LLCD_OK != res || oRes.mHulls.size() != 1 )
		return res;

    pDefaultEncoder->mSingleHull = oRes.mHulls[0];
    pDefaultEncoder->mSingleHull.toLLMesh( meshOut );

	return LLCD_OK;
}

void nd_hacdConvexDecomposition::loadMeshData( const char* fileIn, LLCDMeshData** meshDataOut )
{
	TRACE_FUNC( mTracer );
	static LLCDMeshData meshData;
	memset( &meshData, 0, sizeof( LLCDMeshData ) );
	*meshDataOut = &meshData;
}

int nd_hacdConvexDecomposition::getParameters( const LLCDParam** paramsOut )
{
	TRACE_FUNC( mTracer );
	*paramsOut = mParams;
	return sizeof(mParams)/sizeof(LLCDParam);
}

int nd_hacdConvexDecomposition::getStages( const LLCDStageData** stagesOut )
{
	TRACE_FUNC( mTracer );

	*stagesOut = mStages;
	return sizeof(mStages)/sizeof(LLCDStageData);
}

void nd_hacdConvexDecomposition::setTracer( ndConvexDecompositionTracer * aTracer)
{
	if( mTracer )
		mTracer->release();

	mTracer = aTracer;

	if( mTracer )
		mTracer->addref();
}

