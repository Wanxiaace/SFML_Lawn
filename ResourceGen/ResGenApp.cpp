#include "ResGenApp.h"
#include "../nfd/include/nfd.h"

ResGenApp* gApp;

ResGenApp::ResGenApp(int width, int height, const sgf::String& windowCaptain, bool enabledASync, bool resiziable)
	:GameApp(width,height,windowCaptain,enabledASync,resiziable)
{
	gApp = this;
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImFont* font = io.Fonts->AddFontFromFileTTF("assets/msyh.ttc", 16.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

	mResourceList = new sgf::ResourceList();

	mLeftBar = new TreeBar(this);
	mPreviewPage = new PreviewPage(this);
	mConsolePage = new ConsolePage(this);

	AppendLayer(mLeftBar);
	AppendLayer(mPreviewPage);
	AppendLayer(mConsolePage);
}

ResGenApp::~ResGenApp()
{
	if (mResourceList)
		delete mResourceList;

	delete mLeftBar;
	delete mPreviewPage;
	delete mConsolePage;
}

void ResGenApp::AppendLayer(ImguiLayer* layer)
{
	mLayers.push_back(layer);
}

void ResGenApp::RenderMenuBar()
{
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu(_LS_C("FILE"))) {
			if (ImGui::MenuItem(_LS_C("OPEN"), "Ctrl+O")) {
				char* xmlPath = nullptr;
				
				nfdresult_t result = NFD_OpenDialog("xml", nullptr, &xmlPath);
				
				if(!xmlPath)
					std::cout << "Faild to open" << std::endl;
				else
				{
					std::cout << "Open from: " << xmlPath << std::endl;
					sgf::String xmlPathStr = xmlPath;
					mResourceList->Load(xmlPath);
					mSelectedBoolList = new bool[mResourceList->mResouces.size()];
					memset(mSelectedBoolList,0, mResourceList->mResouces.size());
					
					sgf::String xmlDirPath = xmlPathStr;
#ifdef _WIN32
					xmlDirPath = sgf::StringReplace(xmlDirPath, "\\", "/");
#endif				
					xmlDirPath = xmlDirPath.substr(0, xmlDirPath.rfind("/")) + "/";


					std::cout << xmlDirPath << std::endl;
					mResourceManager.AttachBasePath(xmlDirPath);
					LoadAllResource();

				}
				
			
			};
			ImGui::EndMenu();
		};
		ImGui::EndMenuBar();
	}
	
}

void ResGenApp::LoadAllResource()
{
	if (!mResourceList)
		return;
	mResourceManager.LoadFromResouceList(mResourceList,&mMusicManager);
}

void ResGenApp::DrawImgui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoMove |       // ��ֹ�ƶ�
		ImGuiWindowFlags_NoResize |     // ��ֹ������С
		ImGuiWindowFlags_NoCollapse |   // ��ֹ�۵�
		ImGuiWindowFlags_NoTitleBar |   // �Ƴ�������
		ImGuiWindowFlags_MenuBar;       // ����˵���

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);

	ImGui::Begin("Main DockSpace", nullptr, window_flags);

	ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

	RenderMenuBar();

	ImGui::End();

	for (auto& x : mLayers)
	{
		x->Draw();
	}

	for (auto& x : mLayers)
	{
		x->Update();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ResGenApp::Draw()
{
	mGraphics->Clear();

	mGraphics->ActiveTextureShader();
	mWidgetManager->Draw(mGraphics);

	DrawImgui();

	for (auto& x : mLayers)
	{
		x->SGFDraw(mGraphics);
	}

	mGraphics->Present();

	SDL_GL_SwapWindow(mGameWindow);
}
