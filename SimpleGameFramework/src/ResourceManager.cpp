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

	mMutex.lock();
	mResourcePool[id] = image;
	mMutex.unlock();
}

void sgf::ResourceManager::LoadReanimWithID(const sgf::String& path, sgf::String id)
{
	Reanimation* reanim = new Reanimation();
	reanim->LoadFromFile(path.c_str());
	reanim->mResourceManager = this;
	mMutex.lock();
	mResourcePool[id] = reanim;
	mMutex.unlock();
	//mResourcePool[id] = reanim;
}

void sgf::ResourceManager::LoadFontWithID(const sgf::String& aPath, sgf::String id)
{
	Font* font = new Font();
	font->LoadFromFile(aPath.c_str());
	//mResourcePool[id] = font;
	mMutex.lock();
	mResourcePool[id] = font;
	mMutex.unlock();
}

//请在加载完所有图片后调用此函数
void sgf::ResourceManager::LoadParticleWithID(const sgf::String& aPath, sgf::String id)
{
	Emitter* particle = new Emitter();
	particle->LoadFromFile(aPath.c_str());
	int length = particle->mImageNumber;
	
	mEmitters.push_back(particle);
	mMutex.lock();
	mResourcePool[id] = particle;
	mMutex.unlock();
	//mResourcePool[id] = particle;
}

void sgf::ResourceManager::LoadParticleImages()
{
	for (auto& x : mEmitters)
	{
		Emitter* emitter = (Emitter*)x;
		int length = emitter->mImageNumber;
		for (size_t i = 0; i < length; i++)
		{
			emitter->AttachImage((SimpleImage*)mResourcePool[emitter->mImageIDs[i]]);
		}
	}
}


static void LoadFromResouceListFunc(sgf::ResourceManager* tar,sgf::ResourceList* src, sgf::MusicManager* mus,int beginIndex,int endIndex)
{
	//int len = src->mResouces.size();
	float delta = 100.0f / src->mResouces.size();

	for (int i = beginIndex; i < endIndex; i++) {
		tar->mNowFile += 1;
		auto& x = src->mResouces[i];
		tar->mNowInfo = &x;

		if (x.folder == "image" || x.folder == "image_reanim" || x.folder == "particle") {
			tar->LoadImageWithID((tar->mBasePath + x.path), x.id);
		}
		else if (x.folder == "raxml") {
			tar->LoadReanimWithID((tar->mBasePath + x.path), x.id);
		}

		if (x.folder == "font") {
			tar->LoadFontWithID((tar->mBasePath + x.path), x.id);
		}

		if (x.folder == "chunk") {
			mus->LoadChunkFormFile((tar->mBasePath + x.path).c_str(), x.id);
		}

		if (x.folder == "music") {
			mus->LoadMusicFormFile((tar->mBasePath + x.path).c_str(), x.id);
		}

		if (x.folder == "paxml") {
			tar->LoadParticleWithID((tar->mBasePath + x.path).c_str(), x.id);
		}
		//输出加载的路径
		std::cout << x.path << std::endl;
		tar->mProcess += delta;
	}
}

void sgf::ResourceManager::LoadFromResouceList(ResourceList* src,MusicManager* mus)
{
	mIsLoaded = false;
	mProcess = 0.0f;
	float delta = 100.0f / src->mResouces.size();
	mTotalFile = src->mResouces.size();
	mNowFile = 0;

	int len = src->mResouces.size();
	int stride = len / (LOADING_THREAD_NUM_MAX+1);

	std::vector<std::thread*> loadingThreads;
	for (size_t i = 0; i < LOADING_THREAD_NUM_MAX; i++)
	{
		loadingThreads.push_back(new std::thread(LoadFromResouceListFunc, this, src, mus, i * stride, i * stride + stride));
	}

	LoadFromResouceListFunc(this,src,mus, LOADING_THREAD_NUM_MAX * stride,len);

	for (size_t i = 0; i < LOADING_THREAD_NUM_MAX; i++)
	{
		loadingThreads[i]->join();
	}

	LoadParticleImages();
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
