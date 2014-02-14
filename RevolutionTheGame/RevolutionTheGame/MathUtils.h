#pragma once
#include <cmath>
#include <xmmintrin.h>

namespace GMath
{
    struct Vector3f;
	struct Matrix3f;
	struct Matrix4f;

	__forceinline float RSqrt(float x) {

		long i;
		float y, r;

		y = x * 0.5f;
		i = *reinterpret_cast<long *>( &x );
		i = 0x5f3759df - ( i >> 1 );
		r = *reinterpret_cast<float *>( &i );
		r = r * ( 1.5f - r * r * y );

		return r;
	}

	struct Matrix3f
	{
		float matrix[9];

		inline float& operator[](const int index)
		{
			return matrix[index];
		}

		inline Matrix3f operator*(const Matrix3f& other)
		{
			Matrix3f result;

			result.matrix[0]  = (this->matrix[0] * other.matrix[0]) + (this->matrix[1] * other.matrix[4]) + (this->matrix[2] * other.matrix[8]) + (this->matrix[3] * other.matrix[12]);
			result.matrix[1]  = (this->matrix[0] * other.matrix[1]) + (this->matrix[1] * other.matrix[5]) + (this->matrix[2] * other.matrix[9]) + (this->matrix[3] * other.matrix[13]);
			result.matrix[2]  = (this->matrix[0] * other.matrix[2]) + (this->matrix[1] * other.matrix[6]) + (this->matrix[2] * other.matrix[10]) + (this->matrix[3] * other.matrix[14]);

			result.matrix[4]  = (this->matrix[4] * other.matrix[0]) + (this->matrix[5] * other.matrix[4]) + (this->matrix[6] * other.matrix[8]) + (this->matrix[7] * other.matrix[12]);
			result.matrix[5]  = (this->matrix[4] * other.matrix[1]) + (this->matrix[5] * other.matrix[5]) + (this->matrix[6] * other.matrix[9]) + (this->matrix[7] * other.matrix[13]);
			result.matrix[6]  = (this->matrix[4] * other.matrix[2]) + (this->matrix[5] * other.matrix[6]) + (this->matrix[6] * other.matrix[10]) + (this->matrix[7] * other.matrix[14]);
	
			result.matrix[8]  = (this->matrix[8] * other.matrix[0]) + (this->matrix[9] * other.matrix[4]) + (this->matrix[10] * other.matrix[8]) + (this->matrix[11] * other.matrix[12]);
			result.matrix[9]  = (this->matrix[8] * other.matrix[1]) + (this->matrix[9] * other.matrix[5]) + (this->matrix[10] * other.matrix[9]) + (this->matrix[11] * other.matrix[13]);
			result.matrix[10] = (this->matrix[8] * other.matrix[2]) + (this->matrix[9] * other.matrix[6]) + (this->matrix[10] * other.matrix[10]) + (this->matrix[11] * other.matrix[14]);
	
			result.matrix[12] = (this->matrix[12] * other.matrix[0]) + (this->matrix[13] * other.matrix[4]) + (this->matrix[14] * other.matrix[8]) + (this->matrix[15] * other.matrix[12]);
			result.matrix[13] = (this->matrix[12] * other.matrix[1]) + (this->matrix[13] * other.matrix[5]) + (this->matrix[14] * other.matrix[9]) + (this->matrix[15] * other.matrix[13]);
			result.matrix[14] = (this->matrix[12] * other.matrix[2]) + (this->matrix[13] * other.matrix[6]) + (this->matrix[14] * other.matrix[10]) + (this->matrix[15] * other.matrix[14]);

			return result;
		}

