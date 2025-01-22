#include "PopAnim.h"
#include "GamePacker/GamePacker.h"

sgf::SimpleImage* gDemoImage = nullptr;

sgf::PopAnim::PopAnim()
{

}

sgf::PopAnim::~PopAnim()
{

}

static sgf::PamTransform LoadTransform(const nlohmann::json& js) {
	if (js.size() > 3)
		return {
			js[0], js[1], js[2],
			js[3], js[4], js[5] };
	if (js.size() > 2)
		return {
			1, 0, 0,1,js[0],js[1] };
	else
		return {
			1, 0, 0,1,js[0],js[1] };
}

static sgf::Color LoadColor(const nlohmann::json& js) {
	return {};
}

static sgf::Sprite LoadSprite(const nlohmann::json& js,const sgf::PopAnim* pam) {
	sgf::Sprite result = {};

	result.mName = js["name"];
	result.mDescription = js["description"];
	result.mFrameRate = js["frame_rate"];
	result.mWorkAeraBegin = js["work_area"][0];
	result.mWorkAeraEnd = js["work_area"][1];
	result.mFramesCount = js["frame"].size();

	int frameCounter = 0;

	for (auto& x : js["frame"])
	{
		//sgf::FrameInfo info;
		static sgf::RangeFlag* rangeFlag = nullptr;
		if (!x["label"].is_null())
		{
			//info.mLabel = x["label"];
			result.mFlags.push_back({});
			rangeFlag = &result.mFlags.back();
			rangeFlag->mLabel = x["label"];
			rangeFlag->mIndexBegin = frameCounter;
			rangeFlag->mIndexEnd = result.mFramesCount - 1;
		}
		//info.mIsStop = x["stop"];
		if (x["stop"] && rangeFlag)
			rangeFlag->mIndexEnd = frameCounter;

		/*for (auto& y : x["command"])
		{
			info.mCommandsVec.push_back({ y["command"] ,y["parameter"] });
		}*/
		
		for (auto& y : x["remove"])
		{

			int index = y["index"];
			auto& com = result.mComponentsMap[index];
			com->mWorkRangeEnd = frameCounter;
			//info.mRemovesVec.push_back(index);
		}

		for (auto& y : x["append"])
		{
			sgf::AppendInfo apdInfo;
			if (!y["name"].is_null())
				apdInfo.mName = y["name"];
			apdInfo.mIndex = y["index"];
			apdInfo.mResource = y["resource"];
			apdInfo.mSprite = y["sprite"];
			apdInfo.mAdditive = y["additive"];
			apdInfo.mPreloadFrames = y["preload_frames"];
			apdInfo.mTimescale = y["timescale"];
			//info.mAppendsVec.push_back(apdInfo);
			result.mComponents.push_back(std::make_shared<sgf::SpriteComponent>());
			//TODO map有bug
			auto & com = result.mComponents.back();
			result.mComponentsMap[apdInfo.mIndex] = com;
			
			com->mIndex = apdInfo.mIndex;
			com->mWorkRangeBegin = frameCounter;

			if (apdInfo.mSprite) {
				com->mResources = sgf::DrawableResource(&pam->mSprites[apdInfo.mResource]);
			}
			else {
				com->mResources = sgf::DrawableResource(&pam->mImageReses[apdInfo.mResource]);
			}
		}



		for (auto& y : x["change"])
		{
			/*sgf::ChangeInfo change;
			change.mIndex = y["index"];
			change.mTransform = LoadTransform(y["transform"]);
			change.mColor = LoadColor(y["color"]);
			change.mAnim_frame_num = y["anim_frame_num"];
			info.mChangesVec.push_back(change);*/

			auto& frame = result.mComponentsMap[y["index"]];
			frame->mTransforms.push_back({});
			frame->mTransforms.back().mTransform = LoadTransform(y["transform"]);
		}

		for (auto &x : result.mComponents)
		{
			if (x->mTransforms.size() -1 < frameCounter) {
				x->mTransforms.push_back(x->mTransforms.back());
			}
		}

		//result.mFrames.push_back(info);
		frameCounter++;
	}

	return result;
}

void sgf::PopAnim::LoadFile(const sgf::String& path)
{
	nlohmann::json json = sgf::FileManager::TryToLoadJsonFile(path);
	mVersion = json["version"];
	mFrameRate = json["frame_rate"];
	mWidth = json["size"][0];
	mHeight = json["size"][1];

	mImageNum = json["image"].size();
	for (auto& x : json["image"])
	{
		ImageRes res;
		sgf::String nameStr = x["name"];
		res.mName = nameStr.substr(0, nameStr.find('|'));
		res.mImageID = nameStr.substr(nameStr.find('|') + 1);
		res.mWidth = x["size"][0];
		res.mHeight = x["size"][1];
		res.mTransform = LoadTransform(x["transform"]);

		SimpleImage* img = new SimpleImage();
		img->LoadFromFile(("res/" + res.mName + ".png").c_str());

		res.mImagePtr = img;
		mImageReses.push_back(res);
	}

	mSpritesNum = json["sprite"].size();
	mSprites.reserve(mSpritesNum + 1);//提前分配空间解决bug
	for (auto& x : json["sprite"])
	{
		mSprites.push_back(LoadSprite(x, this));
	}

	mMainSprite = LoadSprite(json["main_sprite"], this);

	std::cout << "Version: " << mVersion << std::endl;
	std::cout << "FPS: " << mFrameRate << std::endl;
	std::cout << "Size: " << mWidth << ", " << mHeight << std::endl;
	std::cout << "ImageNum: " << mImageNum << std::endl;
}

void sgf::PopAnim::Present(sgf::Graphics* g,int index)
{
	for (auto & x : mMainSprite.mComponents)
	{
		if (index >= x->mWorkRangeBegin && index < x->mWorkRangeEnd) {
			x->Present(g, index);
		}
	}
}

void sgf::PamTransform2Matrix(const sgf::PamTransform& transform, glm::mat4x4& transformMatrix)
{
	transformMatrix = glm::mat4x4(1.0f);

	transformMatrix[0][0] = transform.a;
	transformMatrix[1][0] = transform.c;

	transformMatrix[0][1] = transform.b;
	transformMatrix[1][1] = transform.d;

	transformMatrix[3][0] = transform.mAnchorX;
	transformMatrix[3][1] = transform.mAnchorY;

	/*transformMatrix[0][0] = transform.a;
	transformMatrix[0][1] = transform.c;
	transformMatrix[0][3] = transform.mAnchorX;

	transformMatrix[1][0] = transform.b;
	transformMatrix[1][1] = transform.d;
	transformMatrix[1][3] = transform.mAnchorY;*/
}

void sgf::SpriteComponent::Present(sgf::Graphics* g,int index, const glm::mat4x4* tMatrix)
{
	auto& changeInfo = mTransforms[index - mWorkRangeBegin];
	glm::mat4x4 matrix;
	PamTransform2Matrix(changeInfo.mTransform,matrix);
	if(tMatrix)
		mResources.Draw(g, matrix * (*tMatrix));
	else
		mResources.Draw(g, matrix);
	
}

void sgf::DrawableResource::Draw(sgf::Graphics* g, const glm::mat4x4& matrix)
{
	if (mIsSprite) {
		mResSprite->mComponents[0]->Present(g,0, &matrix);
	}
	else {
		glm::mat4x4 mat;
		PamTransform2Matrix(mResImage->mTransform,mat);
		g->DrawImageMatrix(mResImage->mImagePtr, matrix /*glm::mat4x4(1.0f)*/);
	}
}
