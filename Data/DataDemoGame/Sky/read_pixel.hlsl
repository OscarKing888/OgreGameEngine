// read_pixel.hlsl

sampler Tex0 : register(s0);
sampler Tex1 : register(s1);

struct PS_OUT
{
	float4 clr0 : COLOR0;
	float4 clr1 : COLOR1;
};

PS_OUT main( in float2 uv : TEXCOORD0 )
{
	PS_OUT o;
	o.clr0 = tex2D( Tex0, uv )*c;
	o.clr1 = clr;
	return o;
}