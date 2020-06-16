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
	float2 tex_coords : TEXCOORD0;
};

Texture2D tex_color : register(t0);

SamplerState sampl_state : register(s0);

struct pixel
{
	float4 position : SV_POSITION;
	float2 tex_coords : TEXCOORD0;
};

pixel vs(vertex arg, uint instance_id : SV_INSTANCEID)
{
	pixel result;

	result.position = mul(arg.position, transpose(mul(wvp, world_matrices[instance_id])));
	result.tex_coords = arg.tex_coords;	

	return result;
}

float4 ps(pixel arg)
	: SV_TARGET
{
	return tex_color.Sample(sampl_state, arg.tex_coords);
}
