/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __STANDARDMATERIAL_H
#define __STANDARDMATERIAL_H

// include the Core system
#include "../Core/Source/MCore.h"
#include "MemoryCategories.h"
#include "Material.h"


namespace EMotionFX
{

/**
 * The material layer class.
 * A material layer is a texture layer in a material.
 * Examples of layers can be, bumpmaps, diffuse maps, opacity maps, specular maps, etc.
 */
class StandardMaterialLayer
{
	MEMORYOBJECTCATEGORY(StandardMaterialLayer, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_MATERIALS);

	public:
		/**
		 * Standard supported layer types.
		 */
		enum
		{
			LAYERTYPE_UNKNOWN		= 0,	/**< An unknown layer type. */
			LAYERTYPE_AMBIENT		= 1,	/**< Ambient layer. */
			LAYERTYPE_DIFFUSE		= 2,	/**< Diffuse layer. */
			LAYERTYPE_SPECULAR		= 3,	/**< Specular layer. */
			LAYERTYPE_OPACITY		= 4,	/**< Opacity layer. */
			LAYERTYPE_BUMP			= 5,	/**< Bumpmap layer. */
			LAYERTYPE_SELFILLUM		= 6,	/**< Self illumination layer. */
			LAYERTYPE_SHINE			= 7,	/**< Shininess layer. */
			LAYERTYPE_SHINESTRENGTH	= 8,	/**< Shininess strength layer. */
			LAYERTYPE_FILTERCOLOR	= 9,	/**< Filter color layer. */
			LAYERTYPE_REFLECT		= 10,	/**< Reflection layer. */
			LAYERTYPE_REFRACT		= 11,	/**< Refraction layer. */
			LAYERTYPE_ENVIRONMENT	= 12	/**< Environment map layer. */
		};

		/**
		 * Default constructor.
		 * The layer type will be set to LAYERTYPE_UNKNOWN, the amount will be set to 1, and the 
		 * filename will stay uninitialized (empty).
		 */
		StandardMaterialLayer();

		/**
		 * Extended constructor.
		 * @param layerType The layer type.
		 * @param filename The filename, must be without extension or path.
		 * @param amount The amount value, which identifies how intense or active the layer is. The normal range is [0..1].
		 */
		StandardMaterialLayer(const int layerType, const MCore::String& filename, const float amount=1.0f);

		/**
		 * The destructor.
		 */
		virtual ~StandardMaterialLayer();

		/**
		 * Get the filename of the layer.
		 * @result The filename of the texture of the layer, without path or extension.
		 */
		inline const MCore::String& GetFilename() const;

		/**
		 * Set the filename of the texture of the layer.
		 * @param filename The filename, must be without extension or path.
		 */
		inline void SetFilename(const MCore::String& filename);

		/**
		 * Set the amount value.
		 * The amount identifies how active or intense the layer is.
		 * The normal range of the value is [0..1].
		 */
		inline void SetAmount(const float amount);

		/**
		 * Get the amount.
		 * The amount identifies how active or intense the layer is.
		 * The normal range of the value is [0..1].
		 */
		inline float GetAmount() const;

		/**
		 * Get the layer type.
		 * @result The layer type ID.
		 */
		inline int GetType() const;

		/**
		 * Set the layer type.
		 * @param layerType The layer type.
		 */
		inline void SetType(const int typeID);

		/**
		 * Creates an exact copy of this layer.
		 * @result A pointer to an exact copy of the layer.
		 */
		virtual StandardMaterialLayer* Clone();

		/**
		 * Set the u offset (horizontal texture shift).
		 * @param The u offset.
		 */
		inline void SetUOffset(const float uOffset);

		/**
		 * Set the v offset (vertical texture shift).
		 * @param The v offset.
		 */
		inline void SetVOffset(const float vOffset);

		/**
		 * Set the horizontal tiling factor.
		 * @param The horizontal tiling factor.
		 */
		inline void SetUTiling(const float uTiling);

		/**
		 * Set the vertical tiling factor.
		 * @param The vertical tiling factor.
		 */
		inline void SetVTiling(const float vTiling);

		/**
		 * Set the texture rotation.
		 * @param The texture rotation in radians.
		 */
		inline void SetRotationRadians(const float rotationRadians);

