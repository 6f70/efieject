#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/ScsiIo.h>
#include <Protocol/ScsiPassThruExt.h>
#include <IndustryStandard/Atapi.h>
#include <IndustryStandard/Scsi.h>
#include "Scsi.h"
#include "WeenAA.h"

VOID
ShutdownSystem(VOID);
