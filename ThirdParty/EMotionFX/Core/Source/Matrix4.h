/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef MATRIX_H
#define MATRIX_H

// includes
#include "Macros.h"
#include "Vector.h"
#include "PlaneEq.h"
#include "SourceDeclaration.h"
#include "Algorithms.h"


namespace MCore
{

// forward declaration
template <class T> class TQuaternion;

/**
 * A 4x4 matrix template/class.
 * Matrices can for example be used to transform points or vectors.
 * Transforming means moving to another coordinate system. With matrices you can do things like: translate (move), rotate and scale.
 * One single matrix can store a translation, rotation and scale. If we have only a rotation inside the matrix, this means that if we
 * multiply the matrix with a vector, the vector will rotate by the rotation inside the matrix! The cool thing is that you can also
 * concatenate matrices. In other words, you can multiply matrices with eachother. If you have a rotation matrix, like described above, and
 * also have a translation matrix, then multiplying these two matrices with eachother will result in a new matrix, which contains both the
 * rotation and the translation! So when you multiply this resulting matrix with a vector, it will both translate and rotate.
 * But, does it first rotate and then translate in the rotated space? Or does it first translate and then rotate?
 * For example if you want to rotate a planet, while it's moving, you want to rotate it in it's local coordinate system. Like when you spin
 * a globe you can have on your desk. So where it spins around it's own center. However, if the planet is at location (10,10,10) in 3D space for example
 * it is also possible that rotates around the origin in world space (0,0,0). The order of multiplication between matrices matters.
 * This means that (matrixA * matrixB) does not have to not result in the same as (matrixB * matrixA).
 *
 * Here is some information about how the matrices are stored internally:
 * 
 * [00 01 02 03] // m16 offsets <br>
 * [04 05 06 07] <br>
 * [08 09 10 11] <br>
 * [12 13 14 15] <br>
 * 
 * [00 01 02 03] // m44 offsets --> [row][column] <br>
 * [10 11 12 13] <br>
 * [20 21 22 23] <br>
 * [30 31 32 33] <br>
 *  
 * [Xx Xy Xz 0]	// right <br>
 * [Yx Yy Yz 0]	// up <br>
 * [Zx Zy Zz 0]	// forward <br>
 * [Tx Ty Tz 1]	// translation <br>
 *
 */

template <class T>
class TMatrix
{
	DECLARE_CLASS(Matrix<>)

	public:
		/**
 		 * Default constructor.
		 * @param identity If set to true, the constructed matrix will be initialized as identity matrix. In case of false (default) no data is initialized.
		 */
		TMatrix<T>(bool identity=false)																{ if (identity) Identity(); }

		/**
 		 * Copy constructor.
		 * @param m The matrix to copy the data from.
		 */
		TMatrix<T>(const TMatrix<T>& m);

		/**
		 * Construct the matrix from a position, rotation and scale.
		 * @param position The translation part of the matrix.
		 * @param rotation The rotation of the matrix, in form of a quaternion.
		 * @param scale The scale of the matrix.
		 */
		TMatrix<T>(const TVector3<T>& position, const TQuaternion<T>& rotation, const TVector3<T>& scale);

		/**
		 * Construct the matrix from a position and rotation.
		 * @param position The translation part of the matrix.
		 * @param rotation The rotation of the matrix, in form of a quaternion.
		 */
		TMatrix<T>(const TVector3<T>& position, const TQuaternion<T>& rotation);

		/**
 		 * Sets the matrix to identity.
		 * When a matrix is an identity matrix it will have no influence.
		 */
		void Identity();

		/**
 		 * Makes the matrix a scaling matrix.
		 * Values of 1.0 would have no influence on the scale. Values of for example 2.0 would scale up by a factor of two.
		 * @param sx The scaling along the x-axis.
		 * @param sy The scaling along the y-axis.
		 * @param sz The scaling along the z-axis.
		 */
		void SetScaleMatrix(const T sx, const T sy, const T sz);

		/**
 		 * Makes this matrix a translation matrix.
		 * @param tx The translation along the x-axis, in units. 
		 * @param ty The translation along the y-axis, in units.
		 * @param tz The translation along the z-axis, in units.
		 */
		void SetTranslationMatrix(const T tx, const T ty, const T tz);

