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


#include "nd_StructTracer.h"
#include <sstream>
#include <iomanip>

using namespace std;

namespace ndStructTracer
{
	bool doTrace( ndConvexDecompositionTracer *aTracer )
	{
		if( !aTracer )
			return false;

		return ndConvexDecompositionTracer::eTraceData == (aTracer->getLevel() & ndConvexDecompositionTracer::eTraceData) ;
	}

    void traceInstance( VHACD::Vertex const &aValue, ndConvexDecompositionTracer *aTracer )
    {
        stringstream str;
        str << fixed
            << "\tX: " << aValue.mX
            << " Y: " << aValue.mY
            << " Z: " << aValue.mZ;

        aTracer->traceData( str.str().c_str() );
    }
    void traceInstance( VHACD::Triangle const &aValue, ndConvexDecompositionTracer *aTracer )
    {
        stringstream str;
        str << fixed
            << "\tX: " << aValue.mI0
            << " Y: " << aValue.mI1
            << " Z: " << aValue.mI2;

        aTracer->traceData( str.str().c_str() );
    }

	template< typename T> void traceVector( vector< T > const &aVertices, ndConvexDecompositionTracer *aTracer )
	{
		for( size_t i = 0; i < aVertices.size(); ++i )
			traceInstance( aVertices[i], aTracer );
	}

	void traceVertices( vector< VHACD::Vertex > const &aVertices, ndConvexDecompositionTracer *aTracer )
	{
		aTracer->traceData( "Vertices\n" );
		traceVector( aVertices, aTracer );
	}

	void traceTriangles( vector< VHACD::Triangle > const &aTriangles, ndConvexDecompositionTracer *aTracer )
	{
		aTracer->traceData( "Triangles\n" );
		traceVector( aTriangles, aTracer );
	}

	void trace( DecompData const &aData, ndConvexDecompositionTracer *aTracer )
	{
		if( !doTrace( aTracer ) )
			return;

		aTracer->startTraceData("Hulls");

		stringstream str;
		str << "Hulls: " << aData.mHulls.size();
		aTracer->traceData( str.str().c_str() );
		str.seekp(0);

		for( size_t i = 0; i < aData.mHulls.size(); ++i )
		{
			str << "Hull #" << i;
			aTracer->traceData( str.str().c_str() );
			str.seekp(0);

			DecompHull const &oHull( aData.mHulls[i] );
			traceVertices( oHull.mVertices, aTracer );
			traceTriangles( oHull.mTriangles, aTracer );
		}

		aTracer->endTraceData();
	}

	template< typename T> void traceVertex( T aX, T aY, T aZ, ndConvexDecompositionTracer *aTracer )
	{
		stringstream str;
		str << fixed
			<< "X: " << aX
			<< "Y: " << aY
			<< "Z: " << aZ;

		aTracer->traceData( str.str().c_str() );
	}

	template< typename T> void traceTriangle( void *& aPtr, int aStride, ndConvexDecompositionTracer *aTracer )
	{
		T *pData = reinterpret_cast< T* >(aPtr);
		long X( pData[0] ),	Y( pData[1] ),	Z( pData[2] );

		traceVertex( X, Y, Z, aTracer );

		pData += aStride;
		aPtr = pData;
	}

	void traceVertices( int aNumVertices, float const *aVertices, int aStride, ndConvexDecompositionTracer *aTracer )
	{
		for( int i = 0; i < aNumVertices; ++i )
		{
			float X( aVertices[0] ), Y( aVertices[1] ), Z( aVertices[2] );
			traceVertex( X, Y, Z, aTracer );
			aVertices += aStride;
		}
	}

	void traceTriangles( LLCDMeshData const *aData, ndConvexDecompositionTracer *aTracer )
	{
		int nCount = aData->mNumTriangles;
		int nStride = aData->mIndexStrideBytes;
		void *pData = const_cast<void*>( aData->mIndexBase );
		int nIndexType = aData->mIndexType;

		if ( LLCDMeshData::INT_16 == nIndexType )
			nStride /= 2;
		else
			nStride /= 4;
			
		for ( int i = 0; i < nCount; ++i )
		{
			if( LLCDMeshData::INT_16  == nIndexType )
				traceTriangle< hacdUINT16 >( pData, nStride, aTracer );
			else
				traceTriangle< hacdUINT32 >( pData, nStride, aTracer );
		}
	}

	void trace( LLCDMeshData const *aData, bool aVertexBased, ndConvexDecompositionTracer *aTracer )
	{
		if( !doTrace( aTracer ) )
			return;

		aTracer->startTraceData("LLCDMeshData");

		string strVBased("true");
		if( !aVertexBased )
			strVBased = "false";

		stringstream str;
		str << "LLCDMeshData vertex based: " << strVBased
			<< "# vertices " << aData->mNumVertices
			<< "# triangles " << aData->mNumTriangles;

		aTracer->traceData( str.str().c_str() );

		traceVertices( aData->mNumVertices, aData->mVertexBase, aData->mVertexStrideBytes/sizeof(float), aTracer );

		if( !aVertexBased )
			traceTriangles( aData, aTracer );
			
		aTracer->endTraceData();
	}

	void trace( LLCDHull const *aData, ndConvexDecompositionTracer *aTracer )
	{
		if( !doTrace( aTracer ) )
			return;

		aTracer->startTraceData("LLCDHull");

		stringstream str;
		str << "LLCDHull # vertices " << aData->mNumVertices;

		aTracer->traceData( str.str().c_str() );

		traceVertices( aData->mNumVertices, aData->mVertexBase, aData->mVertexStrideBytes/sizeof(float), aTracer );


		aTracer->endTraceData();
	}
};

