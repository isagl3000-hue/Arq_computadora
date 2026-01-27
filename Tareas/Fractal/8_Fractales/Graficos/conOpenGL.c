/**************************
 * Includes
 *
 **************************/

#include <windows.h>
#include <gl/gl.h>


/**************************
 * Function Declarations
 *
 **************************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam);
void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);

/**************
 *Declaraciones
 **************/

#include "logo.h"
#include "koch.h"
#include "rama.h"
#include "misFuncs.h"
#include "Dragon.h"
#include "Hil.h"
#include "Spk.h"
#include "C.h"
#include "G.h"
#include "M.h"
#include "P.h"
#include "X.h"
#include "FM.h"
#include "N.h"
#include "H.h"


#define escX 0.125
#define escY 0.125
#define MX 0.0
#define MY 0.0

void segmento(double, double, double, double);
//DibujaSegmentos dibujaSegmento=segmento;



/**************************
 * WinMain
 *
 **************************/

int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    int iCmdShow)
{
    WNDCLASS wc;
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;        
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "graficos";
    RegisterClass (&wc);

/*    char fractales=0, funciones=0;
    f_RenR miFuncion; */
    misFractales miFractal;
    int nRecursion = 5;
    LOGO miTortuga;
    char *miParametro="rama"; 

    if (iCmdShow>1) miParametro = lpCmdLine;
    if (strlen(lpCmdLine)==0) miParametro="FM";

    
      
      asignaModoDibujarSegmento(segmento, &miTortuga);
      if (strncmp(miParametro, "rama", 4)==0){
        iniciaRama(nRecursion, 10.0, 90, 0.0, -5.0, &miTortuga);
        miFractal = dibujaRama;
      } else if (strncmp(miParametro, "kochII", 6)==0){
        iniciaKoch(nRecursion, 11.0, -5.0, -2.5, &miTortuga);
        miFractal = dibujaKochII;
      } else if (strncmp(miParametro, "kochI", 5)==0){
        iniciaKoch(nRecursion, 10.0, -5.0, 2.5, &miTortuga);
        miFractal = dibujaKochI;
      } else if(strncmp(miParametro, "Dragon", 6)==0){
      	iniciaDragon(13,0.1,3,2,&miTortuga);
      	miFractal = dibujaDragon;
      }else if(strncmp(miParametro, "Hil", 6)==0){
      	iniciaHilbert(6,0.2,-6.5,-6,1,&miTortuga);
      	miFractal = dibujaHilbert;
	  }else if(strncmp(miParametro, "Spk", 6)==0){
      	iniciaSpk(5,0.12,-7.5,0,&miTortuga);
      	miFractal = dibujaSpk;
	  }else if(strncmp(miParametro, "C", 6)==0){
      	iniciaC(20,0.007,-3,-3.5,&miTortuga);
      	miFractal = dibujaC;
	  }else if(strncmp(miParametro, "G", 6)==0){
      	iniciaG(4,12,-6.5,1.5,&miTortuga);
      	miFractal = dibujaG;
	  }else if(strncmp(miParametro, "M", 6)==0){
      	iniciaM(3,8,-4,4,&miTortuga);
      	miFractal = dibujaM;
	  }else if(strncmp(miParametro, "P", 6)==0){
      	iniciaP(72,3.3,-1.5,2.5,0.051,&miTortuga);
      	miFractal = dibujaP;
	  }else if(strncmp(miParametro, "X", 6)==0){
      	iniciaX(3,15,-7.3,0,&miTortuga);
      	miFractal = dibujaX;
	  }else if(strncmp(miParametro, "FM", 6)==0){
      	iniciaFM(3,15,-7.5,0,&miTortuga);
      	miFractal = dibujaFM;
	  }else if(strncmp(miParametro, "N", 6)==0){
      	iniciaN(8,14,-7,5,&miTortuga);
      	miFractal = dibujaN;
	  }else if(strncmp(miParametro, "H", 6)==0){
      	iniciaH(4,14,-7,-4,&miTortuga);
      	miFractal = dibujaH;
	  }
    

    /* create main window */
    hWnd = CreateWindow (
      "graficos", 
      "Ejemplo de gr'aficos (fractales) con OpenGL", 
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      0, 0, 2*256, 2*256,
      NULL, NULL, hInstance, NULL);

    /* enable OpenGL for the window */
    EnableOpenGL (hWnd, &hDC, &hRC);

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */

            glClearColor (1.0f, 1.0f, 1.0f, 0.5f);
            glClear (GL_COLOR_BUFFER_BIT);

            glPushMatrix ();
            glRotatef (theta, 0.0f, 0.0f, 1.0f);
            glColor3d(0.0, 0.0, 0.0);

            
              miFractal();
            
            glPopMatrix ();

            SwapBuffers (hDC);

            //theta += 1.0f;
            Sleep (1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL (hWnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow (hWnd);

    return msg.wParam;
}


/********************
 * Window Procedure
 *
 ********************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_CREATE:
        return 0;
    case WM_CLOSE:
        PostQuitMessage (0);
        return 0;

    case WM_DESTROY:
        return 0;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            return 0;
        }
        return 0;

    default:
        return DefWindowProc (hWnd, message, wParam, lParam);
    }
}


/*******************
 * Enable OpenGL
 *
 *******************/

void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC (hWnd);

    /* set the pixel format for the DC */
    ZeroMemory (&pfd, sizeof (pfd));
    pfd.nSize = sizeof (pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
      PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat (*hDC, &pfd);
    SetPixelFormat (*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext( *hDC );
    wglMakeCurrent( *hDC, *hRC );

}


/******************
 * Disable OpenGL
 *
 ******************/

void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent (NULL, NULL);
    wglDeleteContext (hRC);
    ReleaseDC (hWnd, hDC);
}

void segmento(double x1, double y1, double x2, double y2)
{
  glBegin (GL_LINES);
    glVertex2d (MX+escX*x1, MY+escY*y1);
    glVertex2d (MX+escX*x2, MY+escY*y2);
  glEnd ();
}

