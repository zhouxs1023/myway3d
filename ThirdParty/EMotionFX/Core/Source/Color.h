/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __COLOR_H
#define __COLOR_H

#include <assert.h>
#include "Macros.h"
#include "SourceDeclaration.h"
#include "Algorithms.h"
#include "FastMath.h"


namespace MCore
{

// color component extraction
inline int ExtractRed  (unsigned long col)					{ return ((col>>16) & 0xff); }
inline int ExtractGreen(unsigned long col)					{ return ((col>>8)  & 0xff); }
inline int ExtractBlue (unsigned long col)					{ return (col & 0xff); }
inline int ExtractAlpha(unsigned long col)					{ return (col>>24); }

/**
 * Construct a 32bit DWORD value (unsigned long) from four bytes (0..255) for each color component.
 * @param r The value of the red component.
 * @param g The value of the green component.
 * @param b The value of the blue component.
 * @param a the value of the alpha component.
 * @result A packed 32-bit DWORD, containing the four color components.
 */
inline unsigned long RGBA(int r, int g, int b, int a=255)	{ return ((unsigned long)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff))); }


/**
 * The high precision color template.
 * This template should be used with floats or doubles.
 * Default predefined types are: RGBAColor, FRGBAColor, DRGBAColor.
 * The component values go from 0 to 1, where 0 is black and 1 is white in case all components had this same value.
 * It however is possible to go higher than 1. Color component values can also be clamped, normalized and affected by exposure control.
 */
template <class T>
class TRGBAColor
{
	DECLARE_CLASS(TRGBAColor<>)

	public:
		/**
		 * Default constructor. Color will be set to black (0,0,0,0).
		 */
		inline TRGBAColor<T>()																		: r(0), g(0), b(0), a(0) {}

		/**
		 * Constructor which sets all components to the same given value.
		 * @param value The value to put in all components (r,g,b,a).
		 */
		inline TRGBAColor<T>(const T value)															: r(value), g(value), b(value), a(value) {}

		/**
		 * Constructor which sets each color component.
		 * @param cR The value for red.
		 * @param cG The value for green.
		 * @param cB The value for blue.
		 * @param cA The value for alpha [default=1.0]
		 */
		inline TRGBAColor<T>(const T cR, const T cG, const T cB, const T cA=1.0)					: r(cR), g(cG), b(cB), a(cA) {}

		/**
		 * Copy constructor.
		 * @param col The color to copy the component values from.
		 */
		inline TRGBAColor<T>(const TRGBAColor<T>& col)												: r(col.r), g(col.g), b(col.b), a(col.a) {}

		/**
		 * Constructor to convert a 32-bits DWORD to a high precision color.
		 * @param col The 32-bits DWORD, for example constructed using the MCore::RGBA(...) function.
		 */
		/*explicit*/ TRGBAColor<T>(unsigned long col)												: r(ExtractRed(col)/255.0), g(ExtractGreen(col)/255.0), b(ExtractBlue(col)/255.0), a(ExtractAlpha(col)/255.0) {}

		/**
		 * Set values from 32bit integer (one byte per channel) color
		 * @param color The color value.
		 */
		TRGBAColor<T>(unsigned int color)															: r( float( color & 0xFF ) * 0.0039215686f ), g( float( ( color >> 8 ) & 0xFF ) * 0.0039215686f ), b( float( ( color >> 16 ) & 0xFF ) * 0.0039215686f ), a( float( ( color >> 24 ) & 0xFF ) * 0.0039215686f ) {}

		/**
		 * Set the color component values.
		 * @param cR The value for red.
		 * @param cG The value for green.
		 * @param cB The value for blue.
		 * @param cA The value for alpha.
		 */
		inline void	Set(const T cR, const T cG, const T cB, const T cA)								{ r=cR; g=cG; b=cB; a=cA; }

		/**
		 * Set the color component values.
		 * @param color The color to set.
		 */
		inline void	Set(const TRGBAColor<T>& color)													{ r=color.r; g=color.g; b=color.b; a=color.a; }

