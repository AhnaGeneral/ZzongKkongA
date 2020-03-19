#include "Shaders.hlsl"


struct VS_COLLISIONBOX_INPUT
{
    float3 Center : POSITION;
    float3 Extent : SIZE;
    float3 Orientation : TEXCOORD;
};

struct VS_COLLISIONBOX_OUTPUT
{
    float3 Center : POSITION;
    float3 Extent : SIZE;
    float3 Orientation : TEXCOORD;
};

struct GS_COLLISIONBOX_OUTPUT
{
    float4 Center : SV_POSITION;
    float3 Position : POSITION;
};

VS_COLLISIONBOX_OUTPUT VSCollisionBox(VS_COLLISIONBOX_INPUT input)
{
    VS_COLLISIONBOX_OUTPUT output;

    output.Center = mul(input.Center, (float3x3)gmtxGameObject);
    output.Extent = input.Extent;
    output.Orientation = input.Orientation;

    return(output);
}

[maxvertexcount(36)]
void GS(point VS_COLLISIONBOX_OUTPUT input[1], inout TriangleStream<GS_COLLISIONBOX_OUTPUT> outStream)
{
    float fx = input[0].Extent.x ;
    float fy = input[0].Extent.y ;
    float fz = input[0].Extent.z ;

    float4 pVertices[36];

    pVertices[0] = float4(-fx, +fy, -fz, 1.0f);
    pVertices[1] = float4(+fx, +fy, -fz, 1.0f);
    pVertices[2] = float4(+fx, -fy, -fz, 1.0f);
    pVertices[3] = float4(-fx, +fy, -fz, 1.0f);
    pVertices[4] = float4(+fx, -fy, -fz, 1.0f);
    pVertices[5] = float4(-fx, -fy, -fz, 1.0f);
    pVertices[6] = float4(-fx, +fy, +fz, 1.0f);
    pVertices[7] = float4(+fx, +fy, +fz, 1.0f);
    pVertices[8] = float4(+fx, +fy, -fz, 1.0f);
    pVertices[9] = float4(-fx, +fy, +fz, 1.0f);
    pVertices[10] = float4(+fx, +fy, -fz, 1.0f);
    pVertices[11] = float4(-fx, +fy, -fz, 1.0f);
    pVertices[12] = float4(-fx, -fy, +fz, 1.0f);
    pVertices[13] = float4(+fx, -fy, +fz, 1.0f);
    pVertices[14] = float4(+fx, +fy, +fz, 1.0f);
    pVertices[15] = float4(-fx, -fy, +fz, 1.0f);
    pVertices[16] = float4(+fx, +fy, +fz, 1.0f);
    pVertices[17] = float4(-fx, +fy, +fz, 1.0f);
    pVertices[18] = float4(-fx, -fy, -fz, 1.0f);
    pVertices[19] = float4(+fx, -fy, -fz, 1.0f);
    pVertices[20] = float4(+fx, -fy, +fz, 1.0f);
    pVertices[21] = float4(-fx, -fy, -fz, 1.0f);
    pVertices[22] = float4(+fx, -fy, +fz, 1.0f);
    pVertices[23] = float4(-fx, -fy, +fz, 1.0f);
    pVertices[24] = float4(-fx, +fy, +fz, 1.0f);
    pVertices[25] = float4(-fx, +fy, -fz, 1.0f);
    pVertices[26] = float4(-fx, -fy, -fz, 1.0f);
    pVertices[27] = float4(-fx, +fy, +fz, 1.0f);
    pVertices[28] = float4(-fx, -fy, -fz, 1.0f);
    pVertices[29] = float4(-fx, -fy, +fz, 1.0f);
    pVertices[30] = float4(+fx, +fy, -fz, 1.0f);
    pVertices[31] = float4(+fx, +fy, +fz, 1.0f);
    pVertices[32] = float4(+fx, -fy, +fz, 1.0f);
    pVertices[33] = float4(+fx, +fy, -fz, 1.0f);
    pVertices[34] = float4(+fx, -fy, +fz, 1.0f);
    pVertices[35] = float4(+fx, -fy, -fz, 1.0f);

    GS_COLLISIONBOX_OUTPUT output;

    for (int i = 0; i < 36; ++i)
    {
        output.Position = mul(mul (pVertices[i].xyz, (float3x3 )gmtxView), (float3x3) gmtxProjection);
        output.Center = mul(mul(pVertices[i], gmtxView), gmtxProjection);
        outStream.Append(output);
    }

}


float4 PSCollisionBox(GS_COLLISIONBOX_OUTPUT input) : SV_TARGET
{
    float4 cColor = float4(1.0f,0.0f,0.0f, 1.0f);
    return(cColor);
}