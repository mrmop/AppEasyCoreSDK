/*
java implementation of the UserAgent extension.

Add android-specific functionality here.

These functions are called via JNI from native code.
*/
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
import com.ideaworks3d.marmalade.LoaderAPI;

class UserAgent
{
    public String getUserAgent()
    {
        return System.getProperty("http.agent");
    }
}