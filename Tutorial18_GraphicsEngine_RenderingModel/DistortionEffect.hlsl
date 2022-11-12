
Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);



struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
};
float4 psmain(PS_INPUT input) : SV_TARGET
{
	float2 xy = 2.0 * input.texcoord.xy - 1.0;
	float d = length(xy);
	float4 color = Texture.Sample(TextureSampler, input.texcoord);

	if (d<1.0)
	{
		return color;

	}
	else
	{
		return float4(1, 0, 1, 1) * color;

	}
	return Texture.Sample(TextureSampler,input.texcoord);
}