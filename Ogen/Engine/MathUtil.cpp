// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-11-20
// *************************************************************************************

#include "stdafx.h"
#include "MathUtil.h"

MathUtil::FloatVector MathUtil::LinearSolve_Gauss(const MathUtil::FloatMatrix& mat)
{
	FloatMatrix a = mat;

	assert(!a.empty());
	size_t n = a.size();//.front().size();

	FloatVector x;
	x.resize(n, 0.0f);

	for (size_t k = 0; k < n - 1; ++k)
	{
		//k--;

		if(a[k][k] == 0.0f)
		{
			assert(false && ("Can't solve main column wiht '0'!"));
			return x;
		}

		for (size_t i = k + 1; i < n; ++i)
		{
			a[i][k] = a[i][k] / a[k][k];

			for (size_t j = k + 1; j < n + 1; ++j)
			{
				a[i][j] = a[i][j] - a[i][k] * a[k][j];
			}
		}
	}

	--n;

	if(a[n][n] == 0.0f)
	{
		assert(false && ("a[n][n] == 0"));
		return x;
	}

	x[n] = a[n][n + 1] / a[n][n];

	//++n;

	for (int i = (int)n/* - 1*/; i >= 0; --i)
	{
		double s = 0.0f;

		for (size_t j = i + 1; j <= n; ++j)
		{
			s += a[i][j] * x[j];
		}

		x[i] = (a[i][n + 1] - s) / a[i][i];
	}

	return x;
}

MathUtil::FloatVector MathUtil::LineSolve_MainCol( const MathUtil::FloatMatrix& matrix )
{
	assert(!matrix.empty());
	FloatMatrix mat = matrix;		
	int rowNum = (int)mat.size();
	int colNum = (int)mat.front().size();

	int i = 0;
	int j = 0;
	int changeIndex = 0;
	for (i = 0; i < rowNum; i ++)//行
	{
		//====行交换
		double temp = 0.0f;
		changeIndex = i;
		double tempColMaxNum = mat[i][j];
		for (int k = i; k < rowNum; k ++)//找出前面一个值最大的行
		{
			temp = (fabs(tempColMaxNum) > fabs(mat[k][j])) ? tempColMaxNum : mat[k][j];
			if (tempColMaxNum != temp)
			{
				tempColMaxNum = temp;
				changeIndex = k;
			}
		}
		for (int l = 0; l < colNum; l ++)//与运算过的最后一行交换
		{
			std::swap(mat[i][l], mat[changeIndex][l]);
		}
		//===========

		//===开始消元
		double ratio = 0.0f;
		for (int m = i + 1; m < rowNum; m ++)
		{
			if(mat[m][j] == 0)
				continue;

			ratio = - mat[m][j] / mat[i][j];

			for (int n = 0; n < colNum; n ++)
			{
				double tempNum = mat[m][n] + mat[i][n] * ratio;
				mat[m][n] = tempNum;
			}
		}

		j ++;
	}

	FloatVector fReturnVector;
	fReturnVector.resize(colNum, 0.0f);
	//消元结束，开始回代，计算结果
	for (int m = rowNum - 1; m >= 0; m --)
	{
		double total = 0.0f;
		for (int n = m + 1; n < rowNum; n ++)
		{
			total += fReturnVector[n] * mat[m][n];
		}
		fReturnVector[m] = (mat[m][colNum - 1] - total) / mat[m][m];
	}
	return fReturnVector;
}

bool MathUtil::ProjectPos(Camera* cam, const Vector3& pos, Real& x, Real& y) 
{
#if 0
	Vector3 hcsPosition = cam->getProjectionMatrix() * (cam->getViewMatrix() * pos);

	//x = 1.0f - ((hcsPosition.x * 0.5f) + 0.5f);// 0 <= x <= 1 // left := 0,right := 1
	//y = ((hcsPosition.y * 0.5f) + 0.5f);// 0 <= y <= 1 // bottom := 0,top := 1

	x = hcsPosition.x;
	y = hcsPosition.y;

	return true;
#else
	Vector3 eyeSpacePos = cam->getViewMatrix(true) * pos;
	// z < 0 means in front of cam
	if (eyeSpacePos.z < 0)
	{
		// calculate projected pos
		Vector3 screenSpacePos = cam->getProjectionMatrix() * eyeSpacePos;
		x = screenSpacePos.x;
		y = screenSpacePos.y;

		/*
		x = ((x * 0.5) + 0.5);
		y = 1.0 - (y * 0.5f + 0.5f);
		*/

		return true;
	}
	else
	{
		x = (-eyeSpacePos.x > 0) ? -1 : 1;
		y = (-eyeSpacePos.y > 0) ? -1 : 1;
		return false;
	}
#endif
}

