#include "calculatevertexandib.h"

void Cube:: WaitForGPU()
{
    fenceValue++;

    commandQueue->Signal(
        fence.Get(),
        fenceValue);

    if (fence->GetCompletedValue() < fenceValue)
    {
        fence->SetEventOnCompletion(
            fenceValue,
            fenceEvent);

        WaitForSingleObject(
            fenceEvent,
            INFINITE);
    }

    frameIndex =
        swapChain->GetCurrentBackBufferIndex();
}
