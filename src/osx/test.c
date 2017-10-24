#include <Carbon/Carbon.h>

/*
int main()
{
	    //CGEventFlags flags = CGEventSourceFlagsState(1);

	while(true)
	{
		CGEventSourceStateID eventSource = kCGEventSourceStateCombinedSessionState;
		bool b = CGEventSourceKeyState(eventSource, kVK_Command) && CGEventSourceKeyState(eventSource, kVK_ANSI_Period);
		if(b) {
			cout << "Yes" << endl;
		}
		else
		{
			cout << "No" << endl;
		}
	}

	cout << "Hello world!" << endl;
	return 0;
}
*/

int main(int ArgC, char **ArgV)
{
    Rect rPos;
    WindowRef Handle;
    EventTypeSpec etsEventTypes[2];
     
    rPos.top = 200;
    rPos.left = 300;
    rPos.bottom = 400;
    rPos.right = 500;
 
    //Create the window
    CreateNewWindow(kDocumentWindowClass, kWindowStandardDocumentAttributes | kWindowStandardHandlerAttribute, &rPos, &Handle);
     
    //Show the window
    ShowWindow(Handle);
     
    RunApplicationEventLoop();
    //Remove the winow
    DisposeWindow(Handle);
     
    return 0;
}
