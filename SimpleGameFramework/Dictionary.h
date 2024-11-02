#pragma once
#ifndef __SIMPLE_DICT__
#define __SIMPLE_DICT__

#include "Common.h"

namespace sgf {
	//���ڴ�����Ϸ��Ϣ���ı��Ķ���
	class Dictionary {
	public:
		nlohmann::json mJson;
	public:
		Dictionary();
		~Dictionary();

		void LoadFromFile(const char* path);

		sgf::String Dump() const;
		void Parse(const sgf::String& src);

		template<typename T>
		T At(const sgf::String& key) {
			return mJson.at(key);
		};

		template<typename T>
		void Set(const sgf::String& key,const T& value)
		{
			mJson[key] = value;
		}

		sgf::String GetString(const sgf::String& key);

		sgf::String operator [](const sgf::String& key);
	};
	
}

#endif