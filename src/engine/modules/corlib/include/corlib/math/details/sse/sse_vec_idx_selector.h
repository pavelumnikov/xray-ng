/*

  Copyright (c) 2019, Pavel Umnikov
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.

*/

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