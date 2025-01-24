#pragma once
#ifndef __SIMPLE_GRAPHICS__
#define __SIMPLE_GRAPHICS__
#include "SimpleApp.h"
#include "SimpleImage.h"
#include "Program.h"
#include "Common.h"
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

		///���ø��������Ԫ����������Ⱦ����drawcall���Ӷ��Ż���ͼƬ���ʱ�򿨶ٵ�����
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
		SimpleProgram mBlurProgram;


		glm::mat4x4 mProjectMatrix;

		Point mTransformPosition = {0,0};
		Point mModelTransformPosition = {0,0};
		SimpleProgram* mNowProgram = nullptr;

		std::vector<Vertex> mVerticesBuffer;
		std::vector<SimpleImage*> mImagesBuffer;
		std::vector<glm::mat4x4> mMatrixsBuffer;

		int mVerticesNumber = 0;
		int mImagesNumber = 0;
		int mMatrixsNumber = 0;
		int mTexturesNumber = 0;


	public:
		Graphics(GameApp* gameApp);
		~Graphics() {};

		/// @brief �����Ļ
		void Clear();

		/// @brief ������ɫ
		/// @param srcColor Ŀ����ɫ
		void SetCubeColor(const sgf::Color& srcColor);

		/// @brief ����˵ķ�ʽ�����ɫ
		/// @param srcColor Ŀ����ɫ
		void MulCubeColor(const sgf::Color& srcColor);

		/// @brief ��������뻺����
		/// @param vertices Ŀ�궥������
		/// @param vertexCount Ŀ�궥�����
		void AppendVertices(const Vertex* vertices,int vertexCount);

		/// @brief ����������
		void DrawTriangleArrays();

		/// @brief ����������ɫ��
		void ActiveTextureShader();

		// Filter
		/// @brief ����BoxBlur��ɫ��
		void ActiveBlurShader();

		/// @brief �л���ɫ��
		void SwitchShader(SimpleProgram* prog);

		/// @brief ������,���ú�����Graphics::Move()
		/// @param width ���
		/// @param height �߶�
		void FillRect(int width, int height);
		/// @brief ������
		/// @param src Ŀ�����
		void FillRect(const FloatRect& src);

		/// @brief ��һ����ͼ��������
		/// @param src Ŀ����ͼ
		/// @return ������ظ�����ͼ�򷵻�������������µ���ͼ���԰󶨲���������
		int TryToBindNewTexture(sgf::SimpleImage* src);

		/// @brief ��һ�����󵽻�����
		/// @param src Ŀ�����
		/// @return ���԰󶨾��󲢷�������
		int TryToBindNewMatrix(glm::mat4x4 src);


		/// @brief ����ͼƬ
		/// @param src Ŀ��ͼƬ
		void DrawImage(sgf::SimpleImage* src);

		/// @brief ����ͼƬ����
		/// @param src Ŀ��ͼƬ
		/// @param scaleX �����������
		/// @param scaleY �����������
		void DrawImageScaleF(sgf::SimpleImage* src,float scaleX = 1.0f, float scaleY = 1.0f);

		/// @brief ����ͼƬ����������
		/// @param src Ŀ��ͼƬ
		/// @param width Ŀ����
		/// @param height Ŀ��߶�
		void DrawImageInRect(sgf::SimpleImage* src, float width, float height);

		/// @brief ����ͼƬ��Ӧ�þ���
		/// @param src Ŀ��ͼƬ
		/// @param matrix Ŀ�����
		/// @param oriX ԭ��X(��ʱ����)
		/// @param oriY ԭ��Y(��ʱ����)
		void DrawImageMatrix(sgf::SimpleImage* src, glm::mat4x4 matrix, float oriX = 0, float oriY = 0);

		/// @brief ���ݷ���������Ƭ
		/// @param src Ŀ��ͼƬ
		/// @param rowNum ������
		/// @param columnNum ������
		/// @param targetRow Ŀ����
		/// @param targetColumn Ŀ����
		void DrawImageGridAtlas(sgf::SimpleImage* src, float rowNum, float columnNum, float targetRow, float targetColumn);
		
		/// @brief ���ݷ���������Ƭ��������任
		/// @param src Ŀ��ͼƬ
		/// @param rowNum ������
		/// @param columnNum ������
		/// @param targetRow Ŀ����
		/// @param targetColumn Ŀ����
		/// @param scaleX �����������
		/// @param scaleY �����������
		void DrawImageGridAtlasScaleF(sgf::SimpleImage* src, float rowNum, float columnNum, float targetRow, float targetColumn, float scaleX, float scaleY);

		/// @brief ��������ͼ����һ����,���в�������uv����ϵ����ΧΪ0~1
		/// @param src Ŀ��ͼƬ
		/// @param u ������
		/// @param v ������
		/// @param width ���
		/// @param height �߶�
		void DrawImageAtlas(sgf::SimpleImage* src, float u, float v, float width, float height);
		
		/// @brief ��������ͼ����һ���֣�������任�����в�������uv����ϵ����ΧΪ0~1
		/// @param src Ŀ��ͼƬ
		/// @param u ������
		/// @param v ������
		/// @param width ���
		/// @param height �߶�
		/// @param scaleX �����������
		/// @param scaleY �����������
		void DrawImageAtlasScaleF(sgf::SimpleImage* src, float u, float v, float width, float height, float scaleX, float scaleY);

		/// @brief ƽ��gra��ʱԭ�㵽ĳ��
		/// @param x
		/// @param y
		void Translate(float x, float y);

		void DrawTexture(unsigned int tex, float width, float height);

		void DrawTextureReversed(unsigned int tex, float width, float height);

		/// @brief �ƶ�gra��ʱԭ�㵽ĳ��
		/// @param x
		/// @param y
		void MoveTo(float x, float y);

		/// @brief �ƶ�graԭ�㵽ĳ��
		/// @param x
		/// @param y
		void ModelMoveTo(float x, float y);

		/// @brief ����������Ⱦ��Χ
		/// @param width ���
		/// @param height �߶�
		void ProjectionResize(float width, float height);

		/// @brief �ύ���ж��㲢��Ⱦ
		void Present();

		/// @brief �����������Ԫ����
		/// @return �������֧�ֵ�����Ԫ������һ��Ϊ32
		int GetMaxTextureUnitCount() const;

		/// @brief �ύ���ж���
		void Submit();
		/// @brief ��黺�����Ƿ�ﵽ���ޣ�����Ǿ͵���Present
		void CheckSubmit();

		/// @brief ��黺�����Ƿ���Ԫ��
		bool CheckIsBatchEmpty() const;

		/// @brief ����һ��Color֡���岢�󶨵���ȥ
		/// @param fbo ������֡���崢���ַ
		/// @param tex ������֡�����������ַ
		/// @param width
		/// @param height
		/// @return �Ὣfboָ���ֵ����Ϊ�µ�֡���壬texָ���ֵ��Ϊ�µ�����
		void GenFrameBuffer(unsigned int* fbo, unsigned int* tex, int width, int height);

		/// @brief ��һ��֡�������
		/// @param fbo Ŀ��֡����
		void BindFrameBuffer(unsigned int fbo);

		/// @brief �л�Ϊ��ĻĬ�ϵ�֡�������
		void ResetFrameBuffer();

		void ReleaseFrameBuffer(unsigned int fbo);

		void ReleaseTexture(unsigned int tex);

		/// @brief ���Gra��ʵ�����꣨ԭ������+��ʱԭ�����꣩
		/// @return x��y��ɵ�pair
		Point GetGraphicsTransformPosition();

		/// @brief ��Ҫ���ڻ����Ļ֡������������ݣ����ڽ�ͼ֮��ģ�ʵ�����Ƕ�ȡ��ǰ�󶨵�֡���������
		/// @param x ��ʼ��ȡ��x
		/// @param y ��ʼ��ȡ��y
		/// @param width ��ȡ�Ŀ��
		/// @param height ��ȡ�߶�
		/// @return ���ص�ǰ�󶨵�֡�����ͼ��
		SimpleImage* CaptureScreen(int x, int y, int width, int height);

		/// @brief ���ò��о���
		void SetClipRect(const sgf::FloatRect& src);

		void ClearClipRect();
	};

	static Graphics* CreateFromApp(GameApp* gameApp) { return new Graphics(gameApp); };

}

#endif
