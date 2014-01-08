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

#include <list>
#include "CzXomlVariables.h"
#include "CzFile.h"
#include "CzResources.h"
#include "CzHashes.h"
#include "CzActor.h"
#include "CzSlotArray.h"

// 
// 
//
//
// CzXomlVariable implementation
//
//
//
//
bool CzXomlVariable::checkCondition(eCzXomlConditionOperator condition, const CzString& val)
{
	switch (condition)
	{
	case CO_Equal:
		if (Value == val)
			return true;
		break;
	case CO_NotEqual:
		if (!(Value == val))
			return true;
		break;
	case CO_Greater:
		if (Value.getLength() > val.getLength())
			return true;
		break;
	case CO_GreaterEqual:
		if (Value.getLength() >= val.getLength())
			return true;
		break;
	case CO_Less:
		if (Value.getLength() < val.getLength())
			return true;
		break;
	case CO_LessEqual:
		if (Value.getLength() <= val.getLength())
			return true;
		break;
	case CO_And:
		Value.FindReset();
		if (Value.Find(val.c_str()) >= 0)
			return true;
		break;
	}

	return false;
}

CzXomlVariable* CzXomlVariable::GetVariable(const CzString& value, IzXomlResource* parent)
{
	if (value.getLength() < 2)
		return NULL;
	if (CzString::IsNumber(*value.c_str()))
		return NULL;

	CzXomlVariable* var = NULL;
	CzString variable_name;
	CzString index_name;
	int index = -1;
	if (((CzString&)value).SplitVarIndex(variable_name, index, index_name))
	{
		var = CzXomlVariableManager::FindVariable(variable_name.getHash(), parent);
		if (var != NULL)
		{
			if (var->isArray())
			{
				// If a variable name was provided as index then get its value
				if (index < 0 && !index_name.isEmpty())
				{
					CzXomlVariableInt* index_var = (CzXomlVariableInt*)CzXomlVariableManager::FindVariable(index_name.getHash(), parent);
					if (index_var != NULL)
						index = index_var->NativeValue;
				}

				if (index >= 0)
					var = ((CzXomlVariableArray*)var)->getElement(index);
			}
		}
	}

	return var;
}

bool CzXomlVariable::Save()
{
	if (Persist && Value.c_str() != NULL)
	{
		CzString name = Name;
		name += ".var";
		CzFile file;
		if (file.Open(name.c_str(), "wb"))
		{
			if (!file.Write((void *)Value.c_str(), Value.getLength() + 1))
				return false;
		}
		else
			return false;
	}

	return true;
}

bool CzXomlVariable::Load()
{
	if (Persist)
	{
		CzString name = Name;
		name += ".var";
		CzFile file;
		if (file.Open(name.c_str(), "rb"))
		{
			int len = file.getFileSize();
			if (len <= 0)
				return false;
			CzString s;
			s.allocString(len);
			if (!file.Read((void *)s.c_str(), len))
				return false;
			setValue(s.c_str());
		}
		else
			return false;
	}

	return true;
}

// 
// 
//
//
// CzXomlVariableBool implementation
//
//
//
//
bool CzXomlVariableBool::checkCondition(eCzXomlConditionOperator condition, const CzString& val)
{
	switch (condition)
	{
	case CO_Equal:
	case CO_And:
		if (Value == val)
			return true;
		break;
	case CO_NotEqual:
		if (!(Value == val))
			return true;
		break;
	}

	return false;
}

// 
// 
//
//
// CzXomlVariableFloat implementation
//
//
//
//
bool CzXomlVariableFloat::checkCondition(eCzXomlConditionOperator condition, const CzString& val)
{
	float value = val.getAsFloat();
	switch (condition)
	{
	case CO_Equal:
		if (NativeValue == value)
			return true;
		break;
	case CO_NotEqual:
		if (NativeValue != value)
			return true;
		break;
	case CO_Greater:
		if (NativeValue > value)
			return true;
		break;
	case CO_GreaterEqual:
		if (NativeValue >= value)
			return true;
		break;
	case CO_Less:
		if (NativeValue < value)
			return true;
		break;
	case CO_LessEqual:
		if (NativeValue <= value)
			return true;
		break;
	case CO_And:
		break;
	}

	return false;
}

