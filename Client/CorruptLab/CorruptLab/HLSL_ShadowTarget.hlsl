//#include "Shaders.hlsl"
//
//struct PS_SHADOW_OUTPUT
//{
//	float4 ShadowTex : SV_TARGET0;
//};
//
//float4 VSShdaowTarget(uint nVertexID : SV_VertexID) : SV_POSITION
//{
//	if (nVertexID == 0) return(float4(-1.0f, +1.0f, 0.0f, 1.0f));
//	if (nVertexID == 1) return(float4(+1.0f, +1.0f, 0.0f, 1.0f));
//	if (nVertexID == 2) return(float4(+1.0f, -1.0f, 0.0f, 1.0f));
//	if (nVertexID == 3) return(float4(-1.0f, +1.0f, 0.0f, 1.0f));
//	if (nVertexID == 4) return(float4(+1.0f, -1.0f, 0.0f, 1.0f));
//	if (nVertexID == 5) return(float4(-1.0f, -1.0f, 0.0f, 1.0f));
//
//	return(float4(0, 0, 0, 0));
//}
//
//PS_SHADOW_OUTPUT PSShadowTargeet(float4 position : SV_POSITION)
//{
//	PS_SHADOW_OUTPUT output;
//	// 근데왜 검은색이삼 후달달
//	output.ShadowTex = float4(1.0f, 1.0f, 0.0f, 1.0f); 
//	return output;
//}