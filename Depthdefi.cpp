
#include"depth.h"
LRESULT CALLBACK WndProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    if (m == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(h, m, w, l);
}

void Depth::InitWindow(HINSTANCE h) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = L"DX12";
    RegisterClass(&wc);

    hwnd = CreateWindow(L"DX12", L"Depth Test",
        WS_OVERLAPPEDWINDOW, 100, 100, WIDTH, HEIGHT,
        0, 0, h, 0);

    ShowWindow(hwnd, SW_SHOW);
}

void Depth::InitD3D() {
    ComPtr<IDXGIFactory4> factory;
    CreateDXGIFactory1(IID_PPV_ARGS(&factory));

    D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(&device));

    D3D12_COMMAND_QUEUE_DESC qDesc = {};
    device->CreateCommandQueue(&qDesc,
        IID_PPV_ARGS(&commandQueue));

    // 🔥 FIXED SWAPCHAIN
    DXGI_SWAP_CHAIN_DESC1 sc = {};
    sc.BufferCount = 2;
    sc.Width = WIDTH;
    sc.Height = HEIGHT;
    sc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sc.SampleDesc.Count = 1;   // 🔥 MUST

    factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);

    ComPtr<IDXGISwapChain1> temp;
    factory->CreateSwapChainForHwnd(
        commandQueue.Get(),
        hwnd,
        &sc,
        nullptr,
        nullptr,
        &temp
    );

    temp.As(&swapChain);
    frameIndex = swapChain->GetCurrentBackBufferIndex();

    // RTV
    D3D12_DESCRIPTOR_HEAP_DESC rtvDesc = {};
    rtvDesc.NumDescriptors = 2;
    rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&rtvHeap));

    rtvSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    auto rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();

    for (int i = 0; i < 2; i++) {
        swapChain->GetBuffer(i, IID_PPV_ARGS(&rt[i]));
        device->CreateRenderTargetView(rt[i].Get(), 0, rtvHandle);
        rtvHandle.ptr += rtvSize;
    }

    // DEPTH
    D3D12_DESCRIPTOR_HEAP_DESC dsvDesc = {};
    dsvDesc.NumDescriptors = 1;
    dsvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    device->CreateDescriptorHeap(&dsvDesc, IID_PPV_ARGS(&dsvHeap));

    D3D12_HEAP_PROPERTIES heap = {};
    heap.Type = D3D12_HEAP_TYPE_DEFAULT;

    D3D12_RESOURCE_DESC desc = {};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    desc.Width = WIDTH;
    desc.Height = HEIGHT;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_D32_FLOAT;
    desc.SampleDesc.Count = 1;
    desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE clear = {};
    clear.Format = DXGI_FORMAT_D32_FLOAT;
    clear.DepthStencil.Depth = 1.0f;

    device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE,
        &desc, D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &clear, IID_PPV_ARGS(&depth));

    device->CreateDepthStencilView(depth.Get(), 0,
        dsvHeap->GetCPUDescriptorHandleForHeapStart());

    device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(&allocator));

    device->CreateCommandList(0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        allocator.Get(), 0,
        IID_PPV_ARGS(&cmdList));

    cmdList->Close();

    // Fence
    device->CreateFence(0, D3D12_FENCE_FLAG_NONE,
        IID_PPV_ARGS(&fence));
    fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    // Shader
    const char* shader =
        "struct V{float3 p:POSITION;float4 c:COLOR;};"
        "struct P{float4 p:SV_POSITION;float4 c:COLOR;};"
        "P VS(V v){P o;o.p=float4(v.p,1);o.c=v.c;return o;}"
        "float4 PS(P p):SV_TARGET{return p.c;}";

    ComPtr<ID3DBlob> vs, ps;
    D3DCompile(shader, strlen(shader), 0, 0, 0, "VS", "vs_5_0", 0, 0, &vs, 0);
    D3DCompile(shader, strlen(shader), 0, 0, 0, "PS", "ps_5_0", 0, 0, &ps, 0);

    // Root
    D3D12_ROOT_SIGNATURE_DESC rs = {};
    rs.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    ComPtr<ID3DBlob> sig;
    D3D12SerializeRootSignature(&rs,
        D3D_ROOT_SIGNATURE_VERSION_1, &sig, 0);

    device->CreateRootSignature(0,
        sig->GetBufferPointer(),
        sig->GetBufferSize(),
        IID_PPV_ARGS(&rootSig));

    D3D12_INPUT_ELEMENT_DESC layout[] = {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
        {"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0}
    };

    // PSO (DEPTH ENABLE)
    D3D12_GRAPHICS_PIPELINE_STATE_DESC p = {};
    p.InputLayout = { layout,2 };
    p.pRootSignature = rootSig.Get();
    p.VS = { vs->GetBufferPointer(),vs->GetBufferSize() };
    p.PS = { ps->GetBufferPointer(),ps->GetBufferSize() };

    p.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
    p.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

    p.BlendState.RenderTarget[0].RenderTargetWriteMask =
        D3D12_COLOR_WRITE_ENABLE_ALL;

    p.DepthStencilState.DepthEnable = TRUE;
    p.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    p.DepthStencilState.DepthWriteMask =
        D3D12_DEPTH_WRITE_MASK_ALL;

    p.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    p.SampleMask = UINT_MAX;
    p.PrimitiveTopologyType =
        D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    p.NumRenderTargets = 1;
    p.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    p.SampleDesc.Count = 1;

    device->CreateGraphicsPipelineState(&p,
        IID_PPV_ARGS(&pso));

    // OVERLAPPING TRIANGLES
    Vertex v[] = {
        // 
     
        { 0.2f, 0.6f, 0.2f, 0,0,1,1},
           {-0.6f, 0.6f, 0.2f, 0,0,1,1},
        {-0.2f,-0.6f, 0.2f, 0,0,1,1},

      
       
        { 0.6f, 0.6f, 0.8f, 1,0,0,1},
         {-0.2f, 0.6f, 0.8f, 1,0,0,1},
        { 0.2f,-0.6f, 0.8f, 1,0,0,1},
    };

    UINT size = sizeof(v);

    D3D12_HEAP_PROPERTIES up = {};
    up.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC vbDesc = {};
    vbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    vbDesc.Width = size;
    vbDesc.Height = 1;
    vbDesc.DepthOrArraySize = 1;
    vbDesc.MipLevels = 1;
    vbDesc.SampleDesc.Count = 1;
    vbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    device->CreateCommittedResource(&up,
        D3D12_HEAP_FLAG_NONE, &vbDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        0, IID_PPV_ARGS(&vb));

    void* data;
    vb->Map(0, 0, &data);
    memcpy(data, v, size);
    vb->Unmap(0, 0);

    vbView.BufferLocation = vb->GetGPUVirtualAddress();
    vbView.SizeInBytes = size;
    vbView.StrideInBytes = sizeof(Vertex);
}

