float4x4 g_WorldViewProjection;
float4x4 g_LightsWorldViewProjection;
float4x4 g_World;
float g_Ambient;
float g_FarClip;

Texture g_Texture;

sampler TextureSampler = sampler_state { 
  texture = <g_Texture>; 
  magfilter = LINEAR; 
  minfilter = LINEAR; 
  mipfilter=LINEAR; 
  AddressU = wrap; 
  AddressV = wrap;
};

Texture g_DetailMap;

sampler DetailMapSampler = sampler_state {
  texture = <g_DetailMap>;
  magfilter = LINEAR;
  minfilter = LINEAR;
  mipfilter = LINEAR;
  AddressU = wrap;
  AddressV = wrap;
};

Texture g_ShadowMap;

sampler ShadowMapSampler = sampler_state { 
  texture = <g_ShadowMap>; 
  magfilter = POINT; 
  minfilter = POINT; 
  mipfilter = POINT;
  AddressU = clamp;
  AddressV = clamp;
};

Texture g_BuildingField;

sampler BuildingFieldSampler = sampler_state {
  texture = <g_BuildingField>;
  magfilter = LINEAR;
  minfilter = LINEAR;
  mipfilter = LINEAR;
  AddressU = wrap;
  AddressV = wrap;
};


struct VSInput1 {
  float4 Position : POSITION;
  float4 Color : COLOR0;
  float2 TexCoords : TEXCOORD0;
};

struct VSOutput1 {
  float4 Position : POSITION;
  float4 Pos2DAsSeenByLight : TEXCOORD0;
  float2 TexCoords : TEXCOORD1;
  float4 Color : COLOR0;
  float4 Position3D : TEXCOORD2;
};

struct VSInput2 {
  float4 Position : POSITION;
  float4 Color : COLOR0;
  float2 TexCoords1 : TEXCOORD0;
  float2 TexCoords2 : TEXCOORD1;
};

struct VSOutput2 {
  float4 Position : POSITION;
  float4 Pos2DAsSeenByLight : TEXCOORD0;
  float2 TexCoords1 : TEXCOORD1;
  float2 TexCoords2 : TEXCOORD2;
  float4 Color : COLOR0;
  float4 Position3D : TEXCOORD3;
};

struct PSInput1 {
  float4 Pos2DAsSeenByLight : TEXCOORD0;
  float2 TexCoords : TEXCOORD1;
  float4 Color : COLOR0;
  float4 Position3D : TEXCOORD3;
};

struct PSInput2 {
  float4 Pos2DAsSeenByLight : TEXCOORD0;
  float2 TexCoords1 : TEXCOORD1;
  float2 TexCoords2 : TEXCOORD2;
  float4 Color : COLOR0;
  float4 Position3D : TEXCOORD3;
};

struct PSOutput {
  float4 Color : COLOR0;
};

VSOutput1 ShadowedSceneVS (VSInput1 input) {
  VSOutput1 output = (VSOutput1)0;

  output.Position = mul(input.Position, g_WorldViewProjection);
  output.Pos2DAsSeenByLight = mul(input.Position, g_LightsWorldViewProjection);    
  output.Color = input.Color;
  output.Position3D = mul(input.Position, g_World);
  output.TexCoords = input.TexCoords;

  return output;
}

VSOutput2 ShadowedMultiTexSceneVS (VSInput2 input) {
  VSOutput2 output = (VSOutput2)0;

  output.Position = mul(input.Position, g_WorldViewProjection);
  output.Pos2DAsSeenByLight = mul(input.Position, g_LightsWorldViewProjection);    
  output.Color = input.Color;
  output.Position3D = mul(input.Position, g_World);
  output.TexCoords1 = input.TexCoords1;
  output.TexCoords2 = input.TexCoords2;

  return output;
}
 
