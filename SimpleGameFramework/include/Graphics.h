#pragma once
#ifndef __SIMPLE_GRAPHICS__
#define __SIMPLE_GRAPHICS__
#include "SimpleApp.h"
#include "SimpleImage.h"
#include "Program.h"
#include "Common.h"
#include "../glm/mat4x4.hpp"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include <vector>
#include "GameApp.h"

namespace sgf {
	class GameApp;
	class Graphics;

	class Graphics {
	protected:
		void ResizeCube(float targetX,float targetY,float targetWidth,float targetHeight);
		void SetCubeImagePostion(float u,float v,float width,float height);
		void ReSetCubeImagePostion();
		void SetCubeTextureIndex(float index);
		void SetCubeMatrixIndex(float index);

	public:
		unsigned int mCubeVAO;
		unsigned int mCubeVBO;

		//利用更多的纹理单元和批处理渲染减少drawcall，从而优化在图片多的时候卡顿的问题
		bool mUseBufferOptimize = true;

		Vertex mCubeVertices[6] = {
			{100,	100,	0.0f,	1.0f,	1.0f,	1.0f,	1.0f,	1.0f,	1.0f,	-1,		-1},
			{100,	0,		0.0f,	1.0f,	1.0f,	1.0f,	1.0f,	1.0f,	0.0f,	-1,		-1},
			{0,		0,		0.0f,	1.0f,	1.0f,	1.0f,	1.0f,	0.0f,	0.0f,	-1,		-1},
			{100,	100,	0.0f,	1.0f,	1.0f,	1.0f,	1.0f,	1.0f,	1.0f,	-1,		-1},
			{0,		100,	0.0f,	1.0f,	1.0f,	1.0f,	1.0f,	0.0f,	1.0f,	-1,		-1},
			{0,		0,		0.0f,	1.0f,	1.0f,	1.0f,	1.0f,	0.0f,	0.0f,	-1,		-1},
		};

	public:
		sgf::GameApp* mGameApp = nullptr;
		sgf::Color mCubeColor = { 255,255,255 };
		SimpleProgram mTextureProgram;
		glm::mat4x4 mProjectMatrix;

		Point mTransformPosition = {0,0};
		Point mModelTransformPosition = {0,0};
		SimpleProgram* mNowProgram = nullptr;

		std::vector<Vertex> mVerticesBuffer;
		std::vector<SimpleImage*> mTexturesBuffer;
		std::vector<glm::mat4x4> mMatrixsBuffer;

		int mVerticesNumber = 0;
		int mTexturesNumber = 0;
		int mMatrixsNumber = 0;

	public:
		Graphics(GameApp* gameApp);
		~Graphics() {};

		void Clear();

		void SetCubeColor(const sgf::Color& srcColor);

		void AppendVertices(const Vertex* vertices,int vertexCount);

		void DrawTriangleArrays();

		void ActiveTextureShader();

		void FillRect(int width, int height);
		void FillRect(const FloatRect& src);

		int TryToBindNewTexture(sgf::SimpleImage* src);
		int TryToBindNewMatrix(glm::mat4x4 src);

		void DrawImage(sgf::SimpleImage* src);
		void DrawImageScaleF(sgf::SimpleImage* src,float scaleX = 1.0f, float scaleY = 1.0f);
		void DrawImageInRect(sgf::SimpleImage* src, float width, float height);
		void DrawImageMatrix(sgf::SimpleImage* src, glm::mat4x4 matrix, float oriX = 0, float oriY = 0);

		//根据份数进行切片
		void DrawImageGridAtlas(sgf::SimpleImage* src, float rowNum, float columnNum, float targetRow, float targetColumn);
		
		void DrawImageGridAtlasScaleF(sgf::SimpleImage* src, float rowNum, float columnNum, float targetRow, float targetColumn, float scaleX, float scaleY);

		//绘制纹理图集的一部分,所有参数采用uv坐标系，范围为0~1
		void DrawImageAtlas(sgf::SimpleImage* src, float u, float v, float width, float height);
		
		void DrawImageAtlasScaleF(sgf::SimpleImage* src, float u, float v, float width, float height, float scaleX, float scaleY);

		void Translate(float x, float y);
		void MoveTo(int x, int y);
		void ModelMoveTo(float x, float y);

		void ProjectionResize(float width, float height);
		void Present();

		int GetMaxTextureUnitCount() const;

		//提交所有顶点
		void Submit();
		void CheckSubmit();

		Point GetGraphicsTransformPosition();

	};

	static Graphics* CreateFromApp(GameApp* gameApp) { return new Graphics(gameApp); };

}

#endif
