#ifndef __SGF_GAME_PACKER__
#define __SGF_GAME_PACKER__

#include "../Common.h"
#include "../FileStream.h"
#include <iostream>
#include <vector>

namespace sgf {
	struct PakInFile {
		sgf::String mPath = "";
		void* mData = nullptr;
		int mSize = 0;
		int mFilePointer = 0;
	};

	class GamePacker
	{
	public:
		const char mSymbol[4] = "PKG";//文件特征头
		const int mCookie = 557667;//文件特征头
		int mFilePointer = 0;//文件内容区指针
		int mPackageSize = 0;//
		int mOriginalFileTotalNumber;//总文件个数
		int mOriginalFileTotalSize;//总文件大小
		std::vector<PakInFile> mFiles;
		bool mIsReadingMode = false;
		FileReadStream* mFileStream = nullptr;

	public:
		GamePacker();
		~GamePacker();
		
		void UpdateFileInfo();

		void ReadFromFile(const sgf::String& path);
		void WriteToFile(const sgf::String& path);
		FileReadStream* TryToLoadFile(const sgf::String& path);

		void AppendNewFile(const sgf::String& path, void* pfile, int size);


		void Close();

		int GetDesOffset() const;
		int GetPackageFileSize() const;
		void UpdateAllFilePointer();
	};

	extern std::vector<GamePacker> gPaks;

	void TryToLoadPak(const sgf::String& path);
	FileReadStream* TryToLoadFile(const sgf::String& path);

}

#endif // !__SGF_GAME_PACKER__
