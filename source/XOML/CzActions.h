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

#if !defined(_CZ_ACTIONS_H_)
#define _CZ_ACTIONS_H_

#include "CzUtil.h"
#include "CzXoml.h"
#include "CzXomlVariables.h"

class CzActions;

/**
 @addtogroup XOML
 @{
 */
 
/**
 @struct	CzAction

 @brief	An action is a command that can be executed from XOML.

 <h1>Introduction</h1>

 An action consists of a method and a number of parameters (eiter variabls or values) that can be passed to that method. Note that actions will cache any variables that are passed 
 as parameters osthey do not have to be constantly searched for in the resuorce manager.

 An action also carries a conditions variable which defines a conditions variable that can be used to conditionally execute the action.

 A game or app would be pretty useless if it did nothing other than just sit and look pretty. Users generally expect lots of interaction with their apps. XOML uses the events and 
 actions system to provide such interaction.

 From a XOML point of view an event is something that has happened in the app that some scene or actor needs to know about. For example, when the user taps a button to see help he 
 raises the tapped help button event. An action from XOML's point of view is something that is done in response to an event. Continuing on with the same example, an action that could 
 be carried out in response to the user pressing the help button is to load and show a help scene.

 Scenes and different types of actors all handle a variety of different events (more on this later). Likewise scenes and different actors can carry our different actions. If we 
 look back at our scene and actor property definitions we see many properties with names such as OnCreate, OnDestroy, OnTapped etc.. These properties represent event handlers. 
 When we define one of these properties for an object we tell the system that we want to respond to that event occurring by calling an actions list. 

 An actions list is a list of actions that should be carried out in response to an event occurring. Actions lists can be declared globally, local to a scene or even local to an actor.

 The BasicActions example shows an example how to use events with actions. Lets take a look at the XOML for this example to see how it works:

 @code
<!-- Create rotation animation (we will use this to animate angle of the label) -->
<Animation Name="AngleAnim" Duration="4" Type="float">
    <Frame Time="0" Value="0" />
    <Frame Time="4" Value="360" />
</Animation>
    
<!-- Create a timeline that can be used to animate an actor -->
<Timeline Name="Anim1" AutoPlay="false">
    <Animation Anim="AngleAnim" Target="Angle" />
</Timeline>

<!-- Create a scene -->
<Scene Name="Scene1" Current="true" >

    <!-- Create a label with an animation -->
    <Label Font="serif" Background="Button1Brush" BackgroundColour="80, 80, 255, 255" Size="100, 100" Text="Animation" OnBeginTouch="Spin" NotifyEnd(Touch="StopSpin" Timeline="Anim1">
        <Actions Name="Spin">
            <Action Method="PlayTimeline" />
        </Actions>
        <Actions Name="StopSpin">
            <Action Method="PauseTimeline" />
        </Actions>
    </Label>

</Scene>
 @endcode

 Firstly we create an animation and a timeline that will be used to spin our label. Next we create a label within a scene and assign action lists to the OnBeginTouch and NotifyEnd(Touch 
 events. Next, we define two actions list, one called Spin and another called StopSpin. The first action list calls an action called PlayTimeline which tells the actor to start playing 
 its current timeline. The second actions list calls an action called PauseTimeline which tells the actor to pause playback of its timeline. The OnBeginTouch event is raised when the 
 user starts to press the actor, the NotifyEnd(Touch event is raised when the user stops touching the actor.

 You use the Actions tag to declare a list of actions. Actions can be declared globally so that any scene or actor can call them, local to a scene so that the scene and any actors 
 contained with the scene can call them or local to an actor in which case only that actor can call them. Lets take a look at the properties that are supported by an Actions list:
 - Name (string) - Name of actions group
 - Local (boolean) - When set to false, this actions group will be added to the scenes actions manager instead of the actors actions manager
 - Condition (variable) - A condition variable that must evaluate to true for this action list to be executed (this is an optional feature and can be used to conditionally execute 
 actions lists based on the state of the app, such as the value of certain variables)
 - Tag (string) - Resource tag (used to group resources together)

 An actions list contains a list of action tags. An action tag has the following properties:
 - Method (method name) - A method is the actual action that takes places such as PlayTimeline, SetProperty etc..
 - Param1 to Param5 - Five parameters that can be passed to the action. with exception to parameters that are passed as optional container scenes. Note that the shorter P1 to P5 
 attributes can be substituted for Param1 to Param5. Variables can be passed to any parameter that is not expecting a variable name or optional container scene
 - Condition (variable) - A condition variable that must evaluate to true for this action to executed (this is an optional feature and can be used to conditionally execute actions based 
 on the state of the app, such as the value of certain variables)

 Method is a required property. In some cases Param1 to Param5 may also be required properties (see individual actions)

 Note that when passing a variable to an actions parameter the variable will be searched for in the container scene (or in many cases the optional container scene that was passed). 
 If it is not found there then the global variables space will be searched. If the variable is found then the variables value will be substituted as the parameter. If the variable 
 does not exist then XOML assumes that the variable is a value. 

 For clarity the container scene is the scene that contains the actor that called the action, if the action is called from an actor. If the action was called from a scene then that 
 scene will be the container scene. 

 XOML has an ever growing list of supported actions that can be called to carry out certain functions. Actions are grouped by purpose into the following groups:
 - CzXomlActions_Actor - Actor specific actions
 - CzXomlActions_Animation - Animation specific actions
 - CzXomlActions_Comms - Communications specific actions
 - CzXomlActions_Market - Market specific actions
 - CzXomlActions_Media - Media specific actions
 - CzXomlActions_Program - XOML Program Removal actions
 - CzXomlActions_Resource - Resource specific actions
 - CzXomlActions_Scene - Scene specific actions
 - CzXomlActions_Script - Script specific actions
 - CzXomlActions_Shared - Scene / actor shared actions
 - CzXomlActions_Sys - System specific actions
 - CzXomlActions_Variable - Variable specific actions

 <h1>Actions Scope<h1>
 The scope of an actions list depends on where it was declared. You can declare actions in the following places:
 - Inside actor tags - The actions list will be assigned to the actors actions manager and will be local to the actor and timelines that are local to the actor
 - Inside scene tags - The actions list will be assigned to the scenes actions manager and will be local to the scene and any actors / timelines within that scene
 - Inside timeline tags  - The actions list will be assigned to the actors actions manager if it was declared inside an actor or to the scenes if it was declared inside a scene. if 
 declared globally then the actions list will be assigned to the global actions manager and will be available to all scenes and their actors
 - Declared globally (outside a scene) - The actions list will be assigned to the global actions manager and will be available to all scenes and actors

 <h1>Creating Actions</h1>

 Below we show 3 different ways of creating actions and assigning them to an actor:

 @par XOML Example:
 @code
<!-- Create a label object -->
<Label Font="serif" Background="Button1Brush" BackgroundColour="255, 80, 80, 255" Text="Clipping" WrapPosition="true" OnTick="Update">
	<Actions Name="Update">
		<Action Method="AddProperty" Param1="Position" Param2="2, 2" />
	</Actions>
</Label>
 @endcode

 @par Code Example:
 @code
	// Create label
	CzFont* font = (CzFont*)CZ_GLOBAL_RESOURCE_MANAGER->findResource("serif", CzHashes::Font_Hash);
	IzBrush* background_brush = (IzBrush*)CZ_GLOBAL_RESOURCE_MANAGER->findResource("Button1Brush", CzHashes::Brush_Hash);
	CzIVec2 size(200, 70);
	CZ_NEW_LABEL(scene, label, "label1", CzString("Clipping"), background_brush, font, size, false);
	label->setBackgroundColour(CzColour(255, 80, 80, 255));
	label->setWrapPosition(true);

	// Create an actions list that modifies the Position property of the label
	CzActions* actions = new CzActions();
	actions->setName("Update");
	CzAction* action = new CzAction();
	action->setMethodName("AddProperty");
	actions->addAction(action);
	action->setParameter1("Position");
	action->setParameter2("2, 2");

	// Add actions list to the label actors actions manager
	label->getActionsManager()->addActions(actions);

	// Tie label OnTick event to Update actions list
	label->getEventsManager()->addEvent("OnTick", "Update", true);
	label->setTickEnabled(true);	// When OnTick is specified we need to inform actor that it has been added to list (optimisation)
 @endcode

 @par Code Example (using macros):
 @code
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
 @endcode

 As you can see, by far the easiest way to work with actions, actions lists and events is to use XOML.

 <h1>Creating Custom Actions</h1>

 Actions are a great way of passiong information between XOML and code, but the actions system wouldn't be as useful if it was limited to just the standard set of actions. Lets take 
 a look at how to create our own custom actions that can be called from XOML. 

 To create our own action we follow the following process:
 - Derive a new actions class from IzXomlAction
 - Set the action name in the constructor using setActionName() - This will be the name as it will be used in XOML.
 - Implement the Execute() method of IzXomlAction
 - Add an instance of the action to the actions manager

 Lets take a look at the first 3 stages:

 @code
class CzXomlActions_MyAction : public IzXomlAction
{
	CzXomlActions_MyAction()
	{
		// Set the action name (case insensitive )
		setActionName("myaction");
	}
public:
	void Execute(IzXomlResource* source, CzAction* action)
	{
		CzApp* app = NULL;
		CzScene* scene = NULL;
		CzActor* actor = NULL;

		// Determine the scene, app and possibly actor that called the action
		if (source->getClassTypeHash() == CzHashes::Scene_Hash)
		{
			scene = (CzScene*)source;
			app = scene->getParent();
		}
		else
		if (source->getClassTypeHash() == CzHashes::Actor_Hash)
		{
			actor = (CzActor*)source;
			scene = actor->getScene();
			app = scene->getParent();
		}

		// Get the container / parent that contains the action
		IzXomlResource* cont = (actor != NULL) ? (IzXomlResource*)actor : (IzXomlResource*)scene;

		// Do something with the action
		CzString& param1 = action->getParameter1(cont);
		CzString& param2 = action->getParameter2(cont);
		DoSomething(param1, param2);
	}
};
 @endcode
 
 Note that you do not need to determine the app, scene and actor of the caller if you do not need them all. We have provided them all as an example.
 
 Also note that in this example we call the actions parameter extraction methods CzAction::getParameter1() and CzAction::getParameter2() which will evaluate the parameters that were passed 
 to the action. The evaluation process will check the parameters for variables and if any are found then these variables values will be returned. You do not need to add this step if you 
 do not rely on XOML variables being passed as parameters to your actions. Below is an example of a much simplified action handler:

 @code
class CzXomlActions_MyAction : public IzXomlAction
{
	CzXomlActions_MyAction()
	{
		// Set the action name (case insensitive )
		setActionName("myaction");
	}
public:
	void Execute(IzXomlResource* source, CzAction* action)
	{
		// Do something with the action
		DoSomething(action->Params[0], action->Params[1]);
	}
};
 @endcode

 The last stage of adding your own custom action is to notify the XOML system that your action is available. To do this we call CZ_XOML->addAction():

 CZ_XOML->addAction(new CzXomlActions_MyAction());

 Note that CZ_XOML is a macro that accesses the global XOML singleton.

 We would place this call somewhere in our apps main boot up sequence so it gets called before any XOML parsing that contains this action begins. 

 Now lets take a look at a small XOML example that shows the use of our new action:

 @code
<Actions Name="AnActionsList">
    <Action Method="MyAction" Param1="Hello World!" Param2="Im an action!" />
</Actions>
 @endcode

 <h1>Conditional Actions</h1>

 A conditional action is an action that will be executed only if certain conditions are met. Lets take a look at a short example:
 
 @code
<Actions Name="UpdatePLayerScore">
	<Action Method="AddVar" Param1="PlayerScore" Param2="1" Condition="!PlayerScoreTooHigh" />
	<Action Method="SetVar" Param1="PlayerScore" Param2="0" Condition="PlayerScoreTooHigh" />
</Actions>
 @endcode

 In the above example the Condition attribute contains the PlayerScoreTooHigh variable. This variable is evaluated when the action is ran. if it returns true then the action will be executed. 
 Note that if the condition variable is prefixed with an exlamation mark then the result of the condition will be inverted. In this case the action will only be executed if the result of evaluating 
 the action is false. Conditions can also be applied to the actions list itself. Conditional actions offer a quick and easy way of applying simple logic to your apps directly in XOML without having 
 to resort to code.

 <h1>Some Examples</h1>
 
 Lets take a look at some useful example actions:

 @code
<Action Method="ShowActor" P1="RefundedMessage"/>		- Show an actor
<Action Method="HideActor" P1="RefundedMessage"/>		- Hide an actor
 @endcode

 @code
<Action Method="CallScript" Param1="Guess" />	- Calls a script method
 @endcode

 @code
<Action Method="Inline" Param1="
		local scene = scene.find('Scene1');
		local sound = resource.find('explosion', 'sound', scene);
		media.playSound(sound, true);			 - Executes Lua script inline
		" />
 @endcode

 @code
<Action Method="PlaySound" Param1="explosion" Param2="true" />	- Play a sound effect
 @endcode

 @code
<Action Method="Purchase" P1="coins1" />	- Purchase a product using in-app purchasing
 @endcode

 @code
<Action Method="SetProperty" P1="BackgroundColour" P2="128, 128, 255, 255" P3="Menu" /> - change the background colour of a menu
 @endcode

 @code
<Action Method="LoadFile" Param1="RoundFile" Param2="true" Param3="Round1.txt" Condition="IsRound0" /> - Load a file
 @endcode

 @code
<Action Method="SetVar" Param1="CardIndex" Param2="0" /> - Set the value of a variable
 @endcode

 @code
<Action Method="CallActions" Param1="NextRound" /> - Call anoter actions list
@endcode

 */