PSOutput ShadowedScenePS (PSInput1 input) {
  PSOutput output = (PSOutput)0;

  float2 ProjectedTexCoords;
  ProjectedTexCoords[0] = input.Pos2DAsSeenByLight.x/input.Pos2DAsSeenByLight.w/2.0f +0.5f;
  ProjectedTexCoords[1] = 1.0f - (input.Pos2DAsSeenByLight.y/input.Pos2DAsSeenByLight.w/2.0f +0.5f);

  float diffuseLightingFactor = g_Ambient;
  if ((saturate(ProjectedTexCoords).x == ProjectedTexCoords.x) && (saturate(ProjectedTexCoords).y == ProjectedTexCoords.y)) {
    float depthStoredInShadowMap = tex2D(ShadowMapSampler, ProjectedTexCoords).x;
    float realDistance = input.Pos2DAsSeenByLight.z / g_FarClip;
    if ((realDistance - 1.0f/255.0f) < depthStoredInShadowMap) {
      diffuseLightingFactor = 1.0f;
    }
  }
  output.Color = tex2D(TextureSampler, input.TexCoords) * input.Color * diffuseLightingFactor;

  return output;
}

PSOutput ShadowedMultiTexScenePS (PSInput2 input) {
  PSOutput output = (PSOutput)0;

  float2 ProjectedTexCoords;
  ProjectedTexCoords[0] = input.Pos2DAsSeenByLight.x/input.Pos2DAsSeenByLight.w/2.0f +0.5f;
  ProjectedTexCoords[1] = 1.0f - (input.Pos2DAsSeenByLight.y/input.Pos2DAsSeenByLight.w/2.0f +0.5f);

  float diffuseLightingFactor = g_Ambient;
  if ((saturate(ProjectedTexCoords).x == ProjectedTexCoords.x) && (saturate(ProjectedTexCoords).y == ProjectedTexCoords.y)) {
    float depthStoredInShadowMap = tex2D(ShadowMapSampler, ProjectedTexCoords).x;
    float realDistance = input.Pos2DAsSeenByLight.z / g_FarClip;
    if ((realDistance - 1.0f/255.0f) < depthStoredInShadowMap) {
      diffuseLightingFactor = 1.0f;
    }
  }
  output.Color = tex2D(TextureSampler, input.TexCoords1) * tex2D(DetailMapSampler, input.TexCoords2) * diffuseLightingFactor;
  output.Color *= input.Color;
  
  return output;
}

PSOutput ShadowedSceneWithBuildingFieldPS (PSInput1 input) {
  PSOutput output = (PSOutput)0;

  float2 ProjectedTexCoords;
  ProjectedTexCoords[0] = input.Pos2DAsSeenByLight.x/input.Pos2DAsSeenByLight.w/2.0f +0.5f;
  ProjectedTexCoords[1] = 1.0f - (input.Pos2DAsSeenByLight.y/input.Pos2DAsSeenByLight.w/2.0f +0.5f);

  float diffuseLightingFactor = g_Ambient;
  if ((saturate(ProjectedTexCoords).x == ProjectedTexCoords.x) && (saturate(ProjectedTexCoords).y == ProjectedTexCoords.y)) {
    float depthStoredInShadowMap = tex2D(ShadowMapSampler, ProjectedTexCoords).x;
    float realDistance = input.Pos2DAsSeenByLight.z / g_FarClip;
    if ((realDistance - 1.0f/255.0f) < depthStoredInShadowMap) {
      diffuseLightingFactor = 1.0f;
    }
  }
  float4 buildingField = tex2D(BuildingFieldSampler, input.TexCoords);
  float4 buildingFieldColor = float4(0.81f, 0.33f, 0.74f, 1.0f);
  if (buildingField.r < 0.1f && buildingField.g < 0.1f && buildingField.b < 0.1f) {
    buildingFieldColor = float4(0.55f, 0.81f, 0.74f, 1.0f);
  }
  output.Color = tex2D(TextureSampler, input.TexCoords) * input.Color * diffuseLightingFactor * buildingFieldColor;

  return output;
}

technique ShadowedScene {
  pass Pass0 {
    VertexShader = compile vs_2_0 ShadowedSceneVS();
    PixelShader = compile ps_2_0 ShadowedScenePS();
  }
}

technique ShadowedMultiTexScene {
  pass Pass0 {
    VertexShader = compile vs_2_0 ShadowedMultiTexSceneVS();
    PixelShader = compile ps_2_0 ShadowedMultiTexScenePS();
  }
}

technique ShadowedSceneWithBuildingField {
  pass Pass0 {
    VertexShader = compile vs_2_0 ShadowedSceneVS();
    PixelShader = compile ps_2_0 ShadowedSceneWithBuildingFieldPS();
  }
}