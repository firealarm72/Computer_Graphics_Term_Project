#include "sample/coloring_view.h"

#include "basic/basic_file_loader.h"

using namespace std;

ColoringView::ColoringView(void* data): SampleView(data)
{
}

void ColoringView::OnInit()
{
	string vs = FileLoader::GetInstance()->ReadTxtFile("shader/view_color/color.vs");
	string fs = FileLoader::GetInstance()->ReadTxtFile("shader/view_color/color.fs");
	mViewRenderer->SetProgram(vs, fs);

	string teapot = FileLoader::GetInstance()->ReadTxtFile("obj3d/teapot");

	TexData textJ;
	FileLoader::GetInstance()->ReadTexture("tex/tizen_black.png", textJ);


	float scale = 1.0f;
	mViewRenderer->SetNewModel(teapot, scale);
	mViewRenderer->SetTexture(TEXDATA_GENERAL, &textJ);

	mViewRenderer->Initialize();

	//mViewRenderer->OffAutoRotate();
	mViewRenderer->GetCamera()->SetEye(25, 25, 25);
	mViewRenderer->GetCamera()->SetAt(0, 0, 0);
}
