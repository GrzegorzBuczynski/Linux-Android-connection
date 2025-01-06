#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Unable to open X display\n");
        exit(1);
    }

    Window root = DefaultRootWindow(display);
    XImage *image = XGetImage(display, root, 0, 0, 1920, 1080, AllPlanes, ZPixmap);
    // teraz 'image' zawiera zawartość ekranu
    XDestroyImage(image);
    XCloseDisplay(display);
    return 0;
}
