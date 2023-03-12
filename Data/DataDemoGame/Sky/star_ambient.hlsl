uniform float4x4 worldviewproj;

float4 star_vs( float3 pos : POSITION, 
				float3 nor : NORMAL,
				float2 uv : TEXCOORD0 ,
				out float2 uv0 : TEXCOORD0,
				out float3 uv1 : TEXCOORD1 ) : POSITION
{
	uv0 = uv;
	uv1 = pos;
	return mul(worldviewproj, float4(pos, 1));
}



sampler2D _map : register(s0);
sampler2D _noi : register(s1);
uniform float4 vClr;
uniform float fTime;

float4 star_ps( float2 uv0 : TEXCOORD0,
				float3 uv1 : TEXCOORD1) : COLOR0
{
	float3 n = normalize(uv1);
	float4 c0 = tex2D( _noi, 8.0*n.xz + float2( fTime, 0 ) );
	float4 c1 = tex2D( _noi, 16.0*n.xz + float2( 0, fTime ) );
	c0.w = c1.w = 1.0;
	float4 c = tex2D( _map, uv0 );
	return c*c.w*vClr*(2.0*(c0+c1) - 1.0);
}