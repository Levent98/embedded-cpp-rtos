#include "ErrorManager.hpp"
#include <cassert>

int main()
{
    ErrorManager errors;

    assert(errors.systemHealthy());
    assert(!errors.recoveryRequested());
    assert(!errors.fatalFault());

    errors.reportCrcError();
    assert(errors.crcErrorCount() == 1U);
    assert(errors.systemHealthy());

    errors.reportSensorReadFailure();
    errors.reportSensorReadFailure();
    errors.reportSensorReadFailure();

    assert(errors.sensorReadFailCount() == 3U);
    assert(errors.recoveryRequested());

    errors.clearRecoveryRequest();
    assert(!errors.recoveryRequested());

    errors.reportUartOverflow();
    errors.reportUartOverflow();
    errors.reportUartOverflow();
    errors.reportUartOverflow();
    errors.reportUartOverflow();

    assert(errors.uartOverflowCount() == 5U);
    assert(errors.fatalFault());
    assert(!errors.systemHealthy());
    assert(!errors.shouldFeedWatchdog());

    return 0;
}