struct CzAction
{
	unsigned int			MethodName;		///< Action method type
	CzString				Params[5];		///< Action parameters
	CzXomlVariable*			Variables[5];	///< Cached variable parameters (prevents continually searching for variables)
	unsigned int			Conditions;		///< Name of conditions variable that defines a set of conditions that must be met for this action to be executed
	bool					Not;			///< if true then result of condition will be inversed
	CzActions*				Parent;			///< Parent actions container
#if defined(_DEBUG)
	CzString				_MethodName;
#endif	//_DEBUG
	CzString				_Conditions;
	CzAction() : MethodName(0), Conditions(0), Not(false), Parent(NULL) {}
	void					setMethodName(const char* name)
	{
#if defined(_DEBUG)
		_MethodName = name;
#endif	//_DEBUG
		MethodName = CZ_HASH(name);
	}
	unsigned int			getMethodName()	const					{ return MethodName; }
	void					setParameter1(CzString& p)				{ Params[0] = p; }
	void					setParameter1(const char* p)			{ Params[0] = p; }
	CzString&				getParameter1(IzXomlResource* parent = NULL);
	void					setParameter2(CzString& p)				{ Params[1] = p; }
	void					setParameter2(const char* p)			{ Params[1] = p; }
	CzString&				getParameter2(IzXomlResource* parent = NULL);
	void					setParameter3(CzString& p)				{ Params[2] = p; }
	void					setParameter3(const char* p)			{ Params[2] = p; }
	CzString&				getParameter3(IzXomlResource* parent = NULL);
	void					setParameter4(CzString& p)				{ Params[3] = p; }
	void					setParameter4(const char* p)			{ Params[3] = p; }
	CzString&				getParameter4(IzXomlResource* parent = NULL);
	void					setParameter5(CzString& p)				{ Params[4] = p; }
	void					setParameter5(const char* p)			{ Params[4] = p; }
	CzString&				getParameter5(IzXomlResource* parent = NULL);
	void					setConditions(const char* cond)
	{
		_Conditions = cond;
		Conditions = CZ_HASH(cond);
	}
	unsigned int			getConditions()	const	{ return Conditions; }


};

