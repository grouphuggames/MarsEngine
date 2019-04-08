float4 VS(float4 inPos : POSITION) : SV_POSITION
{
	return inPos;
}

float4 PS() : SV_TARGET
{
	return float4(0.f, 0.f, 1.f, 1.f);
}