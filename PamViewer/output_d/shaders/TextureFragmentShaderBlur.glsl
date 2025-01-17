#version 330 core
out vec4 FragColor;
in vec4 pixelColor;
in vec2 textureCoord;

in float texIndex;

uniform bool useTexture;
uniform sampler2D textures[32];

vec4 BoxBlur(sampler2D tex, vec2 uv) {

    return vec4(1, 1, 1,1);
}

void main()
{ 
    //FragColor *= texture(textures[int(texIndex)],textureCoord);
    
    FragColor = vec4(1.0,1.0,1.0,FragColor.a);

	switch(int(texIndex))
	{
	case 0:  FragColor = BoxBlur(textures[0],  textureCoord); break;
	case 1:  FragColor = BoxBlur(textures[1],  textureCoord); break;
    case 2:  FragColor = BoxBlur(textures[2],  textureCoord); break;
    case 3:  FragColor = BoxBlur(textures[3],  textureCoord); break;
    case 4:  FragColor = BoxBlur(textures[4],  textureCoord); break;
    case 5:  FragColor = BoxBlur(textures[5],  textureCoord); break;
    case 6:  FragColor = BoxBlur(textures[6],  textureCoord); break;
    case 7:  FragColor = BoxBlur(textures[7],  textureCoord); break;
    case 8:  FragColor = BoxBlur(textures[8],  textureCoord); break;
    case 9:  FragColor = BoxBlur(textures[9],  textureCoord); break;
    case 10: FragColor = BoxBlur(textures[10], textureCoord); break;
    case 11: FragColor = BoxBlur(textures[11], textureCoord); break;
    case 12: FragColor = BoxBlur(textures[12], textureCoord); break;
    case 13: FragColor = BoxBlur(textures[13], textureCoord); break;
    case 14: FragColor = BoxBlur(textures[14], textureCoord); break;
    case 15: FragColor = BoxBlur(textures[15], textureCoord); break;
    case 16: FragColor = BoxBlur(textures[16], textureCoord); break;
    case 17: FragColor = BoxBlur(textures[17], textureCoord); break;
    case 18: FragColor = BoxBlur(textures[18], textureCoord); break;
    case 19: FragColor = BoxBlur(textures[19], textureCoord); break;
    case 20: FragColor = BoxBlur(textures[20], textureCoord); break;
    case 21: FragColor = BoxBlur(textures[21], textureCoord); break;
    case 22: FragColor = BoxBlur(textures[22], textureCoord); break;
    case 23: FragColor = BoxBlur(textures[23], textureCoord); break;
    case 24: FragColor = BoxBlur(textures[24], textureCoord); break;
    case 25: FragColor = BoxBlur(textures[25], textureCoord); break;
    case 26: FragColor = BoxBlur(textures[26], textureCoord); break;
    case 27: FragColor = BoxBlur(textures[27], textureCoord); break;
    case 28: FragColor = BoxBlur(textures[28], textureCoord); break;
    case 29: FragColor = BoxBlur(textures[29], textureCoord); break;
    case 30: FragColor = BoxBlur(textures[30], textureCoord); break;
    case 31: FragColor = BoxBlur(textures[31], textureCoord); break;
	}

    FragColor *= FragColor;
    

    if (pixelColor.a > 1.0) {
        float brightenFactor = pixelColor.a;
        FragColor.rgb = clamp(FragColor.rgb * brightenFactor, 0.0, 1.0);
    }

    
}