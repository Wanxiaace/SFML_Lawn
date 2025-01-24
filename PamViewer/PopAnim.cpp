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
			cosf(js[0]), sinf(js[0]), -sinf(js[0]),cosf(js[0]),js[1],js[2] };
	else if (js.size() > 0)
		return { 1, 0, 0,1,js[0],js[1] };
	else return {};
}


static sgf::Color LoadColor(const nlohmann::json& js) {
	return { js[0],js[1],js[2],js[3] };
}

static sgf::Sprite LoadSprite(const nlohmann::json& js,const sgf::PopAnim* pam) {
	sgf::Sprite result = {};

	result.mName = js["name"];
	result.mDescription = js["description"];
	result.mFrameRate = js["frame_rate"];
	result.mWorkAeraBegin = js["work_area"][0];
	result.mWorkAeraEnd = js["work_area"][1] + result.mWorkAeraBegin;
	result.mFramesCount = js["frame"].size();

	int frameCounter = 0;

	sgf::RangeFlag* rangeFlag = nullptr;

	for (auto& x : js["frame"])
	{
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
			com->mWorkRangeEnd = result.mFramesCount;

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
			frame->mChanges.push_back({});
			if (!y["transform"].is_null())
				frame->mChanges.back().mTransform = LoadTransform(y["transform"]);
			else if(frame->mChanges.size() > 1)
				frame->mChanges.back().mTransform = (frame->mChanges.end() - 2)->mTransform;
			if (!y["color"].is_null())
				frame->mChanges.back().mColor = LoadColor(y["color"]);
			else if(frame->mChanges.size() > 1)
				frame->mChanges.back().mColor = (frame->mChanges.end() - 2)->mColor;
		}

		for (auto &x : result.mComponents)
		{
			if (frameCounter <= x->mWorkRangeEnd && x->mChanges.size() - 1 < frameCounter - x->mWorkRangeBegin) {
				x->mChanges.push_back(x->mChanges.back());
			}
		}
		frameCounter++;
	}

	std::sort(result.mComponents.begin(), result.mComponents.end(), 
		[](
			const std::shared_ptr<sgf::SpriteComponent>& a, 
			const std::shared_ptr<sgf::SpriteComponent>& b) -> bool {
				return a->mIndex < b->mIndex;
		});

	return result;
}

void sgf::PopAnim::LoadFromJsonFile(const sgf::String& path)
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

#ifdef _SGF_PAM_DEBUG
		SimpleImage* img = new SimpleImage();
		img->LoadFromFile(("res/" + res.mName + ".png").c_str());

		res.mImagePtr = img;
#endif
		mImageReses.push_back(res);
	}

	mSpritesNum = json["sprite"].size();
	mSprites.reserve(mSpritesNum + 1);//提前分配空间解决bug
	for (auto& x : json["sprite"])
	{
		mSprites.push_back(LoadSprite(x, this));
	}

	mMainSprite = LoadSprite(json["main_sprite"], this);
}

