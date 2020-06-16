cbuffer const_buffer
	: register(b0)
{
	matrix vp : VIEWPROJECTION;
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

pixel vs_draw_instanced(vertex arg, uint instance_id : SV_INSTANCEID)
{
	pixel result;
	result.position = mul(arg.position, transpose(mul(vp, world_matrices[instance_id])));
	return result;
}
