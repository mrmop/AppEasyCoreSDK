// 
//
// AppEasy SDK - Cross Platform Multi-purpose Game and App Engine
//
// Developed by Matthew Hopwood of Pocketeers Limited - www.pocketeers.co.uk
//
// For updates, tutorials and more details check out www.appeasymobile.com
//
// This code is provided free of charge and without any warranty whatsoever. You must ensure that this whole notice is present in all files and derivatives, so the reader knows its origin.
// If you use this SDK in your product then please ensure that you credit AppEasy's usage appropriately. Please see www.appeasymobile.com for licensing details and support
//
//

#include "Marm/CzPlatformMain.h"
#include "CzPlatformSys.h"
#include "CzPlatformDisplay.h"
#include "CzPlatformInput.h"
#include "CzPlatformAudio.h"
#include "CzPlatformComms.h"
#include "CzPlatformFile.h"
#include "CzPlatformFont.h"
#include "CzPlatformVideo.h"
#include "CzPlatformImaging.h"
#include "CzPlatformRender.h"
#include "CzPlatformOther.h"
#include "CzPlatformUI.h"
#include "CzPlatformCam.h"
#include "CzPlatformMarket.h"
#include "CzPlatformFacebook.h"
#include "CzPlatformAds.h"
#include "IwUtil.h"

const char* CzPlatformMain::Platform = "marmalade";

void	CzPlatform_Create()
{
	IwUtilInit();

	CzPlatform::Create();
	PLATFORM->Init(new CzPlatformMain(), true, false);
}

void	CzPlatform_Destroy()
{
	PLATFORM->Release();
	CzPlatform::Destroy();

	IwUtilTerminate();

}

int CzPlatformMain::Init()
{
	PLATFORM->setSys(new CzPlatformSys());
	PLATFORM->setDisplay(new CzPlatformDisplay());
	PLATFORM->setInput(new CzPlatformInput());
	PLATFORM->setAudio(new CzPlatformAudio());
	PLATFORM->setComms(new CzPlatformComms());
	PLATFORM->setFile(new CzPlatformFile());
	PLATFORM->setFont(new CzPlatformFont());
	PLATFORM->setImaging(new CzPlatformImaging());
	PLATFORM->setVideo(new CzPlatformVideo());
	PLATFORM->setRender2d(new CzPlatformRender());
	PLATFORM->setOther(new CzPlatformOther());
	PLATFORM->setUI(new CzPlatformUI());
	PLATFORM->setCam(new CzPlatformCam());
	PLATFORM->setFacebook(new CzPlatformFacebook());
	PLATFORM->setAds(new CzPlatformAds());

//	int os = s3eDeviceGetInt(S3E_DEVICE_OS);
	PLATFORM->setMarket(new CzPlatformMarket());

	return 1;
}

void CzPlatformMain::Release()
{
}

