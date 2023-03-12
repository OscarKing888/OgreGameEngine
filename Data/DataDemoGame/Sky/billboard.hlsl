//
// this shader only works on ogre
// the matrix form and rowcolumn are great differed from directX
// 

uniform row_major float4x4 mW;
uniform row_major float4x4 mV;
uniform row_major float4x4 mP;

uniform float4 vVP;

float4 main( float4 pos : POSITION0, 
			 in float2 uv : TEXCOORD0,
			 out float2 oUV : TEXCOORD0, out float4 oClr : COLOR0 ) : POSITION0
{
	float4x4 m = 
	{
		1,0,0,0,
		0,0,-1,0,
		0,1,0,0,
		0,0,0,1,
	};
	
	float4x4 _mW;
	_mW[0] = mW[0]*float4(1,0,0,0);
	_mW[1] = mW[1]*float4(0,1,0,0);
	_mW[2] = mW[2]*float4(0,0,1,0);
	_mW[3] = mW[3]*float4(0,0,0,1);
	
	pos = mul(mul( pos, _mW ),m);

	float4 p = float4(mW[3].xyz, 1.0);
	p = mul( p, mV );
	
	m = mV;	// bang all the transform on xyz
	m[0].xyz = float3( 1.0, 0.0, 0.0 );
	m[1].xyz = float3( 0.0, 1.0, 0.0 );
	m[2].xyz = float3( 0.0, 0.0, 1.0 );
	m[3].xyz = float3( 0.0, 0.0, 0.0 );
	
	float4 v = mul( float4(pos.xyz+p.xyz, p.w), m );
		
	oUV = uv;
	oClr = 1.0;
	return mul( v, mP );
} 