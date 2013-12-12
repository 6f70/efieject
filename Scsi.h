EFI_STATUS
ScsiEjectAllDevices(VOID);

EFI_STATUS
ScsiEjectDevice(
    EFI_SCSI_IO_PROTOCOL *ScsiIo
    );

EFI_STATUS
ScsiSendCmdStartStopUnit(
    EFI_SCSI_IO_PROTOCOL *ScsiIo,
    UINT8 SubOp
    );

BOOLEAN
ScsiIsOpticalDrive(
    EFI_SCSI_IO_PROTOCOL *ScsiIo
    );

VOID
ScsiPrintDeviceInfo(
    EFI_SCSI_IO_PROTOCOL *ScsiIo
    );
