// REALISTIC DX12 ROTATING 3D CUBE
// Perspective + Camera + Lighting
// Single File

#include "calculatevertexandib.h"


int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE,
    LPSTR,
    int)
{
	Cube *cube = new Cube();
    cube->InitWindow(hInstance);

    cube->InitD3D();

    MSG msg = {};

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(
            &msg,
            nullptr,
            0,
            0,
            PM_REMOVE))
        {
            TranslateMessage(&msg);

            DispatchMessage(&msg);
        }
        else
        {
          cube-> Update();

            cube->Render();
        }
    }

   cube-> WaitForGPU();

   delete cube;

    return 0;
}
