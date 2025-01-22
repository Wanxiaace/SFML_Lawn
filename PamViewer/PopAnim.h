#ifndef __POPANIM_H__

#include "Common.h"
#include "Graphics.h"
#include <iostream>
#include <vector>

namespace sgf {
	struct PamTransform {
		float a;
		float b;
		float c;
		float d;
		float mAnchorX;
		float mAnchorY;
	};

	struct CommandInfo {
		sgf::String mCommand;
		sgf::String mParam;
	};

	struct ImageRes {
		sgf::String mName;
		sgf::String mImageID;
		int mWidth;
		int mHeight;
		PamTransform mTransform;
		sgf::SimpleImage* mImagePtr;
	};

	struct AppendInfo {
		int mIndex;
		sgf::String mName;
		int mResource;

		bool mSprite;
		bool mAdditive;
		int mPreloadFrames;
		float mTimescale = 1.0f;
	};

	struct ChangeInfo {
		PamTransform mTransform;
		sgf::Color mColor;
		int mAnim_frame_num;
	};

	struct FrameInfo {
		sgf::String mLabel;
		bool mIsStop;
		std::vector<CommandInfo> mCommandsVec;
		std::vector<int> mRemovesVec;
		std::vector<AppendInfo> mAppendsVec;
		std::vector<ChangeInfo> mChangesVec;

	};

	struct RangeFlag {
		sgf::String mLabel;
		int mIndexBegin;
		int mIndexEnd;
	};

	struct Sprite;

	class DrawableResource {
	public:
		bool mIsSprite = false;
		union
		{
			const sgf::Sprite* mResSprite = nullptr;
			const sgf::ImageRes* mResImage;
		};

	public:
		DrawableResource() {};

		DrawableResource(const sgf::Sprite* res) 
		{ 
			mIsSprite = true; 
			mResSprite = res; 
		};

		DrawableResource(const sgf::ImageRes* res)
		{ 
			mIsSprite = false; 
			mResImage = res; 
		};

	public:
		void Draw(sgf::Graphics* g,const glm::mat4x4& matrix);
		
	};

	class SpriteComponent {
	public:
		int mIndex;
		int mWorkRangeBegin;
		int mWorkRangeEnd;
		DrawableResource mResources;
		std::vector<ChangeInfo> mTransforms;

		void Present(sgf::Graphics* g, int index,const glm::mat4x4* tMatrix = nullptr);
	};

	struct Sprite {
		sgf::String mName;
		sgf::String mDescription;
		float mFrameRate;
		int mWorkAeraBegin;
		int mWorkAeraEnd;
		int mFramesCount;
		std::vector<FrameInfo> mFrames;
		std::vector<RangeFlag> mFlags;

		std::vector<std::shared_ptr<SpriteComponent>> mComponents;
		std::unordered_map<int, std::shared_ptr<SpriteComponent>> mComponentsMap;

	};

	void PamTransform2Matrix(const sgf::PamTransform& transform, glm::mat4x4& transformMatrix);

	class PopAnim {
	public:
		int mVersion = 0;
		int mFrameRate = 0;

		//Size
		int mWidth = 0;
		int mHeight = 0;

		int mImageNum;
		std::vector<ImageRes> mImageReses;
		int mSpritesNum;
		std::vector<Sprite> mSprites;
		Sprite mMainSprite;

	public:
		PopAnim();
		~PopAnim();

		void LoadFile(const sgf::String& path);
		void Present(sgf::Graphics* g,int index);
	};
}

#endif // !__POPANIM_H__