		/**
 		 * Makes the matrix an rotation matrix along the x-axis.
		 * @param angle The angle to rotate around this axis, in radians.
		 */
		void SetRotationMatrixX(const T angle);

		/**
 		 * Makes the matrix a rotation matrix along the y-axis.
		 * @param angle The angle to rotate around this axis, in radians.
		 */
		void SetRotationMatrixY(const T angle);

		/**
 		 * Makes the matrix a rotation matrix along the z-axis.
		 * @param angle The angle to rotate around this axis, in radians.
		 *
		 */
		void SetRotationMatrixZ(const T angle);

		/**
 		 * Makes the matrix a rotation matrix around the x, y and z axis.
		 * @param yaw The angle to rotate around the y-axis, in radians.
		 * @param pitch The angle to rotate around the x-axis, in radians.
		 * @param roll, The angle to rotate around teh z-axis, in radians.
		 */
		//void SetRotationMatrixXYZ(const T yaw, const T pitch, const T roll);

		/**
 		 * Makes the matrix a rotation matrix around a given axis with a given angle.
		 * @param axis The axis to rotate around.
		 * @param angle The angle to rotate around this axis, in radians.
		 */
		void SetRotationMatrixAxisAngle(const TVector3<T>& axis, const T angle);

		/**
 		 * Makes the matrix a rotation matrix given the euler angles.
		 * @param anglevec The vector containing the angles for each axis, in radians, so (pitch, yaw, roll) as (x,y,z).
		 */
		void SetRotationMatrixEuler(const TVector3<T>& anglevec);

		/**
 		 * Inverse rotate a vector with this matrix.
		 * This means that the vector will be multiplied with the inverse rotation of this matrix.
		 * @param v The vector to rotate.
		 * @result The rotated vector.
		 */
		TVector3<T>	InverseRot(const TVector3<T>& v);

		/**
 		 * Multiply this matrix with another matrix and stores the result in itself.
		 * @param right The matrix to multiply with.
		 */
		void MultMatrix(const TMatrix<T>& right);

		/**
 		 * Multiply this matrix with another matrix, but only multiply the 3x4 part.
		 * So treat the other matrix as 4x3 matrix instead of 4x4 matrix. Stores the result in itself.
		 * @param right The matrix to multiply with.
		 */
		void MultMatrix3x4(const TMatrix<T>& right);

		/**
 		 * Multiply this matrix with the 3x3 rotation part of the other given matrix, and modify itself.
		 * @param right The matrix to multiply with.
		 */
		void MultMatrix3x3(const TMatrix<T>& right);

		/**
 		 * Multiply this matrix with the inverse of the 3x3 rotation part of another given matrix.
		 * In other words, multiply this matrix with an inverse rotation matrix.
		 * @param right The matrix to multiply with.
		 */
		void MultInverseMatrix3x3(const TMatrix<T>& right);

		/**
 		 * Transpose the matrix (swap rows with columns).
		 */
		void Transpose();

		/**
		 * Transpose the 3x3 rotation part of the matrix.
		 * Leaves the translation on place.
		 */
		void TransposeRot();

		/**
		 * Transpose the translation 1x3 translation part.
		 * Leaves the rotation in tact.
		 */
		void TransposeTranslation();

		/**
 		 * Adjoint this matrix.
		 */
		void Adjoint();

		/**
 		 * Inverse this matrix.
		 */
		void Inverse();

		/**
		 * Makes this the inverse tranpose version of this matrix.
		 * The inverse transpose is the transposed version of the inverse.
		 */
		inline void InverseTranspose();

		/**
		 * Returns the inverse transposed version of this matrix.
		 * The inverse transpose is the transposed version of the inverse.
		 * @result The inverse transposed version of this matrix.
		 */
		inline TMatrix<T> InverseTransposed() const;

		/**
 		 * Orthonormalize this matrix (to prevent skewing or other errors).
		 * This normalizes the x, y and z vectors of the matrix.
		 * It makes sure that the axis vectors are perpendicular to eachother.
		 */
		void OrthoNormalize();

