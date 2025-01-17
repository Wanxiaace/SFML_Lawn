#version 330 core
out vec4 FragColor;
in vec4 pixelColor;
in vec2 textureCoord;

in float texIndex;

uniform bool useTexture;
uniform sampler2D textures[32];

vec4 sobelEdgeDetection(sampler2D tex, vec2 uv) {
    vec2 texelSize = 1.0 / textureSize(tex, 0);

    // 计算X方向梯度
    float Gx = texture(tex, uv + vec2(-texelSize.x, -texelSize.y)).r * -1.0 +
               texture(tex, uv + vec2(0.0, -texelSize.y)).r * -2.0 +
               texture(tex, uv + vec2(texelSize.x, -texelSize.y)).r * -1.0 +
               texture(tex, uv + vec2(-texelSize.x, texelSize.y)).r +
               texture(tex, uv + vec2(0.0, texelSize.y)).r * 2.0 +
               texture(tex, uv + vec2(texelSize.x, texelSize.y)).r;

    // 计算Y方向梯度
    float Gy = texture(tex, uv + vec2(-texelSize.x, -texelSize.y)).r * -1.0 +
               texture(tex, uv + vec2(-texelSize.x, 0.0)).r * -2.0 +
               texture(tex, uv + vec2(-texelSize.x, texelSize.y)).r * -1.0 +
               texture(tex, uv + vec2(texelSize.x, -texelSize.y)).r +
               texture(tex, uv + vec2(texelSize.x, 0.0)).r * 2.0 +
               texture(tex, uv + vec2(texelSize.x, texelSize.y)).r;

    // 计算边缘强度
    float edgeStrength = sqrt(Gx * Gx + Gy * Gy);
    
    // 返回一个基于边缘强度的vec4值
    return vec4(1, 1, 1,edgeStrength);
}

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

	vec4 edgeColor = vec4(1.0);
	

	switch(int(texIndex))
	{
	case 0:  edgeColor = sobelEdgeDetection(textures[0],  textureCoord); break;
	case 1:  edgeColor = sobelEdgeDetection(textures[1],  textureCoord); break;
    case 2:  edgeColor = sobelEdgeDetection(textures[2],  textureCoord); break;
    case 3:  edgeColor = sobelEdgeDetection(textures[3],  textureCoord); break;
    case 4:  edgeColor = sobelEdgeDetection(textures[4],  textureCoord); break;
    case 5:  edgeColor = sobelEdgeDetection(textures[5],  textureCoord); break;
    case 6:  edgeColor = sobelEdgeDetection(textures[6],  textureCoord); break;
    case 7:  edgeColor = sobelEdgeDetection(textures[7],  textureCoord); break;
    case 8:  edgeColor = sobelEdgeDetection(textures[8],  textureCoord); break;
    case 9:  edgeColor = sobelEdgeDetection(textures[9],  textureCoord); break;
    case 10: edgeColor = sobelEdgeDetection(textures[10], textureCoord); break;
    case 11: edgeColor = sobelEdgeDetection(textures[11], textureCoord); break;
    case 12: edgeColor = sobelEdgeDetection(textures[12], textureCoord); break;
    case 13: edgeColor = sobelEdgeDetection(textures[13], textureCoord); break;
    case 14: edgeColor = sobelEdgeDetection(textures[14], textureCoord); break;
    case 15: edgeColor = sobelEdgeDetection(textures[15], textureCoord); break;
    case 16: edgeColor = sobelEdgeDetection(textures[16], textureCoord); break;
    case 17: edgeColor = sobelEdgeDetection(textures[17], textureCoord); break;
    case 18: edgeColor = sobelEdgeDetection(textures[18], textureCoord); break;
    case 19: edgeColor = sobelEdgeDetection(textures[19], textureCoord); break;
    case 20: edgeColor = sobelEdgeDetection(textures[20], textureCoord); break;
    case 21: edgeColor = sobelEdgeDetection(textures[21], textureCoord); break;
    case 22: edgeColor = sobelEdgeDetection(textures[22], textureCoord); break;
    case 23: edgeColor = sobelEdgeDetection(textures[23], textureCoord); break;
    case 24: edgeColor = sobelEdgeDetection(textures[24], textureCoord); break;
    case 25: edgeColor = sobelEdgeDetection(textures[25], textureCoord); break;
    case 26: edgeColor = sobelEdgeDetection(textures[26], textureCoord); break;
    case 27: edgeColor = sobelEdgeDetection(textures[27], textureCoord); break;
    case 28: edgeColor = sobelEdgeDetection(textures[28], textureCoord); break;
    case 29: edgeColor = sobelEdgeDetection(textures[29], textureCoord); break;
    case 30: edgeColor = sobelEdgeDetection(textures[30], textureCoord); break;
    case 31: edgeColor = sobelEdgeDetection(textures[31], textureCoord); break;
	}

    FragColor = vec4(1.0,1.0,1.0,FragColor.a);
    FragColor *= edgeColor;
    

    if (pixelColor.a > 1.0) {
        float brightenFactor = pixelColor.a;
        FragColor.rgb = clamp(FragColor.rgb * brightenFactor, 0.0, 1.0);
    }

    
}