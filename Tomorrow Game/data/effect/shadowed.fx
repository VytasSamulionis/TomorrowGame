float4x4 g_WorldViewProjection;
float4x4 g_LightsWorldViewProjection;
float4x4 g_World;
float3 g_LightPos;
float3 g_LightDir;
float g_Ambient;
float g_FarClip;

Texture g_Texture;

sampler TextureSampler = sampler_state { 
  texture = <g_Texture>; 
  magfilter = LINEAR; 
  minfilter = LINEAR; 
  mipfilter=LINEAR; 
  AddressU = mirror; 
  AddressV = mirror;
};

Texture g_ShadowMap;

sampler ShadowMapSampler = sampler_state { 
  texture = <g_ShadowMap>; 
  magfilter = LINEAR; 
  minfilter = LINEAR; 
  mipfilter=LINEAR; 
  AddressU = clamp; 
  AddressV = clamp;
};

float DotProduct(float3 lightPos, float3 pos3D, float3 normal) {
  float3 lightDir = normalize(pos3D - lightPos);
  return dot(-lightDir, normal);    
}

struct VSInput {
  float4 Position : POSITION;
  float3 Normal : NORMAL;
  float2 TexCoords : TEXCOORD0;
};

struct VSOutput {
  float4 Position : POSITION;
  float4 Pos2DAsSeenByLight : TEXCOORD0;
  float2 TexCoords : TEXCOORD1;
  float3 Normal : TEXCOORD2;
  float4 Position3D : TEXCOORD3;
};

struct PSInput {
  float4 Pos2DAsSeenByLight : TEXCOORD0;
  float2 TexCoords : TEXCOORD1;
  float3 Normal : TEXCOORD2;
  float4 Position3D : TEXCOORD3;
};

struct PSOutput {
  float4 Color : COLOR0;
};


VSOutput ShadowedSceneVS (VSInput input) {
  VSOutput output = (VSOutput)0;

  output.Position = mul(input.Position, g_WorldViewProjection);
  output.Pos2DAsSeenByLight = mul(input.Position, g_LightsWorldViewProjection);    
  output.Normal = normalize(mul(input.Normal, (float3x3)g_World));    
  output.Position3D = mul(input.Position, g_World);
  output.TexCoords = input.TexCoords;

  return output;
}
 
PSOutput ShadowedScenePS (PSInput input) {
  PSOutput output = (PSOutput)0;

  float2 ProjectedTexCoords;
  ProjectedTexCoords[0] = input.Pos2DAsSeenByLight.x/input.Pos2DAsSeenByLight.w/2.0f +0.5f;
  ProjectedTexCoords[1] = 1.0f - (input.Pos2DAsSeenByLight.y/input.Pos2DAsSeenByLight.w/2.0f +0.5f);

  float diffuseLightingFactor = g_Ambient;
  if ((saturate(ProjectedTexCoords).x == ProjectedTexCoords.x) && (saturate(ProjectedTexCoords).y == ProjectedTexCoords.y)) {
    float depthStoredInShadowMap = tex2D(ShadowMapSampler, ProjectedTexCoords).x;
    float realDistance = input.Pos2DAsSeenByLight.z / g_FarClip;
    if ((realDistance - 1.0f / 255.0f) < depthStoredInShadowMap) {
      diffuseLightingFactor = saturate (DotProduct(g_LightPos, input.Position3D, input.Normal)) + g_Ambient;
    }
  }
  output.Color = tex2D(TextureSampler, input.TexCoords) * saturate (diffuseLightingFactor);

  return output;
}

PSOutput UnlitScenePS (PSInput input) {
  PSOutput output = (PSOutput)0;
  
  output.Color = tex2D(TextureSampler, input.TexCoords) * g_Ambient;
  return output;
}

technique ShadowedScene {
  pass Pass0 {
    VertexShader = compile vs_2_0 ShadowedSceneVS();
    PixelShader = compile ps_2_0 ShadowedScenePS();
  }
}

technique UnlitScene {
  pass Pass0 {
    VertexShader = compile vs_2_0 ShadowedSceneVS();
    PixelShader = compile ps_2_0 UnlitScenePS();
  }
}