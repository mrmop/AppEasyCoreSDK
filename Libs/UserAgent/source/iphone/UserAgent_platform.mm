/*
 * iphone-specific implementation of the UserAgent extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "UserAgent_internal.h"
#import <UIKit/UIKit.h>


s3eResult UserAgentInit_platform()
{
    // Add any platform-specific initialisation code here
    return S3E_RESULT_SUCCESS;
}

void UserAgentTerminate_platform()
{
    // Add any platform-specific termination code here
}

const char* getUserAgent_platform()
{
    UIWebView *webView = [[UIWebView alloc]initWithFrame:CGRectZero]; 
    NSString *uaString = [webView stringByEvaluatingJavaScriptFromString:@"navigator.userAgent"];
    
    return [uaString UTF8String];
}
