#ifndef __SGF_STRING__
#define __SGF_STRING__

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
	}
}

#endif // !__SGF_STRING__
