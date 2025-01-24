#ifndef __POPANIM_H__
#define __POPANIM_H__

#define _SGF_PAM_DEBUG

#include "Common.h"
#include "Graphics.h"
#include "Animator.h"
#include <iostream>
#include <vector>


enum FrameFlag : unsigned int 
{
	FRAME_FLAG_REMOVES = 0b1,
	FRAME_FLAG_APPENDS = 0b10,
	FRAME_FLAG_CHANGES = 0b100,
	FRAME_FLAG_FRAME_NAME = 0b1000,
	FRAME_FLAG_STOP = 0b10000,
	FRAME_FLAG_COMMANDS = 0b100000,
};

enum MoveFlag : unsigned int
{
	MOVE_FLAG_SRCRECT = 32768,
	MOVE_FLAG_ROTATE = 16384,
	MOVE_FLAG_COLOR = 8192,
	MOVE_FLAG_MATRIX = 4096,
	MOVE_FLAG_LONG_COORDS = 2048,
	MOVE_FLAG_ANIM_FRAME_NUM = 1024,
};

namespace sgf {
	struct PamTransform {
		float a = 1.0f;
		float b = 0.0f;
		float c = 0.0f;
		float d = 1.0f;
		float mAnchorX = 0.0f;
		float mAnchorY = 0.0f;
	};

	std::ostream& operator<<(std::ostream& out,const PamTransform& trans);

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
		sgf::Color mColor = { 1,1,1,1 };
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

	class Sprite;

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
		void Draw(sgf::Graphics* g,const glm::mat4x4& matrix, int index, const sgf::Color& color);
		
	};

	class SpriteComponent {
	public:
		int mIndex;
		int mWorkRangeBegin;
		int mWorkRangeEnd;
		DrawableResource mResources;
		std::vector<ChangeInfo> mChanges;

		void Present(sgf::Graphics* g, int index,const glm::mat4x4* tMatrix = nullptr, const sgf::Color* color = nullptr);
	};

	class Sprite {
	public:
		sgf::String mName;
		sgf::String mDescription;
		float mFrameRate;
		int mWorkAeraBegin;
		int mWorkAeraEnd;
		int mFramesCount;
		std::vector<RangeFlag> mFlags;

		std::vector<std::shared_ptr<SpriteComponent>> mComponents;
		std::unordered_map<int, std::shared_ptr<SpriteComponent>> mComponentsMap;

	public:
		void Draw(sgf::Graphics* g, int index, const glm::mat4x4* matrix = nullptr, const sgf::Color* color = nullptr) const;

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

		//Âýµ½·ÉÆð
		void LoadFromJsonFile(const sgf::String& path); 
		//Faster
		void LoadFromPamFile(const sgf::String& path);
		void Present(sgf::Graphics* g,int index) const;
		void PresentScale(sgf::Graphics* g,int index,float scale) const;
	};
}

#endif // !__POPANIM_H__
