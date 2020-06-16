#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <applibs/log.h>

int main(void)
{
    // This minimal Azure Sphere app repeatedly prints "Tick" then "Tock" to the debug console.
    // Use this app to test that device and SDK installation succeeded that you can build,
    // deploy, and debug an app with Visual Studio, and that you can deploy an app over the air,
    // per the instructions here: https://docs.microsoft.com/azure-sphere/quickstarts/qs-overview
    //
    // It is NOT recommended to use this as a starting point for developing apps; instead use
    // the extensible samples here: https://github.com/Azure/azure-sphere-samples

    const struct timespec sleepTime = {1, 0};
    while (true) {
        Log_Debug("High Level Core tick\n");
        nanosleep(&sleepTime, NULL);
        Log_Debug("High Level Core tock\n");
        nanosleep(&sleepTime, NULL);
    }
}
