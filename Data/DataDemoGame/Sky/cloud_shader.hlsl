uniform row_major float4x4 wvp_matrix;

float4 cloud_vs( in float3 pos : POSITION0,
				 in float3 nor : NORMAL0,
				 in float4 clr : COLOR0,
				 in float2 uv0 : TEXCOORD0,
				 in float3 uv1 : TEXCOORD1,
				 
				 out float3 ouv : TEXCOORD0 ) : POSITION0
{
	ouv = pos;
	return mul( float4(pos, 1.0), wvp_matrix );
}
				 



sampler3D Noise : register(s0);
uniform float3 vCloudDir;
uniform float4 vCloud;			// x = speed, y = scale, z = bias
uniform float4 vClr;
uniform float fTime;

float4 cloud_ps(float3 uv : TEXCOORD0) : COLOR0
{
	float3 n0 = uv;//normalize(uv);
	float3 n1 = uv*vCloud.w;
	
	n0.xyz += vCloudDir.xyz * fTime * vCloud.x;
	n1.xyz += vCloudDir.xyz * fTime * vCloud.x;
	
	float4 noise0 = tex3D(Noise, n0);
	float4 noise1 = tex3D(Noise, n1);
	float4 noise = lerp( noise0, noise1, 0.5 );

	float lrp = saturate(vCloud.y * noise.x + vCloud.z);
	return float4(vClr.xyz + lrp-0.5 , vClr.w * lrp);
}