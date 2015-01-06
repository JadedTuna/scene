#include "scene.h"

#define python static PyObject*
#define PYARGS PyObject *self, PyObject *args

/*
drawing functions go here
*/

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

void horizontal_line(SDL_Surface* screen, int x, int y, int w) {
    int i;
    for (i=0; i <= w; i++) {
        if (x + i >= SX) break;
        if (x < 0) {
            w += x;
            x = 0;
        }

        if (y < 0) break;
        if (y >= SY) break;
        putpixel(screen, x + i, y,
                 SDL_MapRGBA(screen->format,
                     stroke_color.r,
                     stroke_color.g,
                     stroke_color.b,
                     stroke_color.a));
    }
}

void vertical_line(SDL_Surface* screen, int x, int y, int h) {
    int i;
    for (i=0; i <= h; i++) {
        if (y + i >= SY) break;
        if (y < 0) {
            h += y;
            y = 0;
        }
        
        if (x >= SX) break;
        if (x < 0) break;
        putpixel(screen, x, y + i,
                 SDL_MapRGBA(screen->format,
                     stroke_color.r,
                     stroke_color.g,
                     stroke_color.b,
                     stroke_color.a));
    }
}

void RectRGBA(SDL_Surface* screen, int x, int y, int w, int h) {
    // top
    SDL_LockSurface(screen);
    horizontal_line(screen,
             x,
             y,
             w);

    // right
    vertical_line(screen,
             x + w,
             y,
             h);

    // bottom
    horizontal_line(screen,
             x,
             y + h,
             w);

    // left
    vertical_line(screen,
             x,
             y,
             h);

    SDL_UnlockSurface(screen);
}

void strokeEllipseRGBA(SDL_Surface* screen, SDL_Rect* bounds) {
    char _stroke = stroke;
    int x, y, w, h;

    x = bounds->x;
    y = bounds->y;
    w = bounds->w;
    h = bounds->h;

    while (_stroke--) {
        ellipseRGBA(screen,
                    x, y, w, h,

                    stroke_color.r,
                    stroke_color.g,
                    stroke_color.b,
                    stroke_color.a);

        w -= 1;
        h -= 1;
    }
}

void strokeRectRGBA(SDL_Surface* screen, SDL_Rect* bounds) {
    char _stroke = stroke;
    int x, y, w, h;

    x = bounds->x;
    y = bounds->y;
    w = bounds->w;
    h = bounds->h;

    while (_stroke--) {
        RectRGBA(screen,
                      x, y, w, h);

        x += 1;
        y += 1;
        w -= 2;
        h -= 2;
    }
}

/*
scene drawing functions go here
*/

python scene_stroke_weight(PYARGS) {
    if (!PyArg_ParseTuple(args, "i", &stroke))
        return NULL;

    return Py_None;
}

python scene_no_stroke(PYARGS) {
    stroke = 0;

    return Py_None;
}


python scene_no_fill(PYARGS) {
    fill = 0;

    return Py_None;
}

python scene_background(PYARGS) {
    float fr, fg, fb;

    RGBA_Color color;

    if (!PyArg_ParseTuple(args, "fff",
                            &fr,
                            &fg,
                            &fb))
        return NULL;

    color.r = fr * 255;
    color.g = fg * 255;
    color.b = fb * 255;

    bounds.x = 0;
    bounds.y = 0;
    bounds.w = SX;
    bounds.h = SY;

    SDL_FillRect(screen,
                 &bounds,
                 SDL_MapRGB(screen->format,
                            color.r,
                            color.g,
                            color.b));

    Py_INCREF(Py_None);
    return Py_None;
}

python scene_rect(PYARGS) {
    float fx, fy, fw, fh;
    if (!PyArg_ParseTuple(args, "ffff",
                                &fx,
                                &fy,
                                &fw,
                                &fh))
        return NULL;

    bounds.x = fx;
    bounds.y = fy;
    bounds.w = fw;
    bounds.h = fh;

    bounds.y = SY - (bounds.y + bounds.h);

    if (fill)
        SDL_FillRect(
            screen,
            &bounds,

            SDL_MapRGBA(screen->format,
                fill_color.r,
                fill_color.g,
                fill_color.b,
                fill_color.a));

    if (stroke)
        strokeRectRGBA(screen, &bounds);

    Py_INCREF(Py_None);
    return Py_None;
}

python scene_ellipse(PYARGS) {
    if (!PyArg_ParseTuple(args, "iiii",
                                &bounds.x,
                                &bounds.y,
                                &bounds.w,
                                &bounds.h))
        return NULL;

    bounds.y = SY - (bounds.y + bounds.h);

    bounds.w /= 2;
    bounds.h /= 2;

    bounds.y += bounds.w;
    bounds.x += bounds.h;

    if (fill)
        filledEllipseRGBA(
            screen,
            bounds.x,
            bounds.y,
            bounds.w,
            bounds.h,

            fill_color.r,
            fill_color.g,
            fill_color.b,
            fill_color.a);

    if (stroke)
        strokeEllipseRGBA(screen, &bounds);

    Py_INCREF(Py_None);
    return Py_None;
}