static void LoadSpriteBinary(sgf::FileStream* file, sgf::Sprite& sprite,sgf::PopAnim* pam)
{
	sprite.mName = file->ReadString(file->ReadShort());
	sprite.mDescription = file->ReadString(file->ReadShort());
	sprite.mFrameRate = file->ReadInt() / 65536.0;
	sprite.mFramesCount = file->ReadShort();
	sprite.mWorkAeraBegin = file->ReadShort();
	//sprite.mWorkAeraEnd = file->ReadShort();
	file->Pass(2);
	sprite.mWorkAeraEnd = sprite.mWorkAeraBegin + sprite.mFramesCount;

	sgf::RangeFlag* rangeFlag = nullptr;

	for (int i = 0; i < sprite.mFramesCount; i++)
	{
		unsigned char flags = file->ReadByte();

		if (flags & FrameFlag::FRAME_FLAG_REMOVES) {
			unsigned int removeCount = file->ReadByte();

			if (removeCount == 255)
				removeCount = file->ReadShort();


			for (unsigned int j = 0; j < removeCount; j++)
			{
				int removeId = file->ReadShort();
				//std::cerr << "Remove: " << removeId << " " << "at frame: " << i << " " << j+1 << " of: " << removeCount << std::endl;

				if (removeId >= 2047)
				{
					removeId = file->ReadInt();
				}

				auto& com = sprite.mComponentsMap[removeId];
				com->mWorkRangeEnd = i;
				//TODO 实现Remove
			}
		}

		if (flags & FrameFlag::FRAME_FLAG_APPENDS) {
			unsigned int appendCount = file->ReadByte();
			if (appendCount == 255)
				appendCount = file->ReadShort();
			for (unsigned int j = 0; j < appendCount; j++)
			{
				unsigned short infoMask = file->ReadWord();
				int index = infoMask & 2047;
				if (index == 2047)
				{
					index = file->ReadInt();
				}


				bool isSprite = ((infoMask & 32768) != 0);
				bool additive = ((infoMask & 16384) != 0);

				int resource = file->ReadByte();
				if (resource == 255)
				{
					resource = file->ReadShort();
				}

				if ((infoMask & 8192) != 0)
				{
					//preload_frames
					file->ReadShort();
				}

				if ((infoMask & 4096) != 0)
				{
					//name
					file->ReadString(file->ReadShort());
				}

				if ((infoMask & 2048) != 0)
				{
					//TimeScale
					file->ReadInt() / 65536.0f;
				}

				sprite.mComponents.push_back(std::make_shared<sgf::SpriteComponent>());
				auto& com = sprite.mComponents.back();
				sprite.mComponentsMap[index] = com;
				com->mIndex = index;
				com->mWorkRangeBegin = i;
				com->mWorkRangeEnd = sprite.mFramesCount;

				if (isSprite) {
					com->mResources = sgf::DrawableResource(&pam->mSprites[resource]);
				}
				else {
					com->mResources = sgf::DrawableResource(&pam->mImageReses[resource]);
				}

				//com->mResources = resource;
			}
		}

		if (flags & FrameFlag::FRAME_FLAG_CHANGES) {
			int count = file->ReadByte();
			if (count == 255)
				count = file->ReadShort();

			for (int j = 0; j < count; j++)
			{
				unsigned short infoMask = file->ReadWord();

				int index = infoMask & 1023;
				if (index == 1023)
				{
					index = file->ReadInt();
				}

				unsigned int moveFlags = infoMask;

				if (moveFlags & MoveFlag::MOVE_FLAG_MATRIX) {
					auto& com = sprite.mComponentsMap[index];
					com->mChanges.push_back({});
					auto & trans = com->mChanges.back().mTransform;
					trans.a = file->ReadInt() / 65536.0;
					trans.c = file->ReadInt() / 65536.0;
					trans.b = file->ReadInt() / 65536.0;
					trans.d = file->ReadInt() / 65536.0;
				}
				else if (moveFlags & MoveFlag::MOVE_FLAG_ROTATE) {
					auto& com = sprite.mComponentsMap[index];
					com->mChanges.push_back({});
					auto& trans = com->mChanges.back().mTransform;
					double rotate = file->ReadShort() / 1000.0;
					trans.a = cosf(rotate);
					trans.b = sinf(rotate);
					trans.c = -sinf(rotate);
					trans.d = cosf(rotate);
				}
				else {
					auto& com = sprite.mComponentsMap[index];
					com->mChanges.push_back({ 1,0,0,1,0,0 });
				}

				
				auto& com = sprite.mComponentsMap[index];
				auto& changeInfo = com->mChanges.back();

				/*更长的坐标*/
				if (moveFlags & MoveFlag::MOVE_FLAG_LONG_COORDS) {
					changeInfo.mTransform.mAnchorX = file->ReadInt() / 20.0;
					changeInfo.mTransform.mAnchorY = file->ReadInt() / 20.0;
				}
				else {
					changeInfo.mTransform.mAnchorX = file->ReadShort() / 20.0;
					changeInfo.mTransform.mAnchorY = file->ReadShort() / 20.0;
				}

				if (moveFlags & MoveFlag::MOVE_FLAG_SRCRECT) {
					file->Pass(8);//暂时不处理Rect
				}

				if (moveFlags & MoveFlag::MOVE_FLAG_COLOR) {
					file->Pass(4);//暂时不处理Color
				}

				if (moveFlags & MoveFlag::MOVE_FLAG_ANIM_FRAME_NUM) {
					changeInfo.mAnim_frame_num = file->ReadShort();
				}
				else
				{
					changeInfo.mAnim_frame_num = 0;
				}

			}
		}

		if (flags & FrameFlag::FRAME_FLAG_FRAME_NAME) {
			sprite.mFlags.push_back({});
			rangeFlag = &sprite.mFlags.back();
			rangeFlag->mLabel = file->ReadString(file->ReadShort());
			rangeFlag->mIndexBegin = i;
			rangeFlag->mIndexEnd = sprite.mFramesCount - 1;
		}

		if (rangeFlag && (flags & FrameFlag::FRAME_FLAG_STOP))
		{
			rangeFlag->mIndexEnd = i;
		}

		if (flags & FrameFlag::FRAME_FLAG_COMMANDS)
		{
			int count = file->ReadByte();
			//暂时不处理
			for (int j = 0; j < count; j++)
			{
				file->ReadString(file->ReadShort()); //Command
				file->ReadString(file->ReadShort()); //Parameter
			}
		}

		for (auto& x : sprite.mComponents)
		{
			if (i <= x->mWorkRangeEnd && x->mChanges.size() - 1 < i - x->mWorkRangeBegin) {
				x->mChanges.push_back(x->mChanges.back());
			}
		}
	}

	std::sort(sprite.mComponents.begin(), sprite.mComponents.end(),
		[](
			const std::shared_ptr<sgf::SpriteComponent>& a,
			const std::shared_ptr<sgf::SpriteComponent>& b) -> bool {
				return a->mIndex < b->mIndex;
		});
}

