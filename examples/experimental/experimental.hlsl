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

/*cbuffer const_buffer
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

	// diffuse color begin
	//float3 light_direction = -float3(1.0f, 0.0f, 0.0f);
	//float light_intensity = saturate(dot(arg.normal, light_direction));
	
	//return saturate(float4(1.0f, 1.0f, 1.0f, 1.0f) * light_intensity) * texture_color;
	// diffuse color end

	// ambient color begin
	float4 result = float4(1.0f, 0.0f, 1.0f, 1.0f);

	result = saturate(result) * texture_color;
	// ambient color end

	return result;
}*/
