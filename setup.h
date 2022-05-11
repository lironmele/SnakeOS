#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleTextIn.h>

typedef struct
{
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL *stip;
} Protocols;

EFI_STATUS get_protocols(Protocols **protocols);
