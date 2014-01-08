#if !defined(_GAME_H_)
#define _GAME_H_

#include "CzApp.h"

#define	APP	App::getInstance()

//
//
// App is a singleton class that acts as a controller for the whole app
// 
// 
//
class App : public CzApp
{
	CDEFINE_SINGLETON(App)

public:

protected:
	// Properties

public:
	// Properties end

protected:

public:
	void			Init(bool enable_http);
	void			Release();
	bool			Update();
	void			Draw();

	// Implementation of IIwGameXomlResource interface
	int				LoadFromXoml(bool load_children, CzXmlNode* node);

};



#endif // _GAME_H_