AxisAlignedBox MathUtil::GetBoundingBox(SceneNode* pNode, bool worldBound)
{
	AxisAlignedBox box;
	box.setNull();

	_GetBoundingBoxRecursive(pNode, box, worldBound);
	return box;
}

void MathUtil::_GetBoundingBoxRecursive(SceneNode* pNode, AxisAlignedBox& box, bool worldBound)
{
	// 先得取挂上的物体的边界
	SceneNode::ObjectIterator entityIter = pNode->getAttachedObjectIterator();

	while (entityIter.hasMoreElements())
	{
		MovableObject* pObj = entityIter.peekNextValue();

		entityIter.moveNext();

		if(pObj)
		{
			if(worldBound)
			{
				box.merge(pObj->getWorldBoundingBox(true));
			}
			else
			{
				box.merge(pObj->getBoundingBox());
			}
		}
	}

	// 再递归所有的子节点
	SceneNode::ChildNodeIterator nodeIter = pNode->getChildIterator();
	while (nodeIter.hasMoreElements())
	{
		SceneNode* pSNode = dynamic_cast<SceneNode*>(nodeIter.peekNextValue());

		nodeIter.moveNext();

		if(pSNode)
		{
			_GetBoundingBoxRecursive(pSNode, box, worldBound);
		}
	}
}

void MathUtil::ShowBoundingBoxRecursive(SceneNode* pNode, bool show)
{
	pNode->showBoundingBox(show);
	// 再递归所有的子节点
	SceneNode::ChildNodeIterator nodeIter = pNode->getChildIterator();
	while (nodeIter.hasMoreElements())
	{
		SceneNode* pSNode = dynamic_cast<SceneNode*>(nodeIter.peekNextValue());

		nodeIter.moveNext();

		if(pSNode)
		{
			ShowBoundingBoxRecursive(pSNode, show);
		}
	}
}

Vector3 MathUtil::ToAxisOrientation(const Quaternion& q)
{
	Matrix3 mat;
	q.ToRotationMatrix(mat);

	Radian y, p, r;
	mat.ToEulerAnglesYXZ(y, p, r);

	Vector3 o;
	o.y = y.valueDegrees();
	o.x = p.valueDegrees();
	o.z = r.valueDegrees();

	return o;
}

Quaternion MathUtil::MakeQuaternion(float yaw, float pitch, float roll)
{
	Matrix3 mat;
	// yaw	pitch	roll
	// y	x		z
	// up	right	dir
	mat.FromEulerAnglesYXZ(Radian(yaw), Radian(pitch), Radian(roll));
	return Quaternion(mat);
}

Quaternion MathUtil::MakeQuaternion(const Degree& yaw, const Degree& pitch, const Degree& roll)
{
	return MakeQuaternion(yaw.valueRadians(), pitch.valueRadians(), roll.valueRadians());
}

//--------------------------------------------------------------------------------------
// PerlinNoise
//--------------------------------------------------------------------------------------
#define B 0x100
#define BM 0xff

static int p[B + B + 2];
static float g3[B + B + 2][3];
static float g2[B + B + 2][2];
static float g1[B + B + 2];
static bool start = true;

#define N 0x1000
#define NP 12   /* 2^N */
#define NM 0xfff

#define s_curve(t) ( t * t * (3.0f - 2.0f * t) )

#define setup(i,b0,b1,r0,r1)\
	t = vec[i] + N;\
	b0 = ((int)t) & BM;\
	b1 = (b0+1) & BM;\
	r0 = t - (int)t;\
	r1 = r0 - 1.0f;


float PerlinNoise::noise1( float arg )
{
	int bx0, bx1;
	float rx0, rx1, sx, t, u, v, vec[1];

	vec[0] = arg;
	if (start) {
		start = false;
		init();
	}

	setup(0, bx0,bx1, rx0,rx1);

	sx = s_curve(rx0);

	u = rx0 * g1[ p[ bx0 ] ];
	v = rx1 * g1[ p[ bx1 ] ];

	return MathUtil::Lerp(sx, u, v);
}


float PerlinNoise::noise2( const Vector2 & vec )
{
	int bx0, bx1, by0, by1, b00, b10, b01, b11;
	float rx0, rx1, ry0, ry1, *q, sx, sy, a, b, t, u, v;
	register int i, j;

	if (start) {
		start = false;
		init();
	}

	setup(0, bx0,bx1, rx0,rx1);
	setup(1, by0,by1, ry0,ry1);

	i = p[ bx0 ];
	j = p[ bx1 ];

	b00 = p[ i + by0 ];
	b10 = p[ j + by0 ];
	b01 = p[ i + by1 ];
	b11 = p[ j + by1 ];

	sx = s_curve(rx0);
	sy = s_curve(ry0);

#define at2(rx,ry) ( rx * q[0] + ry * q[1] )

	q = g2[ b00 ] ; u = at2(rx0,ry0);
	q = g2[ b10 ] ; v = at2(rx1,ry0);
	a = MathUtil::Lerp(sx, u, v);

	q = g2[ b01 ] ; u = at2(rx0,ry1);
	q = g2[ b11 ] ; v = at2(rx1,ry1);
	b = MathUtil::Lerp(sx, u, v);

	return MathUtil::Lerp(sy, a, b);
}


