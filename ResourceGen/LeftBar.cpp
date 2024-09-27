#include "LeftBar.h"
#include "ResGenApp.h"

void TreeBar::SelectRes(int index)
{
	ResGenApp* app = (ResGenApp*)mApp;
	for (size_t i = 0; i < app->mResourceList->mResouces.size(); i++)
	{
		app->mSelectedBoolList[i] = (i == index);
	}
	
}

void TreeBar::Draw()
{
	ImGui::Begin(_LS_C("RESOUCES"));
	ResGenApp* app = (ResGenApp*)mApp;

	if (app->mResourceList) {
		int i = 0;
		for (auto& x : app->mResourceList->mResouces)
		{
			if (ImGui::Selectable((std::to_string(i + 1) + ". " + x.id).c_str(), &app->mSelectedBoolList[i]))
				SelectRes(i);
			i++;
		}
	}


	ImGui::End();
}

void TreeBar::Update()
{

}
