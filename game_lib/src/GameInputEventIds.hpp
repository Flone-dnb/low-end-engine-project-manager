#pragma once

/** Stores unique IDs of input events. */
struct GameInputEventIds {
    /** Groups action events. */
    enum Action : unsigned char {
        JUMP = 0,
        CROUCH,
    };

    /** Groups axis events. */
    enum Axis : unsigned char {
        MOVE_FORWARD = 0, //< Move forward/back.
        MOVE_RIGHT,       //< Move right/left.
    };
};