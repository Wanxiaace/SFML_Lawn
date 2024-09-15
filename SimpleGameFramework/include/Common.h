#pragma once
#ifndef __SIMPLE_COMMON__
#define __SIMPLE_COMMON__

#ifdef _DEBUG
#define SGF_ASSERT(x) _ASSERT(x);
#else
#define SGF_ASSERT(x) x;
#endif

#define SGF_CONFIRM_LOG(x,y) if(!x)printf(y);


#ifdef _WIN32
#include <Windows.h>
#include "gl/GL.h"
#include "gl/glext.h"
#endif

#include <iostream>

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

 
const int VERTEX_BUFFER_MAX_COUNT = 1200;
const int MATRIX_BUFFER_MAX_COUNT = 200;

const int LOADING_THREAD_NUM_MAX = 10;


static void CheckGLError(const char* file, int line)
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        switch (error)
        {

        case GL_INVALID_ENUM:
            printf("GL Error: GL_INVALID_ENUM %s : %d \n", file, line);
            break;
        case GL_INVALID_VALUE:
            printf("GL Error: GL_INVALID_VALUE %s : %d \n", file, line);
            break;
        case GL_INVALID_OPERATION:
            printf("GL Error: GL_INVALID_OPERATION %s : %d \n", file, line);
            break;
        case GL_STACK_OVERFLOW:
            printf("GL Error: GL_STACK_OVERFLOW %s : %d \n", file, line);
            break;
        case GL_STACK_UNDERFLOW:
            printf("GL Error: GL_STACK_UNDERFLOW %s : %d \n", file, line);
            break;
        case GL_OUT_OF_MEMORY:
            printf("GL Error: GL_OUT_OF_MEMORY %s : %d \n", file, line);
            break;
        default:
            printf("GL Error: 0x%x %s : %d \n", error, file, line);
            break;
        }
    }
};
#define  GL_CALL(x) do{x;CheckGLError(__FILE__,__LINE__);}while(0) 

#include "SDL.h"
#include "SDL_Image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

namespace sgf {
	template<typename T>
	class Rect {
	public:
		T mX, mY;
		T mWidth, mHeight;

		Rect operator*(T mul) {
			return { mX * mul ,mY * mul,mWidth * mul,mHeight * mul };
		}

        bool IsPointIn(T pointX, T pointY) {
            if (pointX > mX && pointX < mX + mWidth && pointY > mY && pointY < mY + mHeight)
                return true;
            return false;
        };

        bool IsOverlap(const Rect& other) const {
            if (mX + mWidth <= other.mX || other.mX + other.mWidth <= mX) return false;
            if (mY + mHeight <= other.mY || other.mY + other.mHeight <= mY) return false;
            return true;
        }
        
	};

	typedef Rect<int> IntRect;
	typedef Rect<float> FloatRect;
	typedef std::string String;
    struct Color {
        float r, g, b, a;
    };
    typedef SDL_FPoint Point;

    struct GameMat44 {
        union {
            float m[4][4];
            struct
            {
                float x1, x2, x3, x4;
                float y1, y2, y3, y4;
                float z1, z2, z3, z4;
                float t1, t2, t3, t4;
            };
        };
    };

    //请只使用英文字符串调用该函数
    static sgf::String StringtoUpper(const sgf::String& src) {
        std::string result;
        for (char c : src) {
            result += std::toupper(c);
        }
        return result;
    }

    static sgf::String StringRemoveFileExtension(const sgf::String& filename) {
        size_t dotPos = filename.find_last_of(".");
        if (dotPos != sgf::String::npos && dotPos > 0) {
            return filename.substr(0, dotPos);
        }
        else {
            return filename;
        }
    }

    static unsigned int TryGetTicks() {
        //可调多倍速
        return SDL_GetTicks();
    }

    static unsigned long long TryGetTicks64() {
        return SDL_GetTicks64();
    }

    static void SRand(unsigned int seed) {
        srand(seed);
    };

    //注意，该函数无法达到rangeMax，最大值是rangeMax-1
    static int Rand(int rangeMin,int rangeMax) {
        int range = rangeMax - rangeMin;
        return rand() % range + rangeMin;
    };

    static float RandF(float rangeMin, float rangeMax) {
        float range = rangeMax - rangeMin;
        if (!int(range * 1000.0f))
            return rangeMin;
        return float(rand() % int(range * 1000.0f)) / 1000.0f + rangeMin;
    };

}
#endif