		/**
		 * Normalizes the matrix, which means that all axis vectors (right, up, forward)
		 * will be made of unit length.
		 */
		void Normalize();

		/**
		 * Returns a normalized version of this matrix.
		 * @result The normalized version of this matrix, where the  right, up and forward vectors are of unit length.
		 */
		inline TMatrix<T> Normalized() const;
		
		/**
 		 * Scale this matrix.
		 * @param sx The factor to scale with along the x-axis.
		 * @param sy The factor to scale with along the y-axis.
		 * @param sz The factor to scale with along the z-axis.
		 */
		void Scale(const T sx, const T sy, const T sz);

		/**
 		 * Rotate this matrix around the x-axis.
		 * @param angle The rotation in radians.
		 */
		void RotateX(const T angle);

		/**
 		 * Rotate this matrix around the y-axis.
		 * @param angle The rotation in radians.
		 */
		void RotateY(const T angle);

		/**
 		 * Rotate this matrix around the z-axis.
		 * @param angle The rotation in radians.
		 */
		void RotateZ(const T angle);

		/**
 		 * Rotate this matrix around the x, y and z axis.
		 * @param yaw The rotation around the y-axis, in radians.
		 * @param pitch The rotation around the x-axis, in radians.
		 * @param roll The rotation around the z-axis, in radians.
		 */
		//void RotateXYZ(const T yaw, const T pitch, const T roll);

		/**
 		 * Multiply a vector with the 3x3 rotation part of this matrix.
		 * @param v The vector to transform.
		 * @result The transformed (rotated) vector.
		 */
		inline TVector3<T> Mul3x3(const TVector3<T> &v);

		/**
 		 * Returns the inversed version of this matrix.
		 * @result The inversed version of this matrix.
		 */
		inline TMatrix<T> Inversed() const															{ TMatrix<T> m(*this); m.Inverse(); return m;	}

		/**
 		 * Returns the transposed version of this matrix.
		 * @result The transposed version of this matrix.
		 */
		inline TMatrix<T> Transposed() const														{ TMatrix<T> m(*this); m.Transpose(); return m; }

		/**
 		 * Returns the adjointed version of this matrix.
		 * @result The adjointed version of this matrix.
		 */
		inline TMatrix<T> Adjointed() const															{ TMatrix<T> m(*this); m.Adjoint(); return m;	}

		/**
 		 * Translate the matrix.
		 * @param x The number of units to add to the current translation along the x-axis.
		 * @param y The number of units to add to the current translation along the y-axis.
		 * @param z The number of units to add to the current translation along the z-axis.
		 */
		inline void	Translate(const T x, const T y, const T z)										{ m44[3][0]+=x;	m44[3][1]+=y; m44[3][2]+=z; }

		/**
 		 * Translate the matrix.
		 * @param t The vector containing the translation to add to the current translation of the matrix.
		 */
		inline void	Translate(const TVector3<T> &t)													{ m44[3][0]+=t.x; m44[3][1]+=t.y; m44[3][2]+=t.z; }

		/**
 		 * Set the right vector (must be normalized).
		 * @param xx The x component of the right vector.
		 * @param xy The y component of the right vector.
		 * @param xz The z component of the right vector.
		 */
		inline void SetRight(const T xx, const T xy, const T xz);

		/**
 		 * Set the right vector.
		 * @param x The right vector, must be normalized.
		 */
		inline void	SetRight(const TVector3<T>& x);

		/**
 		 * Set the up vector (must be normalized).
		 * @param yx The x component of the up vector.
		 * @param yy The y component of the up vector.
		 * @param yz The z component of the up vector.
		 */
		inline void SetUp(const T yx, const T yy, const T yz);

		/**
 		 * Set the up vector (must be normalized).
		 * @param y The up vector.
		 */
		inline void SetUp(const TVector3<T>& y);

		/**
 		 * Set the forward vector (must be normalized).
		 * @param zx The x component of the forward vector.
		 * @param zy The y component of the forward vector.
		 * @param zz The z component of the forward vector.
		 */
		inline void SetForward(const T zx, const T zy, const T zz);

