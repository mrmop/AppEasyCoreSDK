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

#include "CzXoml.h"
#include "CzHashes.h"
#include "CzUtil.h"
#include "CzString.h"
#include "CzApp.h"
#include "CzXomlVariables.h"
#include "CzScene.h"
#include "CzActor.h"
#include "CzActorImage.h"
#include "CzActorText.h"
#include "CzActorConnector.h"
#include "CzActorParticles.h"
#include "CzAnim.h"
#include "CzShapes.h"
#include "CzBox2d.h"
#include "CzCamera.h"
#include "CzStyles.h"
#include "CzTemplates.h"
#include "CzActions.h"
#include "CzScript.h"
#include "CzFont.h"
#include "CzAudio.h"
#include "CzVideo.h"
#include "CzModifier.h"
#include "CzModifierExt.h"
#include "CzProgram.h"
#include "CzUIIcon.h"
#include "CzUILabel.h"
#include "CzUITextBox.h"
#include "CzUIVideoOverlay.h"
#include "CzUIGrid.h"
#include "CzUICanvas.h"
#include "CzUIStackPanel.h"
#include "CzUIWrapPanel.h"
#include "CzUIListBox.h"
#include "CzUISlider.h"
#include "CzUIImageView.h"
#include "CzUIWebView.h"
#include "CzUITextView.h"
#include "CzUITabBar.h"
#include "CzRemoteReq.h"
#include "CzCam.h"
#include "CzMarket.h"
#include "CzActionsActor.h"
#include "CzActionsAnimation.h"
#include "CzActionsComms.h"
#include "CzActionsMarket.h"
#include "CzActionsMedia.h"
#include "CzActionsProgram.h"
#include "CzActionsResource.h"
#include "CzActionsScene.h"
#include "CzActionsScript.h"
#include "CzActionsShared.h"
#include "CzActionsSys.h"
#include "CzActionsVariables.h"
#include "CzActionsTimer.h"
#include "CzXomlResourceManager.h"
#include "CzXomlLoad.h"
#include "CzUserProperty.h"
#include "CzGeometry.h"


const char* CzXoml::DockingNames[] = 
{
	"none", 
	"top", 
	"bottom", 
	"left", 
	"right", 
	"topleft", 
	"topright", 
	"bottomleft", 
	"bottomright", 
};

const char* CzXoml::AspectLockNames[] = 
{
	"none", 
	"x", 
	"y", 
};

const char* CzXoml::AlphaModeNames[] = 
{
	"none", 
	"half", 
	"add", 
	"sub", 
	"blend", 
};

const char* CzXoml::AlignHNames[] = 
{
	"left", 
	"centre", 
	"right", 
};

const char* CzXoml::AlignVNames[] = 
{
	"top", 
	"middle", 
	"bottom", 
};

const char*	CzXoml::SelectTypeNames[] = 
{
	"normal", 
	"toggle", 
	"sticky", 
};

const char*	CzXoml::AxisNames[] = 
{
	"none", 
	"x", 
	"y", 
	"xy", 
};

const char*	CzXoml::OrientationNames[] = 
{
	"horizontal", 
	"vertical", 
};

const char*	CzXoml::InputTypeHintNames[] = 
{
	"text", 
	"number", 
	"password", 
	"email", 
	"url", 
};

// 
// 
// 
// 
// CzXomlProperty implementation
// 
// 
// 
// 

/**
 @fn	CzXomlProperty* CzXomlProperty::createOfType(eCzXomlPropertyType type)

 @brief	Creates a property of the specified type.

 @param	type	The property type.

 @return	The new property.
 */

CzXomlProperty* CzXomlProperty::createOfType(eCzXomlPropertyType type)
{
	CzXomlProperty* prop = new CzXomlProperty();
	prop->Type = type;

	return prop;
}

/**
 @fn	CzXomlProperty* CzXomlProperty::createOfType(const char* type_name)

 @brief	Creates a property of the specified named type.

 @param	type	The property type.

 @return	The new property.
 */

