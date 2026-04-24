#include "dx12.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    DX12App *app=new DX12App();
    vector<DX12App::Vertex> vertices;
    // for square

    vertices.push_back({ -0.5f,  0.5f, 0, 1,0,0 });
    vertices.push_back({ 0.5f,  0.5f, 0, 1,0,0 });
    vertices.push_back({ 0.5f, -0.5f, 0, 0,0,1 });
    vertices.push_back({ -0.5f, -0.5f, 0, 0,0,1 });
	// for rectangle vertices
    vertices.push_back({ -0.19f,  0.5f, 0, 1,0,0 });
    vertices.push_back({ 0.19f,  0.5f, 0, 1,0,0 });
    vertices.push_back({ 0.19f, -0.5f, 0, 0,0,1 });
    vertices.push_back({ -0.19f, -0.5f, 0, 0,0,1 });

	//for rectangle horizontal vertices
    vertices.push_back({ -0.5f,  0.19f, 0, 1,0,0 });
    vertices.push_back({ 0.5f,  0.19f, 0, 1,0,0 });
    vertices.push_back({ 0.5f, -0.19f, 0, 0,0,1 });
    vertices.push_back({ -0.5f, -0.19f, 0, 0,0,1 });

    app->InitWindow(hInstance);
    app->InitD3D(vertices);

    MSG msg = {};

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            app->Render();
        }
    }

    return 0;
}