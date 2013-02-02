/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

template <class T> 
int TNMatrix<T>::GetNumColumns() const
{
	return mNumColumns;
}


template <class T> 
int TNMatrix<T>::GetNumRows() const
{
	return mNumRows;
}


template <class T> 
TNMatrix<T>::TNMatrix(const TNMatrix<T>& m)
{
	mData = NULL;
	*this = m;
}


template <class T> 
TNMatrix<T>::TNMatrix(const int numRows, const int numColumns)
{
	mNumRows	 = numRows;
	mNumColumns	 = numColumns;
	mNumElements = numRows * numColumns;

	if (mNumElements != 0)
	{
		mData = new T[mNumElements];
		memset(mData, 0, sizeof( T ) * mNumElements);
	}
	else
		mData = NULL;
}


template <class T>
TNMatrix<T>::~TNMatrix()
{
	if (mData)
		delete[] mData;
}


template<class T>
TNMatrix<T>& TNMatrix<T>::operator = (const TNMatrix<T>& m)
{
	if (mData)
	{
		if (mNumElements != m.mNumElements)
		{
			delete mData;
			mData = new T[m.mNumElements];
		}
	}
	else
		mData = new T[m.mNumElements];

	mNumRows	 = m.mNumRows;
	mNumColumns	 = m.mNumColumns;
	mNumElements = m.mNumElements;

	memcpy( mData, m.mData, sizeof(T) * mNumElements );

	return *this;
}


template <class T> 
T& TNMatrix<T>::operator()(int row, int col)
{
	assert(!(row > mNumRows || row < 0 || col > mNumColumns || col < 0));

/*	if (row > mNumRows || row < 0 || col > mNumColumns || col < 0)
	{
		LOG( "ERROR: Index out of range.");
		return mData[0];
	}*/

	return mData[col + row * mNumColumns];
}


template <class T> 
TNMatrix<T>& TNMatrix<T>::operator += (const TNMatrix<T>& m)
{
	assert(!(row > mNumRows || row < 0 || col > mNumColumns || col < 0));

/*	if( mNumRows != m.mNumRows || mNumColumns != m.mNumColumns )
	{
		LOG( "ERROR: Trying to add matrices of different sizes." );
		*this = TNMatrix<T>(0,0);
	}
	else*/

	for (int i=0; i<mNumElements; ++i)
		mData[i] += m.mData[i];

	return *this;
}


template <class T> 
TNMatrix<T>& TNMatrix<T>::operator -= (const TNMatrix<T>& m)
{
	assert(!(row > mNumRows || row < 0 || col > mNumColumns || col < 0));

/*	if( mNumRows != m.mNumRows || mNumColumns != m.mNumColumns )
	{
		LOG( "ERROR: Trying to substract matrices of different sizes." );
		*this = TNMatrix<T>(0,0);
	}
	else*/

	for (int i=0; i<mNumElements; ++i)
		mData[i] -= m.mData[i];

	return *this;
}


template <class T>
TNMatrix<T>& TNMatrix<T>::operator *= (const TNMatrix<T>& m)
{
	*this = *this * m;
	return *this;
}


template <class T> 
TNMatrix<T>& TNMatrix<T>::operator *= (const T& c)
{
	for (int i=0; i<mNumElements; ++i)
		mData[i] *= c;

	return *this;
}


template <class T> 
TNMatrix<T>& TNMatrix<T>::operator /= (const T& c)
{
	for (int i=0; i<mNumElements; ++i)
		mData[i] /= c;	// TODO: optimize the divide with a multiply?

	return *this;
}


template <class T> 
TNMatrix<T>& TNMatrix<T>::operator ^= (const int& pow)
{
	*this = *this ^ pow;
	return *this;
}


template<class T> 
bool operator == (const TNMatrix<T>& m1, const TNMatrix<T>& m2)
{
	return ( m1.mNumRows == m2.mNumRows && 
			 m1.mNumColumns == m2.mNumColumns &&
			!memcmp( m1.mData, m2.mData, sizeof(T) * m1.mNumElements ) );
}


