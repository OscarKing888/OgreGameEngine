////////////////////////////////////////////
//hlsl file for generating FixStar
////////////////////////////////////////////

//input information of vertex programe
struct a2v
{
  float4 Position  : POSITION;
  float4 ColAndMag : COLOR;
};


//output information of vertex programe and input information of pixel programe
//QuadPos���������꣬0��0����1��1����vertex programe�����Զ�����
struct v2f
{
  float4 Position  : POSITION;
  float2 QuadPos   : TEXCOORD;
  float4 Color     : COLOR;
  float  PSize     : PSIZE;
};


//output information of pixel programe
struct pixout
{
  float4 Color  : COLOR0;
};



float GetFlickerAmount(in float3 pos, in float4 StarSize)
{
  const float2 tab[8] = 
  {
		float2(0.897907815,-0.347608525),	float2(0.550299290, 0.273586675), float2(0.823885965, 0.098853070), float2(0.922739035,-0.122108860),
		float2(0.800630175,-0.088956800), float2(0.711673375, 0.158864420), float2(0.870537795, 0.085484560), float2(0.956022355,-0.058114540)
	};

	float2 hash = frac(pos.xy * 256);  
  float index = frac(hash.x + (hash.y + 1) * StarSize.w);
  index *= 8;
  
  float f = frac(index);
  int i = (int) index;
  return tab[i].x + f * tab[i].y;
}


v2f StarsVS(a2v IN,
uniform float4x4 worldViewProj,
uniform float4 StarSize )
{
  //��ʼ��
  v2f OUT = (v2f)0;

  float4 pos = IN.Position;
  OUT.Position = mul(worldViewProj, IN.Position);
  OUT.Position.z = OUT.Position.w; 
    
  // On DX9 OUT.QuadPos will be filled by GPU after point sprite expansion

	//IN.ColAndMag.w�����ǵ�������ȣ������������ǵĴ�С
	float appMag = 6.5 + IN.ColAndMag.w * (-1.44 - 6.5);
	
	//���㶥�����ȣ�ÿһ֡StarSize�����һ������仯��ʹ���ǲ�����˸
	float brightness = GetFlickerAmount(pos, StarSize) * pow(5.0, (-appMag - 1.44) / 2.5);
	
	float4 Color = IN.ColAndMag;
	
	OUT.Color = float4(brightness * Color.xyz, brightness);
  OUT.PSize = StarSize.xy;
     
  return OUT; 
} 


pixout StarsPS(v2f IN,
uniform float2 StarParams)
{
  pixout OUT;
	float StarIntensity = StarParams.x;
	float StarTransparent = StarParams.y;
	//���㵱ǰ���ص㵽�㾫������λ�õľ��룬3.5Ϊ������������ָ����˥��
	float2 distCenter = 3.5 * IN.QuadPos.xy - 3.5 * float2(0.5, 0.5);
	float scale = exp(-dot(distCenter, distCenter));

	float3 colCool = IN.Color.xyz;
	float3 colHot = IN.Color.w;
	
	//������ɫ������*��˥�������ɫ + ���Ȳ�������IN.Color.w��vertex programeʱ����Ķ�������
	float3 colFinal = StarIntensity * (colCool * scale + 5 * colHot * pow(scale, 10));
	OUT.Color.xyz = colFinal;
	OUT.Color.w = IN.Color.w * StarTransparent;
  return OUT;
}