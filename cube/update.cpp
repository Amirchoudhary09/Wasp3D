
#include "calculatevertexandib.h"
void Cube:: Update()
{
    angle += 0.01f;

    XMMATRIX world =
        XMMatrixRotationX(angle) * XMMatrixRotationY(angle);

    XMVECTOR eye =
        XMVectorSet(0, 0, -9, 1);

    XMVECTOR target =
        XMVectorSet(0, 0, 0, 1);

    XMVECTOR up =
        XMVectorSet(0, 1, 0, 0);

    XMMATRIX view =
        XMMatrixLookAtLH(
            eye,
            target,
            up);

    XMMATRIX projection =
        XMMatrixPerspectiveFovLH(
            XMConvertToRadians(40.0f),
            width / (float)height,
            0.1f,
            100.0f
        );

    ConstantBuffer cb;

    cb.world =
        XMMatrixTranspose(world);

    cb.view =
        XMMatrixTranspose(view);

    cb.projection =
        XMMatrixTranspose(projection);

    memcpy(
        cbvDataBegin,
        &cb,
        sizeof(cb));
}
