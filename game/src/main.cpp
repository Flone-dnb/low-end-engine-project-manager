// Standard.
#include <stdexcept>

// Custom.
#include "MyGameInstance.h"
#include "game/Window.h"

// External.
#if defined(WIN32)
#include <Windows.h>
#include <crtdbg.h>
#endif

int main() {
    // Enable run-time memory check for debug builds (on Windows).
#if defined(WIN32) && defined(DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#elif defined(WIN32) && !defined(DEBUG)
    OutputDebugStringA("Using release build configuration, memory checks are disabled.");
#endif

    // Create window.
    auto result = WindowBuilder().title("My Game").fullscreen().build();
    if (std::holds_alternative<Error>(result)) [[unlikely]] {
        auto error = std::get<Error>(std::move(result));
        error.addCurrentLocationToErrorStack();
        error.showErrorAndThrowException();
    }
    const auto pWindow = std::get<std::unique_ptr<Window>>(std::move(result));

    pWindow->processEvents<MyGameInstance>();

    return 0;
}