template <class T> 
bool operator != (const TNMatrix<T>& m1, const TNMatrix<T>& m2)
{
	return !( m1 == m2 );
}


template<class T> 
TNMatrix<T> operator + (const TNMatrix<T>& m1, const TNMatrix<T>& m2)
{
	TNMatrix<T> m(0,0);

	assert (m1.mNumRows == m2.mNumRows && m1.mNumColumns == m2.mNumColumns);

/*	if( m1.mNumRows != m2.mNumRows || m1.mNumColumns != m2.mNumColumns )
	{
		LOG( "ERROR: Trying to add matrices of different sizes." );
		return m;
	}*/

	m.SetSize(m1.mNumRows, m1.mNumColumns);

	for (int i=0; i<m.mNumElements; ++i)
		m.mData[i] = m1.mData[i] + m2.mData[i];

	return m;
}


template <class T> 
TNMatrix<T> operator - (const TNMatrix<T>& m1, const TNMatrix<T>& m2)
{
	TNMatrix<T> m(0,0);

	assert (m1.mNumRows == m2.mNumRows && m1.mNumColumns == m2.mNumColumns);
/*	if( m1.mNumRows != m2.mNumRows || m1.mNumColumns != m2.mNumColumns )
	{
		LOG( "ERROR: Trying to substract matrices of different sizes." );
		return m;
	}*/

	m.SetSize( m1.mNumRows, m1.mNumColumns );

	for (int i=0; i<m.mNumElements; ++i)
		m.mData[i] = m1.mData[i] - m2.mData[i];

	return m;
}


template<class T> 
TNMatrix<T> operator * (const TNMatrix<T>& m1, const TNMatrix<T>& m2)
{
	// TODO: this function can be optimized quite a lot
	TNMatrix<T> m(0,0);

	//assert(m1.mNumColumns == m2.mNumRows);
	if( m1.mNumColumns != m2.mNumRows )
	{
		//LOG( "ERROR: Trying to multiply matrices." );
		return m;
	}

	m.SetSize( m1.mNumRows, m2.mNumColumns );

	int pos;
	for (int i=0; i<m.mNumRows; ++i)
	{
		for (int j=0; j<m.mNumColumns; ++j)
		{
			pos = j + i * m.mNumColumns;
			m.mData[pos] = T(0);
			for (int k=0; k<m1.mNumColumns; ++k)
			{
	            m.mData[pos] +=  m1.mData[k + i * m1.mNumColumns] * m2.mData[j + k * m2.mNumColumns];
			}
		}
	}

	return m;
}


template <class T> 
TNMatrix<T> operator * (const TNMatrix<T>& m, const T& c)
{
	TNMatrix aux = m;
	aux *= c;
	return aux;
}


template <class T> 
TNMatrix<T> operator * (const T& c, const TNMatrix<T>& m)
{
	return m * c;
}


template <class T>
TNMatrix<T> operator / (const TNMatrix<T>& m1, const TNMatrix<T>& m2)
{
	return m1 * !m2;
}


template <class T> 
TNMatrix<T> operator / (const TNMatrix<T>& m, const T& c)
{
	TNMatrix<T> aux = m;
	aux /= c;
	return aux;
}


template <class T>
TNMatrix<T> operator / (const T& c, const TNMatrix<T>& m)
{
	return m / c;
}


template<class T> 
TNMatrix<T> operator ~ (const TNMatrix<T>& m)
{
	TNMatrix<T> aux( m.mNumColumns, m.mNumRows );

	for (int i=0; i<m.mNumRows; ++i)
	{
		for (int j=0; j<m.mNumColumns; ++j)
		{
			aux(j, i) = m.mData[i * m.mNumColumns + j];
		}
	}

	return aux;
}


