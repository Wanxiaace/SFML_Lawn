#include "GamePacker.h"

sgf::GamePacker::GamePacker()
{
	UpdateFileInfo();
}

sgf::GamePacker::~GamePacker()
{
	if (mFileStream)
		mFileStream->Close();
}

void sgf::GamePacker::ReadFromFile(const sgf::String& path)
{
	mIsReadingMode = true;
	mFileStream = new FileReadStream();
	mFileStream->OpenFile(path.c_str());
	mFileStream->Seek(FileReadStream::CURSOR_SET,0);
	mFileStream->Read(this, offsetof(GamePacker, mFiles));
	
	for (size_t i = 0; i < mOriginalFileTotalNumber; i++)
	{
		PakInFile file;
		int pathSize = mFileStream->ReadInt();
		file.mSize = mFileStream->ReadInt();
		file.mFilePointer = mFileStream->ReadInt();
		file.mPath = mFileStream->ReadString(pathSize);
		mFiles.push_back(file);
	}

}


void sgf::GamePacker::WriteToFile(const sgf::String& path)
{
	if (mIsReadingMode)
		return;
	FileReadStream outFile;
	outFile.OpenFile(path.c_str(), "wb");
	outFile.Write(this, offsetof(GamePacker, mFiles));
	
	UpdateAllFilePointer();
	GetPackageFileSize();

	for (auto& x : mFiles)
	{
		int pathsize = x.mPath.size();
		outFile.Write(&pathsize, 4);
		int fsize = x.mSize;
		outFile.Write(&fsize, 4);
		int ptr = x.mFilePointer;
		outFile.Write(&ptr, 4);

		outFile.Write(x.mPath.c_str(), pathsize);
	}

	for (auto& x : mFiles)
	{
		outFile.Write(x.mData,x.mSize);
	}


	outFile.Close();
}

sgf::FileReadStream* sgf::GamePacker::TryToLoadFile(const sgf::String& path)
{
	for (auto& x : mFiles)
	{
		if (x.mPath == path) {
			FileReadStream* result = new FileReadStream();
			mFileStream->Seek(FileReadStream::CURSOR_SET, x.mFilePointer);
			char* file = new char[x.mSize];
			mFileStream->Read(file,x.mSize);
			result->OpenMemory(file, x.mSize);
			return result;
		}
	}
	return nullptr;
}

void sgf::GamePacker::Close()
{
	mFileStream->Close();
}

int sgf::GamePacker::GetDesOffset() const
{
	int poffset = offsetof(GamePacker, mFiles);
	for (auto& x : mFiles)
	{
		poffset += x.mPath.size() + 12;
	}
	return poffset;
}

void sgf::GamePacker::UpdateAllFilePointer()
{
	int poffset = GetDesOffset();
	for (auto& x : mFiles)
	{
		x.mFilePointer = poffset;
		poffset += x.mSize;
	}
}

void sgf::GamePacker::AppendNewFile(const sgf::String& path, void* pfile, int size)
{
	if (mIsReadingMode)
		return;
	PakInFile file;
	file.mPath = path;
	file.mData = pfile;
	file.mSize = size;
	mOriginalFileTotalSize += size;
	mOriginalFileTotalNumber += 1;
	mFiles.push_back(file);
	UpdateFileInfo();
}

int sgf::GamePacker::GetPackageFileSize() const
{
	int strSize = GetDesOffset();
	for (auto& x : mFiles)
	{
		strSize += x.mSize;
	}
	return strSize;
}

void sgf::GamePacker::UpdateFileInfo()
{
	mPackageSize = GetPackageFileSize();
	mFilePointer = GetDesOffset();
}

void sgf::TryToLoadPak(const sgf::String& path)
{
	GamePacker pkg;
	pkg.TryToLoadFile(path);
	gPaks.push_back(std::move(pkg));
}

sgf::FileReadStream* sgf::TryToLoadFile(const sgf::String& path)
{
	sgf::FileReadStream* file = nullptr;
	for (auto& x : gPaks)
	{
		file = x.TryToLoadFile(path);
		if (file)
			return file;
	}

	FileReadStream* result = new FileReadStream();
	result->OpenFile(path.c_str());
	return result;
}
