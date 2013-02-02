/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __NMATRIX_H
#define __NMATRIX_H

#include "Macros.h"
#include "SourceDeclaration.h"
#include "Algorithms.h"
#include "Array.h"


namespace MCore
{

/**
 * Multi-dimensional matrix class.
 */
template <class T>
class TNMatrix
{
	DECLARE_CLASS(NMatrix<>);

	public:
		// constructors.
		TNMatrix<T>(const TNMatrix<T>& m);
		TNMatrix<T>(int numRows=6, int numColumns=6);

		// destructor.
		~TNMatrix();

		// Assignment operator.
		TNMatrix<T>& operator = (const TNMatrix<T>& m);

		// size values extraction method.
		inline int GetNumRows() const;
		inline int GetNumColumns() const;

		// subscript operator.
		T& operator () (int row, int col);

		// unary operators.
		inline TNMatrix<T> operator + ()		{ return *this; }
		inline TNMatrix<T> operator - ()		{ return (-1.0 * *this) };

		// combined assignment - calculation operators.
		TNMatrix<T>& operator += (const TNMatrix<T>& m);
		TNMatrix<T>& operator -= (const TNMatrix<T>& m);
		TNMatrix<T>& operator *= (const TNMatrix<T>& m);
		TNMatrix<T>& operator *= (const T& c);
		TNMatrix<T>& operator /= (const T& c);
		TNMatrix<T>& operator ^= (const int& pow);

		// logical operators.
		friend bool operator == (const TNMatrix<T>& m1, const TNMatrix<T>& m2);
		friend bool operator != (const TNMatrix<T>& m1, const TNMatrix<T>& m2);

		// calculation operators.
		friend TNMatrix<T> operator + (const TNMatrix<T>& m1, const TNMatrix<T>& m2);
		friend TNMatrix<T> operator - (const TNMatrix<T>& m1, const TNMatrix<T>& m2);
		friend TNMatrix<T> operator * (const TNMatrix<T>& m1, const TNMatrix<T>& m2);
		friend TNMatrix<T> operator * (const TNMatrix<T>& m, const T& no);
		friend TNMatrix<T> operator * (const T& no, const TNMatrix<T>& m);
		friend TNMatrix<T> operator / (const TNMatrix<T>& m1, const TNMatrix<T>& m2);
		friend TNMatrix<T> operator / (const TNMatrix<T>& m, const T& no);
		friend TNMatrix<T> operator / (const T& no, const TNMatrix<T>& m);
		friend TNMatrix<T> operator ~ (const TNMatrix<T>& m);
		friend TNMatrix<T> operator ! (TNMatrix m);
		friend TNMatrix<T> operator ^ (const TNMatrix<T>& m, const int& pow);

		friend TNMatrix<T> PseudoInverse(const TNMatrix<T>& m);
		friend TNMatrix<T> PseudoInverseSVD(const TNMatrix<T>& m);
		friend TNMatrix<T> PseudoInverse2(const TNMatrix<T>& m);

		// miscellaneous methods.
		void Null(const int row, const int col);
		void Null();
		void Unit(const int row);
		void Unit();
		void SetSize(const int row, const int col);
		void RemoveRow(const int row);
		void RemoveCol(const int col);
		friend TNMatrix<T> ColMatrix(Array<T>& a);
		friend TNMatrix<T> RowMatrix(Array<T>& a);

		// utility methods.
		TNMatrix<T> Solve(const TNMatrix<T>& v) const;
		TNMatrix<T> Adj() const;
		T Det() const;
		T Norm() const;
		T Cofact(const int row, const int col) const;
		T Cond() const;
		int Pivot(const int row);
		int SVD(TNMatrix<T>& U, TNMatrix<T>& S, TNMatrix<T>& V) const;

		// type of matrices.
		bool IsSquare() const; 
		bool IsSingular() const;
		bool IsDiagonal() const;
		bool IsScalar() const;
		bool IsUnit() const;
		bool IsNull() const;
		bool IsSymmetric() const;
		bool IsSkewSymmetric() const;
		bool IsUpperTriangular() const;
		bool IsLowerTriangular() const;

	private:
		T*	mData;
		int mNumRows;
		int mNumColumns;
		int mNumElements;
};

// include the inline code
#include "NMatrix.inl"

// predefifined matrix types
typedef TNMatrix<MReal>		NMatrix;
typedef TNMatrix<float>		FNMatrix;
typedef TNMatrix<double>	DNMatrix;
typedef TNMatrix<int>		INMatrix;

} // namespace MCore


#endif