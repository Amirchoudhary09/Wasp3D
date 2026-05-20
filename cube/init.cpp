
#include "calculatevertexandib.h"

void Cube ::InitD3D()
{
    // FACTORY

    ComPtr<IDXGIFactory4> factory;

    CreateDXGIFactory1(
        IID_PPV_ARGS(&factory));

    // DEVICE

    D3D12CreateDevice(
        nullptr,
        D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(&device));

    // COMMAND QUEUE

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};

    device->CreateCommandQueue(
        &queueDesc,
        IID_PPV_ARGS(&commandQueue));

    // SWAPCHAIN

    DXGI_SWAP_CHAIN_DESC1 swapDesc = {};

    swapDesc.BufferCount = 2;

    swapDesc.Width = width;

    swapDesc.Height = height;

    swapDesc.Format =
        DXGI_FORMAT_R8G8B8A8_UNORM;

    swapDesc.BufferUsage =
        DXGI_USAGE_RENDER_TARGET_OUTPUT;

    swapDesc.SwapEffect =
        DXGI_SWAP_EFFECT_FLIP_DISCARD;

    swapDesc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> tempSwapChain;

    factory->CreateSwapChainForHwnd(
        commandQueue.Get(),
        hwnd,
        &swapDesc,
        nullptr,
        nullptr,
        &tempSwapChain);

    tempSwapChain.As(&swapChain);

    frameIndex =
        swapChain->GetCurrentBackBufferIndex();

    // RTV HEAP

    D3D12_DESCRIPTOR_HEAP_DESC rtvDesc = {};

    rtvDesc.NumDescriptors = 2;

    rtvDesc.Type =
        D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

    device->CreateDescriptorHeap(
        &rtvDesc,
        IID_PPV_ARGS(&rtvHeap));

    rtvDescriptorSize =
        device->GetDescriptorHandleIncrementSize(
            D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle =
        rtvHeap->GetCPUDescriptorHandleForHeapStart();

    for (UINT i = 0; i < 2; i++)
    {
        swapChain->GetBuffer(
            i,
            IID_PPV_ARGS(&renderTargets[i]));

        device->CreateRenderTargetView(
            renderTargets[i].Get(),
            nullptr,
            rtvHandle);

        rtvHandle.ptr +=
            rtvDescriptorSize;
    }

    // COMMAND ALLOCATOR

    device->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(&commandAllocator));

    // COMMAND LIST

    device->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        commandAllocator.Get(),
        nullptr,
        IID_PPV_ARGS(&commandList));

    commandList->Close();

    // ROOT SIGNATURE

    D3D12_ROOT_PARAMETER rootParam = {};

    rootParam.ParameterType =
        D3D12_ROOT_PARAMETER_TYPE_CBV;

    rootParam.Descriptor.ShaderRegister = 0;

    rootParam.ShaderVisibility =
        D3D12_SHADER_VISIBILITY_ALL;

    D3D12_ROOT_SIGNATURE_DESC rootSigDesc = {};

    rootSigDesc.NumParameters = 1;

    rootSigDesc.pParameters = &rootParam;

    rootSigDesc.Flags =
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    ComPtr<ID3DBlob> sigBlob;

    ComPtr<ID3DBlob> errorBlob;

    D3D12SerializeRootSignature(
        &rootSigDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        &sigBlob,
        &errorBlob);

    device->CreateRootSignature(
        0,
        sigBlob->GetBufferPointer(),
        sigBlob->GetBufferSize(),
        IID_PPV_ARGS(&rootSignature));



    ComPtr<ID3DBlob> vsBlob;

    ComPtr<ID3DBlob> psBlob;

    D3DCompileFromFile(
        L"vertex.hlsl",
        nullptr,
        nullptr,
        "VSMain",
        "vs_5_0",
        0,
        0,
        &vsBlob,
        nullptr);

    D3DCompileFromFile(
        L"pixel.hlsl",
        nullptr,
        nullptr,
        "PSMain",
        "ps_5_0",
        0,
        0,
        &psBlob,
        nullptr);


    // INPUT LAYOUT

    D3D12_INPUT_ELEMENT_DESC inputLayout[] =
    {
        {
            "POSITION",
            0,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            0,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
            0
        },

        {
            "COLOR",
            0,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            12,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
            0
        },

        {
            "NORMAL",
            0,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            24,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
            0
        }
    };

    // PSO

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};

    psoDesc.InputLayout =
    {
        inputLayout,
        3
    };

    psoDesc.pRootSignature =
        rootSignature.Get();

    psoDesc.VS =
    {
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize()
    };

    psoDesc.PS =
    {
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize()
    };

    psoDesc.RasterizerState.FillMode =
        D3D12_FILL_MODE_SOLID;

    psoDesc.RasterizerState.CullMode =
        D3D12_CULL_MODE_NONE;

    psoDesc.RasterizerState.DepthClipEnable =
        TRUE;

    psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask =
        D3D12_COLOR_WRITE_ENABLE_ALL;

    psoDesc.DepthStencilState.DepthEnable =
        FALSE;

    psoDesc.DepthStencilState.StencilEnable =
        FALSE;

    psoDesc.SampleMask =
        UINT_MAX;

    psoDesc.PrimitiveTopologyType =
        D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    psoDesc.NumRenderTargets = 1;

    psoDesc.RTVFormats[0] =
        DXGI_FORMAT_R8G8B8A8_UNORM;

    psoDesc.SampleDesc.Count = 1;

    device->CreateGraphicsPipelineState(
        &psoDesc,
        IID_PPV_ARGS(&pipelineState));

