#pragma once
#include "point.hpp"

enum GestureType
{
    Tap,
    TapLeft,
    TapRight,
    TapUp,
    TapDown,
    TapHold,
    SwipeLeft,
    SwipeLeftLong,
    SwipeRight,
    SwipeRightLong,
    SwipeUp,
    SwipeUpLong,
    SwipeDown,
    SwipeDownLong,
    TwoTap,
    TwoTapWide
};

enum GestureAction
{
    Enable,
    Disable,
    ToggleEnabled,
    NextPage,
    PreviousPage,
    ShowClock,
    Suspend
};

struct Tap
{
    struct Point p;
    int hold_time;
};
struct TwoTap
{
    struct Point start;
    struct Point end;
};
struct Swipe
{
    struct Point start;
    struct Point end;
    int velocity;
};
union GestureData {
    struct TwoTap twotap;
    struct Swipe swipe;
};

struct Gesture
{
    enum GestureType type;
    enum GestureAction action;
    union GestureData data;
};
