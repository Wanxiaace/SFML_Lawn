#ifndef __SGF_FILE_STREAM__
#define __SGF_FILE_STREAM__

#include <iostream>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Common.h"

#define CHECK_LOADED if (!mIStream) return;
#define CHECK_LOADED_X(x) if (!mIStream) return x;

namespace sgf {
	class FileStream {
	public:
		enum CursorStyle {
			CURSOR_SET,
			CURSOR_END,
			CURSOR_CUR,
		};

	public:
		bool mIsFile = false;
		bool mIsOpen = false;
		bool mIsReadingMode = false;
		SDL_RWops* mIStream = nullptr;

	public:
		FileStream();
		~FileStream();

		void BindIStream(SDL_RWops* stream) { mIStream = stream; };

		void OpenFile(const sgf::String& path, const sgf::String& mode = "rb", bool throwError = true);
		void OpenFileIfExist(const sgf::String& path, const sgf::String& mode = "rb");
		void OpenMemory(void* pbuffer,int size);

		void Seek(CursorStyle style, int cur) const;
		void Read(void* pbuffer, int size) const;
		void Write(const void* src, int size);
		void Close();
		int Tell() const;
		int GetSize() const;

		template<typename T> 
		T ReadAs() {
			CHECK_LOADED_X(T());
			char* buf = new char[sizeof(T)] {0};
			int size = sizeof(T);
			Read(buf,sizeof(T));
			T result = *(T*)buf;
			return result;
		}
		unsigned int ReadDword() { return ReadAs<unsigned int>(); };
		unsigned long long ReadQword() { return ReadAs<unsigned long long>(); };
		int ReadInt() { return ReadAs<int>(); };
		long long ReadInt64() { return ReadAs<long long>(); };
		sgf::String ReadString(int size);
	};
}

#endif // !__SGF_FILE_STREAM__