template<class T> 
TNMatrix<T> operator ^ (const TNMatrix<T>& m, const int& pow)
{
	TNMatrix<T> aux(0,0);

	assert(m.mNumRows == m.mNumColumns);
/*	if (m.mNumRows != m.mNumColumns)
		return aux; */
	
	aux = m;
	for (i=1; i<pow; ++i)
		aux *= m;

	return aux;
}


template <class T> 
TNMatrix<T> operator ! (TNMatrix<T> m)
{

	assert(m.mNumRows == m.mNumColumns);
/*	if (m.mNumRows != m.mNumColumns)
	{
		LOG( "ERROR: Inversion of a non-square matrix");
		return TNMatrix<T>(0,0);
	}*/

	TNMatrix<T> temp(m.mNumRows, m.mNumColumns);

	temp.Unit();

	T a1, a2, aux;
	for (int k=0; k<m.mNumRows; ++k)
	{
		int indx = m.Pivot(k);
		
		//assert(indx != -1);
		if (indx == -1)
		{
			//LOG( "Error: Inversion of a singular matrix");
			return TNMatrix<T>(0,0);
		}

		if (indx != 0)
		{
			for (int i=0; i<m.mNumColumns; ++i)
			{
				aux = temp( k, i );
				temp( k, i ) = temp( indx, i );
				temp( indx, i ) = aux;
			}
		}

		a1 = m(k, k);
		for (int j=0; j<m.mNumRows; ++j)
		{
			m(k, j) /= a1;
			temp(k, j) /= a1;
		}
		
		for (int i=0; i<m.mNumRows; ++i)
		{
			if (i != k)
			{
	            a2 = m(i, k);

				for (j=0; j<m.mNumRows; ++j)
				{
					m(i, j) -= a2 * m(k, j);
					temp(i, j) -= a2 * temp(k, j);
				}
			}
		}
	}

	return temp;
}


template <class T>
void TNMatrix<T>::Null(const int row, const int col)
{
	SetSize(row, col);
	memset(mData, 0, sizeof(T) * mNumElements);
}


template <class T>
void TNMatrix<T>::Null()
{
	memset(mData, 0, sizeof(T) * mNumElements);
}


template <class T> 
void TNMatrix<T>::Unit(const int row)
{
	Null(row, row);

	for (int i=0; i<row; ++i)
		mData[i + row * i] = T(1);
}


template <class T> 
void TNMatrix<T>::Unit()
{
	if (mNumRows == mNumColumns)
		for (int i=0; i<mNumRows; ++i)
			mData[i + mNumRows * i] = T(1);
}


template <class T>
void TNMatrix<T>::SetSize(const int row, const int col)
{
	if (mNumRows == row && mNumColumns == col)
		return;

	int or = mNumRows;
	int oc = mNumColumns;

	TNMatrix<T> aux(row, col);
	for (int i=0; i<or && i<row; ++i)
	{
		for(int j=0; j<oc && j<col; ++j)
		{
			aux(i, j) = (*this)(i, j);
		}
	}

	*this = aux;
}


template <class T> 
void TNMatrix<T>::RemoveRow(const int row)
{
	if (row >= mNumRows || row < 0)
		return;

	for (int i=row; i<mNumRows-1; ++i)
	{
		for (int j=0; j<mNumColumns; ++j)
		{
			(*this)(i,j) = (*this)(i+1,j);
		}
	}

	SetSize(mNumRows - 1, mNumColumns);
}


template <class T>
void TNMatrix<T>::RemoveCol(const int col)
{
	if (col >= mNumColumns || col < 0)
		return;

	for (int i=0; i<mNumRows-1; ++i)
	{
		for (int j=col; j<mNumColumns; ++j)
		{
			(*this)(i,j) = (*this)(i,j+1);
		}
	}

	SetSize(mNumRows, mNumColumns - 1);
}


