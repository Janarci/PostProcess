
Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);



struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
};

float2 distort(float2 p)
{
	float theta = atan2(p.y, p.x);

	float radius = length(p);

	radius = pow(radius, 2.0);

	p.x = radius * cos(theta);
	p.x = radius * sin(theta);


	return 0.5 * (p + 1.0);
}

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float2 xy = 2.0 * input.texcoord.xy - 1.0;
	float2 uv;
	float d = length(xy);


	uv = input.texcoord.xy;


	//float4 color = Texture.Sample(TextureSampler, uv);

	float vig = uv.x * uv.y * 15.0;
	vig = pow(vig, 0.25);
	const float4 color = { vig, vig	, vig, vig };

	return color;
}