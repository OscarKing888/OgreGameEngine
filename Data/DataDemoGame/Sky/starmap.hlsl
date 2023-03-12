uniform row_major float4x4 mW;
uniform row_major float4x4 mWVP;
uniform float3 vSun = 0.0;
samplerCUBE tex : register(s0);

float4 star_vs( in float3 pos : POSITION, in float3 nor : NORMAL0, in float4 clr : COLOR0,
				out float4 oClr:COLOR0, out float4 oUv : TEXCOORD0, 
				out float4 oNor : TEXCOORD1, out float4 oDir : TEXCOORD2  ): POSITION0
{
	float4x4 m = mW;
	m[3].xyzw = float4(0,0,0,1);		// bang transmition
	
	float4 N = mul( float4( nor, 1.0 ), m );
	
	oDir = float4(normalize(vSun),1.0);
	
	oClr = 1.0;
	oNor = N;
	oUv = N;
	return mul( float4(pos, 1.0), mWVP );
}

float4 star_ps( in float4 uv : TEXCOORD0,
				in float4 nor : TEXCOORD1, 
				in float4 dir : TEXCOORD2, 
				in float4 clr : COLOR0) : COLOR0
{
	float4 c = texCUBE( tex, normalize(uv) );
	
	float4 f = 0;
	if( dir.y>=0 )
		f = dot( normalize(dir.xyz), normalize(nor.xyz) ) * clamp( dir.y*1.5, 0, 1);
	else
		f = c*c.w * clamp(-dir.y*1.5,0,1);
		
	return f;
}