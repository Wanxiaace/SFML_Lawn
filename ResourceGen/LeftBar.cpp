#include "LeftBar.h"
#include "ResGenApp.h"

void TreeBar::SelectRes(int index)
{
	ResGenApp* app = (ResGenApp*)gApp;
	for (size_t i = 0; i < app->mResourceList->mResouces.size(); i++)
	{
		app->mSelectedBoolList[i] = (i == index);
	}
	app->mCurentBoolIndex = index;

	if (app->mResourceList->mResouces[index].folder == "image" || app->mResourceList->mResouces[index].folder == "particle")
	{
		app->mIsAtlas = false;
		app->mCurentImagePointer = app->mResourceManager.GetResourceFast<sgf::SimpleImage>(app->mResourceList->mResouces[index].id);
	}
	else if (app->mResourceList->mResouces[index].folder.size() >= 5
		&& app->mResourceList->mResouces[index].
		folder.substr(app->mResourceList->mResouces[index].folder.size() - 5) == "atlas") {
		app->mIsAtlas = true;
		sgf::String currentId = sgf::StringtoUpper(app->mResourceList->mResouces[index].folder).substr(0, app->mResourceList->mResouces[index].folder.length() - 5) +
			sgf::StringGetCurrentPathWithoutExtension(sgf::StringtoUpper(app->mResourceList->mResouces[index].path));

		app->mCurentImagePointer = app->mCurentImagePointer = 
			app->mResourceManager.GetResourceFast<sgf::SimpleImage>(currentId);
	}
	else if (app->mResourceList->mResouces[index].folder == "raxml") {
		auto reanim = app->mResourceManager.GetResourceFast<sgf::Reanimation>(app->mResourceList->mResouces[index].id);
		if (app->mCurentAnimatorPointer)
			delete app->mCurentAnimatorPointer;
		app->mCurentAnimatorPointer = new sgf::Animator(reanim,gApp);
		app->mCurentAnimatorPointer->SetFrameRangeByTrackName("anim_idle");
		app->mCurentAnimatorPointer->Play();
	}
}

std::vector<sgf::String> strList;
sgf::String headerStr;

static int counter = 0;

static void DoAllFunc(std::function<void(int)> callback) {
	if (ImGui::CollapsingHeader(headerStr.c_str())) {
		for (auto& x : strList)
		{
			if (ImGui::Selectable(x.c_str(),&gApp->mSelectedBoolList[counter])) { callback(counter); };
			counter++;
		}
	}
	else {
		int len = strList.size();
		counter += len;
	}

	strList.clear();
}

static void AppendSelectable(const sgf::String& str) {
	strList.push_back(str);
}

void TreeBar::Draw()
{
	ImGui::Begin(_LS_C("RESOUCES"));
	ResGenApp* app = (ResGenApp*)mApp;

	sgf::String strBuf = "";
	headerStr = "";
	counter = 0;

	if (app->mResourceList) {
		int i = 0;
		for (auto& x : app->mResourceList->mResouces)
		{
			if (x.folder != strBuf) {
				if(headerStr != "")
					DoAllFunc(SelectRes);
				strBuf = x.folder;
				headerStr = _LS_C(sgf::StringtoUpper(x.folder));
			}
			AppendSelectable(std::to_string(i + 1) + ". " + x.id);
			i++;
		}
		if (headerStr != "")
			DoAllFunc(SelectRes);
	}

	ImGui::End();
}

void TreeBar::Update()
{

}
