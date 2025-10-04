#include "MyGameInstance.h"

// Custom.
#include "misc/ProjectPaths.h"
#include "render/FontManager.h"
#include "render/Renderer.h"
#include "input/InputManager.h"
#include "game/node/MeshNode.h"
#include "game/node/physics/CollisionNode.h"
#include "game/node/light/PointLightNode.h"
#include "game/geometry/shapes/CollisionShape.h"
#include "node/MyCharacterNode.h"
#include "GameInputEventIds.hpp"

MyGameInstance::MyGameInstance(Window* pWindow) : GameInstance(pWindow) {}

void MyGameInstance::registerGameTypes() {
    // Register your custom types in the reflection database here.
    // You should do it in this function because editor_lib will call this
    // function in its onGameStarted to allow you to use your game types in the
    // editor.

    ReflectedTypeDatabase::registerType(
        MyCharacterNode::getTypeGuidStatic(), MyCharacterNode::getReflectionInfo());
}

void MyGameInstance::onGameStarted() {
    registerGameTypes();

    getRenderer()->getFontManager().loadFont(
        ProjectPaths::getPathToResDirectory(ResourceDirectory::ENGINE) / "font" / "font.ttf");

    // Register input events.
    {
        // Jump.
        auto optionalError = getInputManager()->addActionEvent(
            GameInputEventIds::Action::JUMP, {KeyboardButton::SPACE, GamepadButton::B});
        if (optionalError.has_value()) [[unlikely]] {
            Error::showErrorAndThrowException("failed to register input event");
        }

        // Crouch.
        optionalError = getInputManager()->addActionEvent(
            GameInputEventIds::Action::CROUCH, {KeyboardButton::LEFT_CONTROL, GamepadButton::A});
        if (optionalError.has_value()) [[unlikely]] {
            Error::showErrorAndThrowException("failed to register input event");
        }

        // Move forward.
        optionalError = getInputManager()->addAxisEvent(
            GameInputEventIds::Axis::MOVE_FORWARD,
            {{KeyboardButton::S, KeyboardButton::W}},
            {GamepadAxis::LEFT_STICK_Y});
        if (optionalError.has_value()) [[unlikely]] {
            Error::showErrorAndThrowException("failed to register input event");
        }

        // Move right.
        optionalError = getInputManager()->addAxisEvent(
            GameInputEventIds::Axis::MOVE_RIGHT,
            {{KeyboardButton::D, KeyboardButton::A}},
            {GamepadAxis::LEFT_STICK_X});
        if (optionalError.has_value()) [[unlikely]] {
            Error::showErrorAndThrowException("failed to register input event");
        }

        // Gamepad look up.
        optionalError = getInputManager()->addAxisEvent(
            GameInputEventIds::Axis::GAMEPAD_LOOK_UP, {{}}, {GamepadAxis::RIGHT_STICK_Y});
        if (optionalError.has_value()) [[unlikely]] {
            Error::showErrorAndThrowException("failed to register input event");
        }

        // Gamepad look right.
        optionalError = getInputManager()->addAxisEvent(
            GameInputEventIds::Axis::GAMEPAD_LOOK_RIGHT, {{}}, {GamepadAxis::RIGHT_STICK_X});
        if (optionalError.has_value()) [[unlikely]] {
            Error::showErrorAndThrowException("failed to register input event");
        }
    }

    // Create a simple world.
    createWorld([this](Node* pRootNode) {
        // Create floor.
        const auto pFloorCollision = pRootNode->addChildNode(std::make_unique<CollisionNode>());
        {
            auto pBoxShape = std::make_unique<BoxCollisionShape>();
            pBoxShape->setHalfExtent(glm::vec3(5.0F, 5.0F, 0.5F));
            pFloorCollision->setShape(std::move(pBoxShape));
            {
                const auto pMesh = pFloorCollision->addChildNode(std::make_unique<MeshNode>());
                pMesh->setRelativeScale(glm::vec3(10.0F, 10.0F, 1.0F));
                pMesh->getMaterial().setDiffuseColor(glm::vec3(1.0F, 0.5F, 0.0F));
            }
        }

        // Create point light.
        const auto pLight = pRootNode->addChildNode(std::make_unique<PointLightNode>());
        pLight->setRelativeLocation(glm::vec3(0.0F, 0.0F, 5.0F));

        // Create cube.
        const auto pCubeCollision = pRootNode->addChildNode(std::make_unique<CollisionNode>());
        pCubeCollision->setRelativeLocation(glm::vec3(2.0F, 0.0F, 1.0F));
        {
            auto pBoxShape = std::make_unique<BoxCollisionShape>();
            pBoxShape->setHalfExtent(glm::vec3(0.5F, 0.5F, 0.5F));
            pCubeCollision->setShape(std::move(pBoxShape));
            {
                pCubeCollision->addChildNode(std::make_unique<MeshNode>());
            }
        }

        // Spawn character.
        auto pCharacter = std::make_unique<MyCharacterNode>();
        pCharacter->setRelativeLocation(glm::vec3(-2.0F, 0.0F, 1.0F));
        pRootNode->addChildNode(std::move(pCharacter));
    });
}
