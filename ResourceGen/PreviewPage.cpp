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
	mReanimPreview->Draw();
}

void PreviewPage::Update()
{
	mImagePreview->Update();
	mReanimPreview->Update();
}

void PreviewPage::SGFDraw(sgf::Graphics* g)
{
	mImagePreview->SGFDraw(g);
	mReanimPreview->SGFDraw(g);
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
		if (i % 10)
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
	mFocused = ImGui::IsWindowFocused();

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	
	ImGuiDrawGrid(draw_list, mWindowPos, mWindowSize, 
		{ mMapPos .x + mDragDelta.x, mMapPos.y + mDragDelta.y },
		10.0f * mScale, IM_COL32(0xff, 0xff, 0xff, 0xA0), IM_COL32(0xff, 0xff, 0xff, 0x20));
	
	if (gApp->mCurentImagePointer) {
		auto img = gApp->mCurentImagePointer;

		draw_list->AddRect(
			{
				mWindowPos.x + mMapPos.x + mDragDelta.x - 1,
				mWindowPos.y + mMapPos.y + mDragDelta.y - 1 },
			{
				mWindowPos.x + mMapPos.x + mDragDelta.x + 1 + img->GetWidth() * mScale,
				mWindowPos.y + mMapPos.y + mDragDelta.y + 1 + img->GetHeight() * mScale }
			, IM_COL32(0xff, 0, 0, 0xff));

		draw_list->AddText(
			{
				mWindowPos.x + mMapPos.x + mDragDelta.x,
				mWindowPos.y + mMapPos.y + mDragDelta.y + img->GetHeight() * mScale
			},IM_COL32(0xff, 0, 0, 0xff),("width: " + std::to_string(int(img->GetWidth())) + " Height: " + std::to_string(int(img->GetHeight()))).c_str()
		);
	}


	ImGui::End();
}



void ImagePreview::SGFDraw(sgf::Graphics* g)
{
	if (!mVisible)
		return;

	g->SetCubeColor({ 1,1,1,1 });
	g->MoveTo(0,0);
	g->SetClipRect({ mWindowPos.x ,mWindowPos.y + 20,mWindowSize.x ,mWindowSize.y - 20 });

	ResGenApp* app = (ResGenApp*)mApp;
	if (app->mCurentImagePointer) {
		g->MoveTo(mMapPos.x + mDragDelta.x, mMapPos.y + mDragDelta.y - 20);
		g->DrawImageScaleF(app->mCurentImagePointer,mScale,mScale);
	}

	g->ClearClipRect();
}

void ImagePreview::Update()
{
	if (!mVisible || !mFocused)
		return;

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

void ReanimPreview::SelectTrack(int index)
{
	ResGenApp* app = (ResGenApp*)gApp;
	for (size_t i = 0; i < 100; i++)
	{
		app->mSelectedTrackBoolList[i] = (i == index);
	}
	app->mCurentTrackIndex = index;

	app->mCurentAnimatorPointer->SetFrameRangeByTrackName(
		(*app->mCurentAnimatorPointer->mReanim->mTracks)[index].mTrackName);
	

}

void ReanimPreview::Draw()
{
	mVisible = ImGui::Begin(_LS_C("REANIM_VIEWER_PAGE"));
	mWindowSize = ImGui::GetWindowSize();
	mWindowPos = ImGui::GetWindowPos();
	mFocused = ImGui::IsWindowFocused();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImGuiDrawGrid(draw_list, mWindowPos, mWindowSize,
		{ mMapPos.x + mDragDelta.x, mMapPos.y + mDragDelta.y },
		10.0f * mScale, IM_COL32(0xff, 0xff, 0xff, 0xA0), IM_COL32(0xff, 0xff, 0xff, 0x20));

	if (gApp->mCurentAnimatorPointer) 
	{

		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
		float childWidth = 150; // �Ӵ��ڿ���
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + contentRegionAvailable.x - childWidth);

		ImGui::BeginChild(_LS_C("REANIM_TRACK_LIST"), ImVec2(childWidth, 0),
			ImGuiChildFlags_Border);

		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
		ImGui::Text(_LS_C("REANIM_TRACK_LIST"));
		
		int i = 0;
		for (auto& x : *gApp->mCurentAnimatorPointer->mReanim->mTracks)
		{
			if (ImGui::Selectable((std::to_string(i + 1) + "." + x.mTrackName).c_str(), &gApp->mSelectedTrackBoolList[i])) {
				SelectTrack(i);
			};
			i++;
		}

		ImGui::EndChild();
	}
	
	ImGui::End();
}

void ReanimPreview::SGFDraw(sgf::Graphics* g)
{
	if (!mVisible)
		return;

	g->SetCubeColor({1,1,1,1});

	g->MoveTo(0, 0);
	g->SetClipRect({ mWindowPos.x ,mWindowPos.y + 20,mWindowSize.x ,mWindowSize.y - 20 });

	ResGenApp* app = (ResGenApp*)mApp;
	if (app->mCurentAnimatorPointer) {
		g->MoveTo(mMapPos.x + mDragDelta.x, mMapPos.y + mDragDelta.y - 20);
		app->mCurentAnimatorPointer->PresentMatrix(g,glm::scale(glm::mat4x4(1.0f),glm::vec3(mScale,mScale,1.0f)));
	}

	g->ClearClipRect();

}

void ReanimPreview::Update()
{
	if (gApp->mCurentAnimatorPointer) {
		gApp->mCurentAnimatorPointer->Update();
	}


	if (!mVisible || !mFocused)
		return;
	ImGuiIO& io = ImGui::GetIO();

	float wheel = io.MouseWheel;
	mScale += wheel * 0.1f;
	if (mScale < 0.5f)
		mScale = 0.5f;

	if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
	{
		mDragDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
	}
	else {
		mMapPos.x += mDragDelta.x;
		mMapPos.y += mDragDelta.y;
		mDragDelta = { 0,0 };
	}

	
}

