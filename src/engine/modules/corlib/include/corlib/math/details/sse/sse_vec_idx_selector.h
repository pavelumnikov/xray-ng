// This file is a part of xray-ng engine
//

#pragma once

#include <Foundation/PC/SSE/SSEFloatInVec.h>

namespace Math
{
	/// Forward declaration
	template< typename T > class VectorIdxSelector;

	/// <summary>
	/// <c>VectorIdxSelector</c> is a vector direction selection by index for floading point
	/// data stored in <c>__m128</c> packed register.
	/// </summary>
	template<>
	__declspec( align( 16 ) )
	class VectorIdxSelector< __m128 >
	{
	private:
		__m128& vec;
		int i;

	public:
		typedef VectorIdxSelector< __m128 > ThisType;
		typedef __m128 ValueType;
		typedef __m128& Reference;
		typedef float_in_vec TypedInVec;

		VectorIdxSelector( Reference vec, int idx );
		operator float( void ) const;
		float operator = ( float scalar );
		TypedInVec operator = ( const TypedInVec& scalar );
		TypedInVec operator = ( const ThisType& rhs );
		TypedInVec operator *= ( float scalar );
		TypedInVec operator *= ( const TypedInVec& rhs );
	};

	inline VectorIdxSelector< __m128 >::VectorIdxSelector( __m128& v, int idx ) : vec( v ), i( idx )
	{
		/// Empty
	}


	/// <summary>
	/// <c>VectorIdxSelector</c> is a vector direction selection by index for integer
	/// data stored in <c>__m128i</c> packed register.
	/// </summary>
	template<>
	__declspec( align( 16 ) )
	class VectorIdxSelector< __m128i >
	{
	private:
		__m128i& vec;
		int i;

	public:
		VectorIdxSelector( __m128i& v, int idx );
	};

	inline VectorIdxSelector< __m128i >::VectorIdxSelector( __m128i& v, int idx ) : vec( v ), i( idx )
	{
		/// Empty
	}
}