/**
 @class	CzActions

 @brief	A collection of actions (called an actions list in XOML).

 XOML allows you to collect together a collection of actions that are executed one after the other. Below is an example that shows how to create an actions list in XOML:

@code
<!-- Action that is called when a correct answer is given -->
<Actions Name="CorrectAnswer">
	<Action Method="AddVar" Param1="RoundScore" Param2="1" />
	<Action Method="HideActor" Param1="SelectedCardNames:0" />
	<Action Method="HideActor" Param1="SelectedCardNames:1" />
	<Action Method="HideActor" Param1="SelectedCardNames:2" />
	<Action Method="AddVar" Param1="CardsFoundCount" Param2="3" />
	<Action Method="SetProperty" Param1="Text" Param2="Correct, well done" Param3="AnswerNotification" />
	<Action Method="SetTimeline" Param1="AnswerNotificationAnim" Param2="AnswerNotification" />
</Actions>
@endcode

 In code to create an actions list is a simple case of instantiating a CzActions class then calliong addAction() to add actions to the actions list. When the actions list is built 
 you can then add it to either the global resources actions manager, a scenes actions manager or an actors actions manager depending on what scope you would like the actions list to have. 
 Its possible for example to add the actions list to a scene then apply it to multiple actors.

 */

class CzActions : public IzXomlResource
{
public:
	// Public access to actions iteration
	typedef CzList<CzAction*>::iterator _Iterator;
	_Iterator					begin() { return Actions.begin(); }
	_Iterator					end() { return Actions.end(); }
	// Properties
protected:
	CzList<CzAction*>			Actions;			// List of actions
	unsigned int				Conditions;			// Name as a string hash of the conditions variable that defines a set of conditions that must be met for this set of actions to be executed
	CzString					_Conditions;		// Name of the conditions variable that defines a set of conditions that must be met for this set of actions to be executed
	bool						Not;				// if true then result of condition will be inversed
public:
	void						addAction(CzAction* action);
	void						removeAction(CzAction* action, bool auto_delete = true);
	void						setConditions(const CzString& conditions)
	{
		Conditions = conditions.getHash();
		_Conditions = conditions;
	}
	unsigned int				getConditions() const							{ return Conditions; }
	void						setNot(bool _not)								{ Not = _not; }
	bool						isNot() const									{ return Not; }
	// Properties end

