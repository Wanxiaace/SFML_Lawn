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
#endif // _WIN32

#include <GL/glew.h>

#include <iostream>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <pugixml.hpp>
#include <nlohmann/json.hpp>

#include <random>
#include <thread>
#include <chrono>
#include <functional>
using namespace std::chrono;

const int VERTEX_BUFFER_MAX_COUNT = 1200;
const int MATRIX_BUFFER_MAX_COUNT = 200;

const int LOADING_THREAD_NUM_MAX = 5;


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

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define SHOW_ERROR(x) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",x,nullptr);
#ifdef _DEBUG
#define SHOW_ERROR_ABORT_EXIT(x) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",x,nullptr); __debugbreak(); abort();
#else
#define SHOW_ERROR_ABORT_EXIT(x) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",x,nullptr); exit(EXIT_FAILURE);
#endif

namespace sgf {
    static void SetStdOutStream(std::ostream& src)
    {
        std::cout.rdbuf(src.rdbuf());
        std::cerr.rdbuf(src.rdbuf());
    };

	template<typename T>
	class Rect {
	public:
		T mX, mY;
		T mWidth, mHeight;

		Rect operator*(T mul) {
			return { mX * mul ,mY * mul,mWidth * mul,mHeight * mul };
		}

        Rect operator+(Rect t2) {
            return { mX + t2.mX , mY + t2.mY,mWidth + t2.mWidth,mHeight + t2.mHeight };
        }

        void operator+=(Rect t2) {
            mX += t2.mX;
            mY += t2.mY;
            mWidth += t2.mWidth;
            mHeight += t2.mHeight;
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

    struct Point
    {
        float x;
        float y;

        void operator*(float mul) { x *= mul, y *= mul; };
        Point operator+(Point p2) { return{ x + p2.x, y + p2.y }; };
        Point operator-(Point p2) { return{ x - p2.x, y - p2.y }; };
    };

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

    static sgf::String StringReplace(const sgf::String& resource_str, const sgf::String& sub_str, const sgf::String& new_str)
    {
        sgf::String dst_str = resource_str;
        sgf::String::size_type pos = 0;
        while ((pos = dst_str.find(sub_str)) != std::string::npos) 
        {
            dst_str.replace(pos, sub_str.length(), new_str);
        }
        return dst_str;
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

    /// @brief 获得相对路径
    static sgf::String StringGetCurrentPath(const sgf::String& filename) {
        size_t dotPos = filename.find_last_of("/");
        if (dotPos != sgf::String::npos && dotPos > 0) {
            return filename.substr(0, dotPos);
        }
        else {
            return filename;
        }
    }

    /// @brief 获得相对路径并去除后缀
    static sgf::String StringGetCurrentPathWithoutExtension(const sgf::String& filename) {
        size_t dotPos = filename.find_last_of("/");
        if (dotPos != sgf::String::npos && dotPos > 0) {
            return StringRemoveFileExtension(filename.substr( dotPos + 1));
        }
        else {
            return filename;
        }
    }

    /// 获得文件是否存在
    static bool IsFileExist(const sgf::String& filename) {
        FILE* file;
        fopen_s(&file, filename.c_str(), "r");
        if (file) {
            fclose(file);
            return true;
        }
        return 0;
    }

    static unsigned int TryGetTicks() {
        //可调多倍速
        return SDL_GetTicks();
    }

    static unsigned long long TryGetTicks64() {
        return SDL_GetTicks64();
    }

    static std::mt19937 m_randomAdapter;

    static void SRand(unsigned int seed) {
        m_randomAdapter.seed(seed);
    };

    //注意，该函数无法达到rangeMax，最大值是rangeMax-1
    static int Rand(int rangeMin,int rangeMax) {
        int range = rangeMax - rangeMin;
        return m_randomAdapter() % range + rangeMin;
    };

    static float RandF(float rangeMin, float rangeMax) {
        float range = rangeMax - rangeMin;
        if (!int(range * 1000.0f))
            return rangeMin;
        return float(m_randomAdapter() % int(range * 1000.0f)) / 1000.0f + rangeMin;
    };

    static void WaitUntil(std::function<bool()> req) {
        while (!req())
        {
            std::this_thread::sleep_for(100ms);
        }
    };
}
#endif
