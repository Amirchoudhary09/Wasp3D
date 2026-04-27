#include <windows.h>
#include <wrl/client.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

using namespace Microsoft::WRL;
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
 LRESULT CALLBACK WndProc(HWND h, UINT m, WPARAM w, LPARAM l);

class Depth {
private:
	HWND hwnd;
	const int WIDTH = 800;
	const int HEIGHT = 600;

	ComPtr<ID3D12Device> device;
	ComPtr<IDXGISwapChain3> swapChain;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<ID3D12CommandAllocator> allocator;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12DescriptorHeap> rtvHeap, dsvHeap;
	ComPtr<ID3D12PipelineState> pso;
	ComPtr<ID3D12RootSignature> rootSig;
	ComPtr<ID3D12Resource> rt[2], depth, vb;

	ComPtr<ID3D12Fence> fence;
	HANDLE fenceEvent;
	UINT64 fenceValue = 0;

	D3D12_VERTEX_BUFFER_VIEW vbView;
	UINT frameIndex, rtvSize;
public:
	void InitWindow(HINSTANCE h);
	void InitD3D();
	void Render();


	struct Vertex { float x, y, z, r, g, b, a; };
};