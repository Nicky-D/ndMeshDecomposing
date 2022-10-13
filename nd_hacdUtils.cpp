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


#include "nd_hacdUtils.h"

hacdPtr init( )
{
    return hacdPtr( VHACD::CreateVHACD() );
}

DecompData decompose( VHACD::IVHACD *aHACD,
                      float *pPoints, uint32_t nPoints, uint32_t *pTriangles, uint32_t nTriangles,
                      VHACD::IVHACD::IUserCallback *pCallback, uint32_t maxHulls, uint32_t maxVerticesPerHull )
{
    VHACD::IVHACD::Parameters oParams;
    DecompData oRet;

    oParams.m_asyncACD = false;
    oParams.m_callback = pCallback;
    oParams.m_maxConvexHulls = maxHulls;
    oParams.m_maxNumVerticesPerCH = maxVerticesPerHull;

	if( ! aHACD->Compute( pPoints, nPoints, pTriangles, nTriangles, oParams ) )
        return oRet;

    while( !aHACD->IsReady() )
    {
    }

	int nHulls = aHACD->GetNConvexHulls();

	for ( int i = 0; i < nHulls; ++i )
	{
        VHACD::IVHACD::ConvexHull oHull;
        if(! aHACD->GetConvexHull(i, oHull) )
            continue;

		DecompHull oHullOut;

        for (auto j = 0; j < oHull.m_triangles.size(); ++j)
            oHullOut.mTriangles.push_back( oHull.m_triangles[j] );
        for (auto j = 0; j < oHull.m_points.size(); ++j)
            oHullOut.mVertices.push_back( oHull.m_points[j] );

        oHullOut.computeLLVertices();
        oHullOut.computeLLTriangles();
		oRet.mHulls.push_back( oHullOut );
	}

	return oRet;
}

tVecLong fromI16( void const *& pPtr, int aStride )
{
	hacdUINT16 const *pVal = reinterpret_cast< hacdUINT16 const * >( pPtr );
	tVecLong oRet( pVal[0], pVal[1], pVal[2] );
	pVal += aStride / 2;
	pPtr = pVal;
	return oRet;
}

tVecLong fromI32( void const *& pPtr, int aStride )
{
	hacdUINT32 const *pVal = reinterpret_cast< hacdUINT32 const * >( pPtr );
	tVecLong oRet( pVal[0], pVal[1], pVal[2] );
	pVal += aStride / 4;
	pPtr = pVal;
	return oRet;
}

LLCDResult setMeshData( const LLCDMeshData* data, bool vertex_based, HACDDecoder *aDec )
{
	if ( !data || !data->mVertexBase || ( data->mNumVertices < 3 ) || ( data->mVertexStrideBytes != 12 && data->mVertexStrideBytes != 16 ) )
		return LLCD_INVALID_MESH_DATA;

	if ( !vertex_based && ( ( data->mNumTriangles < 1 ) || ! data->mIndexBase ) )
		return LLCD_INVALID_MESH_DATA;

	aDec->clear();
	int nCount = data->mNumVertices;
	float const* pVertex = data->mVertexBase;
	int nStride = data->mVertexStrideBytes / sizeof( float );

	for ( int i = 0; i < nCount; ++i )
	{
        aDec->mVertices.push_back( pVertex[0] );
        aDec->mVertices.push_back( pVertex[1] );
        aDec->mVertices.push_back( pVertex[2] );
		pVertex += nStride;
	}

	if ( !vertex_based )
	{
		fFromIXX pFunc = 0;
		nCount = data->mNumTriangles;
		nStride = data->mIndexStrideBytes;
		void const *pData = data->mIndexBase;

		if ( data->mIndexType == LLCDMeshData::INT_16 )
			pFunc = fromI16;
		else
			pFunc = fromI32;

		for ( int i = 0; i < nCount; ++i )
		{
			tVecLong oVal( ( *pFunc )( pData, nStride ) );
            aDec->mTriangles.push_back( oVal[0] );
            aDec->mTriangles.push_back( oVal[1] );
            aDec->mTriangles.push_back( oVal[2] );
		}
	}

	return LLCD_OK;
}
