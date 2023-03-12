sampler2D _ambient : register(s0);
uniform float fTime;

float4 skycap_ambient_ps( float2 uv : TEXCOORD0 ) : COLOR0
{
	uv.x = fTime;
	return tex2D( _ambient, uv );
}