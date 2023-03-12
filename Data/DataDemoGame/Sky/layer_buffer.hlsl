// layer_buffer.hlsl

struct PS_OUT
{
	float4 clr0 : COLOR0;
	float4 clr1 : COLOR1;
};

sampler Tex0 : register(s0);

PS_OUT main( in float2 uv : TEXCOORD0, in float4 c : COLOR0, uniform float4 clr )
{
	PS_OUT o;
	o.clr0 = tex2D( Tex0, uv )*c;
	o.clr1 = clr;
	return o;
}