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

#include "CzPlatform.h"
#include "CzPlatformSys.h"
#include "CzPlatformDisplay.h"
#include "CzPlatformFile.h"
#include "CzPlatformAudio.h"
#include "CzPlatformInput.h"
#include "CzPlatformComms.h"
#include "CzPlatformFont.h"
#include "CzPlatformImaging.h"
#include "CzPlatformVideo.h"
#include "CzPlatformOther.h"
#include "CzPlatformRender.h"
#include "CzPlatformUI.h"
#include "CzPlatformCam.h"
#include "CzPlatformMarket.h"
#include "CzPlatformFacebook.h"
#include "CzPlatformAds.h"

CDECLARE_SINGLETON(CzPlatform)

int CzPlatform::Init(IzPlatform* platform, bool comms, bool use_gl)
{
	Platform = platform;
	Platform->Init();

	// Debug systnm relies on file system so initialise the file system first
	if (File != NULL)
		File->Init();

	CzDebug::Init();

	if (Sys != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Initialising System");
		Sys->Init();
	}
	if (Display != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Initialising Display");
		Display->Init(use_gl);
	}
	if (Input != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Initialising Input");
		Input->Init();
	}
	if (Audio != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Initialising Audio");
		Audio->Init();
	}
	if (comms && Comms != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Initialising Comms");
		Comms->Init();
	}
	if (Font != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Initialising Font");
		Font->Init();
	}
	if (Imaging != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Initialising Imaging");
		Imaging->Init();
	}
	if (Video != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Initialising Video");
		Video->Init();
	}
	if (Other != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Initialising Other");
		Other->Init();
	}
	if (Render != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Initialising Render");
		Render->Init();
	}
	if (UI != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Initialising UI");
		UI->Init();
	}
	if (Cam != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Initialising Cam");
		Cam->Init();
	}
	if (Facebook != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Initialising Facebook");
		Facebook->Init();
	}
	if (Ads != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Initialising Ads");
	}
	Initialised = true;

	return 1;
}

void CzPlatform::Release()
{
	if (Ads != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Releasing Ads");
		Ads->Release();
		delete Ads;
		Ads = NULL;
	}
	if (Facebook != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Releasing Facebook");
		Facebook->Release();
		delete Facebook;
		Facebook = NULL;
	}
	if (Market != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Releasing Market");
		Market->Release();
		delete Market;
		Market = NULL;
	}
	if (Cam != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Releasing Cam");
		Cam->Release();
		delete Cam;
		Cam = NULL;
	}
	if (UI != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Releasing UI");
		UI->Release();
		delete UI;
		UI = NULL;
	}
	if (Other != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Releasing Other");
		Other->Release();
		delete Other;
		Other = NULL;
	}
	if (Render != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Releasing Render");
		Render->Release();
		delete Render;
		Render = NULL;
	}
	if (Video != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Releasing Video");
		Video->Release();
		delete Video;
		Video = NULL;
	}
	if (Imaging != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Releasing Imaging");
		Imaging->Release();
		delete Imaging;
		Imaging = NULL;
	}
	if (Font != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Releasing Font");
		Font->Release();
		delete Font;
		Font = NULL;
	}
	if (Comms != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Releasing Comms");
		Comms->Release();
		delete Comms;
		Comms = NULL;
	}
	if (Audio != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Releasing Audio");
		Audio->Release();
		delete Audio;
		Audio = NULL;
	}
	if (Input != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Releasing Input");
		Input->Release();
		delete Input;
		Input = NULL;
	}
	if (Display != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Releasing Display");
		Display->Release();
		delete Display;
		Display = NULL;
	}
	if (File != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Releasing File");
		File->Release();
		delete File;
		File = NULL;
	}
	if (Sys != NULL)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Releasing Sys");
		Sys->Release();
		delete Sys;
		Sys = NULL;
	}
	Platform->Release();
	SAFE_DELETE(Platform);
	Initialised = false;
}

