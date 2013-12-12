#include "EjectMain.h"


EFI_STATUS
ScsiEjectAllDevices(VOID)
{
    EFI_STATUS  Status;

    UINTN HandleNum;
    EFI_HANDLE  *HandleBuffer;
    Status = gBS->LocateHandleBuffer(
        ByProtocol,
        &gEfiScsiIoProtocolGuid,
        NULL,
        &HandleNum,
        &HandleBuffer);
    if (Status != EFI_SUCCESS) {
        return Status;
    }

    UINTN Index;
    for (Index = 0; Index < HandleNum; Index++) {
        EFI_SCSI_IO_PROTOCOL *ScsiIo;
        Status = gBS->OpenProtocol(
            HandleBuffer[Index],
            &gEfiScsiIoProtocolGuid,
            (VOID**)&ScsiIo,
            gImageHandle,
            NULL,
            EFI_OPEN_PROTOCOL_GET_PROTOCOL);
        if (Status != EFI_SUCCESS) {
            return Status;
        }

        if (ScsiIsOpticalDrive(ScsiIo)) {
            ScsiPrintDeviceInfo(ScsiIo);
            ScsiEjectDevice(ScsiIo);
        }

        gBS->CloseProtocol(
            HandleBuffer[Index],
            &gEfiScsiIoProtocolGuid,
            gImageHandle,
            NULL);
    }
    gBS->FreePool(HandleBuffer);

    return EFI_SUCCESS;
}

EFI_STATUS
ScsiEjectDevice(
    EFI_SCSI_IO_PROTOCOL *ScsiIo
    )
{
    return ScsiSendCmdStartStopUnit(ScsiIo, ATA_CMD_SUBOP_EJECT_DISC);
}

EFI_STATUS
ScsiSendCmdStartStopUnit(
    EFI_SCSI_IO_PROTOCOL *ScsiIo,
    UINT8 SubOp
    )
{
    EFI_SCSI_IO_SCSI_REQUEST_PACKET Packet;
    UINT8 Cdb[12];
    gBS->SetMem(&Packet, sizeof(EFI_SCSI_IO_SCSI_REQUEST_PACKET), 0);
    gBS->SetMem(Cdb, sizeof(Cdb), 0);

    Cdb[0] = ATA_CMD_START_STOP_UNIT;   // 0x1b
    Cdb[4] = SubOp;

    Packet.Timeout          = 0;
    Packet.Cdb              = Cdb;
    Packet.CdbLength        = sizeof(Cdb);
    Packet.DataDirection    = EFI_SCSI_IO_DATA_DIRECTION_WRITE;

    return ScsiIo->ExecuteScsiCommand(
        ScsiIo,
        &Packet,
        NULL);
}

BOOLEAN
ScsiIsOpticalDrive(
    EFI_SCSI_IO_PROTOCOL *ScsiIo
    )
{
    UINT8 DeviceType;
    ScsiIo->GetDeviceType(ScsiIo, &DeviceType);
    return DeviceType == EFI_SCSI_IO_TYPE_CDROM;
}

VOID
ScsiPrintDeviceInfo(
    EFI_SCSI_IO_PROTOCOL *ScsiIo
    )
{
    UINT8 Target[TARGET_MAX_BYTES];
    UINT64 Lun;
    ScsiIo->GetDeviceLocation(ScsiIo, (UINT8**)&Target, &Lun);

    Print(L"Ejecting...\n");
    Print(L"  Target: ");
    UINTN Index;
    for (Index = 0; Index < TARGET_MAX_BYTES; Index++) {
        Print(L"%02X", Target[Index]);
    }
    Print(L"\n  Lun: %16X\n", Lun);
}
