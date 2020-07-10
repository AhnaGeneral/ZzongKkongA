#include "Shaders.hlsl"

float4 VSLobbyScene (uint nVertexID : SV_VertexID) : SV_POSITION
{
	if (nVertexID == 0) return(float4(-1.0f, +1.0f, 0.0f, 1.0f));
	if (nVertexID == 1) return(float4(+1.0f, +1.0f, 0.0f, 1.0f));
	if (nVertexID == 2) return(float4(+1.0f, -1.0f, 0.0f, 1.0f));
	if (nVertexID == 3) return(float4(-1.0f, +1.0f, 0.0f, 1.0f));
	if (nVertexID == 4) return(float4(+1.0f, -1.0f, 0.0f, 1.0f));
	if (nVertexID == 5) return(float4(-1.0f, -1.0f, 0.0f, 1.0f));

	return(float4(0, 0, 0, 0));
}

float4 PSLobbyScene(float4 position : SV_POSITION) : SV_Target
{
	float4 LobbyTex = 
	gtxtLobbyTex[int2(position.xy)];
	//gtxtLobbyTex.Sample(gSamplerState, int2(position.xy));
	return(LobbyTex);
}