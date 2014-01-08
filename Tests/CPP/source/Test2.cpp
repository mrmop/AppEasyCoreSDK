#include "IzPlatformDisplay.h"
#include "App.h"
#include "CzScene.h"
#include "CzActions.h"
#include "CzCamera.h"
#include "CzUIListBox.h"
#include "CzUILabel.h"
#include "CzHashes.h"

void Test2()
{
	// Create main scene
	CZ_NEW_SCENE_WITH_CAM(scene, "Scene1")
	APP->addScene(scene);
	APP->changeScene(scene);
	scene->setExtents(-100, -100, 200, 200);
	scene->setClippingArea(-100, -100, 200, 200);

	// Create label
	CzFont* font = (CzFont*)CZ_GLOBAL_RESOURCE_MANAGER->findResource("serif", CzHashes::Font_Hash);
	IzBrush* background_brush = (IzBrush*)CZ_GLOBAL_RESOURCE_MANAGER->findResource("Button1Brush", CzHashes::Brush_Hash);
	CzIVec2 size(200, 70);
	CZ_NEW_LABEL(scene, label, "label1", CzString("Clipping"), background_brush, font, size, false);
	label->setBackgroundColour(CzColour(255, 80, 80, 255));
	label->setWrapPosition(true);

	// Create an actions list that modifies the Position property of the label
	CZ_NEW_SINGLE_ACTION_P2(actions, a, "Update", "AddProperty", "Position", "2, 2");
	label->getActionsManager()->addActions(actions);

	// Tie label OnTick event to Update actions list
	label->getEventsManager()->addEvent("OnTick", "Update", true);
	label->setTickEnabled(true);	// When OnTick is specified we need to inform actor that it has been added to list (optimisation)

	// Create Test label header
	size = CzIVec2(-100, -10);
	CZ_NEW_LABEL(scene, title_label, "title_label", CzString("Demonstration of Scene Clipping"), background_brush, font, size, true);
	title_label->setBackgroundColour(CzColour(80, 80, 80, 255));
	title_label->setDocking(CzScene::Dock_Top);
	title_label->setIgnoreCamera(true);
}