// 
// 
//
//
// CzXomlVariableInt implementation
//
//
//
//
bool CzXomlVariableInt::checkCondition(eCzXomlConditionOperator condition, const CzString& val)
{
	int value = val.getAsInt();
	switch (condition)
	{
	case CO_Equal:
		if (NativeValue == value)
			return true;
		break;
	case CO_NotEqual:
		if (NativeValue != value)
			return true;
		break;
	case CO_Greater:
		if (NativeValue > value)
			return true;
		break;
	case CO_GreaterEqual:
		if (NativeValue >= value)
			return true;
		break;
	case CO_Less:
		if (NativeValue < value)
			return true;
		break;
	case CO_LessEqual:
		if (NativeValue <= value)
			return true;
		break;
	case CO_And:
		if ((NativeValue & value) == value)
			return true;
		break;
	}

	return false;
}

// 
// 
//
//
// CzXomlVariableVec2 implementation
//
//
//
//
bool CzXomlVariableVec2::checkCondition(eCzXomlConditionOperator condition, const CzString& val)
{
	float values[8];
	int count = ((CzString&)val).getAsListOfFloat(values);
	if (count != 2)
		return false;
	CzVec2 value = CzVec2(values[0], values[1]);
	switch (condition)
	{
	case CO_Equal:
		if (NativeValue == value)
			return true;
		break;
	case CO_NotEqual:
		if (NativeValue != value)
			return true;
		break;
	case CO_Greater:
		if (NativeValue.MagSquared() > value.MagSquared())
			return true;
		break;
	case CO_GreaterEqual:
		if (NativeValue.MagSquared() >= value.MagSquared())
			return true;
		break;
	case CO_Less:
		if (NativeValue.MagSquared() < value.MagSquared())
			return true;
		break;
	case CO_LessEqual:
		if (NativeValue.MagSquared() <= value.MagSquared())
			return true;
		break;
	}

	return false;
}

// 
// 
//
//
// CzXomlVariableVec3 implementation
//
//
//
//
bool CzXomlVariableVec3::checkCondition(eCzXomlConditionOperator condition, const CzString& val)
{
	float values[8];
	int count = ((CzString&)val).getAsListOfFloat(values);
	if (count != 3)
		return false;
	CzVec3 value = CzVec3(values[0], values[1], values[2]);
	switch (condition)
	{
	case CO_Equal:
		if (NativeValue == value)
			return true;
		break;
	case CO_NotEqual:
		if (NativeValue != value)
			return true;
		break;
	case CO_Greater:
		if (NativeValue.MagSquared() > value.MagSquared())
			return true;
		break;
	case CO_GreaterEqual:
		if (NativeValue.MagSquared() >= value.MagSquared())
			return true;
		break;
	case CO_Less:
		if (NativeValue.MagSquared() < value.MagSquared())
			return true;
		break;
	case CO_LessEqual:
		if (NativeValue.MagSquared() <= value.MagSquared())
			return true;
		break;
	}

	return false;
}

// 
// 
//
//
// CzXomlVariableVec4 implementation
//
//
//
//
bool CzXomlVariableVec4::checkCondition(eCzXomlConditionOperator condition, const CzString& val)
{
	float values[8];
	int count = ((CzString&)val).getAsListOfFloat(values);
	if (count != 4)
		return false;
	CzVec4 value = CzVec4(values[0], values[1], values[2], values[3]);
	switch (condition)
	{
	case CO_Equal:
		if (NativeValue == value)
			return true;
		break;
	case CO_NotEqual:
		if (NativeValue != value)
			return true;
		break;
	case CO_Greater:
		if (NativeValue.MagSquared() > value.MagSquared())
			return true;
		break;
	case CO_GreaterEqual:
		if (NativeValue.MagSquared() >= value.MagSquared())
			return true;
		break;
	case CO_Less:
		if (NativeValue.MagSquared() < value.MagSquared())
			return true;
		break;
	case CO_LessEqual:
		if (NativeValue.MagSquared() <= value.MagSquared())
			return true;
		break;
	}

	return false;
}