	CzActions() : IzXomlResource(), Conditions(0), Not(false) { setClassType("actions");	}
	virtual ~CzActions()
	{
		for (_Iterator it = begin(); it != end(); ++it)
			delete *it;
		Actions.clear();
	}

	void			Execute(IzXomlResource* source);

	// Implementation of IzXomlResource interface
	int				LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);
};

/**
 @class	CzActionsCreator

 @brief	CzActionsreator - Creates an instance of an actions object.

 CzActionsCreator enables actions lists to be instantiated from XOML.

 */

class CzActionsCreator : public IzXomlClassCreator
{
public:
	CzActionsCreator()
	{
		setClassName("actions");
	}
	IzXomlResource* CreateInstance(IzXomlResource* parent) { return new CzActions(); }
};

/**
 @class	CzActionsManager

 @brief	Manages a collection of actions lists.

 CzActionsManager manages a collection of actions lists and each actor and scene that is created is assigned its own actions manager. You can retrieve an actors actions manager by calling 
 CzActor::getActionsManager() and a scenes by calling CzScene::getActionsManager(). The global resource manager also has an actions manager of its own which is used to store global actions lists. 
 You can access this via CzGlobalResources::getActionsManager() by using the macro CZ_GLOBAL_ACTIONS_MANAGER.

 */