CzXomlProperty* CzXomlProperty::createOfType(const char* type_name)
{
	unsigned int hash = CzString::CalculateHash(type_name);

	if (hash == CzHashes::string_Hash)
		return createOfType(PT_String);
	else
	if (hash == CzHashes::bool_Hash)
		return createOfType(PT_Bool);
	else
	if (hash == CzHashes::float_Hash)
		return createOfType(PT_Float);
	else
	if (hash == CzHashes::int_Hash)
		return createOfType(PT_Int);
	else
	if (hash == CzHashes::vec2_Hash)
		return createOfType(PT_Vec2);
	else
	if (hash == CzHashes::vec3_Hash)
		return createOfType(PT_Vec3);
	else
	if (hash == CzHashes::vec4_Hash)
		return createOfType(PT_Vec4);
	else
	if (hash == CzHashes::resource_Hash)
		return createOfType(PT_Resource);
	else
	if (hash == CzHashes::var_Hash)
		return createOfType(PT_Var);

	return NULL;
}

/**
 @fn	bool CzXomlProperty::setValue(const CzString& data, bool delta)

 @brief	Sets the value of the property from a string.

 @param	data 	The data.
 @param	delta	true to add to the property.

 @return	true if it succeeds, false if it fails.
 */

bool CzXomlProperty::setValue(const CzString& data, bool delta)
{
	float	float_pool[8];

	switch (Type)
	{
	case PT_String:
		if (delta)
		{
			char* sdata = (char*)p_data;
			if (p_data == NULL)
			{
				sdata = new char [data.getLength() + 1];
				strcpy((char*)sdata, data.c_str());
			}
			else
			{
				int clen = strlen((char*)p_data);
				int len = data.getLength() + clen;
				sdata = new char [len + 1];
				strcpy((char*)sdata, (const char*)p_data);
				strcpy(((char*)sdata) + clen, data.c_str());
				delete [] (char*)p_data;
			}
			p_data = sdata;
			OwnData = true;
		}
		else
		{
			if (p_data != NULL && OwnData)
				delete [] p_data;
			p_data = new char [data.getLength() + 1];
			strcpy((char*)p_data, data.c_str());
			OwnData = true;
		}
		break;
	case PT_Bool:
		if (delta)
			p_bool = !p_bool;
		else
			p_bool = data.getAsBool();
		break;
	case PT_Float:
		if (delta)
			p_float += data.getAsFloat();
		else
			p_float = data.getAsFloat();
		break;
	case PT_Int:
		if (delta)
			p_int += data.getAsInt();
		else
			p_int = data.getAsInt();
		break;
	case PT_Vec2:
		if (data.getAsListOfFloat(float_pool) != 2)
			return false;
		else
		{
			if (delta)
			{
				p_vec[0] += float_pool[0];
				p_vec[1] += float_pool[1];
			}
			else
			{
				p_vec[0] = float_pool[0];
				p_vec[1] = float_pool[1];
			}
		}
		break;
	case PT_Vec3:
		if (data.getAsListOfFloat(float_pool) != 3)
			return false;
		else
		{
			if (delta)
			{
				p_vec[0] += float_pool[0];
				p_vec[1] += float_pool[1];
				p_vec[2] += float_pool[2];
			}
			else
			{
				p_vec[0] = float_pool[0];
				p_vec[1] = float_pool[1];
				p_vec[2] = float_pool[2];
			}
		}
		break;
	case PT_Vec4:
		if (data.getAsListOfFloat(float_pool) != 4)
			return false;
		else
		{
			if (delta)
			{
				p_vec[0] += float_pool[0];
				p_vec[1] += float_pool[1];
				p_vec[2] += float_pool[2];
				p_vec[3] += float_pool[3];
			}
			else
			{
				p_vec[0] = float_pool[0];
				p_vec[1] = float_pool[1];
				p_vec[2] = float_pool[2];
				p_vec[3] = float_pool[3];
			}
		}
		break;
	default:
		return false;
	}

	return true;
}

