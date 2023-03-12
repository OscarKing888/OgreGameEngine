uniform row_major float4x4 wvp_matrix;

float4 vault_vs( in float3 pos : POSITION0,
				 in float3 nor : NORMAL0,
				 in float4 clr : COLOR0,
				 in float2 uv0 : TEXCOORD0,
				 in float3 uv1 : TEXCOORD1,
				 
				 out float3 ouv : TEXCOORD0 ) : POSITION0
{
	ouv = pos;
	return mul( float4(pos, 1.0), wvp_matrix );
}


sampler2D Tex : register(s0);
uniform float fTime;

float4 vault_ps( float3 uv : TEXCOORD0 ) : COLOR0
{
	float2 n;
	float v = 1-dot( normalize(uv.xyz), float3(0.0, 1.0, 0.0) );
	n.x = fTime;
	n.y = clamp(v, 0, 1);
	return tex2D( Tex, n );
}