class CzActionsManager
{
public:
	// Public access to iteration
	typedef CzList<CzActions*>::iterator _Iterator;
	_Iterator				begin()		{ return Actions.begin(); }
	_Iterator				end()		{ return Actions.end(); }

protected:
	// Properties
	CzList<CzActions*>		Actions;			// A collection of actions
	IzXomlResource*			Parent;				// Parent container
public:
	void					addActions(CzActions* actions);
	void					removeActions(CzActions* Actions);
	void					removeActions(unsigned int name_hash);
	CzActions*				findActions(unsigned int name_hash);
	CzActions*				findActions(const char* name);
	void					clearActionss();
	void					setParent(IzXomlResource* scene)
	{
		Parent = scene;
	}
	IzXomlResource*			getParent()								{ return Parent; }
	// Properties end

public:
	CzActionsManager() : Parent(NULL) {}
	virtual ~CzActionsManager() { clearActionss(); }

	// Utility
	static CzActions*	FindActions(const char* name, IzXomlResource* container);
	static CzActions*	FindActions(unsigned int name_hash, IzXomlResource* container);
	static CzActions*	FindActions(unsigned int name_hash, CzScene* container, unsigned int actor_hash);
	static CzActions*	FindActions(const char* name, CzScene* container, const char* actor_name);
};

//
//
//
//
//  Helper macros
//
//
//
//

