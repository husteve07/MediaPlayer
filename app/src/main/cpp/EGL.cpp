//
// Created by huste on 2023-04-21.
//

#include "EGL.h"
#include "Log.h"
#include <android/native_window.h>
#include <EGL/egl.h>
#include <mutex>

class CEGL : public EGL
{
public:
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext  context = EGL_NO_CONTEXT;
    std::mutex mux;

    virtual void Draw()
    {
        mux.lock();
        if(display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE)
        {
            mux.unlock();
            return ;
        }
        eglSwapBuffers(display,surface);
        mux.unlock();
    }


    virtual bool Init(void* win) {
        ANativeWindow *nwin = (ANativeWindow *) win;
        //initializing EGL
        //get EGLDisplay
        Close();
        mux.lock();
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (display == EGL_NO_DISPLAY)
        {
            mux.unlock();
            LOGE("eglGetdisplay failed");
            return false;
        }
        LOGE("eglGetdisplay success");
        //initialize EGL display
        if (EGL_TRUE != eglInitialize(display, 0, 0))
        {
            mux.unlock();
            LOGE("elgInitialize failed");
            return false;
        }
        LOGE("elgInitlization success");

        //get configuration and create surface
        EGLint configSpec[] = {
                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_NONE
        };
        EGLConfig config = 0;
        EGLint numConfig = 0;
        if (EGL_TRUE != eglChooseConfig(display, configSpec, &config, 1, &numConfig))
        {
            mux.unlock();
            LOGE("elgChooseConfig failed");
            return false;
        }
        LOGE("eglConfig success");

        surface = eglCreateWindowSurface(display, config, nwin, NULL);

        //create EGL context
        const EGLint ctxAttr[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
        context = eglCreateContext(display, config, context, ctxAttr);
        if(context == EGL_NO_CONTEXT)
        {
            mux.unlock();
            LOGE("eglCreateContext failed");
            return false;
        }
        LOGE("eglCreateContext success");

        if(EGL_TRUE != eglMakeCurrent(display,surface,surface,context))
        {
            mux.unlock();
            LOGE("eglMakeCurrent failed");
            return false;
        }
        mux.unlock();
        LOGE("eglMakeCurrent success");

        return true;
    }
    virtual void Close()
    {
        mux.lock();
        if(display != EGL_NO_DISPLAY)
        {
            mux.unlock();
            return;
        }
        eglMakeCurrent(display, EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);

        if(surface != EGL_NO_SURFACE)
            eglDestroySurface(display,surface);
        if(surface != EGL_NO_CONTEXT)
            eglDestroyContext(display, context);
        eglTerminate(display);
         display = EGL_NO_DISPLAY;
         surface = EGL_NO_SURFACE;
         context = EGL_NO_CONTEXT;
        mux.unlock();

    }
};

EGL *EGL::Get() {
    static CEGL egl;
    return &egl;
}