		/**
 		 * Set the forward vector (must be normalized).
		 * @param z The forward vector.
		 */
		inline void SetForward(const TVector3<T>& z);

		/**
 		 * Set the translation part of the matrix.
		 * @param tx The translation along the x-axis.
		 * @param ty The translation along the y-axis.
		 * @param tz The translation along the z-axis.
		 */
		inline void	SetTranslation(const T tx, const T ty, const T tz);

		/**
 		 * Set the translation part of the matrix.
		 * @param t The translation vector.
		 */
		inline void	SetTranslation(const TVector3<T>& t);

		/**
 		 * Get the right vector (writable).
		 * @result The right vector (x-axis).
		 */
		inline TVector3<T>&	GetRight();

		/**
 		 * Get the right vector (only readable).
		 * @result The right vector (x-axis).
		 */
		inline const TVector3<T>& GetRight() const;

		/**
 		 * Get the up vector (only readable).
		 * @result The up vector (y-axis).
		 */
		inline const TVector3<T>& GetUp() const;

		/**
 		 * Get the up vector (writable).
		 * @result The up vector (y-axis).
		 */
		inline TVector3<T>& GetUp();

		/**
 		 * Get the forward vector (writable).
		 * @result The forward vector (z-axis).
		 */
		inline TVector3<T>& GetForward();

		/**
 		 * Get the forward vector (only readable).
		 * @result The forward vector (z-axis).
		 */
		inline const TVector3<T>& GetForward() const;

		/**
 		 * Get the translation part of the matrix (only readable).
		 * @result The vector containing the translation.
		 */
		inline const TVector3<T>& GetTranslation() const;

		/**
 		 * Get the translation part of the matrix (writable).
		 * @result The vector containing the translation.
		 */
		inline TVector3<T>& GetTranslation();

		/**
 		 * Calculates the determinant of the matrix.
		 * @result The determinant.
		 */
		T CalcDeterminant() const;

		/**
 		 * Calculates the euler angles.
		 * @result The euler angles, describing the rotation along each axis, in radians.
		 */
		TVector3<T>	CalcEulerAngles() const;

		/**
 		 * Makes this matrix a mirrored version of a specified matrix.
		 * After executing this operation this matrix is the mirrored version of the specified matrix.
		 * @param transform The transformation matrix to mirror (so the original matrix).
		 * @param plane The plane to use as mirror.
		 */
		void Mirror(const TMatrix<T>& transform, const TPlaneEq<T>& plane);

		/**
 		 * Makes this matrix a lookat matrix (also known as camera or view matrix).
		 * @param view The view position, so the position of the camera.
		 * @param target The target position, so where the camera is looking at.
		 * @param up The up vector, describing the roll of the camera, where (0,1,0) would mean the camera is straight up and has no roll and
		 * where (0,-1,0) would mean the camera is upside down, etc.
		 */
		void LookAt(const TVector3<T>& view, const TVector3<T>& target, const TVector3<T>& up);

		/**
 		 * Makes this matrix a perspective projection matrix.
		 * @param fov The field of view, in radians.
		 * @param aspect The aspect ratio (think of values like 0.75 or 1.33).
		 * @param zNear The distance to the near plane.
		 * @param zFar The distance to the far plane.
		 */
		void Projection(const T fov, const T aspect, const T zNear, const T zFar);

		/**
 		 * Makes this matrix an ortho projection matrix, so without perspective.
		 * @param left The left of the image plane.
		 * @param right The right of the image plane.
		 * @param top The top of the image plane.
		 * @param bottom The bottom of the image plane.
		 * @param znear The distance to the near plane.
		 * @param zfar The distance to the far plane.
		 */
		void Ortho(const T left, const T right, const T top, const T bottom, const T znear, const T zfar);

		/**
 		 * Makes this matrix a frustum matrix.
		 * @param left The left of the image plane.
		 * @param right The right of the image plane.
		 * @param top The top of the image plane.
		 * @param bottom The bottom of the image plane.
		 * @param znear The distance to the near plane.
		 * @param zfar The distance to the far plane.
		 */
		void Frustum(const T left, const T right, const T top, const T bottom, const T znear, const T zfar);