/**
 @def	CZ_NEW_SINGLE_ACTION(actions, action, name, method)

 @brief	A macro that creates an actions list containing a single action with no parameters.

 @param	actions	The actions list variable.
 @param	action 	The action variable.
 @param	name   	The actions list name.
 @param	method 	The method name.
 */

#define CZ_NEW_SINGLE_ACTION(actions, action, name, method)		\
	CzActions* actions = new CzActions();						\
	actions->setName(name);										\
	CzAction* action = new CzAction();							\
	action->setMethodName(method);								\
	actions->addAction(action);

/**
 @def	CZ_NEW_SINGLE_ACTION_P1(actions, action, name, method, p1)

 @brief	A macro that creates an actions list containing a single action with one parameter.

 @param	actions	The actions list variable.
 @param	action 	The action variable.
 @param	name   	The actions list name.
 @param	method 	The method name.
 @param	p1	   	The first parameter.
 */

#define CZ_NEW_SINGLE_ACTION_P1(actions, action, name, method, p1)	\
	CZ_NEW_SINGLE_ACTION(actions, action, name, method)				\
	action->setParameter1(p1);

/**
 @def	CZ_NEW_SINGLE_ACTION_P2(actions, action, name, method, p1, p2)

 @brief	A macro that creates an actions list containing a single action with two parameters.

 @param	actions	The actions list variable.
 @param	action 	The action variable.
 @param	name   	The actions list name.
 @param	method 	The method name.
 @param	p1	   	The first parameter.
 @param	p2	   	The second parameter.
 */

#define CZ_NEW_SINGLE_ACTION_P2(actions, action, name, method, p1, p2)	\
	CZ_NEW_SINGLE_ACTION(actions, action, name, method)					\
	action->setParameter1(p1);											\
	action->setParameter2(p2);

/**
 @def	CZ_NEW_SINGLE_ACTION_P3(actions, action, name, method, p1, p2, p3)
 
 @brief	A macro that creates an actions list containing a single action with three parameters.

 @param	actions	The actions list variable.
 @param	action 	The action variable.
 @param	name   	The actions list name.
 @param	method 	The method name.
 @param	p1	   	The first parameter.
 @param	p2	   	The second parameter.
 @param	p3	   	The third parameter.
 */

#define CZ_NEW_SINGLE_ACTION_P3(actions, action, name, method, p1, p2, p3)	\
	CZ_NEW_SINGLE_ACTION(actions, action, name, method)						\
	action->setParameter1(p1);												\
	action->setParameter2(p2);												\
	action->setParameter3(p3);

/**
 @def	CZ_NEW_SINGLE_ACTION_P4(actions, action, name, method, p1, p2, p3, p4)
 @brief	A macro that creates an actions list containing a single action with four parameters.

 @param	actions	The actions list variable.
 @param	action 	The action variable.
 @param	name   	The actions list name.
 @param	method 	The method name.
 @param	p1	   	The first parameter.
 @param	p2	   	The second parameter.
 @param	p3	   	The third parameter.
 @param	p4	   	The fourth parameter.
 */

#define CZ_NEW_SINGLE_ACTION_P4(actions, action, name, method, p1, p2, p3, p4)	\
	CZ_NEW_SINGLE_ACTION(actions, action, name, method)							\
	action->setParameter1(p1);													\
	action->setParameter2(p2);													\
	action->setParameter3(p3);													\
	action->setParameter4(p4);

/**
 @def	CZ_NEW_SINGLE_ACTION_P5(actions, action, name, method, p1, p2, p3, p4, p5)

 @brief	A macro that creates an actions list containing a single action with five parameters.

 @param	actions	The actions list variable.
 @param	action 	The action variable.
 @param	name   	The actions list name.
 @param	method 	The method name.
 @param	p1	   	The first parameter.
 @param	p2	   	The second parameter.
 @param	p3	   	The third parameter.
 @param	p4	   	The fourth parameter.
 @param	p5	   	The fifth parameter.
 */

#define CZ_NEW_SINGLE_ACTION_P5(actions, action, name, method, p1, p2, p3, p4, p5)	\
	CZ_NEW_SINGLE_ACTION(actions, action, name, method)								\
	action->setParameter1(p1);														\
	action->setParameter2(p2);														\
	action->setParameter3(p3);														\
	action->setParameter4(p4);														\
	action->setParameter5(p5);

/// @}

#endif // _CZ_ACTIONS_H_