void CzXomlProperty::toString(CzString& string) const
{
	if (Type == PT_String)
		string = (const char*)p_data;
	else
	if (Type == PT_Int)
		string = p_int;
	else
	if (Type == PT_Float)
		string = p_float;
	else
	if (Type == PT_Bool)
		string = p_bool;
	else
	if (Type == PT_Vec2)
		string.set(CzVec2(p_vec[0], p_vec[1]));
	else
	if (Type == PT_Vec3)
		string.set(CzVec3(p_vec[0], p_vec[1], p_vec[2]));
	else
	if (Type == PT_Vec4)
		string.set(CzVec4(p_vec[0], p_vec[1], p_vec[2], p_vec[3]));
}

/**
 @fn	bool CzXomlProperty::setValue(const CzXomlProperty& data, bool delta)

 @brief	Sets the value of the property from a property.

 @param	data 	The data.
 @param	delta	true to add to the property.

 @return	true if it succeeds, false if it fails.
 */

bool CzXomlProperty::setValue(const CzXomlProperty& prop, bool delta)
{
	if (prop.Type == PT_String)
		return setValue(CzString((const char*)prop.p_data), delta);

	switch (Type)
	{
	case PT_String:
		{
			CzString data;
			prop.toString(data);
			
			if (delta)
			{
				char* sdata = (char*)p_data;
				if (p_data == NULL)
				{
					sdata = new char [data.getLength() + 1];
					strcpy((char*)sdata, data.c_str());
				}
				else
				{
					int clen = strlen((char*)p_data);
					int len = data.getLength() + clen;
					sdata = new char [len + 1];
					strcpy((char*)sdata, (const char*)p_data);
					strcpy(((char*)sdata) + clen, data.c_str());
					delete [] (char*)p_data;
				}
				p_data = sdata;
				OwnData = true;
			}
			else
			{
				if (p_data != NULL && OwnData)
					delete [] p_data;
				p_data = new char [data.getLength() + 1];
				strcpy((char*)p_data, data.c_str());
				OwnData = true;
			}
		}
		break;
	case PT_Bool:
		if (delta)
			p_bool = !p_bool;
		else
		{
			if (prop.Type == PT_Bool)
				p_bool = prop.p_bool;
			else
			if (prop.Type == PT_Int)
				p_bool = prop.p_int != 0;
			else
			if (prop.Type == PT_Float)
				p_bool = prop.p_float != 0;
			else
				return false;
		}
		break;
	case PT_Float:
		if (delta)
		{
			if (prop.Type == PT_Float)
				p_float += prop.p_float;
			else
			if (prop.Type == PT_Int)
				p_float += (float)prop.p_int;
			else
			if (prop.Type == PT_Bool)
				p_float += prop.p_bool ? 1.0f : 0;
			else
				return false;
		}
		else
		{
			if (prop.Type == PT_Float)
				p_float = prop.p_float;
			else
			if (prop.Type == PT_Int)
				p_float = (float)prop.p_int;
			else
			if (prop.Type == PT_Bool)
				p_float = prop.p_bool ? 1.0f : 0;
			else
				return false;
		}
		break;
	case PT_Int:
		if (delta)
		{
			if (prop.Type == PT_Int)
				p_int += prop.p_int;
			else
			if (prop.Type == PT_Float)
				p_int += (int)prop.p_float;
			else
			if (prop.Type == PT_Bool)
				p_int += prop.p_bool ? 1 : 0;
			else
				return false;
		}
		else
		{
			if (prop.Type == PT_Int)
				p_int = prop.p_int;
			else
			if (prop.Type == PT_Float)
				p_int = (int)prop.p_float;
			else
			if (prop.Type == PT_Bool)
				p_int = prop.p_bool ? 1 : 0;
			else
				return false;
		}
		break;
	case PT_Vec2:
		if (delta)
		{
			if (prop.Type == PT_Vec2 || prop.Type == PT_Vec3 || prop.Type == PT_Vec4)
			{
				p_vec[0] += prop.p_vec[0];
				p_vec[1] += prop.p_vec[1];
			}
			else
				return false;
		}
		else
		{
			if (prop.Type == PT_Vec2 || prop.Type == PT_Vec3 || prop.Type == PT_Vec4)
			{
				p_vec[0] = prop.p_vec[0];
				p_vec[1] = prop.p_vec[1];
			}
			else
				return false;
		}
		break;
	case PT_Vec3:
		if (delta)
		{
			if (prop.Type == PT_Vec3 || prop.Type == PT_Vec4)
			{
				p_vec[0] += prop.p_vec[0];
				p_vec[1] += prop.p_vec[1];
				p_vec[2] += prop.p_vec[2];
			}
			else
			if (prop.Type == PT_Vec2)
			{
				p_vec[0] += prop.p_vec[0];
				p_vec[1] += prop.p_vec[1];
			}
			else
				return false;
		}
		else
		{
			if (prop.Type == PT_Vec3 || prop.Type == PT_Vec4)
			{
				p_vec[0] = prop.p_vec[0];
				p_vec[1] = prop.p_vec[1];
				p_vec[2] = prop.p_vec[2];
			}
			else
			if (prop.Type == PT_Vec2)
			{
				p_vec[0] = prop.p_vec[0];
				p_vec[1] = prop.p_vec[1];
			}
			else
				return false;
		}
		break;
	case PT_Vec4:
		if (delta)
		{
			if (prop.Type == PT_Vec4)
			{
				p_vec[0] += prop.p_vec[0];
				p_vec[1] += prop.p_vec[1];
				p_vec[2] += prop.p_vec[2];
				p_vec[3] += prop.p_vec[3];
			}
			else
			if (prop.Type == PT_Vec3)
			{
				p_vec[0] += prop.p_vec[0];
				p_vec[1] += prop.p_vec[1];
				p_vec[2] += prop.p_vec[2];
			}
			else
			if (prop.Type == PT_Vec2)
			{
				p_vec[0] += prop.p_vec[0];
				p_vec[1] += prop.p_vec[1];
			}
			else
				return false;
		}
		else
		{
			if (prop.Type == PT_Vec4)
			{
				p_vec[0] = prop.p_vec[0];
				p_vec[1] = prop.p_vec[1];
				p_vec[2] = prop.p_vec[2];
				p_vec[3] = prop.p_vec[3];
			}
			else
			if (prop.Type == PT_Vec3)
			{
				p_vec[0] = prop.p_vec[0];
				p_vec[1] = prop.p_vec[1];
				p_vec[2] = prop.p_vec[2];
			}
			else
			if (prop.Type == PT_Vec2)
			{
				p_vec[0] = prop.p_vec[0];
				p_vec[1] = prop.p_vec[1];
			}
			else
				return false;
		}
		break;
	}

	return true;
}

