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
    
    switch(int(texIndex))
	{
		case  0: FragColor *= texture(textures[0], textureCoord ); break;
		case  1: FragColor *= texture(textures[1], textureCoord ); break;
		case  2: FragColor *= texture(textures[2], textureCoord ); break;
		case  3: FragColor *= texture(textures[3], textureCoord ); break;
		case  4: FragColor *= texture(textures[4], textureCoord ); break;
		case  5: FragColor *= texture(textures[5], textureCoord ); break;
		case  6: FragColor *= texture(textures[6], textureCoord ); break;
		case  7: FragColor *= texture(textures[7], textureCoord ); break;
		case  8: FragColor *= texture(textures[8], textureCoord ); break;
		case  9: FragColor *= texture(textures[9], textureCoord ); break;
		case 10: FragColor *= texture(textures[10], textureCoord ); break;
		case 11: FragColor *= texture(textures[11], textureCoord ); break;
		case 12: FragColor *= texture(textures[12], textureCoord ); break;
		case 13: FragColor *= texture(textures[13], textureCoord ); break;
		case 14: FragColor *= texture(textures[14], textureCoord ); break;
		case 15: FragColor *= texture(textures[15], textureCoord ); break;
		case 16: FragColor *= texture(textures[16], textureCoord ); break;
		case 17: FragColor *= texture(textures[17], textureCoord ); break;
		case 18: FragColor *= texture(textures[18], textureCoord ); break;
		case 19: FragColor *= texture(textures[19], textureCoord ); break;
		case 20: FragColor *= texture(textures[20], textureCoord ); break;
		case 21: FragColor *= texture(textures[21], textureCoord ); break;
		case 22: FragColor *= texture(textures[22], textureCoord ); break;
		case 23: FragColor *= texture(textures[23], textureCoord ); break;
		case 24: FragColor *= texture(textures[24], textureCoord ); break;
		case 25: FragColor *= texture(textures[25], textureCoord ); break;
		case 26: FragColor *= texture(textures[26], textureCoord ); break;
		case 27: FragColor *= texture(textures[27], textureCoord ); break;
		case 28: FragColor *= texture(textures[28], textureCoord ); break;
		case 29: FragColor *= texture(textures[29], textureCoord ); break;
		case 30: FragColor *= texture(textures[30], textureCoord ); break;
		case 31: FragColor *= texture(textures[31], textureCoord ); break;
	}

    FragColor *= pixelColor;

    if (pixelColor.a > 1.0) {
        float brightenFactor = pixelColor.a;
        FragColor.rgb = clamp(FragColor.rgb * brightenFactor, 0.0, 1.0);
    }

    
}