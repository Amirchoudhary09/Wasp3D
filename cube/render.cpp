
#include "calculatevertexandib.h"
void Cube ::Render()
{
    commandAllocator->Reset();

    commandList->Reset(
        commandAllocator.Get(),
        pipelineState.Get());

    D3D12_RESOURCE_BARRIER barrier = {};

    barrier.Type =
        D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

    barrier.Transition.pResource =
        renderTargets[frameIndex].Get();

    barrier.Transition.StateBefore =
        D3D12_RESOURCE_STATE_PRESENT;

    barrier.Transition.StateAfter =
        D3D12_RESOURCE_STATE_RENDER_TARGET;

    barrier.Transition.Subresource =
        D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    commandList->ResourceBarrier(
        1,
        &barrier);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle =
        rtvHeap->GetCPUDescriptorHandleForHeapStart();

    rtvHandle.ptr +=
        frameIndex * rtvDescriptorSize;

    commandList->OMSetRenderTargets(
        1,
        &rtvHandle,
        FALSE,
        nullptr);

    FLOAT clearColor[] =
    {
       0.0f,
        0.0f,
        0.075f,
        1.0f
    };

    commandList->ClearRenderTargetView(
        rtvHandle,
        clearColor,
        0,
        nullptr);

    D3D12_VIEWPORT viewport =
    {
        0,
        0,
        (float)width,
        (float)height,
        0,
        1
    };

    commandList->RSSetViewports(

        1,
        &viewport);

    D3D12_RECT scissor =
    {
        0,
        0,
        (LONG)width,
        (LONG)height
    };

    commandList->RSSetScissorRects(
        1,
        &scissor);

    commandList->SetGraphicsRootSignature(
        rootSignature.Get());

    commandList->SetGraphicsRootConstantBufferView(
        0,
        constantBuffer->GetGPUVirtualAddress());

    commandList->IASetPrimitiveTopology(
        D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    commandList->IASetVertexBuffers(
        0,
        1,
        &vbView);

    commandList->IASetIndexBuffer(
        &ibView);

    commandList->DrawIndexedInstanced(
        (UINT)indices.size(),
        1,
        0,
        0,
        0);

    barrier.Transition.StateBefore =
        D3D12_RESOURCE_STATE_RENDER_TARGET;

    barrier.Transition.StateAfter =
        D3D12_RESOURCE_STATE_PRESENT;

    commandList->ResourceBarrier(
        1,
        &barrier);

    commandList->Close();

    ID3D12CommandList* lists[] =
    {
        commandList.Get()
    };

    commandQueue->ExecuteCommandLists(
        1,
        lists);

    swapChain->Present(
        1,
        0);

    WaitForGPU();
}