		/**
		 * Get the u offset (horizontal texture shift).
		 * @result The u offset.
		 */
		inline float GetUOffset() const;

		/**
		 * Get the v offset (vertical texture shift).
		 * @result The v offset.
		 */
		inline float GetVOffset() const;

		/**
		 * Get the horizontal tiling factor.
		 * @result The horizontal tiling factor.
		 */
		inline float GetUTiling() const;

		/**
		 * Get the vertical tiling factor.
		 * @result The vertical tiling factor.
		 */
		inline float GetVTiling() const;

		/**
		 * Get the texture rotation.
		 * @result The texture rotation in radians.
		 */
		inline float GetRotationRadians() const;


	protected:
		MCore::String	mFilename;			/**< The filename of the texture, without extension or path. */
		int				mLayerTypeID;		/**< The layer type. See the enum for some possibilities. */
		float			mAmount;			/**< The amount value, between 0 and 1. This can for example represent how intens the layer is. */
		float			mUOffset;			/**< u offset (horizontal texture shift). */
		float			mVOffset;			/**< v offset (vertical texture shift). */
		float			mUTiling;			/**< horizontal tiling factor. */
		float			mVTiling;			/**< vertical tiling factor. */
		float			mRotationRadians;	/**< texture rotation in radians. */
};



/**
 * The material class. 
 * This class describes the material properties of renderable surfaces.
 * Every material can have a set of material layers, which contain the texture information.
 */
class StandardMaterial : public Material
{
	DECLARE_CLASS(StandardMaterial);
	MEMORYOBJECTCATEGORY(StandardMaterial, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_MATERIALS);

	public:
		enum { TYPE_ID = 0x00000001 };

		/**
		 * Constructor.
		 * @param name The name of the material.
		 */
		StandardMaterial(const MCore::String& name);

		/**
		 * Destructor.
		 */
		virtual ~StandardMaterial();

		/**
		 * Set the ambient color.
		 * @param The ambient color.
		 */
		inline void SetAmbient(const MCore::RGBAColor& ambient);

		/**
		 * Set the diffuse color.
		 * @param The diffuse color.
		 */
		inline void SetDiffuse(const MCore::RGBAColor& diffuse);

		/**
		 * Set the specular color.
		 * @param The specular color.
		 */
		inline void SetSpecular(const MCore::RGBAColor& specular);

		/**
		 * Set the self illumination color.
		 * @param The self illumination color.
		 */
		inline void SetEmissive(const MCore::RGBAColor& emissive);

		/**
		 * Set the shine.
		 * @param The shine.
		 */
		inline void SetShine(const float shine);

		/**
		 * Set the shine strength.
		 * @param The shine strength.
		 */
		inline void SetShineStrength(const float shineStrength);

		/**
		 * Set the opacity amount (opacity) [1.0=full opac, 0.0=full transparent].
		 * @param The opacity amount.
		 */
		inline void SetOpacity(const float opacity);

		/**
		 * Set the index of refraction.
		 * @param The index of refraction.
		 */
		inline void SetIOR(const float ior);

		/**
		 * Set double sided flag.
		 * @param True if it is double sided, false if not.
		 */
		inline void SetDoubleSided(const bool doubleSided);

		/**
		 * Set the wireframe flag.
		 * @param True if wireframe rendering is enabled, false if not.
		 */
		inline void SetWireFrame(const bool wireFrame);

		/**
		 * Get the ambient color.
		 * @result The ambient color.
		 */
		inline const MCore::RGBAColor& GetAmbient() const;

		/**
		 * Get the diffuse color.
		 * @result The diffuse color.
		 */
		inline const MCore::RGBAColor& GetDiffuse() const;

		/**
		 * Get the specular color.
		 * @result The specular color.
		 */
		inline const MCore::RGBAColor& GetSpecular() const;

		/**
		 * Get the self illumination color.
		 * @result The self illumination color.
		 */
		inline const MCore::RGBAColor& GetEmissive() const;

		/**
		 * Get the shine.
		 * @result The shine.
		 */
		inline float GetShine() const;

		/**
		 * Get the shine strength.
		 * @result The shine strength.
		 */
		inline float GetShineStrength() const;

