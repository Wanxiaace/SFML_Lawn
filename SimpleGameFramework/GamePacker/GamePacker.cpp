#include "GamePacker.h"

std::vector<sgf::GamePacker> sgf::gPaks;

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

void sgf::GamePacker::ReadFromFileIfExist(const sgf::String& path)
{
	SDL_RWops* rwops = SDL_RWFromFile(path.c_str(),"r");
	if (rwops)
	{
		SDL_RWclose(rwops);
		ReadFromFile(path);
	}
}


void sgf::GamePacker::WriteToFile(const sgf::String& path)
{
	if (mIsReadingMode)
	{
		SHOW_ERROR_ABORT_EXIT("Dont write pak in reading mode");
		return;
	}
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

sgf::FileReadStream* sgf::GamePacker::TryToLoadFilePointer(const sgf::String& path)
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
	{
		SHOW_ERROR_ABORT_EXIT("Dont write pak in reading mode");
		return;
	}
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
	gPaks.push_back(GamePacker());
	GamePacker& pkg = gPaks.back();
	pkg.ReadFromFileIfExist(path);
	
}

sgf::FileReadStream* sgf::TryToLoadFilePointer(const sgf::String& path)
{
	sgf::FileReadStream* file = nullptr;
	for (auto& x : gPaks)
	{
		file = x.TryToLoadFilePointer(path);
		if (file)
			return file;
	}

	FileReadStream* result = new FileReadStream();
	result->OpenFile(path.c_str());

	return result;
}


pugi::xml_document* sgf::TryToLoadXMLFilePointer(const sgf::String& path,pugi::xml_parse_result* error)
{
	sgf::FileReadStream* file = TryToLoadFilePointer(path);
	pugi::xml_document* result = new pugi::xml_document();
	if(error)
		*error = result->load(file->ReadString(file->GetSize()).c_str());
	else
		result->load(file->ReadString(file->GetSize()).c_str());
	delete file;

	return result;
}

pugi::xml_document sgf::TryToLoadXMLFile(const sgf::String& path, pugi::xml_parse_result* error)
{
	sgf::FileReadStream* file = TryToLoadFilePointer(path);
	pugi::xml_document result;
	if (error)
		*error = result.load(file->ReadString(file->GetSize()).c_str());
	else
		result.load(file->ReadString(file->GetSize()).c_str());
	delete file;

	return result;
}

nlohmann::json sgf::TryToLoadJsonFile(const sgf::String& path)
{
	sgf::FileReadStream* file = TryToLoadFilePointer(path);
	nlohmann::json json = nlohmann::json::parse(file->ReadString(file->GetSize()));
	delete file;
	return json;
}
