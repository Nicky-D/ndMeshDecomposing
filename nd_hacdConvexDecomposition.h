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

#include "LLConvexDecomposition.h"
#include "nd_hacdDefines.h"
#include <map>
#include <vector>

struct HACDDecoder;

class nd_hacdConvexDecomposition : public LLConvexDecomposition, public ndConvexDecompositionTracable
{
    static constexpr int DEFAULT_ENCODER_ID{0};
    static  constexpr int FIRST_DECODER_ID{1};

	int mNextDecoderId {FIRST_DECODER_ID };
	int mCurrentDecoderId{FIRST_DECODER_ID};
	std::map< int, HACDDecoder * > mDecoders;

	ndConvexDecompositionTracer *mTracer;

	static LLCDStageData mStages[1];

	static LLCDParam mParams[4];
	static LLCDParam::LLCDEnumItem mMethods[1];
	static LLCDParam::LLCDEnumItem mQuality[(int)DecompQuality::MAX_ENUM];
	static LLCDParam::LLCDEnumItem mSimplify[1];

public:
	virtual ~nd_hacdConvexDecomposition();

	static LLConvexDecomposition* getInstance();
	static LLCDResult initSystem();
	static LLCDResult initThread();
	static LLCDResult quitThread();
	static LLCDResult quitSystem();

	// Generate a decomposition object handle
	void genDecomposition( int& decomp ) override;
	// Delete decomposition object handle
	void deleteDecomposition( int decomp ) override;
	// Bind given decomposition handle
	// Commands operate on currently bound decomposition
	void bindDecomposition( int decomp ) override;

	// Sets *paramsOut to the address of the LLCDParam array and returns
	// the length of the array
	int getParameters( const LLCDParam** paramsOut ) override;
	int getStages( const LLCDStageData** stagesOut ) override ;

	// Set a parameter by name. Returns false if out of bounds or unsupported parameter
	LLCDResult	setParam( const char* name, float val ) override;
	LLCDResult	setParam( const char* name, int val ) override;
	LLCDResult	setParam( const char* name, bool val ) override;
	LLCDResult	setMeshData( const LLCDMeshData* data, bool vertex_based ) override;
	LLCDResult	registerCallback( int stage, llcdCallbackFunc callback ) override;

	LLCDResult	executeStage( int stage ) override;
	LLCDResult	buildSingleHull()  override;

	int getNumHullsFromStage( int stage ) override;

	LLCDResult	getHullFromStage( int stage, int hull, LLCDHull* hullOut ) override;
	LLCDResult  getSingleHull( LLCDHull* hullOut )  override;

	// TODO: Implement lock of some kind to disallow this call if data not yet ready
	LLCDResult	getMeshFromStage( int stage, int hull, LLCDMeshData* meshDataOut ) override;
	LLCDResult	getMeshFromHull( LLCDHull* hullIn, LLCDMeshData* meshOut ) override;

	// For visualizing convex hull shapes in the viewer physics shape display
	LLCDResult generateSingleHullMeshFromMesh( LLCDMeshData* meshIn, LLCDMeshData* meshOut ) override;

	/// Debug
	void loadMeshData( const char* fileIn, LLCDMeshData** meshDataOut ) override;

    void setTracer( ndConvexDecompositionTracer *) override;
private:
    HACDDecoder* getCurrentDecoder()
    {
        if( mDecoders.end() == mDecoders.find(mCurrentDecoderId))
            return nullptr;

        return mDecoders[ mCurrentDecoderId];
    }

	nd_hacdConvexDecomposition();
};
