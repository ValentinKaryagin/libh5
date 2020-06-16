cbuffer const_buffer
	: register(b0)
{
	matrix wvp : WORLDVIEWPROJECTION;
};

struct vertex
{
	float4 position : POSITION;
};

struct pixel
{
	float4 position : SV_POSITION;
};

pixel vs(vertex arg)
{
	pixel result;
	result.position = mul(arg.position, transpose(wvp));
	return result;
}

float4 ps(pixel arg)
	: SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