// 
// 
//
//
// CzXomlVariableCondition implementation
//
//
//
//
void CzXomlVariableCondition::setValue(const char* value, int count)
{
	CzXomlVariable::setValue(value, count);

	// Build condition list
	// Parse out string parameters
	CzList<CzSVec2> params;
	int pos, len;
	while (1)
	{
		pos = Value.getFindIndex();
		len = Value.GetNextString();
		if (len <= 0) break;
		Value.StepFindIndex(1);
		CzSVec2 v(pos, len);
		params.push_back(v);
	}

	Changed = true;

	IzXomlResource* parent = Parent->getParent();

	// Go through parameters to find out what they are
	CzString variable;
	CzString op;
	CzString val;
	CzString join_op;
	for (CzList<CzSVec2>::iterator it = params.begin(); it != params.end(); it++)
	{
		// Get variable name
		variable = Value.getSubString((*it).x, (*it).y); it++;
		if (it == params.end())
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Variable - Found but no operator or value");
			return;
		}

		// Get operator
		op = Value.getSubString((*it).x, (*it).y); it++;
		if (it == params.end())
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Variable - Operator found but no value");
			return;
		}

		// Determine operator
		eCzXomlConditionOperator type;
		op.ToLower();
		if (op == "==")	type = CO_Equal; else
		if (op == "!=")	type = CO_NotEqual; else
		if (op == "gt")	type = CO_Greater; else
		if (op == "lt")	type = CO_Less; else
		if (op == "gte") type = CO_GreaterEqual; else
		if (op == "lte") type = CO_LessEqual; else
		if (op == "and") type = CO_And; else
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Variable - Operator invalid - ", op.c_str());
			return;
		}

		// Get value
		val = Value.getSubString((*it).x, (*it).y); it++;

		// Create the condition
		CzXomlCondition* condition = new CzXomlCondition();
		condition->Value = val;
		condition->Operator = type;
		condition->Variable2 = CzXomlVariable::GetVariable(val, parent);

		// Find the variable
		condition->Variable = CzXomlVariable::GetVariable(variable, parent);
		if (condition->Variable == NULL)
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Variable - Condition variable could not be found ", variable.c_str());
			return;
		}

		// Add condition to the conditions list
		Conditions.push_back(condition);

		// If no more data left then exit
		if (it == params.end())
			break;

		// Get join operator if its present
		join_op = Value.getSubString((*it).x, (*it).y);
		join_op.ToLower();
		if (join_op == "and")
			condition->Join = CJO_And;
		else
		if (join_op == "or")
			condition->Join = CJO_Or;
		else
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Variable - Joining operator invalid", join_op.c_str());
			break;
		}

		if (it == params.end())
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Variable - Joining operator is present but no additional conditions available");
			break;
		}
	}
}

bool CzXomlVariableCondition::isTrue()
{
	bool final_result = false;

	// Check the condition variables
	CzXomlCondition* prev = NULL;
	for (CzList<CzXomlCondition*>::iterator it = Conditions.begin(); it != Conditions.end(); ++it)
	{
		// Find the variable
		CzXomlVariable* var = (*it)->Variable;
		CzXomlVariable* var2 = (*it)->Variable2;
		if (var != NULL)
		{
			bool t1 = false;

			if (var->Type == VT_Condition)	// If condition var then evaluate it
				t1 = var->isTrue();

			bool result = false;
			
			if (var2 != NULL)
			{
				if (var2->Type == VT_Condition)	// If condition var then evaluate it
				{
					bool t2 = var2->isTrue();
					if (var->Type == VT_Condition)
					{
						if ((*it)->Operator == CO_NotEqual)
							result = t1 != t2;
						else
							result = t1 == t2;
					}
					else
						result = var->checkCondition((*it)->Operator, t2 ? "1" : "0");
				}
				else
					result = var->checkCondition((*it)->Operator, var2->getValue());
			}
			else
			{
				if (var->Type == VT_Condition)
				{
					bool val = (*it)->Value.getAsBool();
					if ((*it)->Operator == CO_NotEqual)
						result = t1 != val;
					else
						result = t1 == val;
				}
				else
					result = var->checkCondition((*it)->Operator, (*it)->Value);
			}

			// If joined by AND operator then rest of conditions are irrelevant
			if (!result && (*it)->Join == CJO_And)
			{
				NativeValue = false;
				return false;
			}

			if (prev == NULL)
				final_result = result;
			else
			{
				if (prev->Join == CJO_And)
					final_result = final_result && result;
				else
				if (prev->Join == CJO_Or)
					final_result = final_result || result;
			}
		}
		else
		{
#if defined(_DEBUG)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Variable - Could not be fuond - ", var->Name.c_str());
#endif	// _DEBUG
		}
		prev = *it;
	}
	NativeValue = final_result;

	return NativeValue;
}

