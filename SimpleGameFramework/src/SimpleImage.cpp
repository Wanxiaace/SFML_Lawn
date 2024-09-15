#include "../include/SimpleImage.h"
#include <iostream>


sgf::SimpleImage::SimpleImage()
{

}

sgf::SimpleImage::~SimpleImage()
{
	if (mSurface)
		SDL_FreeSurface(mSurface);
	if (mIsLoadedTexture)
		glDeleteTextures(1,&mTextureHandle);
}

unsigned int sgf::SimpleImage::GenerateTexture()
{
	if (mIsAtlasUnit)
		return mAtlasSrc->GenerateTexture();

	if (mIsLoadedTexture)
		return mTextureHandle;
	unsigned int currentBoundTexture = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&currentBoundTexture);

	GL_CALL(glGenTextures(1, &mTextureHandle););
	GL_CALL(glBindTexture(GL_TEXTURE_2D, mTextureHandle));
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GL_CALL(glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,mSurface->w,mSurface->h,0,GL_RGBA,GL_UNSIGNED_BYTE,mSurface->pixels));
	
	GL_CALL(glBindTexture(GL_TEXTURE_2D, currentBoundTexture));

	mIsLoadedTexture = true;
	return mTextureHandle;
}


void sgf::SimpleImage::ReleaseTexture()
{
	if (mIsLoadedTexture) {
		glDeleteTextures(1, &mTextureHandle);
		mTextureHandle = 0;
		mIsLoadedTexture = false;
	}
}

SDL_Surface* sgf::SimpleImage::LoadFromFile(const char* path)
{
	SDL_Surface* bufferSuface = IMG_Load(path);
	if (!bufferSuface) {
		printf("Failed to load Image at: %s", path);
		return nullptr;
	}

	if (bufferSuface->format->format == SDL_PIXELFORMAT_ABGR8888) {
		mSurface = bufferSuface;
		return mSurface;
	}
	else {
		mSurface = SDL_ConvertSurfaceFormat(bufferSuface, SDL_PIXELFORMAT_ABGR8888, 0);
		SDL_FreeSurface(bufferSuface);
		return mSurface;
	}
}

void sgf::SimpleImage::LoadFromSurface(SDL_Surface* src)
{
	SDL_Surface* bufferSuface = src;
	if (!bufferSuface) {
		return;
	}

	if (bufferSuface->format->format == SDL_PIXELFORMAT_ABGR8888) {
		mSurface = bufferSuface;
	}
	else {
		mSurface = SDL_ConvertSurfaceFormat(bufferSuface, SDL_PIXELFORMAT_ABGR8888, 0);
		SDL_FreeSurface(bufferSuface);
	}
}

void sgf::SimpleImage::DumpToFiles(const char* path) const
{
	if (mSurface)
		IMG_SavePNG(mSurface, path);
}

int sgf::SimpleImage::GetWidth() const
{
	if (mIsAtlasUnit)
		return mSurface->w * mAtlasUnitWidth;
	return mSurface->w;
}

int sgf::SimpleImage::GetHeight() const
{
	if (mIsAtlasUnit)
		return mSurface->h * mAtlasUnitHeight;
	return mSurface->h;
}


