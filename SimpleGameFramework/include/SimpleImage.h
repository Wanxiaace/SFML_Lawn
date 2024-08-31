#pragma once
#ifndef __SIMPLE_SIMPLE_IMAGE__
#define __SIMPLE_SIMPLE_IMAGE__

#include "../include/Common.h"

namespace sgf {
	class SimpleImage {
	public:
		SDL_Surface* mSurface = nullptr;
		bool mIsLoadedTexture = false;
		unsigned int mTextureHandle = 0;
		char* mPixels = nullptr;

	public:
		SimpleImage();
		~SimpleImage();

		unsigned int GenerateTexture();
		void BindTexture();
		void ReleaseTexture();
		SDL_Surface* LoadFromFile(const char* path);
		void LoadFromSurface(SDL_Surface* path);

	};
}
#endif