template<class T> 
TNMatrix<T> TNMatrix<T>::Solve(const TNMatrix<T>& v) const
{
	int i, j, k;
	T a1;

	assert(mNumRows == mNumColumns && mNumColumns == v.mNumRows)
/*	if (!(mNumRows == mNumColumns && mNumColumns == v.mNumRows))
	{
		LOG( "ERROR: Inconsistent matrices in Solve()!");
	}*/

	TNMatrix<T> temp(mNumRows, mNumColumns + v.mNumColumns);
   
	for (i=0; i<mNumRows; ++i)
	{
		for (j=0; j<mNumColumns; ++j)
			temp(i, j) = (*this)(i, j);
		
		for (k=0; k<v.mNumColumns; ++k)
			temp(i, mNumColumns + k) = v.mData[i*v.mNumColumns + k];
	}

	for (k=0; k<mNumRows; ++k)
	{
		int indx = temp.Pivot(k);

		assert(indx != -1);
/*		if (indx == -1)
		{
			LOG( "ERROR: Singular matrix in Solve()!");
		}*/

		a1 = temp(k, k);
		for (j=k; j<temp.mNumColumns; ++j)
			temp(k, j) /= a1;

		for (i=k+1; i<mNumRows; ++i)
		{
			a1 = temp(i, k);
			for (j=k; j<temp.mNumColumns; j++)
				temp(i, j) -= a1 * temp(k, j);
		}
	}
   
	TNMatrix<T> s( v.mNumRows , v.mNumColumns );
	for (k=0; k<v.mNumColumns; ++k)
	{
		for (int m=int(mNumRows)-1; m>=0; m--)	// why typecast to int? :)
		{
			s(m, k) = temp(m, mNumColumns + k);
			for (j=m+1; j<mNumColumns; ++j)
			{
				s(m, k) -= temp(m, j) * s(j, k);
			}
		}
	}
   
	return s;
}


template <class T>
TNMatrix<T> TNMatrix<T>::Adj() const
{
	assert(mNumRows == mNumColumns);
/*	if ( mNumRows != mNumColumns )
	{
		LOG( "ERROR: Adjoin of a non-square matrix.");
	}*/

	TNMatrix<T> temp( mNumRows, mNumColumns );
	for (int i=0; i<mNumRows; ++i)
	{
		for (int j=0; j<mNumColumns; ++j)
		{
			temp(j, i) = Cofact(i, j);
		}
	}
   
	return temp;
}


template <class T>
T TNMatrix<T>::Det() const
{
	int i, j, k;
	T piv, detVal = T(1);

	assert(mNumRows == mNumColumns);
/*	if ( mNumRows != mNumColumns )
	{
		LOG( "ERROR: Determinant a non-square matrix!");
	}*/
   
	TNMatrix<T> temp( *this );
	
	for (k=0; k<mNumRows; ++k)
	{
		int indx = temp.Pivot(k);	
		if (indx == -1)
			return 0;
		
		if (indx != 0)
			detVal = - detVal;

		detVal = detVal * temp(k, k);

		for (i=k+1; i<mNumRows; ++i)
		{
			piv = temp(i, k) / temp(k, k);
			for (j=k+1; j<mNumRows; ++j)
	            temp(i, j) -= piv * temp(k, j);
		}
	}

	return detVal;
}


template<class T> T TNMatrix<T>::Norm() const
{
	T retVal = T(0);

	for (int i=0; i<mNumRows; ++i)
	{
		for (int j=0; j<mNumColumns; ++j)
		{
			retVal += (*this)(i, j) * (*this)(i, j);
		}
	}
	
	retVal = (T)Math::Sqrt(retVal);
	return retVal;
}


