//Przykładowy szybki kod DRM do odczytu bufora wideo

#include <fcntl.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

int main() {
    int drm_fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
    if (drm_fd < 0) {
        perror("Cannot open DRM device");
        return 1;
    }

    drmModeRes *resources = drmModeGetResources(drm_fd);
    if (!resources) {
        perror("Cannot get DRM resources");
        close(drm_fd);
        return 1;
    }

    // Wybierz pierwszy dostępny ekran
    drmModeConnector *connector = drmModeGetConnector(drm_fd, resources->connectors[0]);
    if (!connector || connector->connection != DRM_MODE_CONNECTED) {
        perror("No connected screen found");
        drmModeFreeResources(resources);
        close(drm_fd);
        return 1;
    }

    drmModeCrtc *crtc = drmModeGetCrtc(drm_fd, connector->encoder_id);
    if (!crtc) {
        perror("Cannot get CRTC");
        drmModeFreeConnector(connector);
        drmModeFreeResources(resources);
        close(drm_fd);
        return 1;
    }

    // Mapa pamięci bufora (przykład, różni się w zależności od sprzętu)
    size_t buf_size = crtc->mode.hdisplay * crtc->mode.vdisplay * 4; // Zakładamy 32 bity na piksel
    void *framebuffer = mmap(NULL, buf_size, PROT_READ, MAP_SHARED, drm_fd, crtc->buffer_id);
    if (framebuffer == MAP_FAILED) {
        perror("Failed to mmap framebuffer");
        drmModeFreeCrtc(crtc);
        drmModeFreeConnector(connector);
        drmModeFreeResources(resources);
        close(drm_fd);
        return 1;
    }

    printf("Framebuffer mapped at %p\n", framebuffer);

    // Odczyt bufora klatek
    // Przykład: odczytanie pierwszych pikseli
    unsigned char *pixels = (unsigned char *)framebuffer;
    printf("Pixel data: R=%d, G=%d, B=%d\n", pixels[0], pixels[1], pixels[2]);

    munmap(framebuffer, buf_size);
    drmModeFreeCrtc(crtc);
    drmModeFreeConnector(connector);
    drmModeFreeResources(resources);
    close(drm_fd);

    return 0;
}
