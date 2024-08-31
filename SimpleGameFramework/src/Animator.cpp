#include "../include/Animator.h"

sgf::Animator::Animator()
{
	mReanim = nullptr;
}

sgf::Animator::Animator(sgf::Reanimation* reanim)
{
	Init(reanim);
}

void sgf::Animator::Play(const PlayState& state)
{
	mPlayingState = state;
	mIsPlaying = true;
}

void sgf::Animator::Init(Reanimation* reanim)
{
	mReanim = reanim;
	mFPS = mReanim->mFPS;
	mDeltaRate = 1000.0f / mFPS;

	for (auto& x : *reanim->mTracks)
	{
		mTrackIndicesMap[x.mTrackName] = mExtraInfos.size();
		mExtraInfos.push_back({});
	}
}

void sgf::Animator::Pause()
{
	mIsPlaying = false;
}

std::pair<int, int> sgf::Animator::GetTrackRange(const sgf::String& trackName)
{
	TrackInfo* aTrack = nullptr;
	for (auto& x : *mReanim->mTracks) {
		if (x.mTrackName == trackName) {
			aTrack = &x;
			break;
		}
	}
	if (!aTrack)
		return { 0,0 };

	int rStart = -1;
	int rEnd = -1;
	for (int i = 0; i < aTrack->mFrames.size(); i++) {
		TrackFrameTransform& x = aTrack->mFrames[i];
		if (rStart == -1) {
			if (x.f == 0)
				rStart = i;
		}
		else {
			if (x.f == -1) {
				rEnd = i;
				break;
			}
		}
	}
	if (rEnd == -1) {
		rEnd = aTrack->mFrames.size();
	}
	return { rStart,rEnd - 1 };
}

void sgf::Animator::SetFrameRange(int frameBegin, int frameEnd)
{
	mFrameIndexBegin = frameBegin;
	mFrameIndexEnd = frameEnd;
}

void sgf::Animator::SetFrameRangeByTrackName(const sgf::String& trackName)
{
	auto range = GetTrackRange(trackName);
	SetFrameRange(range.first, range.second);
	mFrameIndexNow = range.first;
}

void sgf::Animator::SetFrameRangeByTrackNameOnce(const sgf::String& trackName, const sgf::String& oriTrackName)
{
	SetFrameRangeByTrackName(trackName);
	mPlayingState = PLAY_ONCE_TO;
	mTargetTrack = oriTrackName;
}

void sgf::Animator::SetTrackVisible(const sgf::String& trackName, bool visible)
{
	mExtraInfos[mTrackIndicesMap[trackName]].mVisible = visible;
}

void sgf::Animator::TrackAttachImage(const sgf::String& trackName, SimpleImage* target)
{
	mExtraInfos[mTrackIndicesMap[trackName]].mAttachedImage = target;
}

void sgf::Animator::TrackAttachAnimator(const sgf::String& trackName, Animator* target)
{
	mExtraInfos[mTrackIndicesMap[trackName]].mAttachedReanim = target;
}

void sgf::Animator::TrackAttachAnimatorMatrix(const sgf::String& trackName, glm::mat4x4* target)
{
	mExtraInfos[mTrackIndicesMap[trackName]].mAttachedReanimMatrix = target;
}

float sgf::Animator::GetTrackVelocity(const sgf::String& trackName)
{
	TrackInfo* aTrack = nullptr;
	aTrack = &mReanim->mTracks->at(mTrackIndicesMap[trackName]);
	if (int(mFrameIndexNow) == int(mFrameIndexBegin))
		return aTrack->mFrames[mFrameIndexNow + 1].x - aTrack->mFrames[mFrameIndexNow].x;
	return aTrack->mFrames[mFrameIndexNow + 1].x - aTrack->mFrames[mFrameIndexNow - 1].x;
}

bool sgf::Animator::GetTrackVisible(const sgf::String& trackName)
{
	int pos = mTrackIndicesMap[trackName];
	if (mExtraInfos[pos].mVisible)
		return ! mReanim->mTracks->at(pos).mFrames[mFrameIndexNow].f;
	else 
		return false;
}

void sgf::Animator::Update()
{
	if (mIsPlaying) {
		unsigned int delta = sgf::TryGetTicks() - mTickBuffer;
		float num = ((float)delta) / mDeltaRate;

		mTickBuffer = sgf::TryGetTicks();
		float final = (mFrameIndexNow + num * mSpeed);
		if (final >= mFrameIndexEnd) {
			switch (mPlayingState)
			{
			case PLAY_REPEAT:
				mFrameIndexNow = mFrameIndexBegin;
				break;
			case PLAY_ONCE_TO:
				SetFrameRangeByTrackName(mTargetTrack);
				mPlayingState = PLAY_REPEAT;
				break;
			case PLAY_ONCE:
				Pause();
				break;
			}
		}
		else {
			mFrameIndexNow = final;
		}

	}
}

static void GetDeltaTransformEx(const sgf::TrackFrameTransform& tSrc, const sgf::TrackFrameTransform& tDst, float tDelta, sgf::TrackFrameTransform& tOutput) {
	tOutput.a = (tDst.a - tSrc.a) * tDelta + tSrc.a;
	tOutput.x = (tDst.x - tSrc.x) * tDelta + tSrc.x;
	tOutput.y = (tDst.y - tSrc.y) * tDelta + tSrc.y;
	tOutput.sx = (tDst.sx - tSrc.sx) * tDelta + tSrc.sx;
	tOutput.sy = (tDst.sy - tSrc.sy) * tDelta + tSrc.sy;
	tOutput.kx = (tDst.kx - tSrc.kx) * tDelta + tSrc.kx;
	tOutput.ky = (tDst.ky - tSrc.ky) * tDelta + tSrc.ky;
	tOutput.f = tSrc.f;
	tOutput.i = tSrc.i;
}

