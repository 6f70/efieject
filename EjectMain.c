#include "EjectMain.h"

#define STALL_MICROSEC  5 * 1000 * 1000


VOID
ShutdownSystem(VOID)
{
    gRT->ResetSystem(
        EfiResetShutdown,
        EFI_SUCCESS,
        0,
        NULL);
}

EFI_STATUS
EFIAPI
UefiMain (
    EFI_HANDLE        ImageHandle,
    EFI_SYSTEM_TABLE  *SystemTable
    )
{
    PrintWeenAA();
    ScsiEjectAllDevices();
    //ShutdownSystem();
    gBS->Stall(STALL_MICROSEC);

    return EFI_SUCCESS;
}

