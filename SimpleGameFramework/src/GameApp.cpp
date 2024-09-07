#include "../include/GameApp.h"

sgf::GameApp::GameApp(int width, int height, const sgf::String& windowCaptain, bool enabledASync,bool resiziable): GameAppBase::GameAppBase(width, height, windowCaptain, enabledASync, resiziable)
{
	mWidgetManager = new WidgetManager();
	UpdateMusicVolume();
	
}

sgf::GameApp::~GameApp()
{
	delete mGraphics;
	delete mWidgetManager;

	GameAppBase::~GameAppBase();
}

void sgf::GameApp::UpdateMusicVolume()
{
	mMusicManager.SetMusicVolume(mMusicVolume / 100.0f);
}

sgf::Graphics* sgf::GameApp::LoadGraphics()
{
	if(!mGraphics)
		mGraphics = new Graphics(this);
	return mGraphics;
}

void sgf::GameApp::SafeDeleteWidget(Widget* target)
{
	mMessageManager.SendGameMessage({ sgf::GameMessage::MessageType::MSG_TYPE_DELETE_WIDGET,mWidgetManager,target});
}

void sgf::GameApp::SafeAppendWidget(Widget* target)
{
	mMessageManager.SendGameMessage({ sgf::GameMessage::MessageType::MSG_TYPE_APPEND_WIDGET,mWidgetManager,target });
}

void sgf::GameApp::DrawImgui()
{
	if (!mShowDebugWindow)
		return;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Analyse Window");
	ImGui::Text("FPS: %d", mFPS);
	ImGui::Text("MouseX: %d", mMouseX);
	ImGui::Text("MouseY: %d", mMouseY);
	ImGui::Checkbox("Allow Moving", &mDragAllowed);
	ImGui::Checkbox("Show Widget Box", &gShowWidgetHitBoxAllowed);
	ImGui::Checkbox("Rotate", &mRotateAllowed);
	ImGui::SliderFloat("Moving Speed", &mMovingSpeed, 1.0f, 5.0f);;
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void sgf::GameApp::LoadResources(const char* resourcesListPath)
{
	mResourceList.Load(resourcesListPath);
	mResourceManager.LoadFromResouceList(&mResourceList, &mMusicManager);
}

void sgf::GameApp::LoadDict(const char* dictPath)
{
	mDictionary.LoadFromFile((mResourceManager.mBasePath + dictPath).c_str());
}

void sgf::GameApp::SetWindowIcon(const char* path)
{
	sgf::SimpleImage icon;
	icon.LoadFromFile(path);
	SetWindowIconFromImage(&icon);
}

void sgf::GameApp::SetWindowIconFromImage(sgf::SimpleImage* image)
{
	SDL_SetWindowIcon(mGameWindow, image->mSurface);
}

void sgf::GameApp::SetWindowCaptain(const sgf::String& title)
{
	SDL_SetWindowTitle(mGameWindow, title.c_str());
}


void sgf::GameApp::Update()
{
	GameAppBase::Update();
	mWidgetManager->Update(this);

	if (mDragAllowed) {
		const unsigned char* keyStates = SDL_GetKeyboardState(NULL);
		if(keyStates[SDL_SCANCODE_W])
			mViewMatrix = glm::translate(mViewMatrix,glm::vec3(0, mMovingSpeed,0));
		if (keyStates[SDL_SCANCODE_S])
			mViewMatrix = glm::translate(mViewMatrix, glm::vec3(0, -mMovingSpeed, 0));
		if (keyStates[SDL_SCANCODE_A])
			mViewMatrix = glm::translate(mViewMatrix, glm::vec3(mMovingSpeed, 0, 0));
		if (keyStates[SDL_SCANCODE_D])
			mViewMatrix = glm::translate(mViewMatrix, glm::vec3(-mMovingSpeed, 0, 0));
	}

	if (mRotateAllowed) {
		mViewMatrix = glm::rotate(mViewMatrix, 0.05f, glm::vec3(0, 0, 1.0f));
	}
}

void sgf::GameApp::Draw()
{
	//mGraphics->Clear();
	mGraphics->ActiveTextureShader();
	mWidgetManager->Draw(mGraphics);

	DrawImgui();
	SDL_GL_SwapWindow(mGameWindow);
}

void sgf::GameApp::CopeEvent(SDL_Event& theEvent)
{
	GameAppBase::CopeEvent(theEvent);

	switch (theEvent.type) {
	case SDL_EventType::SDL_WINDOWEVENT:
		break;
	}
}
