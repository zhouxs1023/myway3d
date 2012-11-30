#pragma once

#include "MWRenderSystem.h"
#include "MWFFT.h"
#include "MWPerlin.h"

namespace Myway {

    class ProjectedGrid
    {
        struct Vertex
        {
            float x, y, z;
            float nx, ny, nz;
        };

    public:
        struct Options
        {
           /// Projected grid complexity (N*N)
			int Complexity;
			/// Strength
			float Strength;
			/// Elevation 
			float Elevation;
			/// Smooth
			bool Smooth;
			/// Force recalculate mesh geometry each frame
			bool ForceRecalculateGeometry;
			/// Choppy waves
			bool ChoppyWaves;
			/// Choppy waves strength
			float ChoppyStrength;

			/** Default constructor
			 */
			Options()
				: Complexity(128)
				, Strength(5.0f)
				, Elevation(5.0f)
				, Smooth(false)
				, ForceRecalculateGeometry(false)
				, ChoppyWaves(true)
				, ChoppyStrength(3.75f)
			{
			}
        };

    public:
		ProjectedGrid();
		ProjectedGrid(const Options &Options);
        ~ProjectedGrid();

		void SetHeight(float height);
        void Update(float elapsedTime);

        RenderOp * GetRender() { return &mRender; }

    protected:
        void _init();
        void _initGeo();
        void _deinit();


        bool _getMinMax(Mat4 & range);
        void _setDisplacementAmplitude(float ampl);
        Vec4 _calculeWorldPosition(const Vec2 &uv, const Mat4& m);

        void _renderGeometry(const Mat4& m, const Vec3& WorldPos);

        void _calculeNormals();
        void _performChoppyWaves();

    protected:
        Options mOptions;

		Vertex *mVertices;
		Vertex * mVerticesChoppyBuffer;

        Vec3 mPos, mNormal;
		Vec4 t_corners0,t_corners1,t_corners2,t_corners3;

	    Plane mPlane, mUpperBound, mLowerBound;

        Camera * mRenderCamera;

        Perlin * mNoise;
        float mWaterHeight;

        RenderOp mRender;
    };
}