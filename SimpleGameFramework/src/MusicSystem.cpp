#include "../include/MusicSystem.h"

sgf::MusicManager::MusicManager()
{

}

sgf::MusicManager::~MusicManager()
{
	for (auto& x :mMusicMap)
	{
		Mix_FreeMusic(x.second);
	}

	for (auto& x : mChunkMap)
	{
		Mix_FreeChunk(x.second);
	}
}

void sgf::MusicManager::PlayChunk(const String& chunkKey, int channel)
{
	MixPlayChunk(mChunkMap[chunkKey],channel);
}

void sgf::MusicManager::PlayMusic(const String& musicKey)
{
	MixPlayMusic(mMusicMap[musicKey]);
}

void sgf::MusicManager::PauseMusic()
{
	Mix_PauseMusic();
}

void sgf::MusicManager::ResumeMusic()
{
	Mix_ResumeMusic();
}

void sgf::MusicManager::FadeOutMusic(int ms)
{
	Mix_FadeOutMusic(ms);
}

void sgf::MusicManager::FadeInMusic(const String& musicKey, int ms)
{
	Mix_FadeInMusic(mMusicMap[musicKey],0,ms);
}

void sgf::MusicManager::SetMusicVolume(float volume)
{
	Mix_VolumeMusic(volume * MIX_MAX_VOLUME);
}

void sgf::MusicManager::SetChunkVolume(float volume,const String& key)
{
	Mix_Chunk* chunk = TryGetChunkByKey(key);
	Mix_VolumeChunk(chunk, volume * MIX_MAX_VOLUME);
}

void sgf::MusicManager::SetChannelVolume(int channel, float volume)
{
	Mix_Volume(channel, volume * MIX_MAX_VOLUME);
}

void sgf::MusicManager::MixPlayChunk(Mix_Chunk* chunk,int channel)
{
	Mix_PlayChannel(channel,chunk,0);
}

void sgf::MusicManager::MixPlayMusic(Mix_Music* music)
{
	Mix_PlayMusic(music, -1);
}

void sgf::MusicManager::LoadChunkFormFile(const char* path,const String& key)
{
	Mix_Chunk* chunk = Mix_LoadWAV(path);
	if (chunk) {
		mChunkMap[key] = chunk;
	}
	else {
		std::cout << "Failed to load Chunk At: " << path << std::endl;
	}
}

void sgf::MusicManager::LoadMusicFormFile(const char* path, const String& key)
{
	Mix_Music* chunk = Mix_LoadMUS(path);
	if (chunk) {
		mMusicMap[key] = chunk;
	}
	else {
		std::cout << "Failed to load Music At: " << path << std::endl;
	}
}

Mix_Chunk* sgf::MusicManager::TryGetChunkByKey(const String& key)
{
	return mChunkMap[key];
}


Mix_Music* sgf::MusicManager::TryGetMusicByKey(const String& key)
{
	return mMusicMap[key];
}
