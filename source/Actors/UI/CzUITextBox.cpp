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

#include "CzUITextBox.h"
#include "CzHashes.h"

CzXomlClassDef* CzUITextBox::UITextBoxClassDef = NULL;

/**
 @fn	void CzUITextBox::InitClass()

 @brief	Initialises the class.
    
 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */

void CzUITextBox::InitClass()
{
	// Create class properties
	UITextBoxClassDef = new CzXomlClassDef();
	UITextBoxClassDef->addProperty(new CzXomlClassProperty("Prompt",		PT_String,		CzUITextBox::_setPrompt,		CzUITextBox::_getPrompt));
	UITextBoxClassDef->addProperty(new CzXomlClassProperty("Variable",		PT_String,		CzUITextBox::_setVariable,		CzUITextBox::_getVariable));
	UITextBoxClassDef->addProperty(new CzXomlClassProperty("TypeHint",		PT_String,		CzUITextBox::_setTypeHint,		CzUITextBox::_getTypeHint));
	UITextBoxClassDef->addProperty(new CzXomlClassProperty("OnTextChanged",	PT_String,		CzUITextBox::_setOnTextChanged,	NULL));
}
void CzUITextBox::ReleaseClass()
{
	if (UITextBoxClassDef != NULL)
	{
		UITextBoxClassDef->Release();
		delete UITextBoxClassDef;
		UITextBoxClassDef = NULL;
	};
}

// //
// //
// //
// //
// //
// //
// //
// //
// CzUITab XOML property setters and getters
// //
// //
// //
// //
// //
// //
// //
// //
bool CzUITextBox::_setPrompt(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUITextBox* actor = (CzUITextBox*)target;

	if (add)
	{
		CzString s = actor->getPrompt();
		s += (const char*)prop.p_data;
		actor->setPrompt(s.c_str());

	}
	else
		actor->setPrompt((const char*)prop.p_data);

	return true;
}

CzXomlProperty	CzUITextBox::_getPrompt(IzXomlResource* target)
{
	return CzXomlProperty(((CzUITextBox*)target)->getPrompt().c_str());
}

bool CzUITextBox::_setVariable(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUITextBox* actor = (CzUITextBox*)target;

	if (add)
	{
		CzString s = actor->getTargetVariable();
		s += (const char*)prop.p_data;
		actor->setTargetVariable(s.c_str());

	}
	else
		actor->setTargetVariable((const char*)prop.p_data);

	return true;
}

CzXomlProperty	CzUITextBox::_getVariable(IzXomlResource* target)
{
	return CzXomlProperty(((CzUITextBox*)target)->getTargetVariable().c_str());
}

bool CzUITextBox::_setTypeHint(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUITextBox* actor = (CzUITextBox*)target;

	CzInput::eInputTypeHint h;
	unsigned int type_hash = CzString::CalculateHash((const char*)prop.p_data);
	if (type_hash == CzString::CalculateHash("text"))
		h = CzInput::Hint_Text;
	else
	if (type_hash == CzString::CalculateHash("number"))
		h = CzInput::Hint_Number;
	else
	if (type_hash == CzString::CalculateHash("password"))
		h = CzInput::Hint_Password;
	else
	if (type_hash == CzString::CalculateHash("email"))
		h = CzInput::Hint_Email;
	else
	if (type_hash == CzString::CalculateHash("url"))
		h = CzInput::Hint_URL;
	else
		return false;

	return true;
}

CzXomlProperty	CzUITextBox::_getTypeHint(IzXomlResource* target)
{
	return CzXomlProperty(CzXoml::InputTypeHintNames[((CzUITextBox*)target)->getTypeHint()]);
}

bool CzUITextBox::_setOnTextChanged(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUITextBox* actor = (CzUITextBox*)target;

	actor->getEventsManager()->addEvent("OnTextChanged", (const char*)prop.p_data, true);

	return true;
}


//
//
//
//
//
// CzUITextBox implementation
//
//
//
//
//
CzUITextBox::CzUITextBox() : CzUILabel()
{
	setActualClassType("textbox");
	TypeHint = CzInput::Hint_Text;
}

void CzUITextBox::NotifyTapped(int index, int x, int y, bool allow_bubble)
{
	if (!Enabled)
		return;

	CzUILabel::NotifyTapped(index, x, y, allow_bubble);

	if (Prompt.isEmpty())
		Prompt = "Enter Text";

	const char* str = CZ_INPUT->showOnScreenKeyboard(Prompt.c_str(), TypeHint, Text.c_str());
	if (str != NULL)
	{
		setText(str);

		// Find and update target variable
		if (!TargetVariable.isEmpty())
		{
			CzXomlVariable* var = CzXomlVariable::GetVariable(TargetVariable, Scene);
			if (var != NULL)
				var->setValue(Text.c_str());
		}
		NotifyTextChanged();
	}

}