		inline Matrix3f operator*=(const Matrix3f& other)
		{
			this->matrix[0]  = (this->matrix[0] * other.matrix[0]) + (this->matrix[1] * other.matrix[4]) + (this->matrix[2] * other.matrix[8]) + (this->matrix[3] * other.matrix[12]);
			this->matrix[1]  = (this->matrix[0] * other.matrix[1]) + (this->matrix[1] * other.matrix[5]) + (this->matrix[2] * other.matrix[9]) + (this->matrix[3] * other.matrix[13]);
			this->matrix[2]  = (this->matrix[0] * other.matrix[2]) + (this->matrix[1] * other.matrix[6]) + (this->matrix[2] * other.matrix[10]) + (this->matrix[3] * other.matrix[14]);

			this->matrix[4]  = (this->matrix[4] * other.matrix[0]) + (this->matrix[5] * other.matrix[4]) + (this->matrix[6] * other.matrix[8]) + (this->matrix[7] * other.matrix[12]);
			this->matrix[5]  = (this->matrix[4] * other.matrix[1]) + (this->matrix[5] * other.matrix[5]) + (this->matrix[6] * other.matrix[9]) + (this->matrix[7] * other.matrix[13]);
			this->matrix[6]  = (this->matrix[4] * other.matrix[2]) + (this->matrix[5] * other.matrix[6]) + (this->matrix[6] * other.matrix[10]) + (this->matrix[7] * other.matrix[14]);

			this->matrix[8]  = (this->matrix[8] * other.matrix[0]) + (this->matrix[9] * other.matrix[4]) + (this->matrix[10] * other.matrix[8]) + (this->matrix[11] * other.matrix[12]);
			this->matrix[9]  = (this->matrix[8] * other.matrix[1]) + (this->matrix[9] * other.matrix[5]) + (this->matrix[10] * other.matrix[9]) + (this->matrix[11] * other.matrix[13]);
			this->matrix[10] = (this->matrix[8] * other.matrix[2]) + (this->matrix[9] * other.matrix[6]) + (this->matrix[10] * other.matrix[10]) + (this->matrix[11] * other.matrix[14]);

			this->matrix[12] = (this->matrix[12] * other.matrix[0]) + (this->matrix[13] * other.matrix[4]) + (this->matrix[14] * other.matrix[8]) + (this->matrix[15] * other.matrix[12]);
			this->matrix[13] = (this->matrix[12] * other.matrix[1]) + (this->matrix[13] * other.matrix[5]) + (this->matrix[14] * other.matrix[9]) + (this->matrix[15] * other.matrix[13]);
			this->matrix[14] = (this->matrix[12] * other.matrix[2]) + (this->matrix[13] * other.matrix[6]) + (this->matrix[14] * other.matrix[10]) + (this->matrix[15] * other.matrix[14]);

			return *this;
		}
	};

	struct Vector3f
	{
		float x;
		float y;
		float z;

		Vector3f() :
			x(0.0f),
			y(0.0f),
			z(0.0f)
		{}

		Vector3f(float _x, float _y, float _z) :
			x(_x),
			y(_y),
			z(_z)
		{}

		inline Vector3f operator+(const Vector3f& other)
		{
			return Vector3f(this->x + other.x, this->y + other.y, this->z + other.z);
		}

		inline Vector3f operator+=(const Vector3f& other)
		{
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			return *this;
		}

		inline Vector3f operator-(const Vector3f& other)
		{
			return Vector3f(this->x - other.x, this->y - other.y, this->z - other.z);
		}

		inline Vector3f operator-=(const Vector3f& other)
		{
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			return *this;
		}

		inline Vector3f operator*(const float& value)
		{
			return Vector3f(x * value, y * value, z * value);
		}

		inline Vector3f operator*(const Matrix3f& mat)
		{
			return Vector3f(
				(x * mat.matrix[0]) + (y * mat.matrix[3]) + (z * mat.matrix[6]),
				(x * mat.matrix[1]) + (y * mat.matrix[4]) + (z * mat.matrix[7]),
				(x * mat.matrix[2]) + (y * mat.matrix[5]) + (z * mat.matrix[8]));
		}

		inline Vector3f operator*=(const Matrix3f& mat)
		{
			this->x = (x * mat.matrix[0]) + (y * mat.matrix[3]) + (z * mat.matrix[6]);
			this->y = (x * mat.matrix[1]) + (y * mat.matrix[4]) + (z * mat.matrix[7]);
			this->z = (x * mat.matrix[2]) + (y * mat.matrix[5]) + (z * mat.matrix[8]);

			return *this;
		}
	};

	struct Matrix4f
	{
		__declspec(align(16)) float matrix[16];

		__forceinline
        float& operator[](const int index)
		{
			return matrix[index];
		}

