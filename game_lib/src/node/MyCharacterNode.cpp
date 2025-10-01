#include "node/MyCharacterNode.h"

// Custom.
#include "math/MathHelpers.hpp"
#include "game/node/CameraNode.h"
#include "game/GameInstance.h"
#include "game/Window.h"
#include "game/geometry/shapes/CollisionShape.h"
#include "GameInputEventIds.hpp"

// External.
#include "nameof.hpp"

namespace {
    constexpr std::string_view sTypeGuid = "ea621160-7d7e-45fe-8067-07efaf42e17d";
}

std::string MyCharacterNode::getTypeGuidStatic() { return sTypeGuid.data(); }
std::string MyCharacterNode::getTypeGuid() const { return sTypeGuid.data(); }

TypeReflectionInfo MyCharacterNode::getReflectionInfo() {
    ReflectedVariables variables;

    return TypeReflectionInfo(
        SimpleCharacterBodyNode::getTypeGuidStatic(),
        NAMEOF_SHORT_TYPE(MyCharacterNode).data(),
        []() -> std::unique_ptr<Serializable> { return std::make_unique<MyCharacterNode>(); },
        std::move(variables));
}

MyCharacterNode::MyCharacterNode() : MyCharacterNode("My Character Node") {}
MyCharacterNode::MyCharacterNode(const std::string& sNodeName) : SimpleCharacterBodyNode(sNodeName) {
    setSerialize(false); // you probably don't want to serialize the character node
    setIsReceivingInput(true);

    getActionEventBindings()[GameInputEventIds::Action::JUMP] =
        ActionEventCallbacks{.onPressed = [&](KeyboardModifiers modifiers) { jump(false); }};

    getActionEventBindings()[GameInputEventIds::Action::CROUCH] =
        ActionEventCallbacks{.onPressed = [&](KeyboardModifiers modifiers) {
            const auto bIsCrouching = getIsCrouching();
            if (!bIsCrouching) {
                // Standing -> crouching.
                charSpeedBeforeCrouched = getMovementSpeed();
                if (trySetIsCrouching(true)) {
                    setMovementSpeed(charSpeedBeforeCrouched * 0.5F);
                }
            } else {
                // Crouching -> standing.
                if (trySetIsCrouching(false)) {
                    setMovementSpeed(charSpeedBeforeCrouched);
                }
            }
            pCameraNode->setRelativeLocation(
                glm::vec3(0.0F, 0.0F, getCameraRelativeHeight(getBodyShape().getHalfHeight())));
        }};

    getAxisEventBindings()[GameInputEventIds::Axis::MOVE_FORWARD] =
        [&](KeyboardModifiers modifiers, float input) { setForwardMovementInput(input); };
    getAxisEventBindings()[GameInputEventIds::Axis::MOVE_RIGHT] =
        [&](KeyboardModifiers modifiers, float input) { setRightMovementInput(input); };

    pCameraNode = addChildNode(std::make_unique<CameraNode>());
    pCameraNode->setSerialize(false); // don't serialize, same as the parent node
}

float MyCharacterNode::getCameraRelativeHeight(float characterCapsuleHalfHeight) {
    return characterCapsuleHalfHeight * 2.1F;
}

void MyCharacterNode::onChildNodesSpawned() {
    SimpleCharacterBodyNode::onChildNodesSpawned();

    pCameraNode->setRelativeLocation(
        glm::vec3(0.0F, 0.0F, getCameraRelativeHeight(getBodyShape().getHalfHeight())));
    pCameraNode->makeActive();

    // Capture mouse cursor.
    const auto pWindow = getGameInstanceWhileSpawned()->getWindow();
    bWasCursorVisible = pWindow->isMouseCursorVisible();
    pWindow->setIsMouseCursorVisible(false);
}

void MyCharacterNode::onDespawning() {
    SimpleCharacterBodyNode::onDespawning();

    // Restore mouse cursor visibility.
    getGameInstanceWhileSpawned()->getWindow()->setIsMouseCursorVisible(bWasCursorVisible);
}

void MyCharacterNode::onMouseMove(double xOffset, double yOffset) {
    SimpleCharacterBodyNode::onMouseMove(xOffset, yOffset);

    applyLookInput(static_cast<float>(xOffset), static_cast<float>(yOffset));
}

void MyCharacterNode::applyLookInput(float xDelta, float yDelta) {
    // Rotate the character's body around local Z (up).
    auto rotation = getRelativeRotation();
    rotation.z += xDelta * rotationSensitivity;
    setRelativeRotation(rotation);

    // Rotate the camera around local X (right).
    rotation = pCameraNode->getRelativeRotation();
    rotation.y += yDelta * rotationSensitivity;
    pCameraNode->setRelativeRotation(rotation);
}
