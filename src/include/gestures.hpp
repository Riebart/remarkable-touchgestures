#pragma once

#include <string>

#include "gesture_definition.hpp"
#include "eventreader.hpp"
#include "ui.hpp"

#define RECOGNIZER_CLASS(name, class_gesture_type)        \
    class name : GestureRecognizer                        \
    {                                                     \
    public:                                               \
        name() : GestureRecognizer()                      \
        {                                                 \
            gesture_string = #name;                       \
            type = class_gesture_type;                    \
        }                                                 \
                                                          \
        void recognize_gestures(struct TouchEvent *event) \
        {                                                 \
            GestureRecognizer::recognize_gestures(event); \
        }                                                 \
                                                          \
    protected:                                            \
        bool cond(int, int, int, int);                    \
    };

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
    virtual void recognize_gestures(struct TouchEvent *event) { single_digit_recognizer(event); }

protected:
    GestureAction action;
    GestureType type;
    const char *gesture_string = "GestureAction";
    struct Segment *segments;
    int keys_down = 0;
    int segment_count = 0;

    // A condition of whether the gesture occurred, based on the position of the touch currently
    // and the change in position since the touch started.
    virtual bool cond(int, int, int, int) { return false; }

    // Take care of the boilerplate of noting when a tap starts.
    int tap_down(struct TouchEvent *);

    bool single_digit_recognizer(struct TouchEvent *);
};

class MultiTapShortRecognizer : GestureRecognizer
{
public:
    MultiTapShortRecognizer() : GestureRecognizer()
    {
        gesture_string = "MultiTapShortRecognizer";
    }

    void recognize_gestures(struct TouchEvent *event);
};

class MultiTapWideRecognizer : GestureRecognizer
{
public:
    MultiTapWideRecognizer() : GestureRecognizer()
    {
        gesture_string = "MultiTapWideRecognizer";
    }

    void recognize_gestures(struct TouchEvent *event);
};

RECOGNIZER_CLASS(LeftTapShortRecognizer, GestureType::TapLeft)
RECOGNIZER_CLASS(LeftSwipeShortRecognizer, GestureType::SwipeLeft)
RECOGNIZER_CLASS(LeftSwipeLongRecognizer, GestureType::SwipeLeftLong)
RECOGNIZER_CLASS(RightTapShortRecognizer, GestureType::TapRight)
RECOGNIZER_CLASS(RightSwipeShortRecognizer, GestureType::SwipeRight)
RECOGNIZER_CLASS(RightSwipeLongRecognizer, GestureType::SwipeRightLong)
RECOGNIZER_CLASS(UpTapShortRecognizer, GestureType::TapUp)
RECOGNIZER_CLASS(UpSwipeShortRecognizer, GestureType::SwipeUp)
RECOGNIZER_CLASS(UpSwipeLongRecognizer, GestureType::SwipeUpLong)
RECOGNIZER_CLASS(DownTapShortRecognizer, GestureType::TapDown)
RECOGNIZER_CLASS(DownSwipeShortRecognizer, GestureType::SwipeDown)
RECOGNIZER_CLASS(DownSwipeLongRecognizer, GestureType::SwipeDownLong)

class LeftTapLongRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *) { throw "NotImplemented"; };
};

class RightTapLongRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *) { throw "NotImplemented"; };
};

class UpTapLongRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *) { throw "NotImplemented"; };
};

class DownTapLongRecognizer : GestureRecognizer
{
public:
    void recognize_gestures(struct TouchEvent *) { throw "NotImplemented"; };
};
