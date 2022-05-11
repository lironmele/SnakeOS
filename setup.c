#include "setup.h"

EFI_STATUS get_protocols(Protocols **protocols)
{
    EFI_STATUS status;

    status = gBS->AllocatePool(
        EfiBootServicesData,
        sizeof(Protocols),
        (VOID **)protocols);
    if (EFI_ERROR(status))
    {
        Print(L"Can't allocate pool for protocols struct.\n");
        return status;
    }

    status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)(&((*protocols)->gop)));

    if (EFI_ERROR(status))
    {
        Print(L"Can't get Graphics Output Protocol pointer.\n");
        return status;
    }

    status = gBS->LocateProtocol(&gEfiSimpleTextInProtocolGuid, NULL, (VOID **)(&((*protocols)->stip)));

    if (EFI_ERROR(status))
    {
        Print(L"Can't get Simple Text Input Protocol pointer.\n");
        return status;
    }

    return EFI_SUCCESS;
}