		/**
		 * Get the handedness of the matrix, which described if the matrix is left- or right-handed.
		 * The value returned by this method is the dot product between the forward vector and the result of the 
		 * cross product between the right and up vector. So: DotProduct( Cross(right, up), forward );
		 * If the value returned by this method is positive we are dealing with a matrix which is in a right-handed
		 * coordinate system, otherwise we are dealing with a left-handed coordinate system.
		 * Performing an odd number of reflections reverses the handedness. An even number of reflections is always 
		 * equivalent to a rotation, so any series of reflections can always be regarded as a single rotation followed
		 * by at most one reflection.
		 * If a reflection is present (think of a mirror) the handedness will be reversed. A reflection can be detected
		 * by looking at the determinant of the matrix. If the determinant is negative, then a reflection is present.
		 * @result The handedness of the matrix. If this value is positive we are dealing with a matrix in a right handed
		 *         coordinate system. Otherwise we are dealing with one in a left-handed coordinate system.
		 * @see IsRightHanded()
		 * @see IsLeftHanded()
		 */
		T CalcHandedness() const;

		/**
		 * Check if this matrix is symmetric or not.
		 * A materix is said to be symmetric if and only if M(i, j) = M(j, i).
		 * That is, a matrix whose entries are symmetric about the main diagonal.
		 * @param tollerance The maximum difference tollerance between the M(i, j) and M(j, i) entries.
		 *                   The reason for having this tollerance is of course floating point inaccuracy which might have
		 *                   caused some entries to be a bit different.
		 * @result Returns true when the matrix is symmetric, or false when not.
		 */
		bool IsSymmetric(const float tollerance = MCore::Math::EPSILON) const;

		/**
		 * Check if this matrix is a diagonal matrix or not.
		 * A matrix is said to be a diagonal matrix when only the entries on the diagonal contain non-zero values.
		 * The tollerance value is needed because of possible floating point inaccuracies.
		 * @param tollerance The maximum difference between 0 and the entry on the diagonal.
		 * @result Returns true when the matrix is a diagonal matrix, otherwise false is returned.
		 */
		bool IsDiagonal(const float tollerance = MCore::Math::EPSILON) const;

		/**
		 * Check if the matrix is orthogonal or not.
		 * A matrix is orthogonal if the vectors in the matrix form an orthonormal set.
		 * This is when the vectors (right, up and forward) are perpendicular to eachother.
		 * If a matrix is orthogonal, the inverse of the matrix is equal to the transpose of the matrix.
		 * This assumption can be used to optimize specific calculations, since the inverse is slower to calculate than
		 * the transpose of the matrix. Also it can speed up by transforming normals with the matrix.
		 * In that example instead of having to use the inverse transpose matrix, you could just use the transpose of the matrix.
		 * @param tollerance The maximum tollerance in the orthonormal test.
		 * @result Returns true when the matrix is orthogonal, otherwise false is returned.
		 */
		bool IsOrthogonal(const float tollerance = MCore::Math::EPSILON) const;

		/**
		 * Check if the matrix is an identity matrix or not.
		 * @param tollerance The maximum error value per entry in the matrix.
		 * @result Returns true if this matrix is an identity matrix, otherwise false is returned.
		 */
		bool IsIdentity(const float tollerance = MCore::Math::EPSILON) const;

		/**
		 * Check if the matrix is left handed or not.
		 * @result Returns true when the matrix is left handed, otherwise false is returned (so then it is right handed).
		 */
		inline bool IsLeftHanded() const;

		/**
		 * Check if the matrix is right handed or not.
		 * @result Returns true when the matrix is right handed, otherwise false is returned (so then it is left handed).
		 */
		inline bool IsRightHanded() const;

		/**
		 * Check if this matrix is a pure rotation matrix or not.
		 * @param tollerance The maximum error in the measurement.
		 * @result Returns true when the matrix represents only a rotation, otherwise false is returned.
		 */
		inline bool IsPureRotationMatrix(const float tollerance = MCore::Math::EPSILON) const;