/**
 @fn	void CzXomlProperty::setToVariable(CzXomlVariable& var)

 @brief	Sets the property to a variable.

 @param [out]	var	The target variable.
 */

void CzXomlProperty::setToVariable(CzXomlVariable& var)
{
	switch (Type)
	{
	case PT_String:
		var.setValue((const char*)p_data);
		break;
	case PT_Bool:
		var.setValue(CzString(p_bool).c_str());
		break;
	case PT_Float:
		var.setValue(CzString(p_float).c_str());
		break;
	case PT_Int:
		var.setValue(CzString(p_int).c_str());
		break;
	case PT_Vec2:
		{
			CzVec2 v;
			v.x = p_vec[0];
			v.y = p_vec[1];
			var.setValue(CzString(v).c_str());
		}
		break;
	case PT_Vec3:
		{
			CzVec3 v;
			v.x = p_vec[0];
			v.y = p_vec[1];
			v.z = p_vec[2];
			var.setValue(CzString(v).c_str());
		}
		break;
	case PT_Vec4:
		{
			CzVec4 v;
			v.x = p_vec[0];
			v.y = p_vec[1];
			v.z = p_vec[2];
			v.w = p_vec[3];
			var.setValue(CzString(v).c_str());
		}
		break;
	}
}

/**
 @fn	void CzXomlClassDef::addProperty(CzXomlClassProperty* prop)

 @brief	Adds a property to the class definition.

 @param [in]	prop	If non-null, the property.
 */

void CzXomlClassDef::addProperty(CzXomlClassProperty* prop)
{
	Properties.push_back(prop);
}

/**
 @fn	void CzXomlClassDef::removeProperty(CzXomlClassProperty* prop)

 @brief	Removes a property from the class definition.

 @param [in]	prop	If non-null, the property.
 */

