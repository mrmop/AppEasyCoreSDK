/*
Generic implementation of the UserAgent extension.
This file should perform any platform-indepedentent functionality
(e.g. error checking) before calling platform-dependent implementations.
*/

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#include "UserAgent_internal.h"
s3eResult UserAgentInit()
{
    //Add any generic initialisation code here
    return UserAgentInit_platform();
}

void UserAgentTerminate()
{
    //Add any generic termination code here
    UserAgentTerminate_platform();
}

const char* getUserAgent()
{
	return getUserAgent_platform();
}