// 
// 
//
//
// CzXomlVariableArray implementation
//
//
//
//
void CzXomlVariableArray::setValue(const char* value, int count)
{
	CzString values;

	if (count < 0)
		values = value;
	else
		values.setString(value, count);
	Changed = true;

	if (values.Contains(',') >= 0)
	{
		// Check to see if we have enough space, if not then enlarge the array to fit
		int occurs = values.Occurrences(',') + 1;
		int diff = occurs - Values.getSize();
		if (diff > 0)
		{
			Values.grow(diff);
			for (int t = 0; t < diff; t++)
			{
				CzXomlVariable* var2 = NULL;
				switch (Type)
				{
				case VT_ArrayString:
					var2 = new CzXomlVariable();
					break;
				case VT_ArrayBool:
					var2 = new CzXomlVariableBool();
					break;
				case VT_ArrayFloat:
					var2 = new CzXomlVariableFloat();
					break;
				case VT_ArrayInt:
					var2 = new CzXomlVariableInt();
					break;
				}

				// Append the variable
				addElement(var2);
			}
		}

		for (int t = 0; t < Values.getSize(); t++)
		{
			int offset = values.getFindIndex();
			int end = values.FindNext(',');
			values.StepFindIndex(1);
			if (end > 0)
				Values.element_at(t)->setValue(values.c_str() + offset, end - offset);
			else
			if (values.getFindIndex() >= values.getLength())
			{
				Values.element_at(t)->setValue(values.c_str() + offset, values.getLength() - offset);
				break;
			}
			if (values.StepFindIndexNoneWhiteSpace() < 0)
				break;
		}
	}
	else
	{
		for (int t = 0; t < Values.getSize(); t++)
			Values.element_at(t)->setValue(value);
	}
}

bool CzXomlVariableArray::checkCondition(eCzXomlConditionOperator condition, const CzString& val)
{
	switch (condition)
	{
	case CO_Equal:			// Check to see if array contains the supplied value
		{
			for (int t = 0; t < Values.getSize(); t++)
			{
				if (Values.element_at(t) != NULL)
				{
					if (Values.element_at(t)->getValue() == val)
						return true;
				}
			}
		}
		break;
	case CO_NotEqual:
		{
			for (int t = 0; t < Values.getSize(); t++)
			{
				if (Values.element_at(t) != NULL)
				{
					if (Values.element_at(t)->getValue() == val)
						return false;
				}
			}
			return true;
		}
		break;
	case CO_And:
		{
			for (int t = 0; t < Values.getSize(); t++)
			{
				CzXomlVariable* var = Values.element_at(t);
				if (var != NULL)
				{
					CzString& value = var->getValue();
					value.FindReset();
					if (value.Find(val.c_str()) >= 0)
						return true;
				}
			}
		}
		break;
	}

	return false;
}

void CzXomlVariableArray::BindXML(const char* binding)
{
	CzString s = binding;
	CzSlotArray<CzString*>* split = s.Split(':');
	if (split->element_at(0) != NULL)
	{
		IzXomlResource* parent = Parent->getParent();
		CzXomlVariable* var = CzXomlVariable::GetVariable(*split->element_at(0), parent);
		if (var == NULL)
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Variable - XML variable could not be found ", split->element_at(0)->c_str());
			split->clear(true);
			delete split;
			return;
		}
		if (!var->isXML())
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Variable - Is not an XML variable ", split->element_at(0)->c_str());
			split->clear(true);
			delete split;
			return;
		}
		XmlVar = (CzXomlVariableXML*)var;

		if (split->element_at(1) != NULL)
			setXmlTagName(split->element_at(1)->c_str());
		else
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Variable - BindXML tag name is missing");
			split->clear(true);
			delete split;
			XmlVar = NULL;
			return;
		}
		if (split->element_at(2) != NULL)
			setXmlAttribName(split->element_at(2)->c_str());
		else
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Variable - BindXML attribute name is missing");
			split->clear(true);
			delete split;
			XmlVar = NULL;
			return;
		}
	}

	split->clear(true);
	delete split;
}

void CzXomlVariableArray::UpdateFromXML()
{
	if (XmlVar != NULL && XmlVar->Modified && XmlVar->getRoot() != NULL)
	{
		Values.clear(true);
		CzXmlNodeList*	pNodes = new CzXmlNodeList();

		XmlVar->getRoot()->getNamedNodes(XmlTagNameHash, pNodes);

		for (CzXmlNodeList::iterator it = pNodes->begin(); it != pNodes->end(); ++it)
		{
			CzXmlAttribute* attrib = (*it)->getAttribute(XmlAttribNameHash);
			if (attrib != NULL)
			{
				CzXomlVariable* var = new CzXomlVariable();
				var->setValue(attrib->getValue().c_str());
				Values.add(var);
			}
		}
		Values.resize(Values.count());
		delete pNodes;
	}
}

void CzXomlVariableArray::Update()
{
	CzXomlVariable::Update();
	UpdateFromXML();
	for (int t = 0; t < Values.getSize(); t++)
	{
		CzXomlVariable* var = Values.element_at(t);
		if (var != NULL)
			var->Update();
	}
}


bool CzXomlVariableArray::Save()
{
	if (Persist)
	{
		CzString name = Name;
		name += ".var";
		CzFile file;
		if (file.Open(name.c_str(), "wb"))
		{
			for (int t = 0; t < Values.getSize(); t++)
			{
				CzXomlVariable* var = Values.element_at(t);
				int len = var->getValue().getLength() + 1;
				if (!file.Write((void *)&len, 4))
					return false;

				if (len > 0)
				{
					if (!file.Write((void *)var->getValue().c_str(), len))
						return false;
				}
				
			}
		}
		else
			return false;
	}

	return true;
}

