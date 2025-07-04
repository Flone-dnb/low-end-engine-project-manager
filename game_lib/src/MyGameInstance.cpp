#include "MyGameInstance.h"

// Custom.
#include "render/FontManager.h"
#include "render/Renderer.h"
#include "misc/ProjectPaths.h"

class Node;

MyGameInstance::MyGameInstance(Window* pWindow) : GameInstance(pWindow) {}

void MyGameInstance::registerGameTypes() {
    // Register your custom types in the reflection database here.
    // You should do it in this function because editor_lib will call this
    // function in its onGameStarted to allow you to use your game types in the
    // editor.
    //
    // For example:
    // registerType(MyNode::getTypeGuidStatic(), MyNode::getReflectionInfo());
}

void MyGameInstance::onGameStarted() {
    registerGameTypes();

    getRenderer()->getFontManager().loadFont(
        ProjectPaths::getPathToResDirectory(ResourceDirectory::GAME) / "font" / "font.ttf");

    createWorld([](Node* pRootNode) {
        // TODO
    });
}