float PerlinNoise::noise3( float vec[3] )
{
	int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
	float rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
	register int i, j;

	if (start) {
		start = false;
		init();
	}

	setup(0, bx0,bx1, rx0,rx1);
	setup(1, by0,by1, ry0,ry1);
	setup(2, bz0,bz1, rz0,rz1);

	i = p[ bx0 ];
	j = p[ bx1 ];

	b00 = p[ i + by0 ];
	b10 = p[ j + by0 ];
	b01 = p[ i + by1 ];
	b11 = p[ j + by1 ];

	t  = s_curve(rx0);
	sy = s_curve(ry0);
	sz = s_curve(rz0);

#define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

	q = g3[ b00 + bz0 ] ; u = at3(rx0,ry0,rz0);
	q = g3[ b10 + bz0 ] ; v = at3(rx1,ry0,rz0);
	a = MathUtil::Lerp(t, u, v);

	q = g3[ b01 + bz0 ] ; u = at3(rx0,ry1,rz0);
	q = g3[ b11 + bz0 ] ; v = at3(rx1,ry1,rz0);
	b = MathUtil::Lerp(t, u, v);

	c = MathUtil::Lerp(sy, a, b);

	q = g3[ b00 + bz1 ] ; u = at3(rx0,ry0,rz1);
	q = g3[ b10 + bz1 ] ; v = at3(rx1,ry0,rz1);
	a = MathUtil::Lerp(t, u, v);

	q = g3[ b01 + bz1 ] ; u = at3(rx0,ry1,rz1);
	q = g3[ b11 + bz1 ] ; v = at3(rx1,ry1,rz1);
	b = MathUtil::Lerp(t, u, v);

	d = MathUtil::Lerp(sy, a, b);

	return MathUtil::Lerp(sz, c, d);
}

void PerlinNoise::normalize2(float v[2])
{
	float s;

	s = sqrtf(v[0] * v[0] + v[1] * v[1]);
	v[0] = v[0] / s;
	v[1] = v[1] / s;
}

void PerlinNoise::normalize3( float v[3] )
{
	float s;

	s = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] = v[0] / s;
	v[1] = v[1] / s;
	v[2] = v[2] / s;
}

void PerlinNoise::init( void )
{
	int i, j, k;

	for (i = 0 ; i < B ; i++) {
		p[i] = i;

		g1[i] = (float)((rand() % (B + B)) - B) / B;

		for (j = 0 ; j < 2 ; j++)
			g2[i][j] = (float)((rand() % (B + B)) - B) / B;
		normalize2(g2[i]);

		for (j = 0 ; j < 3 ; j++)
			g3[i][j] = (float)((rand() % (B + B)) - B) / B;
		normalize3(g3[i]);
	}

	while (--i) {
		k = p[i];
		p[i] = p[j = rand() % B];
		p[j] = k;
	}

	for (i = 0 ; i < B + 2 ; i++) {
		p[B + i] = p[i];
		g1[B + i] = g1[i];
		for (j = 0 ; j < 2 ; j++)
			g2[B + i][j] = g2[i][j];
		for (j = 0 ; j < 3 ; j++)
			g3[B + i][j] = g3[i][j];
	}
}


/**
*	Produces a weighted sum of multiple harmonics of basic Perlin noise.
*	It tries to return a value between 0.0 and 1.0
*
*	@param	vec				the input coordinates
*	@param	persistence		the cumulative relative weighting for each harmonic.
*	@param	frequency		the frequency of the harmonics
*	@param	nIterations		the number of harmonics to blend
*/
float PerlinNoise::sumHarmonics2(const Vector2& vec, float persistence, float frequency, float nIterations)
{
	Vector2 curr(vec);
	curr *= frequency;
	Vector2 norm(vec);
	norm.normalise();
	norm *= frequency;

	float sum = 0.f;
	float scale = 1.f;
	float i = 0.f;

	while (i < nIterations)
	{
		sum += (scale * this->noise2(curr));
		scale *= persistence;
		norm += norm;	//1, 2, 4, 8, 16 etc.
		i += 1.f;
		curr += norm;
	}

	return sum + 0.5f;
}

