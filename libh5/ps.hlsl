struct pixel
{
	float4 position : SV_POSITION;
};

float4 ps(pixel arg)
	: SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
