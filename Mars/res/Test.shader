cbuffer cbPerObject
{
	float4x4 wvp;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VS_OUTPUT VS(float4 inPos : POSITION, float4 inColor : COLOR)
{
	VS_OUTPUT output;
	output.position = mul(inPos, wvp);
	output.color = inColor;

	return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
	return input.color;
}
