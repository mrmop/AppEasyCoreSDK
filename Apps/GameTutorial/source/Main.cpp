// Game headers
#include "App.h"

// Include platform
#include "CzPlatform.h"

int main()
{
	// Init platform
	CzPlatform_Create();

	// Init App
	App::Create();
	APP->Init(true);

	// Main Game Loop
	while (!PLATFORM_SYS->CheckAppQuit())
	{
		// Update App
		if (!APP->Update())
			break;

		// Draw App
		APP->Draw();
	}

	// Shut down App
	APP->Release();
	App::Destroy();

	// Shut down platform
	CzPlatform_Destroy();

	return 0;
}