		__forceinline
        Matrix4f operator*(const Matrix4f& other)
		{
			Matrix4f result;

			result.matrix[0]  = (this->matrix[0] * other.matrix[0]) + (this->matrix[1] * other.matrix[4]) + (this->matrix[2] * other.matrix[8]) + (this->matrix[3] * other.matrix[12]);
			result.matrix[1]  = (this->matrix[0] * other.matrix[1]) + (this->matrix[1] * other.matrix[5]) + (this->matrix[2] * other.matrix[9]) + (this->matrix[3] * other.matrix[13]);
			result.matrix[2]  = (this->matrix[0] * other.matrix[2]) + (this->matrix[1] * other.matrix[6]) + (this->matrix[2] * other.matrix[10]) + (this->matrix[3] * other.matrix[14]);
			result.matrix[3]  = (this->matrix[0] * other.matrix[3]) + (this->matrix[1] * other.matrix[7]) + (this->matrix[2] * other.matrix[11]) + (this->matrix[3] * other.matrix[15]);

			result.matrix[4]  = (this->matrix[4] * other.matrix[0]) + (this->matrix[5] * other.matrix[4]) + (this->matrix[6] * other.matrix[8]) + (this->matrix[7] * other.matrix[12]);
			result.matrix[5]  = (this->matrix[4] * other.matrix[1]) + (this->matrix[5] * other.matrix[5]) + (this->matrix[6] * other.matrix[9]) + (this->matrix[7] * other.matrix[13]);
			result.matrix[6]  = (this->matrix[4] * other.matrix[2]) + (this->matrix[5] * other.matrix[6]) + (this->matrix[6] * other.matrix[10]) + (this->matrix[7] * other.matrix[14]);
			result.matrix[7]  = (this->matrix[4] * other.matrix[3]) + (this->matrix[5] * other.matrix[7]) + (this->matrix[6] * other.matrix[11]) + (this->matrix[7] * other.matrix[15]);
		
			result.matrix[8]  = (this->matrix[8] * other.matrix[0]) + (this->matrix[9] * other.matrix[4]) + (this->matrix[10] * other.matrix[8]) + (this->matrix[11] * other.matrix[12]);
			result.matrix[9]  = (this->matrix[8] * other.matrix[1]) + (this->matrix[9] * other.matrix[5]) + (this->matrix[10] * other.matrix[9]) + (this->matrix[11] * other.matrix[13]);
			result.matrix[10] = (this->matrix[8] * other.matrix[2]) + (this->matrix[9] * other.matrix[6]) + (this->matrix[10] * other.matrix[10]) + (this->matrix[11] * other.matrix[14]);
			result.matrix[11] = (this->matrix[8] * other.matrix[3]) + (this->matrix[9] * other.matrix[7]) + (this->matrix[10] * other.matrix[11]) + (this->matrix[11] * other.matrix[15]);
		
			result.matrix[12] = (this->matrix[12] * other.matrix[0]) + (this->matrix[13] * other.matrix[4]) + (this->matrix[14] * other.matrix[8]) + (this->matrix[15] * other.matrix[12]);
			result.matrix[13] = (this->matrix[12] * other.matrix[1]) + (this->matrix[13] * other.matrix[5]) + (this->matrix[14] * other.matrix[9]) + (this->matrix[15] * other.matrix[13]);
			result.matrix[14] = (this->matrix[12] * other.matrix[2]) + (this->matrix[13] * other.matrix[6]) + (this->matrix[14] * other.matrix[10]) + (this->matrix[15] * other.matrix[14]);
			result.matrix[15] = (this->matrix[12] * other.matrix[3]) + (this->matrix[13] * other.matrix[7]) + (this->matrix[14] * other.matrix[11]) + (this->matrix[15] * other.matrix[15]);

			return result;
		}

