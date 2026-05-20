#pragma once

#ifndef CALCULATEVERTEXANDIB_H
#define CALCULATEVERTEXANDIB_H

#include <wrl/client.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>


#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

#include <DirectXMath.h>
#include <windows.h>
#include <vector>
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

// STRUCTS

class Cube {
private:
    struct ConstantBuffer
    {
        XMMATRIX world;

        XMMATRIX view;

        XMMATRIX projection;
    };

    struct Vertex
    {
        XMFLOAT3 pos;

        XMFLOAT3 color;

        XMFLOAT3 normal;
    };
    // GLOBALS

    HWND hwnd = nullptr;

    UINT width = 1280;

    UINT height = 720;

    ComPtr<ID3D12Device> device;

    ComPtr<IDXGISwapChain3> swapChain;

    ComPtr<ID3D12CommandQueue> commandQueue;

    ComPtr<ID3D12DescriptorHeap> rtvHeap;

    ComPtr<ID3D12Resource> renderTargets[2];

    ComPtr<ID3D12CommandAllocator> commandAllocator;

    ComPtr<ID3D12GraphicsCommandList> commandList;

    ComPtr<ID3D12Fence> fence;

    UINT64 fenceValue = 0;

    HANDLE fenceEvent = nullptr;

    UINT frameIndex = 0;

    UINT rtvDescriptorSize = 0;

    ComPtr<ID3D12RootSignature> rootSignature;

    ComPtr<ID3D12PipelineState> pipelineState;

    ComPtr<ID3D12Resource> vertexBuffer;

    ComPtr<ID3D12Resource> indexBuffer;

    ComPtr<ID3D12Resource> constantBuffer;

    D3D12_VERTEX_BUFFER_VIEW vbView = {};

    D3D12_INDEX_BUFFER_VIEW ibView = {};

    UINT8* cbvDataBegin = nullptr;

    float angle = 0.0f;
    std::vector<Vertex> verts;
    std::vector<UINT> indices;
public:
    void calculatevertex(float size, float h, int segments, vector<Vertex>& verts);
    void calculateinexbuffer(int segments, UINT vertsPerFace, vector<UINT>& indices);
    void InitWindow(HINSTANCE hInstance);
    void InitD3D();
    void Render();
    void Update();
    void WaitForGPU();
    static  LRESULT CALLBACK WndProc(
        HWND hwnd,
        UINT msg,
        WPARAM wParam,
        LPARAM lParam);
    ~Cube() {
        if (fenceEvent)
        {
            CloseHandle(fenceEvent);
        }
	}
};


#endif // !CALCULATEVALUE_H
