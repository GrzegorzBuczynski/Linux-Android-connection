Dostęp do bufora przez aplikację:

Jeżeli chcesz zrealizować bardziej zaawansowane operacje na buforze ekranu, jak przechwytywanie obrazu lub manipulacja, można użyć bibliotek X11 w C, np. X11.h. W tym przypadku konieczne będzie użycie funkcji takich jak XGetImage do przechwycenia obrazu z bufora:
c
Copy code
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
Powyższy przykład przechwytuje obraz z całego ekranu w rozdzielczości 1920x1080.
Korzystanie z narzędzi do przechwytywania ekranu:

Jeśli potrzebujesz zrzutów ekranu lub nagrywania ekranu, narzędzia takie jak scrot lub flameshot mogą być użyteczne. Są one wygodne w użyciu i mogą być uruchamiane z terminala lub z GUI.
