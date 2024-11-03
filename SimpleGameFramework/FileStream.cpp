#include "FileStream.h"

sgf::FileReadStream::FileReadStream()
{

}

sgf::FileReadStream::~FileReadStream()
{
	Close();
}

void sgf::FileReadStream::OpenFile(const sgf::String& path, const sgf::String& mode,bool throwError)
{
	mIStream = SDL_RWFromFile(path.c_str(), mode.c_str());
	if (mIStream)
	{
		mIsFile = true;
		mIsOpen = true;
	}
	else if(throwError) {
		SHOW_ERROR_ABORT_EXIT(("Failed To Load File At: " + path + " in Mode:" + mode).c_str());
	}
}

void sgf::FileReadStream::OpenFileIfExist(const sgf::String& path, const sgf::String& mode)
{
	OpenFile(path, mode, false);
}

void sgf::FileReadStream::OpenMemory(void* pbuffer, int size)
{
	mIStream = SDL_RWFromMem(pbuffer,size);
	mIsOpen = true;
}

void sgf::FileReadStream::Seek(CursorStyle style, int cur) const
{
	CHECK_LOADED;

	switch (style)
	{
	case sgf::FileReadStream::CursorStyle::CURSOR_SET:
		SDL_RWseek(mIStream, cur, RW_SEEK_SET);
		break;
	case sgf::FileReadStream::CursorStyle::CURSOR_END:
		SDL_RWseek(mIStream, cur, RW_SEEK_END);
		break;
	case sgf::FileReadStream::CursorStyle::CURSOR_CUR:
		SDL_RWseek(mIStream, cur, RW_SEEK_CUR);
		break;
	default:
		break;
	}
}

void sgf::FileReadStream::Read(void* pbuffer, int size) const
{
	CHECK_LOADED;
	SDL_RWread(mIStream,pbuffer,size,1);
}

void sgf::FileReadStream::Write(const void* src, int size)
{
	CHECK_LOADED;
	SDL_RWwrite(mIStream,src,size,1);
}

void sgf::FileReadStream::Close()
{
	CHECK_LOADED;
	if(mIsOpen)
		SDL_RWclose(mIStream);
	mIsOpen = false;
}

int sgf::FileReadStream::Tell() const
{
	CHECK_LOADED_X(-1);
	return SDL_RWtell(mIStream);
}

int sgf::FileReadStream::GetSize() const
{
	return SDL_RWsize(mIStream);
}

sgf::String sgf::FileReadStream::ReadString(int size)
{
	CHECK_LOADED_X("");
	char* buf = new char[size+1];
	memset(buf,0,size+1);
	Read(buf,size);
	std::string result = buf;
	delete[] buf;
	return result;
}

