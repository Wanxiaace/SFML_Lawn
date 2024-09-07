#include "../include/ResourceManager.h"
#include "../include/SimpleImage.h"
#include "../pugixml/pugixml.hpp"
#include "../include/Reanimation.h"
#include "../include/Font.h"
#include "../include/Particle.h"

sgf::ResourceManager::ResourceManager()
{

}

sgf::ResourceManager::~ResourceManager()
{
	for (auto & x: mResourcePool)
	{
		delete x.second;
	}
}

void sgf::ResourceManager::LoadImageWithID(const sgf::String& path, sgf::String id)
{
	SimpleImage* image = new SimpleImage();
	image->LoadFromFile(path.c_str());
	mResourcePool[id] = image;
}

void sgf::ResourceManager::LoadReanimWithID(const sgf::String& path, sgf::String id)
{
	Reanimation* reanim = new Reanimation();
	reanim->LoadFromFile(path.c_str());
	reanim->mResourceManager = this;
	mResourcePool[id] = reanim;
}

void sgf::ResourceManager::LoadFontWithID(const sgf::String& aPath, sgf::String id)
{
	Font* font = new Font();
	font->LoadFromFile(aPath.c_str());
	mResourcePool[id] = font;
}

//请在加载完所有图片后调用此函数
void sgf::ResourceManager::LoadParticleWithID(const sgf::String& aPath, sgf::String id)
{
	Emitter* particle = new Emitter();
	particle->LoadFromFile(aPath.c_str());
	int length = particle->mImageNumber;
	for (size_t i = 0; i < length; i++)
	{
		particle->AttachImage((SimpleImage*)mResourcePool[particle->mImageIDs[i]]);
		std::cout << particle->mImageIDs[i] << std::endl;
		std::cout << (void*)mResourcePool[particle->mImageIDs[i]] << std::endl;
	}
	mResourcePool[id] = particle;
}


void sgf::ResourceManager::LoadFromResouceList(ResourceList* src,MusicManager* mus)
{
	mIsLoaded = false;
	mProcess = 0.0f;
	float delta = 100.0f / src->mResouces.size();
	mTotalFile = src->mResouces.size();
	mNowFile = 0;
	for (auto& x : src->mResouces) {
		mNowFile += 1;
		mNowInfo = &x;
		if (x.folder == "image" || x.folder == "image_reanim" || x.folder == "particle") {
			LoadImageWithID((mBasePath + x.path), x.id);
		}
		else if (x.folder == "raxml") {
			LoadReanimWithID((mBasePath + x.path), x.id);
		}

		if (x.folder == "font") {
			LoadFontWithID((mBasePath + x.path), x.id);
		}

		if (x.folder == "chunk") {
			mus->LoadChunkFormFile((mBasePath + x.path).c_str(), x.id);
		}

		if (x.folder == "music") {
			mus->LoadMusicFormFile((mBasePath + x.path).c_str(), x.id);
		}

		if (x.folder == "paxml") {
			LoadParticleWithID((mBasePath + x.path).c_str(), x.id);
		}

		std::cout << x.path << std::endl;
		mProcess += delta;

	}
	mIsLoaded = true;
}


void sgf::ResourceList::Load(const char* path)
{
	mPath = sgf::String(path);
	pugi::xml_document doc;
	doc.load_file(path);

	for (auto& x : *doc.children().begin()) {
		//std::cout << x.name() << std::endl;
		for (auto& y : x.children()) {
			ResouceInfo aInf;
			sgf::String fPath = x.name() + sgf::String("/") + y.attribute("path").as_string();
			std::cout << "Path: " << fPath << std::endl;

			std::cout << "ID: " << y.attribute("id").as_string() << std::endl;

			aInf.path = fPath;
			aInf.id = y.attribute("id").as_string();
			aInf.folder = x.attribute("folder").as_string();
			mResouces.push_back(aInf);
		}
	}
}