bool CzXomlVariableArray::Load()
{
	if (Persist)
	{
		CzString name = Name;
		name += ".var";
		CzFile file;
		CzString s;
		if (file.Open(name.c_str(), "rb"))
		{
			int len = file.getFileSize();
			if (len <= 0)
				return false;

			for (int t = 0; t < Values.getSize(); t++)
			{
				CzXomlVariable* var = Values.element_at(t);
				int len = 0;
				if (!file.Read(&len, 4))
					break;
				if (len > 0)
				{
					s.allocString(len);
					if (!file.Read((void*)s.c_str(), len))
						break;
					var->setValue(s.c_str());
				}
			}
		}
		else
			return false;
	}

	return true;
}

// 
// 
//
//
// CzXomlVariableArrayBool implementation
//
//
//
//
bool CzXomlVariableArrayBool::checkCondition(eCzXomlConditionOperator condition, const CzString& val)
{
	bool res = val.getAsBool();
	switch (condition)
	{
	case CO_Equal:			// Check to see if array contains the supplied value
		{
			for (int t = 0; t < Values.getSize(); t++)
			{
				if (Values.element_at(t) != NULL)
				{
					if (((CzXomlVariableBool*)Values.element_at(t))->NativeValue != res)
						return false;
				}
			}
			return true;
		}
		break;
	case CO_NotEqual:
		{
			for (int t = 0; t < Values.getSize(); t++)
			{
				if (Values.element_at(t) != NULL)
				{
					if (((CzXomlVariableBool*)Values.element_at(t))->NativeValue == res)
						return false;
				}
			}
			return true;
		}
		break;
	case CO_And:
		{
			for (int t = 0; t < Values.getSize(); t++)
			{
				if (Values.element_at(t) != NULL)
				{
					if (((CzXomlVariableBool*)Values.element_at(t))->NativeValue == res)
						return true;
				}
			}
		}
		break;
	}

	return false;
}

// 
// 
//
//
// CzXomlVariableArrayInt implementation
//
//
//
//
bool CzXomlVariableArrayInt::checkCondition(eCzXomlConditionOperator condition, const CzString& val)
{
	int res = val.getAsInt();
	switch (condition)
	{
	case CO_Equal:			// Check to see if array contains the supplied value
		{
			for (int t = 0; t < Values.getSize(); t++)
			{
				if (Values.element_at(t) != NULL)
				{
					if (((CzXomlVariableInt*)Values.element_at(t))->NativeValue == res)
						return true;
				}
			}
		}
		break;
	case CO_NotEqual:
		{
			for (int t = 0; t < Values.getSize(); t++)
			{
				if (Values.element_at(t) != NULL)
				{
					if (((CzXomlVariableInt*)Values.element_at(t))->NativeValue == res)
						return false;
				}
			}
			return true;
		}
		break;
	case CO_Greater:
		{
			for (int t = 0; t < Values.getSize(); t++)
			{
				if (Values.element_at(t) != NULL)
				{
					if (((CzXomlVariableInt*)Values.element_at(t))->NativeValue > res)
						return true;
				}
			}
		}
		break;
	case CO_GreaterEqual:
		{
			for (int t = 0; t < Values.getSize(); t++)
			{
				if (Values.element_at(t) != NULL)
				{
					if (((CzXomlVariableInt*)Values.element_at(t))->NativeValue >= res)
						return true;
				}
			}
		}
		break;
	case CO_Less:
		{
			for (int t = 0; t < Values.getSize(); t++)
			{
				if (Values.element_at(t) != NULL)
				{
					if (((CzXomlVariableInt*)Values.element_at(t))->NativeValue < res)
						return true;
				}
			}
		}
		break;
	case CO_LessEqual:
		{
			for (int t = 0; t < Values.getSize(); t++)
			{
				if (Values.element_at(t) != NULL)
				{
					if (((CzXomlVariableInt*)Values.element_at(t))->NativeValue <= res)
						return true;
				}
			}
		}
		break;
	case CO_And:
		{
			for (int t = 0; t < Values.getSize(); t++)
			{
				if (Values.element_at(t) != NULL)
				{
					if ((((CzXomlVariableInt*)Values.element_at(t))->NativeValue & res) == res)
						return true;
				}
			}
		}
		break;
	}

	return false;
}