void CzXomlClassDef::removeProperty(CzXomlClassProperty* prop)
{
	Properties.remove(prop);
}

/**
 @fn	CzXomlClassProperty* CzXomlClassDef::findProperty(unsigned int name_hash)

 @brief	Searches for the first named property.

 @param	name_hash	The name of the property as a string hash.

 @return	null if it fails, else the found property.
 */

CzXomlClassProperty* CzXomlClassDef::findProperty(unsigned int name_hash)
{
	for (CzList<CzXomlClassProperty*>::iterator it = Properties.begin(); it != Properties.end(); ++it)
	{
		if ((*it)->getNameHash() == name_hash)
			return *it;
	}

	return NULL;
}


//
// 
// 
//
// CzXoml Implementation
// 
// 
// 
//
CDECLARE_SINGLETON(CzXoml)

/**
 @fn	IzXomlClassCreator* CzXoml::findClass(unsigned int name_hash)

 @brief	Searches the XOML system for the named class

 @param	name_hash	The class name as a string hash.

 @return	null if it fails, else the found class.
 */

IzXomlClassCreator* CzXoml::findClass(unsigned int name_hash)
{
	for (_Iterator it = ClassCreators.begin(); it != ClassCreators.end(); ++it)
	{
		if ((*it)->getClassNameHash() == name_hash)
			return *it;
	}

	return NULL;
}

/**
 @fn	IzXomlClassCreator* CzXoml::findClass(const char* name)

 @brief	Searches the XOML system for the named class

 @param	name_hash	The class name.

 @return	null if it fails, else the found class.
 */

IzXomlClassCreator* CzXoml::findClass(const char* name)
{
	return findClass(CzString::CalculateHash(name));
}

/**
 @fn	IzXomlAction* CzXoml::findAction(unsigned int name_hash)

 @brief	Searches the XOML action system for the named XOML action

 @param	name_hash	The action name as a string hash.

 @return	null if it fails, else the found action.
 */

IzXomlAction* CzXoml::findAction(unsigned int name_hash)
{
	for (_ActionsIterator it = Actions.begin(); it != Actions.end(); ++it)
	{
		if ((*it)->getActionNameHash() == name_hash)
			return *it;
	}

	return NULL;
}

/**
 @fn	IzXomlAction* CzXoml::findAction(const char* name)

 @brief	Searches the XOML action system for the named XOML action

 @param	name		The action name.

 @return	null if it fails, else the found action.
 */

IzXomlAction* CzXoml::findAction(const char* name)
{
	return findAction(CzString::CalculateHash(name));
}

/**
 @fn	void CzXoml::Init()

 @brief	Initialises the XOML system.

 Initialises the XOML system, this includes:
 - Adding all of the XOML class creators to XOML for each of the classes that should be available in XOML
 - Adding all of the XOML action methods to XOML that should be available to be called from actions.
 - Adding all of the XOML modifiers that should be made available to scenes and actions.

 */

