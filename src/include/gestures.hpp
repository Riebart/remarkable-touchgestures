#pragma once
#include "eventreader.hpp"

struct Gesture;

struct Segment
{
    struct Point start;
    struct Point end;
};

class GestureRecognizer
{
    // Since gesture recognizers require state of slots, promote the function call to a class
    // so that individual recognizers can have their own state management.
public:
    GestureRecognizer();
    virtual void recognize_gestures(struct TouchEvent *){};

protected:
    // Take care of the boilerplate of noting when a tap starts.
    int tap_down(struct TouchEvent *);

    struct Segment *segments;
    int keys_down = 0;
    int segment_count = 0;
};

class MultiTapShortRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *);
};

class MultiTapWideRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *);
};

class LeftTapShortRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *);
};

class LeftTapLongRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *) { throw "NotImplemented"; };
};

class LeftSwipeShortRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *);
};

class LeftSwipeLongRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *);
};

class RightTapShortRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *);
};

class RightTapLongRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *) { throw "NotImplemented"; };
};

class RightSwipeShortRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *);
};

class RightSwipeLongRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *);
};

class UpTapShortRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *);
};

class UpTapLongRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *) { throw "NotImplemented"; };
};

class UpSwipeShortRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *);
};

class UpSwipeLongRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *);
};

class DownTapShortRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *);
};

class DownTapLongRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *) { throw "NotImplemented"; };
};

class DownSwipeShortRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *);
};

class DownSwipeLongRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *);
};

void recognize_tap(struct TouchEvent *);
void recognize_shortswipe(struct TouchEvent *);
void recognize_longswipe(struct TouchEvent *);

void interpret_gesture(struct Gesture *);
