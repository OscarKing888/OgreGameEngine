uniform float4 vOffset;
uniform float fBrightness;

sampler2D baseMap : register(s0);
//sampler2D maskMap : register(s1);


float4 moon_ps( float2 uv : TEXCOORD0 ) : COLOR0
{
   float4 c0 = tex2D( baseMap, uv );
   //float4 c1 = tex2D( maskMap, uv );
   
   float2 vOffsetUV = clamp( uv+vOffset, 0, 1 );

   float4 d0 = tex2D( baseMap, vOffsetUV );
   //float4 d1 = tex2D( maskMap, vOffsetUV );
   
   float m0 = c0.w * (1-d0.w);
   //float m1 = c1.w * (1-d1.w);
   
   return float4((m0*c0).xyz*fBrightness,1);//(m1 + (m0*c0))*fBrightness;
   
}