		/**
		 * Clear the color component values. Set them all to 0, so the color turns into black.
		 */
		inline void	Clear()																			{ r=g=b=a=0; }

		/**
		 * Clamp all color component values in a range of 0..1
		 * This can screw up your colors of course. Unless you just want to clamp we advise you to use
		 * the Exposure method for exposure control or the Normalize method.
		 * @result The clamped color.
		 */
		inline TRGBAColor<T>& Clamp()																{ r=MCore::Clamp<T>(r, 0.0f, 1.0f); g=MCore::Clamp<T>(g, 0.0, 1.0); b=MCore::Clamp<T>(b, 0.0, 1.0); a=MCore::Clamp<T>(a, 0.0, 1.0); return *this; }

		/**
		 * Returns the length of the color components (r, g, b), just like you calculate the length of a vector.
		 * The higher the length value, the more bright the color will be.
		 * @result The length of the vector constructed by the r, g and b components.
		 */
		inline T CalcLength() const																	{ return Math::Sqrt(r*r + g*g + b*b); }

		/**
		 * Calculates and returns the intensity of the color. This gives an idea of how bright the color would be on the screen.
		 * @result The intensity.
		 */
		inline T CalcIntensity() const																{ return r*0.212671 + g*0.715160 + b*0.072169; }

		/**
		 * Checks if this color is close to another given color.
		 * @param col The other color to compare with.
		 * @param distSq The square distance of maximum tollerance of the difference. A value of 0 would mean the colors have to be exactly the same.
		 * @result Returns true when the colors are the same (or close enough to eachother). Otherwise false is returned.
		 */
		inline bool	IsClose(const TRGBAColor<T>& col, const T& distSq=EPSILON) const				{ T cR=(r-col.r); cR*=cR; if (cR>distSq) return false; T cG=(g-col.g); cR+=cG*cG; if (cR>distSq) return false; T cB=(b-col.b); cR+=cB*cB; if (cR>distSq) return false; T cA=(a-col.a); cR+=cA*cA; return (cR<distSq); }

		/**
		 * Convert this high precision color to a 32-bit DWORD value.
		 * In order to work correctly, the color component values must be in range of 0..1. So they have to be clamped, normalized or exposure controlled before calling this method.
		 * @result The 32-bit DWORD (unsigned long) value where each byte is a color component.
		 */
		inline unsigned long ToDWORD() const														{ return MCore::RGBA((int)(r*255), (int)(g*255), (int)(b*255), (int)(a*255)); }


		/**
		 * Perform exposure control on the color components.
		 * This will give much better results than just clamping the values between 0 and 1.
		 * @param exposure The exposure value [default=1.5]
		 * @result Returns itself, but now with exposure control performed on it.
		 */
		inline TRGBAColor<T>& Exposure(const double exposure = 1.5)
		{
			r = 1.0 - Math::Exp(-r * exposure);
			g = 1.0 - Math::Exp(-g * exposure);
			b = 1.0 - Math::Exp(-b * exposure);
			return *this;
		}


		/**
		 * Smart normalizes the color. This means it will scale the values in a range of 0..1 if they are out of range.
		 * It picks the value of the component with the highest value. And makes this into a value of 1, while scaling
		 * the others down with the correct amounts. So it's not really normalizing like you normalize a vector, but it
		 * is a little bit more intelligent and will only perform it when needed, so it won't touch the colors if
		 * they are already in range of 0..1.
		 * @result The normalized color.
		 */
		inline TRGBAColor<T>& Normalize()
		{
			T maxVal = 1;

			if (r > maxVal) maxVal=r;
			if (g > maxVal) maxVal=g;
			if (b > maxVal) maxVal=b;

			T mul = 1.0 / maxVal;

			r *= mul;
			g *= mul;
			b *= mul;

			return *this;
		}