// 
// 
//
//
// CzXomlVariableArrayFloat implementation
//
//
//
//
bool CzXomlVariableArrayFloat::checkCondition(eCzXomlConditionOperator condition, const CzString& val)
{
	float res = val.getAsFloat();
	switch (condition)
	{
	case CO_Equal:			// Check to see if array contains the supplied value
		{
			for (int t = 0; t < Values.getSize(); t++)
			{
				if (Values.element_at(t) != NULL)
				{
					if (((CzXomlVariableFloat*)Values.element_at(t))->NativeValue == res)
						return true;
				}
			}
		}
		break;
	case CO_NotEqual:
		{
			for (int t = 0; t < Values.getSize(); t++)
			{
				if (Values.element_at(t) != NULL)
				{
					if (((CzXomlVariableFloat*)Values.element_at(t))->NativeValue == res)
						return false;
				}
			}
			return true;
		}
		break;
	case CO_Greater:
		{
			for (int t = 0; t < Values.getSize(); t++)
			{
				if (Values.element_at(t) != NULL)
				{
					if (((CzXomlVariableFloat*)Values.element_at(t))->NativeValue > res)
						return true;
				}
			}
		}
		break;
	case CO_GreaterEqual:
		{
			for (int t = 0; t < Values.getSize(); t++)
			{
				if (Values.element_at(t) != NULL)
				{
					if (((CzXomlVariableFloat*)Values.element_at(t))->NativeValue >= res)
						return true;
				}
			}
		}
		break;
	case CO_Less:
		{
			for (int t = 0; t < Values.getSize(); t++)
			{
				if (Values.element_at(t) != NULL)
				{
					if (((CzXomlVariableFloat*)Values.element_at(t))->NativeValue < res)
						return true;
				}
			}
		}
		break;
	case CO_LessEqual:
		{
			for (int t = 0; t < Values.getSize(); t++)
			{
				if (Values.element_at(t) != NULL)
				{
					if (((CzXomlVariableFloat*)Values.element_at(t))->NativeValue <= res)
						return true;
				}
			}
		}
		break;
	}

	return false;
}

// 
// 
//
//
// CzXomlVariableXML implementation
//
//
//
//
void CzXomlVariableXML::setValue(const char* value, int count)
{
	CzXomlVariable::setValue(value, count);
	Changed = true;
	if (value == NULL)
		return;

	SAFE_DELETE(Parser);
	Parser = new CzXmlParser();

	// Load and parse the xml file then create a copy of the tree (we need a copy because the pooled xml tags may get re-used)
	CzDataInput* data = new CzDataInput(true);
	data->setData((char*)Value.c_str(), Value.getLength());
	if (Parser->Parse(data) != XmlErrorNone)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Variable - Error parsing XML file for variable - ", Name.c_str());
	else
		Root = Parser->getRoot();
}

// 
// 
//
//
// CzXomlVariableManager implementation
//
//
//
//

void CzXomlVariableManager::addVariable(CzXomlVariable* variable)
{
#if defined(CZ_REMOVE_DUPLICATES)
	if (findVariable(variable->Name.getHash()))
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "VariableManager - Already exists, variable was replaced - ", variable->Name.c_str());
		removeVariable(variable->Name.getHash());
	}
#endif
	Variables.push_back(variable);
	variable->Parent = this;
}

CzXomlVariable* CzXomlVariableManager::addVariable(const char* name, eCzXomlVariableType type)
{
#if defined(CZ_REMOVE_DUPLICATES)
	CzXomlVariable* var = findVariable(CzString::CalculateHash(name));
	if (var != NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "VariableManager - Already exists, variable was replaced - ", name);
		removeVariable(var);
	}
#endif
	CzXomlVariable* v = CreateVariable(name, type);
	Variables.push_back(v);
	v->Parent = this;

	return v;
}

CzXomlVariable* CzXomlVariableManager::addVariable(const char* name, eCzXomlVariableType type, const char* value, int size)
{
#if defined(CZ_REMOVE_DUPLICATES)
	CzXomlVariable* var = findVariable(CzString::CalculateHash(name));
	if (var != NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "VariableManager - Already exists, variable was replaced - ", name);
		removeVariable(var);
	}
#endif
	CzXomlVariable* v = CreateVariable(name, type);
	if (v->isArray())
	{
		if (size == 0)
			size = 1;
		((CzXomlVariableArray*)v)->Init(size);
	}
	v->setValue(value);
	Variables.push_back(v);
	v->Parent = this;

	return v;
}

void CzXomlVariableManager::removeVariable(CzXomlVariable* var)
{
	removeVariable(var->Name.getHash());
}

void CzXomlVariableManager::removeVariable(unsigned int name_hash)
{
	// Renove named Actions from the manager
	for (_Iterator it = Variables.begin(); it != Variables.end(); ++it)
	{
		if (name_hash == (*it)->Name.getHash())
		{
			delete *it;
			Variables.erase(it);
			break;
		}
	}
}