void CzXoml::Init()
{
	// Set up class property systems
	CzScene::InitClass();
	CzActor::InitClass();
	CzActorConnector::InitClass();
	CzActorImage::InitClass();
	CzActorParticles::InitClass();
	CzActorText::InitClass();
	CzUIBase::InitClass();
	CzUIGrid::InitClass();
	CzUIImageView::InitClass();
	CzUILabel::InitClass();
	CzUIListBox::InitClass();
	CzUISlider::InitClass();
	CzUIStackPanel::InitClass();
	CzUITabBar::InitClass();
	CzUITab::InitClass();
	CzUITextBox::InitClass();
	CzUITextView::InitClass();
	CzUIVideoOverlay::InitClass();
	CzUIWebView::InitClass();
	CzUIWrapPanel::InitClass();
	CzCamera::InitClass();
	CzActorParticle::InitClass();
	CzRemoteReq::InitClass();

	// Add some basic classes to the XOML tags namespace
	addClass(new CzActionsCreator());
	addClass(new CzTemplateCreator());
	addClass(new CzTemplateFromCreator());
	addClass(new CzStyleCreator());
	addClass(new CzAnimCreator());
	addClass(new CzBrushCreator());
	addClass(new CzXomlAddVariableCreator());
	addClass(new CzXomlLoadCreator());
	addClass(new CzUserPropertyListCreator());
	addClass(new CzAnimTimelineCreator());
	addClass(new CzActorImageCreator());
	addClass(new CzActorTextCreator());
	addClass(new CzUILabelCreator());
	addClass(new CzUIIconCreator());
	addClass(new CzUITextBoxCreator());
	addClass(new CzUICanvasCreator());
	addClass(new CzUIStackPanelCreator());
	addClass(new CzUIWrapPanelCreator());
	addClass(new CzUIGridCreator());
	addClass(new CzUIListBoxCreator());
	addClass(new CzUISliderCreator());
	addClass(new CzUIImageViewCreator());
	addClass(new CzUITextViewCreator());
	addClass(new CzUITabCreator());
	addClass(new CzUITabsCreator());
	addClass(new CzUITabBarCreator());
	addClass(new CzActorParticlesCreator());
	addClass(new CzActorConnectorCreator());
	addClass(new CzXomlTimerCreator());
	addClass(new CzXomlBindingsCreator());
	addClass(new CzBox2dMaterialCreator());
	addClass(new CzBox2dJointsCreator());
	addClass(new CzBox2dFixturesCreator());
	addClass(new CzModifierManagerCreator());
	addClass(new CzGeometryCreator());
	addClass(new CzImageCreator());
	addClass(new CzDataFileCreator());
	addClass(new CzScriptCreator());
	addClass(new CzSceneCreator());
	addClass(new CzCameraCreator());
	addClass(new CzShapeCreator());
	addClass(new CzSoundCreator());
	addClass(new CzFontCreator());
	addClass(new CzVideoCreator());
	addClass(new CzMarketCreator());
	addClass(new CzUIWebViewCreator());
	addClass(new CzUIVideoOverlayCreator());
	addClass(new CzRemoteReqCreator());
	addClass(new CzVideoCamCreator());
	addClass(new CzProgramCreator());
	addClass(new CzAppCreator());

	// Add actions
	for (int t = 0; t < CzXomlActions_Script::Action_Max; t++)
		addAction(new CzXomlActions_Script((CzXomlActions_Script::eActionType)t));
	for (int t = 0; t < CzXomlActions_Shared::Action_Max; t++)
		addAction(new CzXomlActions_Shared((CzXomlActions_Shared::eActionType)t));
	for (int t = 0; t < CzXomlActions_Actor::Action_Max; t++)
		addAction(new CzXomlActions_Actor((CzXomlActions_Actor::eActionType)t));
	for (int t = 0; t < CzXomlActions_Animation::Action_Max; t++)
		addAction(new CzXomlActions_Animation((CzXomlActions_Animation::eActionType)t));
	for (int t = 0; t < CzXomlActions_Timer::Action_Max; t++)
		addAction(new CzXomlActions_Timer((CzXomlActions_Timer::eActionType)t));
	for (int t = 0; t < CzXomlActions_Variable::Action_Max; t++)
		addAction(new CzXomlActions_Variable((CzXomlActions_Variable::eActionType)t));
	for (int t = 0; t < CzXomlActions_Media::Action_Max; t++)
		addAction(new CzXomlActions_Media((CzXomlActions_Media::eActionType)t));
	for (int t = 0; t < CzXomlActions_Program::Action_Max; t++)
		addAction(new CzXomlActions_Program((CzXomlActions_Program::eActionType)t));
	for (int t = 0; t < CzXomlActions_Scene::Action_Max; t++)
		addAction(new CzXomlActions_Scene((CzXomlActions_Scene::eActionType)t));
	for (int t = 0; t < CzXomlActions_Sys::Action_Max; t++)
		addAction(new CzXomlActions_Sys((CzXomlActions_Sys::eActionType)t));
	for (int t = 0; t < CzXomlActions_Comms::Action_Max; t++)
		addAction(new CzXomlActions_Comms((CzXomlActions_Comms::eActionType)t));
	for (int t = 0; t < CzXomlActions_Market::Action_Max; t++)
		addAction(new CzXomlActions_Market((CzXomlActions_Market::eActionType)t));
	for (int t = 0; t < CzXomlActions_Resource::Action_Max; t++)
		addAction(new CzXomlActions_Resource((CzXomlActions_Resource::eActionType)t));

	// Add modifiers to modifiers list
	CZ_MODS->addModifier(new CzModCollisionNotifyCreator());
	CZ_MODS->addModifier(new CzModFollowHeadingCreator());
	CZ_MODS->addModifier(new CzModScriptCreator());
}

