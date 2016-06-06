#include "sample/sample_launcher.h"
#include "gles_sample.h"

#include "sample/coloring_view.h"

SampleLauncher* SampleLauncher::instance = nullptr;

SampleLauncher::SampleLauncher() :
	curView(nullptr),
	mParent(nullptr)
{
}

SampleLauncher::~SampleLauncher()
{
	if (instance)
		delete instance;
}

SampleLauncher* SampleLauncher::GetInstance()
{
	if (!instance)
	{
		instance = new SampleLauncher();
	}
	return instance;
}

void SampleLauncher::Release()
{
	if (curView)
	{
		delete curView;
		curView = nullptr;
	}

	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

Evas_Object* SampleLauncher::InitSampleView(void* data)
{
	appdata_s* ad = static_cast<appdata_s *>(data);
	int sampleNum = ad->cur_sample_num;

	mParent = ad->win;

	if (curView)
	{
		delete curView;
		curView = nullptr;
	}

	switch (sampleNum)
	{
	case 0:
		break;
	case 1:
		curView = new ColoringView(ad);
		break;
	default:
		break;
	}

	return curView->GetGLView();
}

