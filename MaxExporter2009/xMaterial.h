#pragma once

namespace MaxPlugin {

	class xMaterial
	{
	public:
		xMaterial();
		~xMaterial();

		void SetDoubleSide(BOOL b) { mDoubleSide = b; }
		BOOL GetDoubleSide() const { return mDoubleSide; }

		void SetBlendMode(BLEND_MODE mode) { mBlendMode = mode; }
		BLEND_MODE GetBlendMode() const { return mBlendMode; }

		void SetEmissive(const Color4 & color) { mEmissive = color; }
		const Color4 & GetEmissive() const { return mEmissive; }

		void SetAmbient(const Color4 & color) { mAmbient = color; }
		const Color4 & GetAmbient() const { return mAmbient; }

		void SetDiffuse(const Color4 & color) { mDiffuse = color; }
		const Color4 & GetDiffuse() const { return mDiffuse; }

		void SetSpecular(const Color4 & color) { mSpecular = color; }
		const Color4 & GetSpecular() const { return mSpecular; }

		void SetSpecularPower(float power) { mSpecularPower = power; }
		float GetSpecularPower() const { return mSpecularPower; }


		void SetEmissiveMap(const TString128 & filename) { mEmissiveMap = filename; }
		const TString128 & GetEmissiveMap() const { return mEmissiveMap; }

		void SetDiffuseMap(const TString128 & filename) { mDiffuseMap = filename; }
		const TString128 & GetDiffuseMap() const { return mDiffuseMap; }

		void SetNormalMap(const TString128 & filename) { mNormalMap = filename; }
		const TString128 & GetNormalMap() const { return mNormalMap; }

		void SetSpecularMap(const TString128 & filename) { mSpecularMap = filename; }
		const TString128 & GetSpecularMap() const { return mSpecularMap; }

	protected:
		BOOL mDoubleSide;
		BLEND_MODE mBlendMode;

		Color4 mEmissive;
		Color4 mAmbient;
		Color4 mDiffuse;
		Color4 mSpecular;
		float mSpecularPower;

		TString128 mEmissiveMap;
		TString128 mDiffuseMap;
		TString128 mNormalMap;
		TString128 mSpecularMap;
	};
}