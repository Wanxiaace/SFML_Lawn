#version 330 core
out vec4 FragColor;
in vec4 pixelColor;
in vec2 textureCoord;

in float texIndex;

uniform bool useTexture;
uniform sampler2D textures[32];

void main()
{ 
    //FragColor *= texture(textures[int(texIndex)],textureCoord);
    FragColor = vec4(1.0);
    
    if(texIndex != -1)
        FragColor *= texture(textures[int(texIndex)],textureCoord);

    FragColor *= pixelColor;

    if (pixelColor.a > 1.0) {
        float brightenFactor = pixelColor.a;
        FragColor.rgb = clamp(FragColor.rgb * brightenFactor, 0.0, 1.0);
    }

    
}