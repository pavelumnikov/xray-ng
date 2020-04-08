// This file is a part of xray-ng engine
//

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