void CzUITextBox::NotifyTextChanged()
{
	ProcessEventActions(CzHashes::OnTextChanged_Hash);
}

int CzUITextBox::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	if (parent->getClassTypeHash() != CzHashes::Scene_Hash && parent->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Textbox - Needs to be declared inside a scene or an actor - ", DebugInfo.c_str());
		return 0;
	}

	// Load main actor attributes
	int ret = CzUILabel::LoadFromXoml(parent, false, node);
	if (ret <= 0)
		return ret;

	// Process text box specific attributes
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Variable_Hash)
			TargetVariable = (*it)->getValue();
		else
		if (name_hash == CzHashes::Prompt_Hash)
			Prompt = (*it)->getValue();
		else
		if (name_hash == CzHashes::TypeHint_Hash)
		{
			unsigned int type_hash = (*it)->getValue().getHash();
			if (type_hash == CzString::CalculateHash("number"))
				TypeHint = CzInput::Hint_Number;
			else
			if (type_hash == CzString::CalculateHash("password"))
				TypeHint = CzInput::Hint_Password;
			else
			if (type_hash == CzString::CalculateHash("email"))
				TypeHint = CzInput::Hint_Email;
			else
			if (type_hash == CzString::CalculateHash("url"))
				TypeHint = CzInput::Hint_URL;
			else
				TypeHint = CzInput::Hint_Text;
		}
		else
		if (name_hash == CzHashes::OnTextChanged_Hash)
			EventsManager->addEvent("OnTextChanged", (*it)->getValue().c_str());
	}

	if (load_children)
	{
		// Process inner tags
		if (!CZ_XOML->Process(this, node))
			return false;
	}

	return true;
}

/**
 @fn	bool CzUITextBox::getProperty(const char* property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the actor.

 Returns the named property of the actor. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types. Note that as 
 CzUITextBox is derived from CzUILabel, all CzUILabel properties will also be checked against.

 @param	property_name		Name of the property.
 @param [out]	prop		The property data that.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUITextBox::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	if (CzUILabel::getProperty(property_name, prop))
		return true;

	int err = IzXomlResource::getProperty(UITextBoxClassDef, this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UITextBoxClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Textbox - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUITextBox::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the UITextBox.
    
 Sets the named property of this UITextBox. The properties value supplied will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
bool CzUITextBox::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	if (CzUILabel::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(UITextBoxClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UITextBoxClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Textbox - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUITextBox::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the actor.

 Sets the named property of this actor. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced. Note that as CzUITextBox is derived from CzUILabel, all CzUILabel properties will also be checked against.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in]	data		The property data.
 @param	delta				true to property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUITextBox::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	if (CzUILabel::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(UITextBoxClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UITextBoxClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Textbox - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUITextBox::UpdateFromAnimation(CzAnimInstance *animation)

 @brief	Updates actor property from supplied animation.

 Updates the actors property that is specified by the supplied animation. When a timeline animation targets the property of an actor this method will be called by the animation to move the 
 animations frame data into its target property. Note that as CzUITextBox is derived from CzUILabel, all CzUILabel properties will also be checked against.

 @param [in]	animation	If non-null, the animation.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUITextBox::UpdateFromAnimation(CzAnimInstance *animation)
{
	if (CzUILabel::UpdateFromAnimation(animation))
		return true;

	int err = IzXomlResource::setProperty(UITextBoxClassDef, this, animation);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UITextBoxClassDef->findProperty(animation->getTargetPropertyHash());
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Textbox - UpdateFromAnimation - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUITextBox::UpdateBinding(unsigned int property_name, CzXomlVariable* var)

 @brief	Updates an actor property from the supplied XOML variable.

 Updates a single property from the value of the bound XOML variable. Note that as CzUITextBox is derived from CzUILabel, all CzUILabel properties will also be checked against.

 @param	property_name  	Name of the property.
 @param [in,out]	var	If non-null, the variable.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUITextBox::UpdateBinding(unsigned int property_name, CzXomlVariable* var)
{
	if (!var->Modified || var->getValue().isEmpty())
		return false;

	if (CzUILabel::UpdateBinding(property_name, var))
		return true;

	int err = IzXomlResource::setProperty(UITextBoxClassDef, this, property_name, var);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UITextBoxClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Textbox - UpdateBindings - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}




