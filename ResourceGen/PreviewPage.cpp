#include "PreviewPage.h"
#include "ResGenApp.h"

void PreviewPage::Draw()
{
	ImGui::Begin(_LS_C("PREVIEW"));

	ImGui::Button(_LS_C("IMAGE_VIEWER"));
	ImGui::Button(_LS_C("REANIM_VIEWER"));
	ImGui::Button(_LS_C("PARTICLE_VIEWER"));
	ImGui::Button(_LS_C("ATLAS_VIEWER"));
	ImGui::Button(_LS_C("FONT_VIEWER"));

	ImGui::End();

	mImagePreview->Draw();
}

void PreviewPage::Update()
{
	mImagePreview->Update();
}

void PreviewPage::SGFDraw(sgf::Graphics* g)
{
	mImagePreview->SGFDraw(g);
}


static void ImGuiDrawGrid(
	ImDrawList* draw_list, ImVec2 window_pos, ImVec2 window_size, ImVec2 pos_offset, float grid_spacing, ImU32 grid_color,ImU32 grid_color_shallow)
{
	pos_offset.x -= int(pos_offset.x) / int(grid_spacing * 10) * grid_spacing * 10;
	pos_offset.y -= int(pos_offset.y) / int(grid_spacing * 10) * grid_spacing * 10;


	draw_list->AddRectFilled(window_pos, { window_pos.x + window_size.x ,window_pos.y + window_size.y }, IM_COL32(0x69,0x62,0x5d,0xff));

	int num_lines_x = (int)(window_size.x / grid_spacing) + 1;
	int num_lines_y = (int)(window_size.y / grid_spacing) + 1;


	for (int i = 0; i < num_lines_x; ++i)
	{
		float x = window_pos.x + i * grid_spacing + pos_offset.x;
		if(i % 10)
			draw_list->AddLine(ImVec2(x, window_pos.y), ImVec2(x, window_pos.y + window_size.y), grid_color_shallow);
		else
			draw_list->AddLine(ImVec2(x, window_pos.y), ImVec2(x, window_pos.y + window_size.y), grid_color);
	}

	
	for (int i = 0; i < num_lines_y; ++i)
	{
		float y = window_pos.y + i * grid_spacing + pos_offset.y;
		if (i % 10)
			draw_list->AddLine(ImVec2(window_pos.x, y), ImVec2(window_pos.x + window_size.x, y), grid_color_shallow);
		else
			draw_list->AddLine(ImVec2(window_pos.x, y), ImVec2(window_pos.x + window_size.x, y), grid_color);
	}
}

void ImagePreview::Draw()
{
	mVisible = ImGui::Begin(_LS_C("IMAGE_VIEWER_PAGE"));
	mWindowSize = ImGui::GetWindowSize();
	mWindowPos = ImGui::GetWindowPos();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImGuiDrawGrid(draw_list, mWindowPos, mWindowSize, 
		{ mMapPos .x + mDragDelta.x, mMapPos.y + mDragDelta.y },
		10.0f * mScale, IM_COL32(0xff, 0xff, 0xff, 0xA0), IM_COL32(0xff, 0xff, 0xff, 0x20));

	ImGui::End();
}



void ImagePreview::SGFDraw(sgf::Graphics* g)
{
	if (!mVisible)
		return;
	g->MoveTo(0,0);
	g->SetClipRect({ mWindowPos.x ,mWindowPos.y + 20,mWindowSize.x ,mWindowSize.y - 20 });

	ResGenApp* app = (ResGenApp*)mApp;
	if (app->mCurentImageIndex != -1) {
		g->MoveTo(mMapPos.x + mDragDelta.x, mMapPos.y + mDragDelta.y - 20);
		g->DrawImageScaleF(app->mResourceManager.GetResourceFast<sgf::SimpleImage>(app->mResourceList->mResouces[app->mCurentImageIndex].id),mScale,mScale);
	}

	g->Present();
	g->ClearClipRect();
}

void ImagePreview::Update()
{
	ImGuiIO& io = ImGui::GetIO();
	
	float wheel = io.MouseWheel;
	mScale += wheel * 0.1f;
	if (mScale < 0.5f)
		mScale = 0.5f;

	if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
	{
		mDragDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
		//std::cout << "Mouse dragging: (" << mDragDelta.x << ", " << mDragDelta.y << ")" << std::endl;
	}
	else {
		mMapPos.x += mDragDelta.x;
		mMapPos.y += mDragDelta.y;
		mDragDelta = { 0,0 };
	}
}