template <class T>
int TNMatrix<T>::Pivot(const int row)
{
	int i, k = row;
	double amax,temp;

  	amax = -1.0;
	for (i=row; i<mNumRows; ++i)
	{
		if ((temp = abs((*this)( i, row ))) > amax && temp != 0.0)
		{
			amax = temp;
			k = i;
		}
	}
	
	if ((*this)(k, row) == T(0))
		return -1;
  
	if (k != row)
	{
		T aux;
		for (i=0; i<mNumColumns; ++i)
		{
			aux = (*this)(k, i);
			(*this)(k, i) = (*this)(row, i);
			(*this)(row, i) = aux;
		}
		return k;
	}
	
	return 0;
}


template<class T> 
T TNMatrix<T>::Cofact(const int row, const int col) const
{
	assert(mNumRows == mNumColumns);
/*	if (mNumRows != mNumColumns)
	{
		LOG( "ERROR: Cofactor of a non-square matrix!");
	}*/


	assert(row < mNumRows && col < mNumColumns);
/*	if (row > mNumRows || col > mNumColumns)
	{
		LOG( "ERROR: Index out of range!");
	}*/

	TNMatrix<T> temp(mNumRows-1, mNumColumns-1);

	int i, i1, j, j1;
	for (i=i1=0; i<mNumRows; ++i)
	{
		if (i == row)
	        continue;

		for (j=j1=0; j<mNumColumns; ++j)
		{
      		if (j == col)
				continue;

      		temp(i1,j1) = (*this)(i,j);
			j1++;
		}
		i1++;
	}

	T  cof = temp.Det();	
	if ((row + col) % 2 == 1)
		cof = -cof;

	return cof;
}


template <class T> 
T TNMatrix<T>::Cond() const
{
   TNMatrix<T> inv = !(*this);
   return (Norm() * inv.Norm());
}


template <class T> 
bool TNMatrix<T>::IsSquare() const
{
	return (mNumColumns == mNumRows);
}


template <class T> 
bool TNMatrix<T>::IsSingular() const
{
	if (mNumRows != mNumColumns)
		return false;
   
	return (Det() == T(0));
}


template <class T>
bool TNMatrix<T>::IsDiagonal() const
{
	if (mNumRows != mNumColumns)
		return false;

	for (int i=0; i<mNumRows; ++i)
	{
		for (int j=0; j<mNumColumns; ++j)
		{
	        if (i != j && (*this)(i, j) != T(0))
				return false;
		}
	}

	return true;
}


template <class T>
bool TNMatrix<T>::IsScalar() const
{
	if (!IsDiagonal())
		return false;
	
	T v = *mData;	
	for (int i=0; i<mNumRows; ++i)
	{
		if ((*this)(i, i) != v)
	        return false;
	}

	return true;
}


template <class T>
bool TNMatrix<T>::IsUnit() const
{
	return (IsScalar() && *mData == T(1));
}


template <class T>
bool TNMatrix<T>::IsNull() const
{
	for (int i=0; i<mNumElements; ++i)
	{
		if (mData[i] != T(0))
			return false;
	}

	return true;
}


template <class T>
bool TNMatrix<T>::IsSymmetric() const
{
	if (mNumRows != mNumColumns)
		return false;

	for (int i=0; i<mNumRows; ++i)
	{
		for (int j=i+1; j<mNumColumns; ++j)
		{
			if ((*this)(i, j) != (*this)(j, i))
				return false;
		}
	}

	return true;
}


template<class T> 
bool TNMatrix<T>::IsSkewSymmetric() const
{
	if (mNumRows != mNumColumns)
		return false;

	for (int i=0; i<mNumRows; ++i)
	{
		for (int j=i; j<mNumColumns; ++j)
		{
			if ((*this)(i, j) != -(*this)(j, i))
				return false;
		}
	}

	return true;
}


template <class T>
bool TNMatrix<T>::IsUpperTriangular() const
{
	if (mNumRows != mNumColumns)
		return false;

	for (int i=1; i<mNumRows; ++i)
	{
		for (int j=0; j<i-1; ++j)
		{
			if ((*this)(i, j) != T(0))
				return false;
		}
	}

	return true;
}


