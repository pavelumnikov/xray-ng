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

#include <Foundation/Shared.h>
#include <Foundation/PC/SSE/SSEFloatInVec.h>
#include <Foundation/PC/SSE/SSEMathVector3.h>

namespace Math
{
	/// Forward declaration
	class Vector4;
	class Matrix3;

	/// <summary>
	/// <c>Quaternion</c> is a generalized mathematic quaternion implementation.
	/// </summary>
	__declspec( align( 16 ) )
	class Quaternion
	{
	public:
		Quaternion( void );
		Quaternion( float qx, float qy, float qz, float qw );
		Quaternion( const float_in_vec& qx, const float_in_vec& qy, const float_in_vec& qz, const float_in_vec& qw );
		Quaternion( const Vector3& xyz, float qw );
		Quaternion( const Vector3& xyz, const float_in_vec& qw );
		Quaternion( const Vector4& rhs );
		Quaternion( const Matrix3& rotation );
		Quaternion( const float scalar );
		Quaternion( const float_in_vec& scalar );
		Quaternion( __m128 vec );
		Quaternion( const Quaternion& rhs );

		Quaternion& operator = ( const Quaternion& rhs );
		Quaternion& operator = ( const Vector3& rhs );

		Quaternion& set_x( float qx );
		Quaternion& set_x( const float_in_vec& qx );
		Quaternion& set_y( float qy );
		Quaternion& set_y( const float_in_vec& qy );
		Quaternion& set_z( float qz );
		Quaternion& set_z( const float_in_vec& qz );
		Quaternion& set_w( float qw );
		Quaternion& set_w( const float_in_vec& qw );

		Quaternion& SetElement( int element, float scalar );
		Quaternion& SetElement( int element, const float_in_vec& scalar );
		const float_in_vec GetElement( int element ) const;

		const float_in_vec GetX( void ) const;
		const float_in_vec GetY( void ) const;
		const float_in_vec GetZ( void ) const;
		const float_in_vec GetW( void ) const;

		const Vector3 GetVector3( void ) const;
	};
}