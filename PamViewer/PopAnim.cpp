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
	else if (js.size() == 3)
		return {
			cosf(js[0]), sinf(js[0]), -sinf(js[0]),cosf(js[0]),js[1],js[2]};
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
		static sgf::RangeFlag* rangeFlag = nullptr;
		if (!x["label"].is_null())
		{
			result.mFlags.push_back({});
			rangeFlag = &result.mFlags.back();
			rangeFlag->mLabel = x["label"];
			rangeFlag->mIndexBegin = frameCounter;
			rangeFlag->mIndexEnd = result.mFramesCount - 1;
		}
		if (x["stop"] && rangeFlag)
			rangeFlag->mIndexEnd = frameCounter;

		for (auto& y : x["remove"])
		{

			int index = y["index"];
			auto& com = result.mComponentsMap[index];
			com->mWorkRangeEnd = frameCounter;
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
			auto& frame = result.mComponentsMap[y["index"]];
			frame->mTransforms.push_back({});
			frame->mTransforms.back().mTransform = LoadTransform(y["transform"]);
		}

		for (auto &x : result.mComponents)
		{
			if (x->mTransforms.size() - 1 < frameCounter - x->mWorkRangeBegin) {
				x->mTransforms.push_back(x->mTransforms.back());
			}
		}
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
	mMainSprite.Draw(g,index);
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

}

void sgf::SpriteComponent::Present(sgf::Graphics* g,int index, const glm::mat4x4* tMatrix)
{
	auto& changeInfo = mTransforms[index];
	glm::mat4x4 matrix;
	PamTransform2Matrix(changeInfo.mTransform,matrix);
	if(tMatrix)
		mResources.Draw(g, (*tMatrix) * matrix, index);
	else
		mResources.Draw(g, matrix, index);
	
}

void sgf::DrawableResource::Draw(sgf::Graphics* g, const glm::mat4x4& matrix, int index)
{
	if (mIsSprite) {
		int childFrame = 
			(index - mResSprite->mWorkAeraBegin) 
			% (mResSprite->mWorkAeraEnd - mResSprite->mWorkAeraBegin);
		mResSprite->Draw(g, childFrame, &matrix);
	}
	else {
		glm::mat4x4 mat = glm::mat4x4(1.0f);
		PamTransform2Matrix(mResImage->mTransform,mat);
		float scaleX = mResImage->mWidth / mResImage->mImagePtr->GetWidth();
		float scaleY = mResImage->mHeight / mResImage->mImagePtr->GetHeight();
		mat = glm::scale(mat, glm::vec3({ scaleX,scaleY,1.0f}));//除以缩放
		g->DrawImageMatrix(mResImage->mImagePtr, matrix * mat);
	}
}

void sgf::Sprite::Draw(sgf::Graphics* g, int index, const glm::mat4x4* matrix) const
{
	for (auto& x : mComponents)
	{
		if (index >= x->mWorkRangeBegin && index <= x->mWorkRangeEnd) {
			if(matrix)
				x->Present(g, index - x->mWorkRangeBegin, matrix);
			else
				x->Present(g, index - x->mWorkRangeBegin);
		}
	}
}
