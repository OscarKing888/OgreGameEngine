uniform float4 surface_diffuse_colour;

float4 LightColor_PS(
float2 uv : TEXCOORD0,
uniform sampler2D tex0 : register(s0)) : COLOR
{
	float4 clr = tex2D(tex0, uv);
	return clr * surface_diffuse_colour;
}