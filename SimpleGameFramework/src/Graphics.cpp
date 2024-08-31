#include "../include/Graphics.h"
#include <string>

sgf::Graphics::Graphics(SimpleApp* gameApp)
{
	mTextureProgram.LoadFromFile("assets/shaders/TextureVertexShader.glsl", "assets/shaders/TextureFragmentShader.glsl");
	
	glGenVertexArrays(1, &mCubeVAO);

	glGenBuffers(1, &mCubeVBO);

	glBindVertexArray(mCubeVAO);

	ProjectionResize(gameApp->mWidth, gameApp->mHeight);
	mGameApp = gameApp;
}

void sgf::Graphics::Clear()
{
	glClearColor(0,0,0,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


void sgf::Graphics::SetCubeColor(const sgf::Color& srcColor)
{
	mCubeColor = srcColor;
	for (size_t i = 0; i < 6; i++)
	{
		mCubeVertices[i].r = srcColor.r;
		mCubeVertices[i].g = srcColor.g;
		mCubeVertices[i].b = srcColor.b;
		mCubeVertices[i].a = srcColor.a;
	}
}

void sgf::Graphics::AppendVertices(const Vertex* vertices, int vertexCount)
{
	for (size_t i = 0; i < vertexCount; i++)
	{
		mVerticesBuffer.push_back(vertices[i]);
		
	}
	mVerticesNumber += vertexCount;
	/*mSimpleVerticesCount = vertexCount;
	glBindVertexArray(mCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER,mCubeVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex) * vertexCount, vertices,GL_STATIC_DRAW);
	sgf::Vertex::InformationInit();*/
}


void sgf::Graphics::ActiveTextureShader()
{
	if (mNowProgram == &mTextureProgram)
		return;
	mNowProgram = &mTextureProgram;
	mTextureProgram.Use();
}

void sgf::Graphics::DrawTriangleArrays()
{
	glBindVertexArray(mCubeVAO);

	/*glPolygonMode(GL_FRONT, GL_LINE);
	glLineWidth(5.0f);*/

	GL_CALL(glDrawArrays(GL_TRIANGLES, 0, mVerticesNumber));
	
}


void sgf::Graphics::FillRect(int width, int height)
{
	Point graPos = GetGraphicsTransformPosition();
	for (size_t i = 0; i < 6; i++)
	{
		mCubeVertices[i].x = graPos.x;
		mCubeVertices[i].y = graPos.y;
	}
	mCubeVertices[0].x += width;
	mCubeVertices[0].y += height;
	mCubeVertices[1].x += width;

	mCubeVertices[3].x += width;
	mCubeVertices[3].y += height;
	mCubeVertices[4].y += height;

	AppendVertices(mCubeVertices,6);

	CheckSubmit();
}

void sgf::Graphics::FillRect(const FloatRect& src)
{
	MoveTo(src.mX, src.mY);
	FillRect(src.mWidth,src.mHeight);
}


int sgf::Graphics::TryToBindNewTexture(sgf::SimpleImage* src)
{
	auto itr = std::find(mTexturesBuffer.begin(), mTexturesBuffer.end(), src);
	if (itr != mTexturesBuffer.end())
		return itr - mTexturesBuffer.begin();
	int unitMaxCount = GetMaxTextureUnitCount();
	GLenum textureUnit = GL_TEXTURE0 + mTexturesNumber;
	if (mTexturesNumber < unitMaxCount) {
		unsigned int texureHandle = src->GenerateTexture();
		glActiveTexture(textureUnit);
		
		glBindTexture(GL_TEXTURE_2D, texureHandle);
		GLint textureArrayIndex = glGetUniformLocation(mNowProgram->mProgram, ("textures[" + std::to_string(mTexturesNumber) + "]").c_str());

		glUniform1i(textureArrayIndex, mTexturesNumber);
		mTexturesBuffer.push_back(src);
	}
	mTexturesNumber++;
	return mTexturesNumber - 1;
}

int sgf::Graphics::TryToBindNewMatrix(glm::mat4x4 src)
{
	//auto itr = std::find(mMatrixsBuffer.begin(), mMatrixsBuffer.end(), src);
	
	//if (itr != mMatrixsBuffer.end())
		//return itr - mMatrixsBuffer.begin();

	mMatrixsBuffer.push_back(src);
	mMatrixsNumber++;
	return mMatrixsNumber - 1;
}

void sgf::Graphics::DrawImage(sgf::SimpleImage* src)
{
	int tex = TryToBindNewTexture(src);
	for (size_t i = 0; i < 6; i++)
	{
		mCubeVertices[i].texIndex = tex;
	}

	FillRect(src->mSurface->w, src->mSurface->h);

	for (size_t i = 0; i < 6; i++)
	{
		mCubeVertices[i].texIndex = -1;
	}
}

void sgf::Graphics::DrawImageScaleF(sgf::SimpleImage* src, float scaleX, float scaleY)
{
	int tex = TryToBindNewTexture(src);
	for (size_t i = 0; i < 6; i++)
	{
		mCubeVertices[i].texIndex = tex;
	}

	FillRect(src->mSurface->w * scaleX, src->mSurface->h * scaleY);

	for (size_t i = 0; i < 6; i++)
	{
		mCubeVertices[i].texIndex = -1;
	}
}

void sgf::Graphics::DrawImageInRect(sgf::SimpleImage* src, float width, float height)
{
	int tex = TryToBindNewTexture(src);
	for (size_t i = 0; i < 6; i++)
	{
		mCubeVertices[i].texIndex = tex;
	}

	FillRect(width, height);

	for (size_t i = 0; i < 6; i++)
	{
		mCubeVertices[i].texIndex = -1;
	}
}

void sgf::Graphics::DrawImageMatrix(sgf::SimpleImage* src, glm::mat4x4 matrix, float oriX, float oriY)
{
	Point graPos = GetGraphicsTransformPosition();
	float matrixPosition = TryToBindNewMatrix(glm::translate(glm::mat4x4(1.0f), glm::vec3(graPos.x+ oriX, graPos.y+ oriY, 0)) * matrix);
	int tex = TryToBindNewTexture(src);

	for (size_t i = 0; i < 6; i++)
	{
		mCubeVertices[i].texIndex = tex;
		mCubeVertices[i].matrixIndex = matrixPosition;

		mCubeVertices[i].x = 0;
		mCubeVertices[i].y = 0;
	}

	mCubeVertices[0].x += src->mSurface->w;
	mCubeVertices[0].y += src->mSurface->h;
	mCubeVertices[1].x += src->mSurface->w;

	mCubeVertices[3].x += src->mSurface->w;
	mCubeVertices[3].y += src->mSurface->h;
	mCubeVertices[4].y += src->mSurface->h;

	AppendVertices(mCubeVertices, 6);
	CheckSubmit();

	for (size_t i = 0; i < 6; i++)
	{
		mCubeVertices[i].texIndex = -1;
		mCubeVertices[i].matrixIndex = -1;
	}
}
/*float matrixPosition = TryToBindNewMatrix(matrix);

	for (size_t i = 0; i < 6; i++)
	{
		mCubeVertices[i].matrixIndex = matrixPosition;
	}

	DrawImage(src);

	for (size_t i = 0; i < 6; i++)
	{
		mCubeVertices[i].matrixIndex = -1;
	}*/


void sgf::Graphics::Translate(float x, float y)
{
	mTransformPosition.x += x;
	mTransformPosition.y += y;
}

void sgf::Graphics::MoveTo(int x, int y)
{
	mTransformPosition.x = x;
	mTransformPosition.y = y;
}

void sgf::Graphics::ModelMoveTo(float x, float y)
{
	mModelTransformPosition.x = x;
	mModelTransformPosition.y = y;
}

void sgf::Graphics::ProjectionResize(float width, float height)
{
	mProjectMatrix = glm::ortho(0.0f, width,
		height, 0.0f, -1.0f, 1.0f);
	mTextureProgram.Use();
	GL_CALL(glUniformMatrix4fv(glGetUniformLocation(mTextureProgram.mProgram, "proj"), 1, GL_FALSE, glm::value_ptr(mProjectMatrix)););
}

void sgf::Graphics::Present()
{
	Submit();
	DrawTriangleArrays();

	mVerticesNumber = 0;
	mTexturesNumber = 0;
	mMatrixsNumber = 0;

	mVerticesBuffer.clear();
	mTexturesBuffer.clear();
	mMatrixsBuffer.clear();
}

void sgf::Graphics::Submit()
{
	glBindVertexArray(mCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER,mCubeVBO);
	//����
	glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex)*mVerticesNumber,mVerticesBuffer.data(),GL_STATIC_DRAW);
	glUniformMatrix4fv(glGetUniformLocation(mNowProgram->mProgram,"view"),1,GL_FALSE, (float*) & mGameApp->mViewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(mNowProgram->mProgram,"posMatrix"),mMatrixsNumber,GL_FALSE, (float*) mMatrixsBuffer.data());

	Vertex::InformationInit();
}

void sgf::Graphics::CheckSubmit()
{
	if (mVerticesNumber >= VERTEX_BUFFER_MAX_COUNT)
		Present();
	if (mMatrixsNumber >= MATRIX_BUFFER_MAX_COUNT)
		Present();
	if (mTexturesNumber >= GetMaxTextureUnitCount())
		Present();
}

sgf::Point sgf::Graphics::GetGraphicsTransformPosition()
{
	return { mTransformPosition.x + mModelTransformPosition.x,
		mTransformPosition.y + mModelTransformPosition.y,};
}
