#include "IzPlatformDisplay.h"
#include "App.h"
#include "CzScene.h"
#include "CzActions.h"
#include "CzCamera.h"
#include "CzUIListBox.h"
#include "CzUILabel.h"
#include "CzHashes.h"

extern void Test1();
extern void Test2();

class Actions_Test : public IzXomlAction
{
public:
	enum ActionType
	{
		Action_RunTest		= 0,	
		Action_Max			= 1,
	};
protected:
	ActionType	Type;
	Actions_Test() {}
public:
	Actions_Test(ActionType type)
	{
		Type = type;
		switch (Type)
		{
		case Action_RunTest:
			setActionName("runtest");
			break;
		}
	}

	void Execute(IzXomlResource* source, CzAction* action)
	{
		CzApp* game = NULL;
		CzScene* scene = NULL;
		CzActor* actor = NULL;

		if (source->getClassTypeHash() == CzHashes::Scene_Hash)
		{
			scene = (CzScene*)source;
			game = scene->getParent();
		}
		else
		if (source->getClassTypeHash() == CzHashes::Actor_Hash)
		{
			actor = (CzActor*)source;
			scene = actor->getScene();
			game = scene->getParent();
		}
		IzXomlResource* cont = (actor != NULL) ? (IzXomlResource*)actor : (IzXomlResource*)scene;

		switch (Type)
		{
		case Action_RunTest:
			{
				// Show the HUD scene
				APP->findScene("HUD")->setVisible(true);

				// Tell main scene to scroll out
				CzAnimTimeline* timeline = CZ_GLOBAL_TIMELINE_MANAGER->findTimeline("SceneOut");
				APP->findScene("Tests")->setTimeline(timeline);
				timeline->restart();

				// Load the test scene
				switch (action->getParameter1().getAsInt())
				{
				case 1:	Test1();	break;
				case 2:	Test2();	break;
				}
			}
			break;
		}
	}
};



void UIStyle()
{
	// Load serif ttf and add it to global resource manager
	CZ_NEW_FONT(font, "serif", "Serif.ttf", 8);
	CZ_GLOBAL_RESOURCE_MANAGER->addResource(font);

	// Load UI Image atlas and add it to global resource manager
	CZ_NEW_IMAGE(ui_image, "ui_image", "ui.png", true);
	CZ_GLOBAL_RESOURCE_MANAGER->addResource(ui_image);

	// Create UI brushes and add to global resource manager
	CZ_NEW_IMAGE9_BRUSH(b1, "Button1Brush", ui_image, CzIRect(320, 70, 200, 70), CzIRect(7, 8, 186, 54));
	CZ_GLOBAL_RESOURCE_MANAGER->addResource(b1);
	CZ_NEW_IMAGE9_BRUSH(b2, "Button2Brush", ui_image, CzIRect(320, 140, 200, 70), CzIRect(8, 8, 184, 54));
	CZ_GLOBAL_RESOURCE_MANAGER->addResource(b2);
	CZ_NEW_IMAGE_BRUSH(b3, "CheckBoxOffBrush", ui_image, CzIRect(320, 210, 48, 48));
	CZ_GLOBAL_RESOURCE_MANAGER->addResource(b3);
	CZ_NEW_IMAGE_BRUSH(b4, "CheckBoxOnBrush", ui_image, CzIRect(368, 210, 48, 48));
	CZ_GLOBAL_RESOURCE_MANAGER->addResource(b4);
	CZ_NEW_IMAGE9_BRUSH(b5, "PanelBrush", ui_image, CzIRect(0, 0, 320, 320), CzIRect(12, 15, 296, 290));
	CZ_GLOBAL_RESOURCE_MANAGER->addResource(b5);
	CZ_NEW_IMAGE9_BRUSH(b6, "SmallPanelBrush", ui_image, CzIRect(320, 0, 200, 70), CzIRect(8, 8, 184, 54));
	CZ_GLOBAL_RESOURCE_MANAGER->addResource(b6);
	CZ_NEW_IMAGE9_BRUSH(b7, "ButtonBrush2", ui_image, CzIRect(0, 458, 222, 68), CzIRect(8, 7, 206, 54));
	CZ_GLOBAL_RESOURCE_MANAGER->addResource(b7);
	
	// Load test_image and add it to global resource manager
	CZ_NEW_IMAGE(test_image, "test_image", "test_image.jpg", true);
	CZ_GLOBAL_RESOURCE_MANAGER->addResource(test_image);
	CZ_NEW_IMAGE_BRUSH(b8, "test_image", test_image, CzIRect(0, 0, 1024, 768));
	CZ_GLOBAL_RESOURCE_MANAGER->addResource(b8);

	// Create scene scroll-in animation
	CZ_NEW_ANIM(in_anim, "SceneIn", 1, CzAnimFrame::FT_Vec2);
	CZ_NEW_ANIM_FRAME_VEC2(in_anim, 0, -1000, 0, CzAnimFrame::AT_QuadOut);
	CZ_NEW_ANIM_FRAME_VEC2(in_anim, 1, 0, 0, CzAnimFrame::AT_QuadOut);
	CZ_GLOBAL_RESOURCE_MANAGER->addResource(in_anim);
	// Create scene scroll-in animation timeline
	CZ_NEW_TIMELINE(scene_in_timeline, "SceneIn", in_anim, 0, true, 1, "Position");
	CZ_GLOBAL_TIMELINE_MANAGER->addTimeline(scene_in_timeline);

	// Create scene scroll-out animation
	CZ_NEW_ANIM(out_anim, "SceneOut", 1, CzAnimFrame::FT_Vec2);
	CZ_NEW_ANIM_FRAME_VEC2(out_anim, 0, 0, 0, CzAnimFrame::AT_QuadIn);
	CZ_NEW_ANIM_FRAME_VEC2(out_anim, 1, -1000, 0, CzAnimFrame::AT_QuadIn);
	CZ_GLOBAL_RESOURCE_MANAGER->addResource(out_anim);
	// Create scene scroll-out animation timeline
	CZ_NEW_TIMELINE(scene_out_timeline, "SceneOut", out_anim, 0, true, 1, "Position");
	CZ_GLOBAL_TIMELINE_MANAGER->addTimeline(scene_out_timeline);
}

