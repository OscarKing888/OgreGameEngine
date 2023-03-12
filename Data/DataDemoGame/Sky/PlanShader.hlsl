uniform row_major float4x4 mWVP;

float4 main( float4 pos : POSITION0, float2 uv : TEXCOORD0, 
			 out float2 oUV : TEXCOORD0, out float4 oClr : COLOR0 ) : POSITION0
{
	float4 p = pos/750.0f;
	pos.y +=  -0.2*dot(p.xz, p.xz)*750.0f;
	oUV = uv;
	oClr = 1.0;
	return mul( pos, mWVP );
} 