/**
 @fn	void CzXoml::Release()

 @brief	Releases the XOML system.

 Releases teh XOML system but does not delete it. 

 */

void CzXoml::Release()
{
	// Clean up actions
	for (_ActionsIterator it = actions_begin(); it != actions_end(); ++it)
	{
		delete *it;
	}
	Actions.clear();

	// Clean up class creators
	for (_Iterator it = begin(); it != end(); ++it)
	{
		delete *it;
	}
	ClassCreators.clear();

	// Release class property systems
	CzRemoteReq::ReleaseClass();
	CzActorParticle::ReleaseClass();
	CzCamera::ReleaseClass();
	CzUIWrapPanel::ReleaseClass();
	CzUIWebView::ReleaseClass();
	CzUIVideoOverlay::ReleaseClass();
	CzUITextView::ReleaseClass();
	CzUITextBox::ReleaseClass();
	CzUITab::ReleaseClass();
	CzUITabBar::ReleaseClass();
	CzUIStackPanel::ReleaseClass();
	CzUISlider::ReleaseClass();
	CzUIListBox::ReleaseClass();
	CzUILabel::ReleaseClass();
	CzUIImageView::ReleaseClass();
	CzUIGrid::ReleaseClass();
	CzUIBase::ReleaseClass();
	CzActorText::ReleaseClass();
	CzActorParticles::ReleaseClass();
	CzActorImage::ReleaseClass();
	CzActorConnector::ReleaseClass();
	CzActor::ReleaseClass();
	CzScene::ReleaseClass();
}

/**
 @fn	bool CzXoml::Process(IzXomlResource* parent, const char* filename, bool reset_xml_pools)

 @brief	Loads and processes the supplied XOML file.

 @param [in,out]	parent	If non-null, the parent.
 @param	filename		  	Filename of the file.
 @param	reset_xml_pools   	true to reset XML pools (unused).

 @return	true if it succeeds, false if it fails.
 */

bool CzXoml::Process(IzXomlResource* parent, const char* filename, bool reset_xml_pools)
{
//	if (reset_xml_pools)
//		CzXmlParser::PoolsReset();
	CzXmlParser*	xml = new CzXmlParser();

	// Load and parse the xml file
	if (xml->Parse(filename) == XmlErrorNone)
	{
		// Process the XOML data
		bool error = Process(parent, xml->getRoot()->getFirstNode());
		delete xml;
		return error;
	}
	delete xml;

	return false;
}

/**
 @fn	bool CzXoml::Process(IzXomlResource* parent, CzXmlNode* node)

 @brief	Processes the supplied XOML node tree.

 Walks the supplied XOML tree defined by node (the trees root node) instantiating all valid XOML tags that are found.

 @param [in,out]	parent	If non-null, the parent.
 @param [in,out]	node  	If non-null, the root node.

 @return	true if it succeeds, false if it fails.
 */

