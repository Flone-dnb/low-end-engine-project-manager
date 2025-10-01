#pragma once

// Custom.
#include "game/node/physics/SimpleCharacterBodyNode.h"

class CameraNode;

class MyCharacterNode : public SimpleCharacterBodyNode {
public:
    MyCharacterNode();

    /**
     * Creates a new node with the specified name.
     *
     * @param sNodeName Name of this node.
     */
    MyCharacterNode(const std::string& sNodeName);

    virtual ~MyCharacterNode() override = default;

    /**
     * Returns reflection info about this type.
     *
     * @return Type reflection.
     */
    static TypeReflectionInfo getReflectionInfo();

    /**
     * Returns GUID of the type, this GUID is used to retrieve reflection information from the reflected type
     * database.
     *
     * @return GUID.
     */
    static std::string getTypeGuidStatic();

    /**
     * Returns GUID of the type, this GUID is used to retrieve reflection information from the reflected type
     * database.
     *
     * @return GUID.
     */
    virtual std::string getTypeGuid() const override;

protected:
    /**
     * Called after @ref onSpawning when this node and all of node's child nodes (at the moment
     * of spawning) were spawned.
     *
     * @warning If overriding you must call the parent's version of this function first
     * (before executing your logic) to execute parent's logic.
     *
     * @remark Generally you might want to prefer to use @ref onSpawning, this function
     * is mostly used to do some logic related to child nodes after all child nodes were spawned
     * (for example if you have a camera child node you can make it active in this function).
     */
    virtual void onChildNodesSpawned() override;

    /**
     * Called before this node is despawned from the world to execute custom despawn logic.
     *
     * @warning If overriding you must call the parent's version of this function first
     * (before executing your logic) to execute parent's logic.
     *
     * @remark This node will be marked as despawned after this function is called.
     * @remark This function is called after all child nodes were despawned.
     * @remark @ref getSpawnDespawnMutex is locked while this function is called.
     */
    virtual void onDespawning() override;

    /**
     * Called when the window received mouse movement.
     *
     * @remark This function will only be called while this node is spawned.
     *
     * @param xOffset  Mouse X movement delta in pixels (plus if moved to the right,
     * minus if moved to the left).
     * @param yOffset  Mouse Y movement delta in pixels (plus if moved up,
     * minus if moved down).
     */
    virtual void onMouseMove(double xOffset, double yOffset) override;

private:
    /**
     * Returns relative height that @ref pCameraNode should have.
     *
     * @return Height.
     */
    static float getCameraRelativeHeight(float characterCapsuleHalfHeight);

    /**
     * Applies rotation to the camera.
     *
     * @param xDelta Horizontal rotation.
     * @param yDelta Vertical rotation.
     */
    void applyLookInput(float xDelta, float yDelta);

    /** Rotation multiplier for the camera. */
    float rotationSensitivity = 0.075F;

    /** Character speed before crouched. */
    float charSpeedBeforeCrouched = 0.0F;

    /** Child camera node. */
    CameraNode* pCameraNode = nullptr;

    /** State of the mouse cursor when the character spawned. */
    bool bWasCursorVisible = false;
};