template <class T>
bool TNMatrix<T>::IsLowerTriangular() const
{
	if (mNumRows != mNumColumns)
		return false;

	for (int i=0; i<mNumRows-1; ++i)
	{
		for (int j=i+1; j<mNumColumns; ++j)
		{
			if ((*this)(i, j) != T(0))
				return false;
		}
	}

	return true;
}


template <class T>
TNMatrix<T> ColMatrix(Array<T>& a)
{
	TNMatrix<T> m;
	int l = a.GetLength();

	m.SetSize( 1, l );
	memcpy(m.mData, a.GetPtr(), sizeof(T) * l);

	return m;
}


template <class T>
TNMatrix<T> RowMatrix(Array<T>& a)
{
	TNMatrix<T> m;
	int l = a.GetLength();

	m.SetSize(l, 1);
	memcpy(m.mData, a.GetPtr(), sizeof(T) * l); 

	return m;
}


// Temporal!!!!!!!!!!!!!
template <class T> 
TNMatrix<T> PseudoInverse(const TNMatrix<T>& m)
{
	// If (m * ~m) is singular it should be applied (singular values decomposition) SVD
	TNMatrix<T> Ps = (~m) * !( m * (~m) );

	if (Ps.GetNumColumns() == m.GetNumRows() && Ps.GetNumRows() == m.GetNumColumns())
		return Ps;
	else
		return PseudoInverseSVD( m );
}


template <class T> 
TNMatrix<T> PseudoInverseSVD(const TNMatrix<T>& m)
{
	TNMatrix<T> U;
	TNMatrix<T> S;
	TNMatrix<T> V;

	m.SVD( U, S, V );

	for (int j=0; j<S.GetNumColumns(); j++)
	{
		if (fabs(S(j, j )) > 0.0000001)
			S(j,j) = 1.0 / S(j,j);
	}
	
	return V * S * ~U;
}


template <class T> 
TNMatrix<T> PseudoInverse2(const TNMatrix<T>& m)
{
	int i, k;
	
	TNMatrix<MReal> A, Ap;
	TNMatrix<MReal> a, b, c, d;

	A = m;
	A.SetSize( A.GetNumRows(), 1 );
	a = A;

	if (a.IsNull())
		Ap = a;
	else
		Ap = ~a / (~a * a)(0,0);

	const int numRow = m.GetNumRows();
	const int numCol = m.GetNumColumns();
	for (k=1; k<numCol; ++k)
	{
		A = m; 
		A.SetSize( A.GetNumRows(), k );

		for (i=0; i<numRow; ++i)
			a(i, 0) = m(i, k);

		d = Ap * a;
		c = a - A * d;

		if (!c.IsNull())
			b = ~c / (~c * c)(0,0);
		else
			b = ( ~d * Ap ) / ( T(1) + (~d * d)(0,0) );

		Ap = Ap - (d * b);

		Ap.SetSize(Ap.GetNumRows() + 1, Ap.GetNumColumns());

		const int apCols = Ap.GetNumColumns();
		for (i=0; i<apCols; i++)
			Ap(k, i) = b(0, i);
	}

	TNMatrix<T> Q = m * Ap * m;	// TODO: eeeeh, remove? or is Ap modified by the operator * ?
	return Ap;
}


// TODO: can we remove these defines? (they are used by SVD)
#define WITHV   1
#define WITHU   2