void HUDScene()
{
	// Create HUD scene
	CZ_NEW_SCENE_WITH_CAM(scene, "HUD")
	APP->addScene(scene);
	scene->setLayer(9);
	scene->setVisible(false);

	// Create a label button that the user can taop to go back to the menu
	CzFont* font = (CzFont*)CZ_GLOBAL_RESOURCE_MANAGER->findResource("serif", CzHashes::Font_Hash);
	IzBrush* background_brush = (IzBrush*)CZ_GLOBAL_RESOURCE_MANAGER->findResource("Button1Brush", CzHashes::Brush_Hash);
	CzIVec2 size = CzIVec2(-50, -10);
	CZ_NEW_LABEL(scene, label, "MenuButton", CzString("Menu"), background_brush, font, size, false);
	label->setDocking(CzScene::Dock_Bottom);
	label->setBackgroundColour(CzColour(55, 55, 55, 255));
	label->setSelectedBackgroundColour(CzColour(128, 128, 255, 255));

	// Create an actions list that is ran when the user taps the label
	CzActions* actions = new CzActions();
	actions->setName("Tapped");
	CzAction* action = new CzAction();
	action->setMethodName("KillScene");
	action->setParameter1("Scene1");
	actions->addAction(action);
	action = new CzAction();
	action->setMethodName("SetCurrentScene");
	action->setParameter1("Tests");
	actions->addAction(action);
	action = new CzAction();
	action->setMethodName("HideScene");
	action->setParameter1("HUD");
	actions->addAction(action);
	action = new CzAction();
	action->setMethodName("SetTimeline");
	action->setParameter1("SceneIn");
	action->setParameter2("Tests");
	actions->addAction(action);
	label->getActionsManager()->addActions(actions);

	// Tie label events to actions
	label->getEventsManager()->addEvent("OnTapped", "Tapped", true);
}

void Start()
{
	// Add our custom test actions to XOML actions system
	// These actions allow us to call back code with parameters from XOML 
	for (int t = 0; t < Actions_Test::Action_Max; t++)
		CZ_XOML->addAction(new Actions_Test((Actions_Test::ActionType)t));

	// Create UI style elements
	UIStyle();

	// Create main scene
	CZ_NEW_SCENE_WITH_CAM(scene, "Tests")
	APP->addScene(scene);
	APP->changeScene(scene);

	// Create scene Exit Actions list
	CZ_NEW_SINGLE_ACTION(actions_exit, a1, "Exit", "Exit");
	scene->getActionsManager()->addActions(actions_exit);

	// Create scene Device Resume Actions list
	CZ_NEW_SINGLE_ACTION_P3(actions_resume, a2, "DeviceResume", "SetProperty", "BackgroundColour", "128, 128, 255, 255", "Menu");
	scene->getActionsManager()->addActions(actions_resume);

	// Tie scene events to actions
	scene->getEventsManager()->addEvent("OnKeyBack", "Exit", true);
	scene->getEventsManager()->addEvent("OnDeviceResume", "DeviceResume", true);

	// Create the menu
	CZ_NEW_LISTBOX(scene, menu_listbox, "Menu", (CzBrushImage9*)CZ_GLOBAL_RESOURCE_MANAGER->findResource("PanelBrush", CzHashes::Brush_Hash), -100, -100);

	// Populate menu listbox with items
	CzFont* font = (CzFont*)CZ_GLOBAL_RESOURCE_MANAGER->findResource("serif", CzHashes::Font_Hash);
	IzBrush* background_brush = (IzBrush*)CZ_GLOBAL_RESOURCE_MANAGER->findResource("Button1Brush", CzHashes::Brush_Hash);
	CzIVec2 size = CzIVec2(-80, -8);
	for (int t = 1; t < 60; t++)
	{
		// Create a label that serbes as a menu item
		CzString name = "Test";
		name += t;
		CZ_NEW_LABEL(scene, label, name.c_str(), name, background_brush, font, size, false);
		label->setLinkedTo(menu_listbox);
		label->setDepth(0);
		label->setBackgroundColour(CzColour(55, 55, 55, 255));
		label->setSelectedBackgroundColour(CzColour(128, 128, 255, 255));

		// Create an actions list that is ran when the user taps the label
		CZ_NEW_SINGLE_ACTION_P1(actions, a, "Tapped", "RunTest", CzString(t).c_str());
		label->getActionsManager()->addActions(actions);

		// Tie label events to actions
		label->getEventsManager()->addEvent("OnTapped", "Tapped", true);
	}

	// Create HUD scene
	HUDScene();
	
}
