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
