#include "../include/Reanimation.h"
#include "../pugixml/pugixml.hpp"

sgf::Reanimation::Reanimation()
{
	mResourceManager = nullptr;
}

sgf::Reanimation::~Reanimation()
{
	delete mTracks;
}

void sgf::Reanimation::LoadFromFile(const char* filePath)
{
	mTracks = new std::vector<TrackInfo>;
	pugi::xml_document mDoc = pugi::xml_document();
	auto result = mDoc.load_file(filePath);
	if (!result) {
		std::cout << "Loading " << filePath << " Error with: " << std::endl;
		std::cout << result.description() << std::endl;
	}

	for (auto& x : mDoc.children()) {
		String TagName = x.name();
		if (TagName == "fps") {
			mFPS = x.text().as_int();
			//std::cout << "FPS: " << mFPS << std::endl;
		}
		else if (TagName == "track") {
			TrackInfo aInfo = TrackInfo{ "" };
			int aCounter = 0;
			for (auto y : x.children()) {
				String yTagName = y.name();
				if (yTagName == "name") {
					aInfo.mTrackName = y.text().as_string();
				}
				else if (yTagName == "t") {
					TrackFrameTransform aFInfo = { 0,0,0,0,1.0f,1.0f,0,0,"" };
					if (aInfo.mFrames.size() > 0) {
						aFInfo = aInfo.mFrames[aCounter - 1];
					}
					for (auto z : y.children()) {
						String zTagName = z.name();
						if (zTagName == "x") {
							aFInfo.x = z.text().as_float();
						}
						if (zTagName == "y") {
							aFInfo.y = z.text().as_float();
						}
						if (zTagName == "sx") {
							aFInfo.sx = z.text().as_float();
						}
						if (zTagName == "sy") {
							aFInfo.sy = z.text().as_float();
						}
						if (zTagName == "kx") {
							aFInfo.kx = z.text().as_float();
						}
						if (zTagName == "ky") {
							aFInfo.ky = z.text().as_float();
						}
						if (zTagName == "a") {
							aFInfo.a = z.text().as_int();
						}
						if (zTagName == "f") {
							aFInfo.f = z.text().as_int();
						}
						if (zTagName == "i") {
							aFInfo.i = z.text().as_string();
						}
					}
					aInfo.mFrames.push_back(aFInfo);
					aCounter++;
				}
			}
			mTracks->push_back(aInfo);
		}
	}

	mIsLoaded = true;
}




void sgf::Reanimation::Present(Graphics* g,int frameIndex)
{
	Reanimation* R = this;
	int OffsetX = 0;
	int OffsetY = 0;
	float fScale = 1.0f;

	for (int i = 0; i < R->mTracks->size(); i++) {
		auto& x = (R->mTracks)->at(i);
		if (!x.mAvailable)
			continue;
		TrackFrameTransform aSource = x.mFrames[frameIndex];
		if (aSource.i != "" && !aSource.f) {
			static glm::mat4x4 animMatrix = glm::mat4x4(1.0f);
			Point graPos = g->GetGraphicsTransformPosition();
			TransformToMatrixEx(aSource, &animMatrix, fScale, fScale, graPos.x, graPos.y);

			SimpleImage* targetImage = (SimpleImage*)mResourceManager->mResourcePool[aSource.i];
			if (targetImage) {
				//g->DrawImageMatrixFixed(targetImage,animMatrix);
				
				float matrixPosition = g->TryToBindNewMatrix(animMatrix);
				int tex = g->TryToBindNewTexture(targetImage);

				for (size_t i = 0; i < 6; i++)
				{
					g->mCubeVertices[i].matrixIndex = matrixPosition;
					g->mCubeVertices[i].texIndex = tex;
				}

				g->mCubeVertices[0].x = targetImage->mSurface->w;
				g->mCubeVertices[0].y = targetImage->mSurface->h;
				g->mCubeVertices[1].x = targetImage->mSurface->w;

				g->mCubeVertices[3].x = targetImage->mSurface->w;
				g->mCubeVertices[3].y = targetImage->mSurface->h;
				g->mCubeVertices[4].y = targetImage->mSurface->h;

				g->AppendVertices(g->mCubeVertices, 6);

				g->CheckSubmit();

				for (size_t i = 0; i < 6; i++)
				{
					g->mCubeVertices[i].texIndex = -1;
					g->mCubeVertices[i].matrixIndex = -1;
				}
				
			}
		}

	}
}

void sgf::TransformToMatrixEx(
	sgf::TrackFrameTransform& src, glm::mat4x4* dest, float ScaleX, float ScaleY, float pX, float pY)
{
	float aSkewX = (src.kx) * -0.017453292f;
	float aSkewY = (src.ky) * -0.017453292f;

	sgf::GameMat44* destg = (sgf::GameMat44*)dest;
	destg->x1 = cos(aSkewX);
	destg->x2 = -sin(aSkewX);

	destg->y1 = sin(aSkewY);
	destg->y2 = cos(aSkewY);

	destg->t1 = src.x * ScaleX + pX;
	destg->t2 = src.y * ScaleY + pY;
	destg->t3 = 1.0f;

	*dest = glm::scale(*dest, glm::vec3(src.sx * ScaleX, src.sy * ScaleY, 1.0f));
}
