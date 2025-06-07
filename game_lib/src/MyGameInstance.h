#pragma once

// Custom.
#include "game/GameInstance.h"

class Window;

/** Game's game instance. */
class MyGameInstance : public GameInstance {
public:
    /**
     * Creates a new game instance.
     *
     * @param pWindow Do not delete (free). Valid pointer to the game's window.
     */
    MyGameInstance(Window* pWindow);

    /** Register custom game types in the reflection database. */
    static void registerGameTypes();

    virtual ~MyGameInstance() override = default;

protected:
    /**
     * Called after GameInstance's constructor is finished and created
     * GameInstance object was saved in GameManager (that owns GameInstance).
     *
     * At this point you can create and interact with the game world and etc.
     */
    virtual void onGameStarted() override;
};
