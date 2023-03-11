// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-08-24
// *************************************************************************************

#pragma once
#include "EngineExport.h"
#include "EnginePrerequisites.h"

/**
*	This class is the Perlin noise.
*	What is Perlin noise: Perlin noise is a noise function (seeded random
*	number generator) which is just several Interpolated Noise functions at different
*	scales/frequencies added together. 
*/
class _EngineExport PerlinNoise
{
public:
	//herein lies Perlin's original implementation
	/* coherent noise function over 1, 2 or 3 dimensions */
	/* (copyright Ken Perlin) */
	void init(void);
	float noise1( float arg );
	float noise2( const Vector2& vec );
	float noise3( float vec[3] );
	void normalize2( float v[2] );
	void normalize3( float v[3] );

	// sumHarmonics2( input, 0.75f, frequency, 3.f );
	float sumHarmonics2(const Vector2& vec, float persistence,
		float frequency, float nIterations);	
};

//--------------------------------------------------------------------------------------

class _EngineExport MathUtil
{
public:

	template<class T>
	T Clamp(T val, T absVal)
	{
		Math::Clamp(val, -absVal, absVal);
	}

	static bool IsPowerOfTwo(int val)
	{
		return !(val & (val - 1));
	}

	// 将一个值从一个范围(A)内投影到另一个范围内(B)
	static float ProjectValueWithRange(float value, float minA, float maxA, float minB, float maxB)
	{
		return (value - minA) / (maxA - minA) * (maxB - minB) + minB;
	}

	/// returns true if in front of the cam, and fills x,y with clamped screencoords in [-1;1]
	// cam->getProjectionMatrix() is cached inside ogre
	static bool	ProjectPos(Camera* cam, const Vector3& pos, Real& x, Real& y) ;

	/**
	*  This function linearly interpolates from a source range into a
	*  destination range.
	*
	*  @param x            The value to interpolate.
	*  @param src_a        The source range minimum.
	*  @param src_b        The source range maximum.
	*  @param dst_a        The destination range minimum.
	*  @param dst_b        The destination range maximum.
	*
	*  @returns            The interpolated value of x in the source range
	*                      in the destination range.  For example if 
	*                      x = src_a then the result is dst_b, if x is the
	*                      mid point of [src_a, src_b] then the result
	*                      is the mid point of [dst_a, dst_b].
	*/
	template<typename SRC, typename DST>
	static inline DST 
		Lerp
		(
		SRC                 const &x, 
		SRC                 const &src_a, 
		SRC                 const &src_b, 
		DST                 const &dst_a,
		DST                 const &dst_b    
		)
	{
		return (DST)((dst_b - dst_a)*(x - src_a)/(src_b - src_a)) + dst_a;
	}


	/**
	*  This function also linearly interpolates from a source range into a
	*  destination range.  It is slightly slower than Lerp because it checks
	*	for the degenerate case in the source in which case it returns dsa_t.
	*
	*  @param x            The value to interpolate.
	*  @param src_a        The source range minimum.
	*  @param src_b        The source range maximum.
	*  @param dst_a        The destination range minimum.
	*  @param dst_b        The destination range maximum.
	*
	*  @returns            The interpolated value of x in the source range
	*                      in the destination range.  For example if 
	*                      x = src_a then the result is dst_b, if x is the
	*                      mid point of [src_a, src_b] then the result
	*                      is the mid point of [dst_a, dst_b].
	*/
	template<typename SRC, typename DST>
	static inline DST 
		SafeLerp
		(
		SRC                 const &x, 
		SRC                 const &src_a, 
		SRC                 const &src_b, 
		DST                 const &dst_a,
		DST                 const &dst_b    
		)
	{
		if (src_a == src_b)
			return dst_a;
		else
			return Lerp(x, src_a, src_b, dst_a, dst_b);
	}


	/**
	*  Linearly interpolate over an interval.
	*
	*  @param t            The t-value to interpolate with (typically in
	*                      [0, 1]).
	*  @param a            The lower value of the range.
	*  @param b            The upper value of the range.
	*
	*  @returns            a + t(b - a).
	*/
	template<typename TYPE>
	static inline TYPE
		Lerp
		(
		TYPE                const &t,
		TYPE                const &a,
		TYPE                const &b
		)
	{
		return a + t*(b - a);
	}

	typedef std::vector<double>	FloatVector;

	template<class T>
	class TMatrix : public std::vector< std::vector<T> >
	{
	public:

		typedef typename std::vector<T>	Vector;

		TMatrix()
		{
		}

		// m行, n列
		TMatrix(size_t m, size_t n, const T* pValMatrix)
		{
			clear();
			for (size_t i = 0; i < m; ++i)
			{
				Vector v;
				for (size_t j = 0; j < n; ++j)
				{
					v.push_back(*(pValMatrix + i * n + j));
				}
				push_back(v);
			} // for
		}

		// m行, n列 初始化为0
		TMatrix(size_t m, size_t n)
		{
			clear();
			Vector v;
			v.resize(n, 0);
			resize(m, v);
		}
	};

	typedef TMatrix<double>	FloatMatrix;

	// 线性方程组求解 高斯消元法
	static FloatVector LinearSolve_Gauss(const FloatMatrix& mat);

	// 线性方程组求解 列主元素法
	static FloatVector LineSolve_MainCol(const FloatMatrix& mat);

	static AxisAlignedBox GetBoundingBox(SceneNode* pNode, bool worldBound);

	static void _GetBoundingBoxRecursive(SceneNode* pNode, AxisAlignedBox& box, bool worldBound);

	static void ShowBoundingBoxRecursive(SceneNode* pNode, bool show);

	// 返回角度值，y-yaw x-pitch z-roll
	static Vector3 ToAxisOrientation(const Quaternion& q);

	// 根据弧度值生成Quaternion
	static Quaternion MakeQuaternion(float yaw, float pitch, float roll);
	static Quaternion MakeQuaternion(const Degree& yaw, const Degree& pitch, const Degree& roll);

};