		/**
		 * Get the opacity amount [1.0=full opac, 0.0=full transparent].
		 * @result The opacity amount.
		 */
		inline float GetOpacity() const;

		/**
		 * Get the index of refraction.
		 * @result The index of refraction.
		 */
		inline float GetIOR() const;

		/**
		 * Get double sided flag.
		 * @result True if it is double sided, false if not.
		 */
		inline bool GetDoubleSided() const;

		/**
		 * Get the wireframe flag.
		 * @result True if wireframe rendering is enabled, false if not.
		 */
		inline bool GetWireFrame() const;

		/**
		 * Add a given layer to this material.
		 * @param layer The layer to add.
		 */
		inline void AddLayer(MCore::Pointer< StandardMaterialLayer > layer);

		/**
		 * Get the number of texture layers in this material.
		 * @result The number of layers.
		 */
		inline int GetNumLayers() const;

		/**
		 * Get a specific layer.
		 * @param nr The material layer number to get.
		 * @result A pointer to the material layer.
		 */
		inline MCore::Pointer< StandardMaterialLayer > GetLayer(const int nr) const;

		/**
		 * Remove a specified material layer (also deletes it from memory).
		 * @param nr The material layer number to remove.
		 */
		inline void RemoveLayer(const int nr);

		/**
		 * Remove a specified material layer (also deletes it from memory).
		 * @param layer A pointer to the layer to remove.
		 */
		inline void RemoveLayer(MCore::Pointer< StandardMaterialLayer > layer);

		/**
		 * Removes all material layers from this material (includes deletion from memory).
		 */
		inline void RemoveAllLayers();

		/**
		 * Find the layer number which is of the given type.
		 * If you for example want to search for a diffuse layer, you make a call like:
		 *
		 * int layerNumber = material->FindLayer( StandardMaterialLayer::LAYERTYPE_DIFFUSE );
		 * 
		 * This will return a value the layer number, which can be accessed with the GetLayer(layerNumber) method.
		 * A value of -1 will be returned in case no layer of the specified type could be found.
		 * @param layerType The layer type you want to search on, for a list of valid types, see the enum inside StandardMaterialLayer.
		 * @result Returns the layer number or -1 when it could not be found.
		 */
		inline int FindLayer(const int layerType) const;

		/**
		 * Creates a clone of the material, including it's layers.
		 * @result A pointer to an exact clone (copy) of the material.
		 */
		virtual MCore::Pointer< Material > Clone();

		/**
		 * Get the unique type ID of this type of material.
		 * @result The unique ID of this material type.
		 */
		virtual int GetType() const					{ return TYPE_ID; }

		/**
		 * Get the string that is a description or the class name of this material type.
		 * @result The string containing the description or class name.
		 */
		virtual const char* GetTypeString() const	{ return "StandardMaterial"; }

	protected:
		MCore::Array< MCore::Pointer< StandardMaterialLayer > > mLayers;	/**< StandardMaterial layers. */
		MCore::RGBAColor	mAmbient;			/**< Ambient color. */
		MCore::RGBAColor	mDiffuse;			/**< Diffuse color. */
		MCore::RGBAColor	mSpecular;			/**< Specular color. */
		MCore::RGBAColor	mEmissive;			/**< Self illumination color. */
		float				mShine;				/**< The shine value, from the phong component (the power). */
		float				mShineStrength;		/**< Shine strength. */
		float				mOpacity;			/**< The opacity amount [1.0=full opac, 0.0=full transparent]. */
		float				mIOR;				/**< Index of refraction. */
		bool				mDoubleSided;		/**< Double sided?. */
		bool				mWireFrame;			/**< Render in wireframe?. */

		// added by cogito
		// 2.0에서는 Material에 있던 클래스인데 2.1에서 이곳으로 옮겨옴 - 박주형
		// 참고 2.0의 Material클래스는 2.1에서 StandardMaterial로 바뀜
	public:
		DWORD               mShaderMask;
		INT                 mAlphaRef;
		INT                 mSrcBlend;
		INT                 mDstBlend;
		DWORD               mTexFxn;
		FLOAT               mTexFxnUParm, mTexFxnVParm, mTexFxnSub0Parm;
};

#include "StandardMaterial.inl"

} // namespace EMotionFX


#endif