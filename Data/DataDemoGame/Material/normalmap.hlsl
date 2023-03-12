
void vs_main_2_0(
float4 pos : POSITION,
float2 uv  : TEXCOORD0,
uniform float4x4 worldMatrix,
uniform float4x4 viewProjMatrix,
out float4 oPos : POSITION,
out float4 oPosObj : TEXCOORD0 
, out float4 oUVMisc : TEXCOORD1 // xy = uv, z = camDepth
, out float4 oUV0 : TEXCOORD2
, out float4 oUV1 : TEXCOORD3
)
{
	float4 worldPos = mul(worldMatrix, pos);
	oPosObj = pos;
	oUV0.xy =  uv.xy;
	oUV0.zw =  uv.xy;
	oUV1.xy =  uv.xy;
	oUV1.zw =  uv.xy;
	oPos = mul(viewProjMatrix, worldPos);
	oUVMisc.xy = uv.xy;
}


float4 expand(float4 v)
{ 
	return v * 2 - 1;
}

float4 ps_main_2_0(
	float2 uv : TEXCOORD0,
	uniform sampler2D diffuseMap : register(s0),
	uniform sampler2D normalMap : register(s1),
	//uniform float3 fogColour, 
	//float fogVal : COLOR,
	// parameters
	uniform float4 lightDiffuse0, // object space
	uniform float4 lightDir0//,
	//uniform float4 lightDiffuse1,
	//uniform float4 lightDir1,
	//uniform float3 ambient
	) : COLOR
{
	lightDir0 = normalize(lightDir0);
	
	float3 clr = tex2D(diffuseMap, uv).xyz;
	float3 nml = expand(tex2D(normalMap, uv)).xyz;
	
	float3 diffuse = lightDiffuse0.rgb * dot(nml, lightDir0.xyz) * clr;
	//clr = lightDiffuse1 + lightDir1 + ambient;
	clr = diffuse;
	//clr = lerp(clr, fogColour, fogVal);
	//clr = dot(lightDir1.xyz, nml);
	return float4(clr, 1);
}