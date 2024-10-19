#version 330 core
out vec4 FragColor;
in vec4 pixelColor;
in vec2 textureCoord;

in float texIndex;

uniform bool useTexture;
uniform sampler2D textures[32];

vec4 boxBlur(sampler2D tex, vec2 uv, int radius) {
    vec4 sum = vec4(0.0);
    int size = (radius * 2 + 1) * (radius * 2 + 1);
    float weight = 1.0 / float(size);
    for (int x = -radius; x <= radius; ++x) {
        for (int y = -radius; y <= radius; ++y) {
            vec2 offset = vec2(x, y) / textureSize(tex, 0);
            sum += texture(tex, uv + offset) * weight;
        }
    }

    return sum;
}

void main()
{ 
    FragColor = vec4(1.0);

    if(int(texIndex) != -1)
    {
        //FragColor *= texture(textures[int(texIndex)], textureCoord );
        FragColor *= boxBlur(textures[int(texIndex)], textureCoord,2 );
    }
    FragColor *= pixelColor;

    if (pixelColor.a > 1.0) {
        float brightenFactor = pixelColor.a;
        FragColor.rgb = clamp(FragColor.rgb * brightenFactor, 0.0, 1.0);
    }
}