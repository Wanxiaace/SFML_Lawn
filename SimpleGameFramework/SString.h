#ifndef __SGF_STRING_H__
#define __SGF_STRING_H__

#include <string>


namespace sgf {
    typedef std::string String;

	namespace SString {
        //��ֻʹ��Ӣ���ַ������øú���
        String StringtoUpper(const String& src);

        String StringReplace(const String& resource_str, const String& sub_str, const String& new_str);
        
        String StringRemoveFileExtension(const String& filename);

        /// @brief ������·��
        String StringGetCurrentPath(const String& filename);

        /// @brief ������·����ȥ����׺
        String StringGetCurrentPathWithoutExtension(const String& filename);

        bool IsCharInString(char cha,const String& str);

        template<class T>
        String StrParse(const T& src) { return std::to_string(src); };


        template<class T>
        static T StrAs(const String& src) { return T(); };

        template<>
        static int StrAs(const String& src) {
            return std::atoi(src.c_str());
        }

        template<>
        static float StrAs(const String& src) {
            return std::atof(src.c_str());
        }

        template<>
        static double StrAs(const String& src) {
            return std::atof(src.c_str());
        }

        template<>
        static long long StrAs(const String& src) {
            return std::atoll(src.c_str());
        }
    }
}

#define _SS(x) sgf::String(x)

#endif // !__SGF_STRING_H__
