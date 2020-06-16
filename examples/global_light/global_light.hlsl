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
	float3 normal : NORMAL;
};

Texture2D tex_color : register(t0);

SamplerState sampl_state : register(s0);

struct pixel
{
	float4 position : SV_POSITION;
	float2 tex_coords : TEXCOORD0;
	float3 normal : NORMAL;
};

pixel vs(vertex arg, uint instance_id : SV_INSTANCEID)
{
	pixel result;

	matrix transform = transpose(mul(wvp, world_matrices[instance_id]));

	result.position = mul(arg.position, transform);
	result.tex_coords = arg.tex_coords;

	result.normal = normalize(mul(arg.normal, (float3x3)transpose(world_matrices[instance_id])));

	return result;
}

float4 ps(pixel arg)
	: SV_TARGET
{
	float4 texture_color = tex_color.Sample(sampl_state, arg.tex_coords);
	float4 result = float4(0.8f, 0.498039f, 0.196078f, 1.0f);

	result = saturate(result) * texture_color;

	return result;
}
