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
	AppendLayer(mLeftBar);
}

ResGenApp::~ResGenApp()
{
	if (mResourceList)
		delete mResourceList;
	delete mLeftBar;
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
					mResourceList->Load(xmlPath);
					mSelectedBoolList = new bool[mResourceList->mResouces.size()];
					memset(mSelectedBoolList,0, mResourceList->mResouces.size());

				}
				
			
			};
			ImGui::EndMenu();
		};
		ImGui::EndMenuBar();
	}
	
}

void ResGenApp::DrawImgui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoMove |       // ½ûÖ¹ÒÆ¶¯
		ImGuiWindowFlags_NoResize |     // ½ûÖ¹µ÷Õû´óÐ¡
		ImGuiWindowFlags_NoCollapse |   // ½ûÖ¹ÕÛµþ
		ImGuiWindowFlags_NoTitleBar |   // ÒÆ³ý±êÌâÀ¸
		ImGuiWindowFlags_MenuBar;       // ÔÊÐí²Ëµ¥À¸

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

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	for (auto& x : mLayers)
	{
		x->Update();
	}
}
