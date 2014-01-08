#include "IzPlatformDisplay.h"
#include "App.h"
#include "CzScene.h"
#include "CzActions.h"
#include "CzCamera.h"
#include "CzUIListBox.h"
#include "CzUILabel.h"
#include "CzHashes.h"

void Test1()
{
	// Create main scene
	CZ_NEW_SCENE_WITH_CAM(scene, "Scene1")
	APP->addScene(scene);
	APP->changeScene(scene);

	// Create position animation
	CZ_NEW_ANIM(pos_anim, "pos_anim", 5, CzAnimFrame::FT_Vec2);
	CZ_NEW_ANIM_FRAME_VEC2(pos_anim, 0,		0, 0,		CzAnimFrame::AT_QuadIn);
	CZ_NEW_ANIM_FRAME_VEC2(pos_anim, 1,		100, 100,	CzAnimFrame::AT_QuadOut);
	CZ_NEW_ANIM_FRAME_VEC2(pos_anim, 2,		0, 100,		CzAnimFrame::AT_QuadIn);
	CZ_NEW_ANIM_FRAME_VEC2(pos_anim, 3,		-100, 100,	CzAnimFrame::AT_QuadOut);
	CZ_NEW_ANIM_FRAME_VEC2(pos_anim, 4,		0, 0,		CzAnimFrame::AT_QuadIn);
	scene->getResourceManager()->addResource(pos_anim);
	// Create angle animation
	CZ_NEW_ANIM(ang_anim, "ang_anim", 5, CzAnimFrame::FT_Float);
	CZ_NEW_ANIM_FRAME_FLOAT(ang_anim, 0,	0,			CzAnimFrame::AT_Linear);
	CZ_NEW_ANIM_FRAME_FLOAT(ang_anim, 2.5,	180,		CzAnimFrame::AT_Linear);
	CZ_NEW_ANIM_FRAME_FLOAT(ang_anim, 5,	0,			CzAnimFrame::AT_Linear);
	scene->getResourceManager()->addResource(ang_anim);
	// Create combined animation timeline
	CZ_NEW_TIMELINE(pos_anim1, "SceneIn", pos_anim, 0, true, 0, "Position");
	CZ_ADD_TIMELINE(pos_anim1, ang_anim, 0, true, 0, "Angle");
	scene->getTimelinesManager()->addTimeline(pos_anim1);

	// Assign timeline to scene and start the animation
	scene->setTimeline(pos_anim1);
	pos_anim1->restart();

	// Create label
	CzFont* font = (CzFont*)CZ_GLOBAL_RESOURCE_MANAGER->findResource("serif", CzHashes::Font_Hash);
	IzBrush* background_brush = (IzBrush*)CZ_GLOBAL_RESOURCE_MANAGER->findResource("Button1Brush", CzHashes::Brush_Hash);
	CzIVec2 size(200, 70);
	CZ_NEW_LABEL(scene, label, "label1", CzString("Scene Animation"), background_brush, font, size, false);
	label->setBackgroundColour(CzColour(255, 80, 80, 255));

	// Create Test label header
	size = CzIVec2(-100, -10);
	CZ_NEW_LABEL(scene, title_label, "title_label", CzString("Demonstration of Scene Animation"), background_brush, font, size, true);
	title_label->setBackgroundColour(CzColour(80, 80, 80, 255));
	title_label->setDocking(CzScene::Dock_Top);
	title_label->setIgnoreCamera(true);
}