void sgf::PopAnim::LoadFromPamFile(const sgf::String& path)
{
	sgf::FileStream * file = sgf::FileManager::TryToLoadFilePointer(path);
	if (!file)
		SHOW_ERROR_ABORT_EXIT(("Failed to load PAM in: " + path).c_str());
	unsigned int cookie = file->ReadInt();
	printf("Cookie: %X \n", cookie);

	mVersion = file->ReadInt();

	if (mVersion < 6)
		SHOW_ERROR_ABORT_EXIT("the version of PAM is too low !");

	mFrameRate = file->ReadByte();

	file->Pass(4);

	mWidth = file->ReadShort() / 20;
	mHeight = file->ReadShort() / 20;

	mImageNum = file->ReadShort();

	for (int i = 0; i < mImageNum; i++)
	{
		mImageReses.push_back({});
		ImageRes& res = mImageReses.back();
		sgf::String nameStr = file->ReadString(file->ReadShort());

		res.mName = nameStr.substr(0, nameStr.find('|'));
		res.mImageID = nameStr.substr(nameStr.find('|') + 1);

		res.mWidth = file->ReadShort();
		res.mHeight = file->ReadShort();
		res.mTransform = {
			file->ReadInt() / 1310720.0f,
			file->ReadInt() / 1310720.0f,
			file->ReadInt() / 1310720.0f,
			file->ReadInt() / 1310720.0f,
			file->ReadShort() / 20.0f,
			file->ReadShort() / 20.0f,
		};

#ifdef _SGF_PAM_DEBUG
		SimpleImage* img = new SimpleImage();
		img->LoadFromFile(("res/" + res.mName + ".png").c_str());
		res.mImagePtr = img;
#endif
	}

	mSpritesNum = file->ReadShort();

	std::cout << "SpritesNum: " << mSpritesNum << std::endl;
	mSprites.reserve(mSpritesNum + 1);

	for (int i = 0; i < mSpritesNum; i++)
	{
		mSprites.push_back({});
		LoadSpriteBinary(file, mSprites.back(),this);
	}
	
	if (file->ReadBool()) {
		LoadSpriteBinary(file, mMainSprite, this);
	}
	

	file->Close();
}

void sgf::PopAnim::Present(sgf::Graphics* g,int index) const
{
	mMainSprite.Draw(g,index);
}

void sgf::PopAnim::PresentScale(sgf::Graphics* g, int index, float scale) const
{
	glm::mat4x4 scaleMat = glm::mat4x4(1.0f);
	scaleMat = glm::scale(scaleMat, glm::vec3(scale, scale, 1.0f));
	mMainSprite.Draw(g, index, &scaleMat);
}

std::ostream& sgf::operator<<(std::ostream& out, const PamTransform& trans)
{
	out << trans.a << " " 
		<< trans.b << " " 
		<< trans.c << " " 
		<< trans.d << " " 
		<< trans.mAnchorX << " "
		<< trans.mAnchorY;
	return out;
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

void sgf::SpriteComponent::Present(sgf::Graphics* g,int index, const glm::mat4x4* tMatrix, const sgf::Color* color)
{
	auto& changeInfo = mChanges[index];
	glm::mat4x4 matrix;
	PamTransform2Matrix(changeInfo.mTransform,matrix);
	if (tMatrix)
		if(color)
			mResources.Draw(g, (*tMatrix) * matrix, index, changeInfo.mColor * (*color));
		else
			mResources.Draw(g, (*tMatrix) * matrix, index, changeInfo.mColor);
	else
		if (color)
			mResources.Draw(g, matrix, index, changeInfo.mColor * (*color));
		else
			mResources.Draw(g, matrix, index, changeInfo.mColor);
	
}

void sgf::DrawableResource::Draw(sgf::Graphics* g, const glm::mat4x4& matrix, int index, const sgf::Color& color)
{
	if (mIsSprite) {
		int childFrame =
			(index - mResSprite->mWorkAeraBegin)
			% (mResSprite->mWorkAeraEnd - mResSprite->mWorkAeraBegin);
		mResSprite->Draw(g, childFrame, &matrix, &color);
	}
	else {
		glm::mat4x4 mat = glm::mat4x4(1.0f);
		PamTransform2Matrix(mResImage->mTransform, mat);
		float scaleX = mResImage->mWidth / mResImage->mImagePtr->GetWidth();
		float scaleY = mResImage->mHeight / mResImage->mImagePtr->GetHeight();
		mat = glm::scale(mat, glm::vec3({ scaleX,scaleY,1.0f }));//除以缩放

		auto colorTemp = g->mCubeColor;
		g->MulCubeColor(color);
		g->DrawImageMatrix(mResImage->mImagePtr, matrix * mat);

		g->SetCubeColor(colorTemp);
	}
}

void sgf::Sprite::Draw(sgf::Graphics* g, int index, const glm::mat4x4* matrix,const sgf::Color* color) const
{

	for (auto& x : mComponents)
	{
		if (index >= x->mWorkRangeBegin && index <= x->mWorkRangeEnd) {
			x->Present(g, index - x->mWorkRangeBegin, matrix, color);
		}
	}
}
