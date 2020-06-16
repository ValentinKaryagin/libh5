cbuffer const_buffer
	: register(b0)
{
	matrix wvp : WORLDVIEWPROJECTION;
};

StructuredBuffer<matrix> world_matrices
	: register(t0);

struct vertex
{
	float4 position : POSITION;
};

struct pixel
{
	float4 position : SV_POSITION;
};

pixel vs(vertex arg, uint instance_id : SV_INSTANCEID)
{
	pixel result;
	result.position = mul(arg.position, transpose(mul(wvp, world_matrices[instance_id])));
	return result;
}

float4 ps(pixel arg)
	: SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
