Texture2D ColorTexture;

SamplerState ColorSampler;

struct VS_OUTPUT
{
	float4 Position: SV_Position;
	float2 TextureCoordinate : TEXCOORD;
};

float4 main(VS_OUTPUT IN) : SV_TARGET
{
	float4 returnTexture = ColorTexture.Sample(ColorSampler, IN.TextureCoordinate);
	if (returnTexture.a < 0.1f)
	{
		discard;
	}
	return returnTexture;
}