		/**
		 * Check if this matrix contains a reflection or not.
		 * @param tollerance The maximum error in the measurement.
		 * @result Returns true when the matrix represents a reflection, otherwise false is returned.
		 */	
		inline bool IsReflective(const float tollerance = MCore::Math::EPSILON) const;

		/**
		 * Prints the matrix into the logfile or debug output, using MCore::LOG().
		 * Please note that the values are printed using floats or doubles. So it is not possible
		 * to use this method for printing matrices of vectors or something other than real numbers.
		 */
		void Print();

		/**
 		 * Returns a translation matrix.
		 * @param v The translation of the matrix.
		 * @result The translation matrix having the specified translation.
		 */
		static inline TMatrix<T> TranslationMatrix(const TVector3<T>& v)							{ TMatrix<T> m; m.SetTranslationMatrix(v.x, v.y, v.z); return m; }

		/**
 		 * Returns a rotation matrix along the x-axis. 
		 * @param rad The angle of rotation, in radians.
		 * @result A rotation matrix.
		 */
		static inline TMatrix<T> RotationMatrixX(const T rad)										{ TMatrix<T> m; m.SetRotationMatrixX(rad); return m; }

		/**
 		 * Returns a rotation matrix along the y-axis. 
		 * @param rad The angle of rotation, in radians.
		 * @result A rotation matrix.
		 */
		static inline TMatrix<T> RotationMatrixY(const T rad)										{ TMatrix<T> m; m.SetRotationMatrixY(rad); return m; }

		/**
 		 * Returns a rotation matrix along the z-axis. 
		 * @param rad The angle of rotation, in radians.
		 * @result A rotation matrix.
		 */
		static inline TMatrix<T> RotationMatrixZ(const T rad)										{ TMatrix<T> m; m.SetRotationMatrixZ(rad); return m; }

		/**
 		 * Returns a rotation matrix along the x, y and z-axis. 
		 * @param eulerAngles The euler angles in radians.
		 * @result A rotation matrix.
		 */
		static inline TMatrix<T> RotationMatrixEuler(const TVector3<T>& eulerAngles)				{ TMatrix<T> m; m.SetRotationMatrixEuler( eulerAngles ); return m; }

		/**
 		 * Returns a rotation matrix from a given axis and angle.
		 * @param axis The axis to rotate around.
		 * @param angle The angle of rotation, in radians.
		 * @result A rotation matrix.
		 */
		static inline TMatrix<T> RotationMatrixAxisAngle(const TVector3<T> &axis, const T angle)	{ TMatrix<T> m; m.SetRotationMatrixAxisAngle(axis, angle); return m; }

		/**
 		 * Returns a scale matrix from a given scaling factor.
		 * @param s The vector containing the scaling factors for each axis.
		 * @result A scaling matrix.
		 */
		static inline TMatrix<T> ScaleMatrix(const T s)												{ TMatrix<T> m; m.SetScaleMatrix(s, s, s); return m; }

		/**
 		 * Returns a scaling matrix.
		 * @param x The scaling factor along the x-axis.
		 * @param y The scaling factor along the y-axis.
		 * @param z The scaling factor along the z-axis.
		 * @result A scaling matrix.
		 */
		static inline TMatrix<T> ScaleMatrix(const T x, const T y, const T z)						{ TMatrix<T> m; m.SetScaleMatrix(x,y,z); return m; }

		// operators
		TMatrix<T>	operator +  (const TMatrix<T>& right) const;					
		TMatrix<T>	operator -  (const TMatrix<T>& right) const;					
		TMatrix<T>	operator *  (const TMatrix<T>& right) const;
		TMatrix<T>&	operator += (const TMatrix<T>& right);
		TMatrix<T>&	operator -= (const TMatrix<T>& right);
		TMatrix<T>&	operator *= (const TMatrix<T>& right);
		inline void	operator  = (const TMatrix<T>& right);

		// attributes
		union
		{
    		T	m16[16];
    		T	m44[4][4];
		};
};


// default types
typedef TMatrix<MReal>	Matrix;
typedef TMatrix<double>	DMatrix;
typedef TMatrix<float>	FMatrix;


// include template code
#include "Matrix4.inl"

}	// namespace MCore


#endif