void CzXomlVariableManager::setVariable(unsigned int name_hash, const char* value)
{
	CzXomlVariable* var = findVariable(name_hash);
	if (var != NULL)
	{
		var->setValue(value);
	}
}

void CzXomlVariableManager::setVariable(const char* name, const char* value)
{
	setVariable(CzString::CalculateHash(name), value);
}

CzXomlVariable* CzXomlVariableManager::findVariable(unsigned int name_hash)
{
	for (_Iterator it = Variables.begin(); it != Variables.end(); ++it)
	{
		if ((*it)->Name.getHash() == name_hash)
		{
			return *it;
		}
	}

	return NULL;
}

CzXomlVariable* CzXomlVariableManager::findVariable(const char* name)
{
	return findVariable(CzString::CalculateHash(name));
}

void CzXomlVariableManager::clearVariables()
{
	// Remove and delete all images from the manager
	for (_Iterator it = Variables.begin(); it != Variables.end(); ++it)
	{
		delete *it;
	}
	Variables.clear();
}

CzXomlVariable* CzXomlVariableManager::CreateVariable(const char* name, eCzXomlVariableType type)
{
	// Create variable of specified type
	switch (type)
	{
		case VT_String: 
			{
				CzXomlVariable* var = new CzXomlVariable();
				var->Parent = this;
				var->Name = name;
				var->Type = type;

				return var;
			}
			break;
		case VT_Bool: 
			{
				CzXomlVariableBool* var = new CzXomlVariableBool();
				var->Parent = this;
				var->Name = name;
				var->Type = type;

				return var;
			}
			break;
		case VT_Float: 
			{
				CzXomlVariableFloat* var = new CzXomlVariableFloat();
				var->Parent = this;
				var->Name = name;
				var->Type = type;

				return var;
			}
			break;
		case VT_Int: 
			{
				CzXomlVariableInt* var = new CzXomlVariableInt();
				var->Parent = this;
				var->Name = name;
				var->Type = type;

				return var;
			}
			break;
		case VT_Vec2: 
			{
				CzXomlVariableVec2* var = new CzXomlVariableVec2();
				var->Parent = this;
				var->Name = name;
				var->Type = type;

				return var;
			}
			break;
		case VT_Vec3: 
			{
				CzXomlVariableVec3* var = new CzXomlVariableVec3();
				var->Parent = this;
				var->Name = name;
				var->Type = type;

				return var;
			}
			break;
		case VT_Vec4: 
			{
				CzXomlVariableVec4* var = new CzXomlVariableVec4();
				var->Parent = this;
				var->Name = name;
				var->Type = type;

				return var;
			}
			break;
		case VT_Condition: 
			{
				CzXomlVariableCondition* var = new CzXomlVariableCondition();
				var->Parent = this;
				var->Name = name;
				var->Type = type;

				return var;
			}
			break;
		case VT_ArrayString: 
			{
				CzXomlVariableArray* var = new CzXomlVariableArray();
				var->Parent = this;
				var->Name = name;
				var->Type = type;

				return var;
			}
			break;
		case VT_ArrayBool: 
			{
				CzXomlVariableArrayBool* var = new CzXomlVariableArrayBool();
				var->Parent = this;
				var->Name = name;
				var->Type = type;

				return var;
			}
			break;
		case VT_ArrayFloat: 
			{
				CzXomlVariableArrayFloat* var = new CzXomlVariableArrayFloat();
				var->Parent = this;
				var->Name = name;
				var->Type = type;

				return var;
			}
			break;
		case VT_ArrayInt: 
			{
				CzXomlVariableArrayInt* var = new CzXomlVariableArrayInt();
				var->Parent = this;
				var->Name = name;
				var->Type = type;

				return var;
			}
			break;
		case VT_XML: 
			{
				CzXomlVariableXML* var = new CzXomlVariableXML();
				var->Parent = this;
				var->Name = name;
				var->Type = type;

				return var;
			}
			break;
	}

	return NULL;
}

CzXomlVariable* CzXomlVariableManager::FindVariable(const char* name, IzXomlResource* container)
{
	return FindVariable(CzString::CalculateHash(name), container);
}

