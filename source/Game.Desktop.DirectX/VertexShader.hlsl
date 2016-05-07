cbuffer CBufferPerObject
{
	float4x4 WorldViewProjection : WORLDVIEWPROJECTION;
	float XAnimationFrame : XANIMATIONFRAME;
	bool FlipX : FLIPX;
	bool FlipY : FLIPY;
}

struct VS_INPUT
{
	float4 ObjectPosition: POSITION;
	float2 TextureCoordinate : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 Position: SV_Position;
	float2 TextureCoordinate : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	OUT.Position = mul(IN.ObjectPosition, WorldViewProjection);

	// Flip texture UV's and offset to correct sprite in grid
	float uOffset = (XAnimationFrame * .250f) + .125f;
	float vOffset = 0.5f;

	if (!FlipX && !FlipY)
	{
		OUT.TextureCoordinate = IN.TextureCoordinate;
	}
	else if (FlipX && !FlipY)
	{
		OUT.TextureCoordinate = float2(mul(IN.TextureCoordinate.x, -1.0f) + uOffset, IN.TextureCoordinate.y);
	}
	else if (!FlipX && FlipY)
	{
		OUT.TextureCoordinate = float2(IN.TextureCoordinate.x, mul(IN.TextureCoordinate.y, -1.0f) + vOffset);
	}
	else
	{
		OUT.TextureCoordinate = float2(mul(IN.TextureCoordinate.x, -1.0f) + uOffset, mul(IN.TextureCoordinate.y, -1.0f) + vOffset);
	} 

	return OUT;
}
