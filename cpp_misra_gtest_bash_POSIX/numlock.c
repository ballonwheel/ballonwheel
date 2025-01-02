#include <X11/extensions/XTest.h>
#include <X11/keysym.h>

int main()
    {
    Display* disp = XOpenDisplay( NULL );
    if( disp == NULL )
	return 1;
    XTestFakeKeyEvent( disp, XKeysymToKeycode( disp, XK_Num_Lock ), True, CurrentTime );
    XTestFakeKeyEvent( disp, XKeysymToKeycode( disp, XK_Num_Lock ), False, CurrentTime );
    XCloseDisplay( disp );
    return 0;
    }