CzXomlVariable* CzXomlVariableManager::FindVariable(unsigned int name_hash, IzXomlResource* container)
{
	CzXomlVariable* var = NULL;
	
	// If container class is an actor then check the actors scenes variables manager
	if (container != NULL && container->getClassTypeHash() == CzHashes::Actor_Hash)
		container = ((CzActor*)container)->getScene();
	// If container class is a scene then check the scenes variables manager
	if (container != NULL && container->getClassTypeHash() == CzHashes::Scene_Hash)
		var = (CzXomlVariable*)((CzScene*)container)->getVariableManager()->findVariable((name_hash));

	// If the variable has no container or the variable was not found in the supplied container then check the global variables manager
	if (var == NULL)
		var = (CzXomlVariable*)CZ_GLOBAL_RESOURCES->getVariableManager()->findVariable((name_hash));

	return var;
}

//
// 
// 
//
// CzXomlAddVariable Implementation
// 
// 
// 
//
int CzXomlAddVariable::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	CzString name, value, type;
	int size = 0;
	CzString* condition = NULL;
	CzString* bind_xml = NULL;
	bool persist = false;
	bool instant = false;

	// Process variable specific attributes
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
			name = (*it)->getValue();
		else
		if (name_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Value_Hash)
			value = (*it)->getValue();
		else
		if (name_hash == CzHashes::Type_Hash)
			type = (*it)->getValue();
		else
		if (name_hash == CzHashes::Size_Hash)
			size = (*it)->getValueAsInt();
		else
		if (name_hash == CzHashes::Condition_Hash)
			condition = &(*it)->getValue();
		else
		if (name_hash == CzHashes::BindXML_Hash)
			bind_xml = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Persist_Hash)
			persist = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Instant_Hash)
			instant = (*it)->getValueAsBool();
	}

	if (condition != NULL)
	{
		// Find the condition variable
		CzScene* scene = NULL;
		if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
			scene = (CzScene*)parent;
		bool condition_not = false;
		CzXomlVariable* var = NULL;
		if (*(condition->c_str()) == '!')
		{
			condition_not = true;
			CzString cond = condition->c_str() + 1;
			var = CzXomlVariable::GetVariable(cond, scene);
		}
		else
			var = CzXomlVariable::GetVariable(*condition, scene);
		if (var != NULL)
		{
			bool res = var->isTrue();
			if (condition_not)
				res = !res;
			if (!res)
				return -1;
		}
#if defined (_DEBUG)
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "AddVariable - Condition variable not found - ", condition->c_str(), DebugInfo.c_str());
#endif // _DEBUG
	}

	if (!name.isEmpty())
	{
		// Determine type of variable
		eCzXomlVariableType vtype = VT_String;
		if (!type.isEmpty())
		{
			unsigned int type_hash = type.getHash();
			if (type_hash == CzHashes::string_Hash)
				vtype = VT_String;
			else
			if (type_hash == CzHashes::bool_Hash)
				vtype = VT_Bool;
			else
			if (type_hash == CzHashes::float_Hash)
				vtype = VT_Float;
			else
			if (type_hash == CzHashes::int_Hash)
				vtype = VT_Int;
			else
			if (type_hash == CzHashes::vec2_Hash)
				vtype = VT_Vec2;
			else
			if (type_hash == CzHashes::vec3_Hash)
				vtype = VT_Vec3;
			else
			if (type_hash == CzHashes::vec4_Hash)
				vtype = VT_Vec4;
			else
			if (type_hash == CzHashes::condition_Hash)
				vtype = VT_Condition;
			else
			if (type_hash == CzHashes::arraystring_Hash)
				vtype = VT_ArrayString;
			else
			if (type_hash == CzHashes::arraybool_Hash)
				vtype = VT_ArrayBool;
			else
			if (type_hash == CzHashes::arrayfloat_Hash)
				vtype = VT_ArrayFloat;
			else
			if (type_hash == CzHashes::arrayint_Hash)
				vtype = VT_ArrayInt;
			else
			if (type_hash == CzHashes::xml_Hash)
				vtype = VT_XML;
			else
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "AddVariable - Invalid variable type", DebugInfo.c_str());
				return 0;
			}
		}

		// If we are declared inside a scene then variable is local to the scene else global
		CzXomlVariable* var = NULL;
		if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
			var = ((CzScene*)parent)->getVariableManager()->addVariable(name.c_str(), vtype, value.c_str(), size);
		else
			var = CZ_GLOBAL_RESOURCES->getVariableManager()->addVariable(name.c_str(), vtype, value.c_str(), size);

		if (var != NULL)
		{
			if (bind_xml != NULL && var->isArray())
				((CzXomlVariableArray*)var)->BindXML(bind_xml->c_str());

			if (instant)
				persist = true;

			if (persist)
			{
				var->setPersistent(persist);
				var->setInstantSave(instant);
				var->Load();
			}
		}
	}
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "AddVariable - Variable must be named", DebugInfo.c_str());
		return 0;
	}

	return -1;
}

