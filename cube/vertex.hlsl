cbuffer CB : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 projection;
};

struct VSInput
{
	float3 pos : POSITION;
	float3 color : COLOR;
	float3 normal : NORMAL;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
};

PSInput VSMain(VSInput input)
{
	PSInput output;

	float4 worldPos =
        mul(float4(input.pos, 1), world);

	output.pos =
        mul(
            mul(worldPos, view),
            projection
        );

	output.color = input.color;

	return output;
}
