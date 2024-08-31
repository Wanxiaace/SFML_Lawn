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

    static std::string StringToGBK(const String& src_str) {
        int len = MultiByteToWideChar(CP_UTF8, 0, src_str.c_str(), -1, NULL, 0);
        wchar_t* wszGBK = new wchar_t[len + 1];
        memset(wszGBK, 0, len * 2 + 2);
        MultiByteToWideChar(CP_UTF8, 0, src_str.c_str(), -1, wszGBK, len);
        len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
        char* szGBK = new char[len + 1];
        memset(szGBK, 0, len + 1);
        WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
        std::string strTemp(szGBK);
        if (wszGBK) delete[] wszGBK;
        if (szGBK) delete[] szGBK;
        return strTemp;
    };

    static String GBKToString(const char* src_str)
    {
        int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, NULL, 0);
        wchar_t* wstr = new wchar_t[len + 1];
        memset(wstr, 0, len + 1);
        MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
        len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
        char* str = new char[len + 1];
        memset(str, 0, len + 1);
        WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
        String strTemp = str;
        if (wstr) delete[] wstr;
        if (str) delete[] str;
        return strTemp;
    }

    static unsigned int TryGetTicks() {
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