		__forceinline
        Matrix4f operator*=(const Matrix4f& other)
		{
			this->matrix[0]  = (this->matrix[0] * other.matrix[0]) + (this->matrix[1] * other.matrix[4]) + (this->matrix[2] * other.matrix[8]) + (this->matrix[3] * other.matrix[12]);
			this->matrix[1]  = (this->matrix[0] * other.matrix[1]) + (this->matrix[1] * other.matrix[5]) + (this->matrix[2] * other.matrix[9]) + (this->matrix[3] * other.matrix[13]);
			this->matrix[2]  = (this->matrix[0] * other.matrix[2]) + (this->matrix[1] * other.matrix[6]) + (this->matrix[2] * other.matrix[10]) + (this->matrix[3] * other.matrix[14]);
			this->matrix[3]  = (this->matrix[0] * other.matrix[3]) + (this->matrix[1] * other.matrix[7]) + (this->matrix[2] * other.matrix[11]) + (this->matrix[3] * other.matrix[15]);

			this->matrix[4]  = (this->matrix[4] * other.matrix[0]) + (this->matrix[5] * other.matrix[4]) + (this->matrix[6] * other.matrix[8]) + (this->matrix[7] * other.matrix[12]);
			this->matrix[5]  = (this->matrix[4] * other.matrix[1]) + (this->matrix[5] * other.matrix[5]) + (this->matrix[6] * other.matrix[9]) + (this->matrix[7] * other.matrix[13]);
			this->matrix[6]  = (this->matrix[4] * other.matrix[2]) + (this->matrix[5] * other.matrix[6]) + (this->matrix[6] * other.matrix[10]) + (this->matrix[7] * other.matrix[14]);
			this->matrix[7]  = (this->matrix[4] * other.matrix[3]) + (this->matrix[5] * other.matrix[7]) + (this->matrix[6] * other.matrix[11]) + (this->matrix[7] * other.matrix[15]);

			this->matrix[8]  = (this->matrix[8] * other.matrix[0]) + (this->matrix[9] * other.matrix[4]) + (this->matrix[10] * other.matrix[8]) + (this->matrix[11] * other.matrix[12]);
			this->matrix[9]  = (this->matrix[8] * other.matrix[1]) + (this->matrix[9] * other.matrix[5]) + (this->matrix[10] * other.matrix[9]) + (this->matrix[11] * other.matrix[13]);
			this->matrix[10] = (this->matrix[8] * other.matrix[2]) + (this->matrix[9] * other.matrix[6]) + (this->matrix[10] * other.matrix[10]) + (this->matrix[11] * other.matrix[14]);
			this->matrix[11] = (this->matrix[8] * other.matrix[3]) + (this->matrix[9] * other.matrix[7]) + (this->matrix[10] * other.matrix[11]) + (this->matrix[11] * other.matrix[15]);

			this->matrix[12] = (this->matrix[12] * other.matrix[0]) + (this->matrix[13] * other.matrix[4]) + (this->matrix[14] * other.matrix[8]) + (this->matrix[15] * other.matrix[12]);
			this->matrix[13] = (this->matrix[12] * other.matrix[1]) + (this->matrix[13] * other.matrix[5]) + (this->matrix[14] * other.matrix[9]) + (this->matrix[15] * other.matrix[13]);
			this->matrix[14] = (this->matrix[12] * other.matrix[2]) + (this->matrix[13] * other.matrix[6]) + (this->matrix[14] * other.matrix[10]) + (this->matrix[15] * other.matrix[14]);
			this->matrix[15] = (this->matrix[12] * other.matrix[3]) + (this->matrix[13] * other.matrix[7]) + (this->matrix[14] * other.matrix[11]) + (this->matrix[15] * other.matrix[15]);

			return *this;
		}

	    __forceinline
        static Matrix4f Identity()
	    {
		    Matrix4f matrix;

		    matrix[0]  = 1.0f;
		    matrix[1]  = 0.0f;
		    matrix[2]  = 0.0f;
		    matrix[3]  = 0.0f;

		    matrix[4]  = 0.0f;
		    matrix[5]  = 1.0f;
		    matrix[6]  = 0.0f;
		    matrix[7]  = 0.0f;

		    matrix[8]  = 0.0f;
		    matrix[9]  = 0.0f;
		    matrix[10] = 1.0f;
		    matrix[11] = 0.0f;

		    matrix[12] = 0.0f;
		    matrix[13] = 0.0f;
		    matrix[14] = 0.0f;
		    matrix[15] = 1.0f;

		    return matrix;
	    }

	    __forceinline
        static Matrix4f Scale(const Vector3f& scale)
	    {
		    Matrix4f matrix;

		    matrix[0]  = scale.x;
		    matrix[1]  = 0.0f;
		    matrix[2]  = 0.0f;
		    matrix[3]  = 0.0f;

		    matrix[4]  = 0.0f;
		    matrix[5]  = scale.y;
		    matrix[6]  = 0.0f;
		    matrix[7]  = 0.0f;

		    matrix[8]  = 0.0f;
		    matrix[9]  = 0.0f;
		    matrix[10] = scale.z;
		    matrix[11] = 0.0f;

		    matrix[12] = 0.0f;
		    matrix[13] = 0.0f;
		    matrix[14] = 0.0f;
		    matrix[15] = 1.0f;

		    return matrix;
	    }
	};

	struct Vector2f
	{
		float tu;
		float tv;

		Vector2f(float _tu, float _tv) :
			tu(_tu),
			tv(_tv)
		{}

		inline Vector2f operator+(const Vector2f& other)
		{
			return Vector2f(this->tu + other.tu, this->tv + other.tv);
		}

		inline Vector2f operator+=(const Vector2f& other)
		{
			this->tu += other.tu;
			this->tv += other.tv;
			return *this;
		}

		inline Vector2f operator-(const Vector2f& other)
		{
			return Vector2f(this->tu - other.tu, this->tv - other.tv);
		}