void sgf::Animator::Present(Graphics* g)
{
	Reanimation* R = mReanim;
	int OffsetX = 0;
	int OffsetY = 0;
	float fScale = 1.0f;

	for (int i = 0; i < R->mTracks->size(); i++) {
		auto& x = (R->mTracks)->at(i);
		if (!x.mAvailable)
			continue;
		if (!mExtraInfos[i].mVisible)
			continue;

		float transformDelta = mFrameIndexNow - int(mFrameIndexNow);
		TrackFrameTransform aSource = x.mFrames[mFrameIndexNow];
		if (int(mFrameIndexNow) < mFrameIndexEnd)//线性插值
			GetDeltaTransformEx(x.mFrames[mFrameIndexNow], x.mFrames[mFrameIndexNow+1],transformDelta,aSource);
		if (!aSource.f) {
			if (!mExtraInfos[i].mAttachedReanim) {
				SimpleImage* targetImage;
				if (mExtraInfos[i].mAttachedImage)
					targetImage = mExtraInfos[i].mAttachedImage;
				else
				{
					if (aSource.i == "")
						continue;
					targetImage = (SimpleImage*)R->mResourceManager->mResourcePool[aSource.i];
				}

				glm::mat4x4 animMatrix = glm::mat4x4(1.0f);
				Point graPos = g->GetGraphicsTransformPosition();
				TransformToMatrixEx(aSource, &animMatrix, fScale, fScale, 0, 0);

				if (targetImage) {
					g->DrawImageMatrix(targetImage, animMatrix);
				}
			}
			else{
				glm::mat4x4 animMatrix = glm::mat4x4(1.0f);
				TransformToMatrixEx(aSource, &animMatrix, fScale, fScale,0, 0);
				if (mExtraInfos[i].mAttachedReanimMatrix)
					animMatrix *= (*mExtraInfos[i].mAttachedReanimMatrix);
				mExtraInfos[i].mAttachedReanim->PresentMatrix(g, animMatrix);
			}
		}
		else {
			if (mExtraInfos[i].mAttachedReanim) {
				glm::mat4x4 animMatrix = glm::mat4x4(1.0f);
				TransformToMatrixEx(aSource, &animMatrix, fScale, fScale, 0, 0);
				if (mExtraInfos[i].mAttachedReanimMatrix)
					animMatrix *= (*mExtraInfos[i].mAttachedReanimMatrix);
				mExtraInfos[i].mAttachedReanim->PresentMatrix(g, animMatrix);
			}
		}
	}
}

void sgf::Animator::PresentMatrix(Graphics* g,const glm::mat4x4& mat)
{
	Reanimation* R = mReanim;
	int OffsetX = 0;
	int OffsetY = 0;
	float fScale = 1.0f;

	for (int i = 0; i < R->mTracks->size(); i++) {
		auto& x = (R->mTracks)->at(i);
		if (!x.mAvailable)
			continue;
		if (!mExtraInfos[i].mVisible)
			continue;

		float transformDelta = mFrameIndexNow - int(mFrameIndexNow);
		TrackFrameTransform aSource = x.mFrames[mFrameIndexNow];
		if (int(mFrameIndexNow) < mFrameIndexEnd)//线性插值
			GetDeltaTransformEx(x.mFrames[mFrameIndexNow], x.mFrames[mFrameIndexNow + 1], transformDelta, aSource);
		if (!aSource.f) {
			if (!mExtraInfos[i].mAttachedReanim) {
				SimpleImage* targetImage;
				if (mExtraInfos[i].mAttachedImage)
					targetImage = mExtraInfos[i].mAttachedImage;
				else
				{
					if (aSource.i == "")
						continue;
					targetImage = (SimpleImage*)R->mResourceManager->mResourcePool[aSource.i];
				}

				glm::mat4x4 animMatrix = glm::mat4x4(1.0f);
				Point graPos = g->GetGraphicsTransformPosition();
				TransformToMatrixEx(aSource, &animMatrix, fScale, fScale, 0, 0);
				animMatrix = mat * animMatrix;
				animMatrix = glm::translate(animMatrix,glm::vec3(0.0f,0.0f,-1.0f));

				if (targetImage) {
					g->DrawImageMatrix(targetImage,animMatrix);
				}
			}
			else {
				glm::mat4x4 animMatrix = glm::mat4x4(1.0f);
				TransformToMatrixEx(aSource, &animMatrix, fScale, fScale, 0, 0);
				mExtraInfos[i].mAttachedReanim->PresentMatrix(g, animMatrix);
			}
		}
		else {
			if (mExtraInfos[i].mAttachedReanim) {
				glm::mat4x4 animMatrix = glm::mat4x4(1.0f);
				TransformToMatrixEx(aSource, &animMatrix, fScale, fScale, 0, 0);
				mExtraInfos[i].mAttachedReanim->PresentMatrix(g, animMatrix);
			}
		}
	}
}