bool CzXoml::Process(IzXomlResource* parent, CzXmlNode* node)
{
//CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "CzXoml::ProcessEnter: Processing: ", node->GetName().c_str());
//CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "CzXoml::ProcessEnter: Memory used: ", CzString(PLATFORM_SYS->getUsedMemory()).c_str());
	if (node == NULL)
		return false;

	node->ToLower(true);
	CzString out;
	CzString info;
	if (g_CzShowTracing > 0)
		info.allocString(128);
	if (g_CzShowTracing > 1)
		out.allocString(512);

	for (CzXmlNode::_Iterator it = node->begin(); it != node->end(); it++)
	{
		unsigned int name_hash = (*it)->GetName().getHash();
		CzXmlNode* n = *it;

		// Check to see if we support the class
		bool found = false;
		for (_Iterator it2 = begin(); it2 != end(); ++it2)
		{
			if ((*it2)->getClassNameHash() == name_hash)
			{
				found = true;

				if (PLATFORM_SYS->getFreeMemory() < 512 * 1024)
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "XOML - Free memory is low: ", CzString(PLATFORM_SYS->getUsedMemory()).c_str(), n->getParserFileName(), n->Line);

				if (g_CzShowTracing > 1)
				{
					out = n->GetName().c_str();
					out += " - ";
					n->PrintAttributes(out);
					CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "XOML - ", out.c_str(), n->getParserFileName(), n->Line);
				}

				// We do support the class so call the classes creator to instantiate it
				IzXomlResource* instance = (*it2)->CreateInstance(parent);
				if (instance != NULL)
				{
					// Set debug info
					if (g_CzShowTracing > 0)
					{
						info = ":";
						info += (*it)->getParserFileName();
						info += ": Line ";
						info += (*it)->Line;
						instance->setDebugInfo(info.c_str());
					}

					// Load up from XOML
					int success = instance->LoadFromXoml(parent, true, *it);
					if (success > 0)
					{
						instance->setContainer(parent);
						instance->PostLoadFromXoml(parent, *it);
					}
					else
					{
						delete instance;
						if (success == 0)
						{
							CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "XOML - Near XOML tag ", n->GetName().c_str(), n->getParserFileName(), n->Line);
							return false;
						}
					}
				}

/*				if (g_CzShowTracing > 1)
				{
					CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "XOML - Instance created: ", n->GetName().c_str(), n->getParserFileName(), n->Line);
					out = "    ";
					n->PrintAttributes(out);
					CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, out.c_str());
				}*/

				break;
			}
		}

//		if (!found)
//			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "XOML - No XOML creator present for tag - ", (*it)->GetName().c_str());

	}

	return true;
}

/**
 @fn	bool CzXoml::ProcessNode(IzXomlResource* parent, CzXmlNode* node)

 @brief	Processes the supplied XOML node tree.

 Walks the supplied XOML tree defined by node (the trees root node) instantiating all valid XOML tags that are found.

 @param [in,out]	parent	If non-null, the parent.
 @param [in,out]	node  	If non-null, the root node.

 @return	true if it succeeds, false if it fails.
 */

IzXomlResource* CzXoml::ProcessNode(IzXomlResource* parent, CzXmlNode* node)
{
	if (node == NULL)
		return NULL;

	node->ToLower(true);
	unsigned int name_hash = node->GetName().getHash();

	// Check to see if we support the class
	for (_Iterator it2 = begin(); it2 != end(); ++it2)
	{
		if ((*it2)->getClassNameHash() == name_hash)
		{
			if (g_CzShowTracing > 1)
			{
				CzString out;
				out.allocString(512);
				out = node->GetName().c_str();
				out += " - ";
				node->PrintAttributes(out);
				CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "XOML - ", out.c_str());
			}

			// We do support the class so call the classes creator to instantiate it
			IzXomlResource* instance = (*it2)->CreateInstance(parent);
			if (instance != NULL)
			{
				// Load up from XOML
				int success = instance->LoadFromXoml(parent, true, node);
				if (success > 0)
				{
					instance->setContainer(parent);
					instance->PostLoadFromXoml(parent, node);
				}
				else
				{
					delete instance;
					if (success == 0)
						CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "XOML - Near XOML tag ", node->GetName().c_str());
					return NULL;
				}
			}
			return instance;
		}
	}

	return NULL;
}

/**
 @fn	void CzXoml::ApplyStyle(CzStyle* style, CzXmlNode* node)

 @brief	Applies a style to the node.

 Applies a sets of style attributes to an XML node. Used by the styling system to modify XOML tags.

 @param [in,out]	style	If non-null, the style.
 @param [in,out]	node 	If non-null, the node.
 */

void CzXoml::ApplyStyle(CzStyle* style, CzXmlNode* node)
{
	// Add style properties to nodes
	for (CzStyle::_Iterator it = style->begin(); it != style->end(); ++it)
	{
		node->AddAttribute((*it)->Name, (*it)->Value, node->getParser());
	}
}