template <class T> 
int TNMatrix<T>::SVD(TNMatrix<T>& U, TNMatrix<T>& S, TNMatrix<T>& V) const
{
	MReal* W	= new MReal[mNumColumns];
	MReal* temp	= new MReal[mNumElements];

	U.SetSize(mNumRows, mNumColumns);
	V.SetSize(mNumRows, mNumColumns);

	//svd( (MReal *)mData, mNumRows, mNumColumns, (MReal *)U.mData, W, (MReal *)V.mData, 0.00001, 3, temp );
	MReal* a = (MReal*)mData;
	int m = mNumRows;
	int n = mNumColumns;

	if (m < n)
	{
		(~(*this)).SVD(U, S, V);
		TNMatrix<T> aux = U;
		U = V;
		V = aux;

		delete[] W;
		delete[] temp;

		return 0;
	}

	MReal* u = (MReal*)U.mData;
	MReal* w = W;
	MReal* v = (MReal*)V.mData;
	MReal eps = Math::EPSILON; //(MReal)0.000000001;
	int flags = 3;

	MReal TINY = Math::EPSILON; //(MReal)0.000000000001;	/* The smallest representable value */
	int i,j,k,l,l1;
	MReal tol = TINY / eps;
	MReal c,f,g,h,s,x,y,z;

	memcpy(u, a, sizeof(MReal) * m * n);

	l = 0;	// TODO: check if this is correct

	// reduce the u matrix to bidiagonal form with Householder transforms
	g = (x = 0.0);
	for (i=0; i<n; ++i)
	{
		temp[i] = g;
		s = 0.0;
		l = i + 1;

		for (j=i; j<m; ++j)
			s += u[j * n + i] * u[j * n + i];
        
		if (s < tol) 
			g = 0.0;
		else 
		{
			f = u[i * n + i];
			g = (f < 0.0) ? Math::Sqrt(s) : -Math::Sqrt(s);
			h = f * g - s;
			u[i * n + i] = f - g;
			for (j=l; j<n; ++j) 
			{
				s = 0.0;
				for(k = i; k < m; ++k) 
					s += u[k * n + i] * u[k * n + j];

				f = s / h;
				
				for (k=i; k<m; ++k) 
					u[k * n + j] += f * u[k * n + i];
			}
		}

		w[i] = g;
		s = 0.0;

		for (j=l; j<n; ++j) 
			s += u[i * n + j] * u[i * n + j];

		if (s < tol) 
			g = 0.0;
		else 
		{
			f = u[i * n + i + 1];
			g = (f < 0.0) ? Math::Sqrt(s) : -Math::Sqrt(s);
			h = f * g - s;
			u[i * n + i + 1] = f - g;
			
			for (j=l; j<n; ++j) 
				temp[j] = u[i * n + j] / h;

			for (j=l; j<m; ++j) 
			{
				s = 0.0;
				for (k=l; k<n; ++k) 
					s += u[j * n + k] * u[i*n+k];

				for (k=l; k<n; ++k) 
					u[j*n+k] += s * temp[k];
			}
		}

		y = Math::Abs(w[i]) + Math::Abs(temp[i]);

		if (y > x) 
			x = y;                
	}

	
	// now accumulate right-hand transforms if we are building v too
	for (i=n-1; i>=0; --i) 
	{
		if (g != 0.0) 
		{
			h = u[i * n + i + 1] * g;
			for (j=l; j<n; ++j) 
				v[j * n + i] = u[i * n + j] / h;

			for (j=l; j<n; ++j) 
			{
				s = 0.0;
				for (k=l; k<n; ++k) 
					s += u[i * n + k] * v[k * n + j];

				for (k=l; k<n; ++k) 
					v[k * n + j] += s * v[k * n + i];
			}
		}
			
		for (j=l; j<n; ++j) 
			v[i * n + j] = (v[j * n + i] = 0.0);
		
		v[i * n + i] = 1.0;
		g = temp[i];
		l = i;
	}

	// now accumulate the left-hand transforms
	for (i=n-1; i>=0; --i) 
	{
		l = i + 1;
		g = w[i];
		
		for (j=l; j<n; ++j) 
			u[i * n + j] = 0.0;

		if (g != 0.0) 
		{
			h = u[i * n + i] * g;
			for (j=l; j<n; ++j) 
			{
				s = 0.0;
				for (k=l; k<m; ++k) 
					s += u[k * n + i] * u[k * n + j];

				f = s / h;
				for (k=i; k<m; ++k) 
					u[k * n + j] += f * u[k * n + i];
			}
			for (j=i; j<m; ++j) 
				u[j * n + i] /= g;
		}
		else 
		{
			for (j=i; j<m; ++j) 
				u[j * n + i] = 0.0;
		}

		u[i * n + i] += 1.0;
	}

	
	// now diagonalise the bidiagonal form. BEWARE GOTO's IN THE LOOP!! :(
	// TODO: get rid of the GOTO's. This is not Basic! :)
	eps = eps * x;

	for (k=n-1; k>=0; --k) 
	{
	testsplitting:
		for (l=k; l>=0; --l) 
		{
			if (Math::Abs(temp[l]) <= eps) 
				goto testconvergence;

			if (Math::Abs(w[l - 1]) <= eps)
				goto cancellation;
		}
	}

	// cancellation of temp[l] if l > 0;
cancellation:
	c = 0.0;
	s = 1.0;
	l1 = l - 1;
	for (i=l; i<=k; ++i) 
	{
		f = s * temp[i];
		temp[i] *= c;
		if (Math::Abs(f) <= eps) 
			goto testconvergence;
		g = w[i];
		h = (w[i] = Math::Sqrt(f * f + g * g));
		c = g/h;
		s = -f/h;
		if (flags & WITHU) 
		{
			for (j = 0; j < m; ++j) 
			{
				y = u[j * n + l1];
				z = u[j * n + i];
				u[j * n + l1] = y * c + z * s;
				u[j * n + i] = -y * s + z * c;
			}
		}
	}

testconvergence:
	z = w[k];
	if (l == k) 
		goto convergence;
	// shift from bottom 2x2 minor
	x = w[l];
	y = w[k - 1];
	g = temp[k - 1];
	h = temp[k];
	f = ((y - z)*(y + z) + (g - h)*(g + h)) / (2 * h * y);
	g = Math::Sqrt(f * f + 1);
	f = ((x - z)*(x + z) + h*(y/((f < 0.0)?f-g:f+g) - h)) / x;

	// next QR transformation
	c = (s = 1);
	for (i=l+1; i<=k; ++i) 
	{
		g = temp[i];
		y = w[i];
		h = s * g;
		g *= c;
		temp[i - 1] = (z = Math::Sqrt(f * f + h * h));
		c = f / z;
		s = h/z;
		f = x * c + g * s;
		g = -x * s + g * c;
		h = y * s;
		y *= c;
		if (flags & WITHV)
		{
			for (j=0; j<n; ++j) 
			{
				x = v[j * n + i - 1];
				z = v[j * n + i];
				v[j * n + i - 1] = x * c + z * s;
				v[j * n + i] = -x * s + z * c;
			}
		}
		w[i - 1] = (z = Math::Sqrt(f * f + h * h));
		c = f / z;
		s = h / z;
		f = c * g + s * y;
		x = -s * g + c * y;
		if (flags & WITHU) 
		{
			for (j = 0 ; j < m; ++j) 
			{
				y = u[j * n + i - 1];
				z = u[j * n + i];
				u[j * n + i - 1] = y * c + z * s;
				u[j * n + i] = -y * s + z * c;
			}
		}
	}
	temp[l] = 0.0;
	temp[k] = f;
	w[k] = x;
	goto testsplitting;

convergence:
	if (z < 0.0) 
	{
		// w[k] is made non-negative
		w[k] = -z;
		if (flags & WITHV)
		{
			for (j = 0; j < n; ++j) 
			{
				v[j * n + k] = -v[j * n + k];
			}
		}
	}

	V.SetSize( n, n );
	S.SetSize( n, n );
	for(i=0; i<n; i++)
	{
		S( i, i ) = w[i];
	}

	delete[] W;
	delete[] temp;

	return 0;
}