		// operators
		inline bool					operator==(const TRGBAColor<T>& col) const				{ return ((r==col.r) && (g==col.g) && (b==col.b) && (a==col.a)); }
		inline const TRGBAColor<T>&	operator*=(const TRGBAColor<T>& col)					{ r*=col.r; g*=col.g; b*=col.b; a*=col.a; return *this; }
		inline const TRGBAColor<T>&	operator+=(const TRGBAColor<T>& col)					{ r+=col.r; g+=col.g; b+=col.b; a+=col.a; return *this; }
		inline const TRGBAColor<T>&	operator-=(const TRGBAColor<T>& col)					{ r-=col.r; g-=col.g; b-=col.b; a-=col.a; return *this; }
		inline const TRGBAColor<T>&	operator*=(const float m)								{ r*=m; g*=m; b*=m; a*=m; return *this; }
		inline const TRGBAColor<T>&	operator*=(const double m)								{ r*=m; g*=m; b*=m; a*=m; return *this; }
		inline const TRGBAColor<T>&	operator/=(const float d)								{ T ooD=1.0/d; r*=ooD; g*=ooD; b*=ooD; a*=ooD; return *this; }
		inline const TRGBAColor<T>&	operator/=(const double d)								{ T ooD=1.0/d; r*=ooD; g*=ooD; b*=ooD; a*=ooD; return *this; }
		inline const TRGBAColor<T>&	operator= (const TRGBAColor<T>& col)					{ r=col.r; g=col.g; b=col.b; a=col.a; return *this; }
		inline const TRGBAColor<T>&	operator= (const T colorValue)							{ r=colorValue; g=colorValue; b=colorValue; a=colorValue; return *this; }

		// attributes
		T	r;	/**< Red component. */
		T	g;	/**< Green component. */
		T	b;	/**< Blue component. */
		T	a;	/**< Alpha component. */
};

// operators
template <class T> inline TRGBAColor<T> operator*(const TRGBAColor<T>& colA,	const TRGBAColor<T>& colB)				{ return TRGBAColor<T>(colA.r*colB.r, colA.g*colB.g, colA.b*colB.b, colA.a*colB.a); }
template <class T> inline TRGBAColor<T> operator+(const TRGBAColor<T>& colA,	const TRGBAColor<T>& colB)				{ return TRGBAColor<T>(colA.r+colB.r, colA.g+colB.g, colA.b+colB.b, colA.a+colB.a); }
template <class T> inline TRGBAColor<T> operator-(const TRGBAColor<T>& colA,	const TRGBAColor<T>& colB)				{ return TRGBAColor<T>(colA.r-colB.r, colA.g-colB.g, colA.b-colB.b, colA.b-colB.b); }
template <class T> inline TRGBAColor<T> operator*(const TRGBAColor<T>& colA,	const float m)							{ return TRGBAColor<T>(colA.r*m, colA.g*m, colA.b*m, colA.a*m); }
template <class T> inline TRGBAColor<T> operator*(const TRGBAColor<T>& colA,	const double m)							{ return TRGBAColor<T>(colA.r*m, colA.g*m, colA.b*m, colA.a*m); }
template <class T> inline TRGBAColor<T> operator*(const float m,				const TRGBAColor<T>& colB)				{ return TRGBAColor<T>(m*colB.r, m*colB.g, m*colB.b, m*colB.a); }
template <class T> inline TRGBAColor<T> operator*(const double m,				const TRGBAColor<T>& colB)				{ return TRGBAColor<T>(m*colB.r, m*colB.g, m*colB.b, m*colB.a); }
template <class T> inline TRGBAColor<T> operator/(const TRGBAColor<T>& colA,	const float d)							{ T ooD=1.0/d; return TRGBAColor<T>(colA.r*ooD, colA.g*ooD, colA.b*ooD, colA.a*ooD); }
template <class T> inline TRGBAColor<T> operator/(const TRGBAColor<T>& colA,	const double d)							{ T ooD=1.0/d; return TRGBAColor<T>(colA.r*ooD, colA.g*ooD, colA.b*ooD, colA.a*ooD); }


// default types
typedef TRGBAColor<MReal>	RGBAColor;
typedef TRGBAColor<float>	FRGBAColor;
typedef TRGBAColor<double>	DRGBAColor;

}	// namespace MCore


#endif