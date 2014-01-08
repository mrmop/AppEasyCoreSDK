/*

  Copyright 2012 Pocketeers Limited.  All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary information of Pocketeers Limited 
  that is used in an actual commercial product known as cOnnecticOns. This source code has been provided as a working 
  example of the Cz Engine. You are hereby granted permission to use this code to learn to create your own products
  using the Cz Engnie as long as the product(s) in question vary significantly in design to cOnnecticOns.

*/

#include "App.h"

CDECLARE_SINGLETON(App)

extern "C"
{
   #include "lua.h"
   #include "lauxlib.h"
   #include "lualib.h"
}

//
//
//
// Game singleton implementation
//
//
//

void App::Init(bool enable_http)
{
	// Init base game
	CzApp::Init(enable_http);

	// Display ads view
	CZ_XOML->Process(this, "Start.xml");
	setTimePaused(false);	// resets time
}

void App::Release()
{
	// Release game
	CzApp::Release();
}

bool App::Update()
{
	if (!CzApp::Update())
		return false;

	return true;
}

void App::Draw()
{
	CzApp::Draw();
}


int App::LoadFromXoml(bool load_children, CzXmlNode* node)
{
	// Process inner tags
	if (!CZ_XOML->Process(this, node))
		return 0;

	return 1;
}