void Depth::Render() {
    allocator->Reset();
    cmdList->Reset(allocator.Get(), pso.Get());

    D3D12_RESOURCE_BARRIER b = {};
    b.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    b.Transition.pResource = rt[frameIndex].Get();
    b.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    b.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

    cmdList->ResourceBarrier(1, &b);

    auto rtv = rtvHeap->GetCPUDescriptorHandleForHeapStart();
    rtv.ptr += frameIndex * rtvSize;

    auto dsv = dsvHeap->GetCPUDescriptorHandleForHeapStart();

    FLOAT color[] = { 0.2f,0.2f,0.2f,1 };
    cmdList->ClearRenderTargetView(rtv, color, 0, 0);
    cmdList->ClearDepthStencilView(dsv,
        D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, 0);

    cmdList->OMSetRenderTargets(1, &rtv, FALSE, &dsv);

    D3D12_VIEWPORT vp = { 0,0,(float)WIDTH,(float)HEIGHT,0,1 };
    D3D12_RECT sc = { 0,0,WIDTH,HEIGHT };
    cmdList->RSSetViewports(1, &vp);
    cmdList->RSSetScissorRects(1, &sc);

    cmdList->IASetPrimitiveTopology(
        D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    cmdList->IASetVertexBuffers(0, 1, &vbView);

    cmdList->DrawInstanced(6, 1, 0, 0);

    b.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    b.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    cmdList->ResourceBarrier(1, &b);

    cmdList->Close();

    ID3D12CommandList* lists[] = { cmdList.Get() };
    commandQueue->ExecuteCommandLists(1, lists);

    swapChain->Present(1, 0);

    // GPU WAIT
    fenceValue++;
    commandQueue->Signal(fence.Get(), fenceValue);
    if (fence->GetCompletedValue() < fenceValue) {
        fence->SetEventOnCompletion(fenceValue, fenceEvent);
        WaitForSingleObject(fenceEvent, INFINITE);
    }

    frameIndex = swapChain->GetCurrentBackBufferIndex();
}