python scene_fill(PYARGS) {
    float fr, fg, fb, fa = 1.0;

    if (!PyArg_ParseTuple(args, "fff|f",
                                &fr,
                                &fg,
                                &fb,
                                &fa))
        return NULL;

    fill_color.r = fr * 255;
    fill_color.g = fg * 255;
    fill_color.b = fb * 255;
    fill_color.a = fa * 255;

    Py_INCREF(Py_None);
    return Py_None;
}

python scene_stroke(PYARGS) {
    float fr, fg, fb, fa = 1.0;

    if (!PyArg_ParseTuple(args, "fff",
                                &fr,
                                &fg,
                                &fb,
                                &fa))
        return NULL;

    stroke_color.r = fr * 255;
    stroke_color.g = fg * 255;
    stroke_color.b = fb * 255;
    stroke_color.a = fa * 255;

    Py_INCREF(Py_None);
    return Py_None;
}

/*
other functions
*/

python runtime_error(const char* message) {
    PyErr_SetString(PyExc_RuntimeError, message);
    return NULL;
}

/*
Init SDL
*/
python scene_init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return runtime_error(SDL_GetError());

    char* SCENE_SIZE = getenv("SCENE_SIZE");
    if (SCENE_SIZE) {
        sscanf(SCENE_SIZE, "%d %d", &SX, &SY);
    }
    screen = SDL_SetVideoMode(SX, SY, 0, SDL_SWSURFACE);
    SDL_WM_SetCaption("scene", NULL);

    Py_INCREF(Py_None);
    return Py_None;
}

/*
Create display
*/
python scene_run(PYARGS) {
    PyObject* scene;

    int orientation;
    int frame_interval;
    unsigned char anti_alias;
    char running = 1;
    char touch   = 0;

    int  prev_x = 0;
    int  prev_y = 0;

    if (!PyArg_ParseTuple(args, "Oiib",
                            &scene,
                            &orientation,
                            &frame_interval,
                            &anti_alias))
        return NULL;

    Py_INCREF(scene);

    if (!PyObject_CallMethod(scene, "_setup_scene", "ii",
                             SX, SY)) {
        PyErr_Print();
    }
    for (;;) {
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button != SDL_BUTTON_LEFT)
                        continue;

                    prev_x = event.button.x;
                    prev_y = SY - event.button.y;

                    touch  = 1;
                    if (!PyObject_CallMethod(scene, "_touch_began",
                                             "iis",
                                             prev_x,
                                             prev_y,
                                             "scene")) {
                        PyErr_Print();
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (!touch)
                        continue;

                    if (!PyObject_CallMethod(scene, "_touch_moved",
                                             "iiiis",
                                             event.button.x,
                                             SY - event.button.y,
                                             prev_x,
                                             prev_y,
                                             "scene")) {
                        PyErr_Print();
                    }

                    prev_x = event.button.x;
                    prev_y = SY - event.button.y;
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button != SDL_BUTTON_LEFT ||
                        !touch)
                        continue;

                    prev_x = event.button.x;
                    prev_y = SY - event.button.y;

                    touch  = 0;
                    if (!PyObject_CallMethod(scene, "_touch_ended",
                                             "iis",
                                             event.button.x,
                                             SY - event.button.y,
                                             "scene")) {
                        PyErr_Print();
                    }
                    break;
            }
        }
        if (!running) break;
        
        if (!PyObject_CallMethod(scene, "_draw", "i", 0)) {
            PyErr_Print();
        }

        SDL_Flip(screen);
        SDL_Delay(frame_interval);
    }

    if (!PyObject_CallMethod(scene, "_stop", NULL)) {
        PyErr_Print();
    }
    SDL_Quit();

    Py_INCREF(Py_None);
    Py_DECREF(scene);
    return Py_None;
}

static PyMethodDef sceneMethods[] = {
    {"stroke_weight", scene_stroke_weight, METH_VARARGS, ""},
    {"no_stroke", scene_no_stroke, METH_VARARGS, ""},
    {"no_fill", scene_no_fill, METH_VARARGS, ""},
    {"background", scene_background, METH_VARARGS, ""},
    {"rect", scene_rect, METH_VARARGS, ""},
    {"ellipse", scene_ellipse, METH_VARARGS, ""},
    {"fill", scene_fill, METH_VARARGS, ""},
    {"stroke", scene_stroke, METH_VARARGS, ""},
    {"run", scene_run, METH_VARARGS, ""},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC init_scene(void) {
    scene_init();
    (void) Py_InitModule("_scene", sceneMethods);
}
