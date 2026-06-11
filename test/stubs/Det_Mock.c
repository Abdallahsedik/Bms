#include "Det.h"

uint8 LastReportedError = 0xFF; // 0xFF indicates no error reported

Std_ReturnType Det_ReportError(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId) {
    LastReportedError = ErrorId;
    return E_OK;
}