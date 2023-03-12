uniform row_major float4x4 wvp_matrix;

float4 star_vs( float3 pos : POSITION0, 
				float3 nor : NORMAL0,
				float2 uv : TEXCOORD0 ,
				out float2 uv0 : TEXCOORD0 ) : POSITION
{
	uv0 = uv;
	return mul(float4(pos, 1), wvp_matrix);
}


sampler2D _map : register(s0);
sampler2D _noi : register(s1);
uniform float4 vClr;
uniform float fTime;

float4 star_ps( float2 uv0 : TEXCOORD0) : COLOR0
{
	float4 c0 = tex2D( _noi, 08.0*uv0 + float2( fTime, 0 ) );
	float4 c1 = tex2D( _noi, 16.0*uv0 + float2( 0, fTime ) );
	c0.w = c1.w = 1.0;
	float4 c = tex2D( _map, uv0 );
	return c*c.w*vClr*(2.0*(c0+c1) - 1.0);
}