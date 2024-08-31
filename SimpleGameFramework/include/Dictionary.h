#pragma once
#ifndef __SIMPLE_DICT__
#define __SIMPLE_DICT__

#include "json.hpp"
#include "Common.h"

namespace sgf {
	class Dictionary {
	public:
		nlohmann::json mJson;
	public:
		Dictionary();
		~Dictionary();

		void LoadFromFile(const char* path);

		sgf::String Dump() const;
		void Parse(const sgf::String& src);

		sgf::String At(const sgf::String& key);
		void Set(const sgf::String& key,const sgf::String& value);

		sgf::String operator [](const sgf::String& key);
	};
	
}

#endif