// PROCEDURAL CUBE
// 6 FACES USING SAME GRID LOGIC

    float size = 3.0f;

    float h = size / 2.0f;

    int segments = 100;

    UINT vertsPerFace =
        (segments + 1) *
        (segments + 1);
    calculatevertex(size, h, segments, verts);
    calculateinexbuffer(segments, vertsPerFace, indices);
    // LOOP FOR ALL 6 FACES
   



    UINT vbSize =
        (UINT)(
            verts.size() *
            sizeof(Vertex));

    UINT ibSize =
        (UINT)(
            indices.size() *
            sizeof(UINT));

    D3D12_HEAP_PROPERTIES heapProps = {};

    heapProps.Type =
        D3D12_HEAP_TYPE_UPLOAD;

    // VERTEX BUFFER

    D3D12_RESOURCE_DESC vbDesc = {};

    vbDesc.Dimension =
        D3D12_RESOURCE_DIMENSION_BUFFER;

    vbDesc.Width =
        vbSize;

    vbDesc.Height = 1;

    vbDesc.DepthOrArraySize = 1;

    vbDesc.MipLevels = 1;

    vbDesc.SampleDesc.Count = 1;

    vbDesc.Layout =
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &vbDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertexBuffer));

    UINT8* vbData = nullptr;

    vertexBuffer->Map(
        0,
        nullptr,
        reinterpret_cast<void**>(&vbData));

    memcpy(
        vbData,
        verts.data(),
        vbSize);

    vertexBuffer->Unmap(
        0,
        nullptr);

    vbView.BufferLocation =
        vertexBuffer->GetGPUVirtualAddress();

    vbView.SizeInBytes =
        vbSize;

    vbView.StrideInBytes =
        sizeof(Vertex);

    // INDEX BUFFER

    D3D12_RESOURCE_DESC ibDesc = {};

    ibDesc.Dimension =
        D3D12_RESOURCE_DIMENSION_BUFFER;

    ibDesc.Width =
        ibSize;

    ibDesc.Height = 1;

    ibDesc.DepthOrArraySize = 1;

    ibDesc.MipLevels = 1;

    ibDesc.SampleDesc.Count = 1;

    ibDesc.Layout =
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &ibDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&indexBuffer));

    UINT8* ibData = nullptr;

    indexBuffer->Map(
        0,
        nullptr,
        reinterpret_cast<void**>(&ibData));

    memcpy(
        ibData,
        indices.data(),
        ibSize);

    indexBuffer->Unmap(
        0,
        nullptr);

    ibView.BufferLocation =
        indexBuffer->GetGPUVirtualAddress();

    ibView.SizeInBytes =
        ibSize;

    ibView.Format =
        DXGI_FORMAT_R32_UINT;

    // CONSTANT BUFFER

    UINT cbSize =
        (sizeof(ConstantBuffer) + 255) & ~255;

    D3D12_RESOURCE_DESC cbDesc = {};

    cbDesc.Dimension =
        D3D12_RESOURCE_DIMENSION_BUFFER;

    cbDesc.Width =
        cbSize;

    cbDesc.Height = 1;

    cbDesc.DepthOrArraySize = 1;

    cbDesc.MipLevels = 1;

    cbDesc.SampleDesc.Count = 1;

    cbDesc.Layout =
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &cbDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constantBuffer));

    constantBuffer->Map(
        0,
        nullptr,
        reinterpret_cast<void**>(&cbvDataBegin));

    // FENCE

    device->CreateFence(
        0,
        D3D12_FENCE_FLAG_NONE,
        IID_PPV_ARGS(&fence));

    fenceEvent =
        CreateEvent(
            nullptr,
            FALSE,
            FALSE,
            nullptr);
}
