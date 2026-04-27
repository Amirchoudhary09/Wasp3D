
#include"depth.h"

int WINAPI WinMain(HINSTANCE h, HINSTANCE, LPSTR, int) {
	Depth* depth = new Depth();
   depth->InitWindow(h);
   depth->InitD3D();

    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            depth->Render();
        }
    }
}