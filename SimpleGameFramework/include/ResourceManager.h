#pragma once
#ifndef __SIMPLE_RESOURCE_MANAGER__
#define __SIMPLE_RESOURCE_MANAGER__
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Common.h"
#include "MusicSystem.h"

namespace sgf{

	struct ResouceInfo {
		sgf::String path = "";
		sgf::String id = "";
		sgf::String folder = "";
	};

	class ResourceList {
	public:
		sgf::String mPath;
		std::vector<ResouceInfo> mResouces;
	public:
		ResourceList() {};
		~ResourceList() {};

		void Load(const char* path);
	};

	class ResourceManager {
	public:
		std::unordered_map<sgf::String, void*> mResourcePool;
		ResouceInfo* mNowInfo = nullptr;
		bool mIsLoaded = false;
		float mProcess = 0;
		int mTotalFile = 0;
		int mNowFile = 0;

	public:
		ResourceManager();
		~ResourceManager();

		void LoadImageWithID(const sgf::String& aPath, sgf::String id);
		void LoadReanimWithID(const sgf::String& aPath, sgf::String id);
		void LoadFontWithID(const sgf::String& aPath, sgf::String id);
		void LoadParticleWithID(const sgf::String& aPath, sgf::String id);

		void LoadFromResouceList(ResourceList* src,MusicManager* mus);
		template <typename T>
		T* GetResource(const sgf::String& key)
		{
			if (mResourcePool.find(key) == mResourcePool.end())
				return nullptr;
			return (T*)mResourcePool[key];
		}

		//更加的快速，不起用资源检查
		template <typename T>
		T* GetResourceFast(const sgf::String& key)
		{
			return (T*)mResourcePool[key];
		}
	};
}

#endif