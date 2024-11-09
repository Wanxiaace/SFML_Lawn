#ifndef __SGF_STRING__
#define __SGF_STRING__

#include <string>

namespace sgf {
    typedef std::string String;

	namespace SString {
        //请只使用英文字符串调用该函数
        String StringtoUpper(const String& src);

        String StringReplace(const String& resource_str, const String& sub_str, const String& new_str);
        
        String StringRemoveFileExtension(const String& filename);

        /// @brief 获得相对路径
        String StringGetCurrentPath(const String& filename);

        /// @brief 获得相对路径并去除后缀
        String StringGetCurrentPathWithoutExtension(const String& filename);

        bool IsCharInString(char cha,const String& str);
	}
}

#endif // !__SGF_STRING__
