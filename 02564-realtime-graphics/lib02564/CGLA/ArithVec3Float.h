#ifndef __CGLA__ARITHVEC3FLOAT_H__
#define __CGLA__ARITHVEC3FLOAT_H__

#include "ArithVecFloat.h"

namespace CGLA {

	template<class T, class V>
	class ArithVec3Float: public ArithVecFloat<T,V,3>
	{
	public:

		/// Construct a 3D float vector.
		ArithVec3Float(T a, T b, T c): ArithVecFloat<T,V,3>(a,b,c) {}

		/// Construct a 3D float vector.
		ArithVec3Float() {}

		/** Get the vector in spherical coordinates.
				The first argument (theta) is inclination from the vertical axis.
				The second argument (phi) is the angle of rotation about the vertical 
				axis. The third argument (r) is the length of the vector. */
		void get_spherical( T&, T&, T& ) const;

		/** Assign the vector in spherical coordinates.
				The first argument (theta) is inclination from the vertical axis.
				The second argument (phi) is the angle of rotation about the vertical 
				axis. The third argument (r) is the length of the vector. */
		void set_spherical( T, T, T);
		
	};

	/// Returns cross product of arguments
	template<class T, class V>
	inline V cross( const ArithVec3Float<T,V>& x, 
									const ArithVec3Float<T,V>& y ) 
	{
		return V( x[1] * y[2] - x[2] * y[1], 
							x[2] * y[0] - x[0] * y[2], 
							x[0] * y[1] - x[1] * y[0] );
	}

	/** Compute basis of orthogonal plane.
			Given a vector Compute two vectors that are orthogonal to it and 
			to each other. */
	template<class T, class V>
	void orthogonal(const ArithVec3Float<T,V>&,
									ArithVec3Float<T,V>&,
									ArithVec3Float<T,V>&);

}

#endif

