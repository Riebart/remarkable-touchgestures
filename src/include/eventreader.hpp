#pragma once
#include "point.hpp"
#include "gestures.hpp"

enum FingerStatus
{
    Untracked = 0,
    Down,
    Up,
    Move
};

struct TouchEvent
{
    int x, y, slot;
    struct Point position;
    struct Point raw_position;

    long time;
    enum FingerStatus status;
};

void process_touch(class GestureRecognizer **recognizers, int num_recognizers);
