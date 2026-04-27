#include "Application.hpp"

#ifdef ENABLE_FREERTOS
#include "RtosTaskManager.hpp"
#endif

int main()
{
    static Application app;
    app.initialize();

#ifdef ENABLE_FREERTOS
    static RtosTaskManager rtos(app);
    rtos.start();
#else
    app.run();
#endif

    while (1) {
    }
}