		inline Vector2f operator-=(const Vector2f& other)
		{
			this->tu -= other.tu;
			this->tv -= other.tv;
			return *this;
		}
	};

	struct Vector4f
	{
		//__m128 vec;
		__declspec(align(16))
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};

		Vector4f() :
			x(0.0f),
			y(0.0f),
			z(0.0f),
			w(0.0f)
		{}

		Vector4f(float _x, float _y, float _z, float _w) :
			x(_x),
			y(_y),
			z(_z),
			w(_w)
		{}

		inline Vector4f operator+(const Vector4f& other)
		{
			return Vector4f(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w);
		}

		inline Vector4f operator+=(const Vector4f& other)
		{
			__m128 *me = (__m128 *)this;
			__m128 *right = (__m128 *)&other;

			*me = _mm_add_ps(*me, *right);
			return *this;
		}
	};

	struct VertexRgb3f
	{
		Vector3f pos;
		Vector3f col;
	};

	struct Vertex
	{
		Vector3f pos;
		Vector3f nor;
		Vector2f tex;
		Vector3f tangent;
		Vector3f bitangent;

		Vertex(Vector3f _pos, Vector3f _nor, Vector2f _tex) :
			pos(_pos),
			nor(_nor),
			tex(_tex)
		{}
	};


	inline float Dot(const Vector3f& left, const Vector3f& right)
	{
		// a · b = ax * bx + ay * by + az * bz
		return (left.x * right.x + left.y * right.y + left.z * right.z);
	}

	inline Vector3f Cross(const Vector3f& left, const Vector3f& right)
	{
		//cx = aybz - azby
		//cy = azbx - axbz
		//cz = axby - aybx
		return Vector3f(left.y * right.z - left.z * right.y,
						left.z * right.x - left.x * right.z,
						left.x * right.y - left.y * right.x);
	}

	inline Vector3f Normalize(const Vector3f& vec)
	{
		float invLen = RSqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
		return Vector3f(vec.x * invLen, vec.y * invLen, vec.z * invLen);
	}

	inline Matrix4f Translate(const Vector3f& pos)
	{
		Matrix4f matrix;

		matrix[0]  = 1.0f;
		matrix[1]  = 0.0f;
		matrix[2]  = 0.0f;
		matrix[3]  = 0.0f;

		matrix[4]  = 0.0f;
		matrix[5]  = 1.0f;
		matrix[6]  = 0.0f;
		matrix[7]  = 0.0f;

		matrix[8]  = 0.0f;
		matrix[9]  = 0.0f;
		matrix[10] = 1.0f;
		matrix[11] = 0.0f;

		matrix[12] = pos.x;
		matrix[13] = pos.y;
		matrix[14] = pos.z;
		matrix[15] = 1.0f;

		return matrix;
	}

	inline Matrix4f MatrixRotationY(float angle)
	{
		Matrix4f matrix;

		matrix[0]  = cosf(angle);
		matrix[1]  = 0.0f;
		matrix[2]  = -sinf(angle);
		matrix[3]  = 0.0f;

		matrix[4]  = 0.0f;
		matrix[5]  = 1.0f;
		matrix[6]  = 0.0f;
		matrix[7]  = 0.0f;

		matrix[8]  = sinf(angle);
		matrix[9]  = 0.0f;
		matrix[10] = cosf(angle);
		matrix[11] = 0.0f;

		matrix[12] = 0.0f;
		matrix[13] = 0.0f;
		matrix[14] = 0.0f;
		matrix[15] = 1.0f;

		return matrix;
	}

	inline Matrix4f BuildPerspectiveFovLHMatrix(float fieldOfView, float screenAspect, float screenNear, float screenDepth)
	{
		Matrix4f matrix;

		matrix[0] = 1.0f / (screenAspect *tan(fieldOfView * 0.5f));
		matrix[1] = 0.0f;
		matrix[2] = 0.0f;
		matrix[3] = 0.0f;

		matrix[4] = 0.0f;
		matrix[5] = 1.0f / tan(fieldOfView * 0.5f);
		matrix[6] = 0.0f;
		matrix[7] = 0.0f;

		matrix[8] = 0.0f;
		matrix[9] = 0.0f;
		matrix[10] = screenDepth / (screenDepth - screenNear);
		matrix[11] = 1.0f;

		matrix[12] = 0.0f;
		matrix[13] = 0.0f;
		matrix[14] = (-screenNear * screenDepth) / (screenDepth - screenNear);
		matrix[15] = 0.0f;

		return matrix;
	}
} // End of GMath