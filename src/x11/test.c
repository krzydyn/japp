#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
// https://seasonofcode.com/posts/how-x-window-managers-work-and-how-to-write-one-part-ii.html
int main(void)
{
    Display *display;
    Window window;
    XEvent event;
    const char *msg = "Hello, World!";
    int s;
 
    /* open connection with the server */
    display = XOpenDisplay(NULL);
    if (display == NULL)
    {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }
 
    s = DefaultScreen(display);
 
    /* create window */
    window = XCreateSimpleWindow(display, RootWindow(display, s), 100, 100, 200, 200, 1,
                           BlackPixel(display, s), WhitePixel(display, s));

	XStoreName(display, window, "Demo app");
 
    /* select kind of events we are interested in */
    XSelectInput(display, window, 0x3ffff);
 
    /* map (show) the window */
    XMapWindow(display, window);
 
    /* event loop */
    for (;;)
    {
        XNextEvent(display, &event);
 
        /* draw or redraw the window */
        if (event.type == Expose)
        {
            XFillRectangle(display, window, DefaultGC(display, s), 20, 20, 10, 10);
            XDrawString(display, window, DefaultGC(display, s), 50, 50, msg, strlen(msg));
        }
        /* exit on key press */
		switch (event.type) {
		case KeyPress:
			printf("KeyPress: key=%d, mask=%x\n", event.xkey.keycode, event.xkey.state);
			break;
		case KeyRelease:
			printf("KeyPress: key=%d, mask=%x\n", event.xkey.keycode, event.xkey.state);
			break;
		case MotionNotify:
			printf("MotionNotify\n");
			break;
		case ButtonPress:
			printf("ButtonPress\n");
			break;
		case ButtonRelease:
			printf("ButtonRelease\n");
			break;
		case Expose:
			printf("Expose\n");
			break;
		case FocusIn:
			printf("FocusIn\n");
			break;
		case FocusOut:
			printf("FocusOut\n");
			break;
		default:
			printf("event type = %d(0x%x)\n", event.type, event.type);
			break;
		}

        if (event.type == KeyPress && event.xkey.keycode == 53) {
			printf("KeyPress: key=%d, mask=%x\n", event.xkey.keycode, event.xkey.state);
            break;
		}
    }
 
    /* close connection to server */
    XCloseDisplay(display);
 
    return 0;
 }
