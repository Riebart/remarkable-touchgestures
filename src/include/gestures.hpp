#pragma once

#include <string>

#include "gesture_definition.hpp"
#include "eventreader.hpp"
#include "ui.hpp"

#define RECOGNIZER_CLASS(name)                                 \
    class name : GestureRecognizer                             \
    {                                                          \
    public:                                                    \
        name(GestureAction action) : GestureRecognizer(action) \
        {                                                      \
            gesture_string = #name;                            \
        }                                                      \
                                                               \
        void recognize_gestures(struct TouchEvent *event)      \
        {                                                      \
            GestureRecognizer::recognize_gestures(event);      \
        }                                                      \
                                                               \
    protected:                                                 \
        bool cond(int, int, int, int);                         \
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
    GestureRecognizer(GestureAction);
    virtual void recognize_gestures(struct TouchEvent *event) { single_digit_recognizer(event); }

protected:
    GestureAction action;
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

class MultiTapNarrowRecognizer : GestureRecognizer
{
public:
    MultiTapNarrowRecognizer(GestureAction action) : GestureRecognizer(action)
    {
        gesture_string = "MultiTapNarrowRecognizer";
    }

    void recognize_gestures(struct TouchEvent *event);
};

class MultiTapWideRecognizer : GestureRecognizer
{
public:
    MultiTapWideRecognizer(GestureAction action) : GestureRecognizer(action)
    {
        gesture_string = "MultiTapWideRecognizer";
    }

    void recognize_gestures(struct TouchEvent *event);
};

RECOGNIZER_CLASS(LeftTapShortRecognizer);
RECOGNIZER_CLASS(LeftSwipeShortRecognizer);
RECOGNIZER_CLASS(LeftSwipeLongRecognizer);
RECOGNIZER_CLASS(RightTapShortRecognizer);
RECOGNIZER_CLASS(RightSwipeShortRecognizer);
RECOGNIZER_CLASS(RightSwipeLongRecognizer);
RECOGNIZER_CLASS(UpTapShortRecognizer);
RECOGNIZER_CLASS(UpSwipeShortRecognizer);
RECOGNIZER_CLASS(UpSwipeLongRecognizer);
RECOGNIZER_CLASS(DownTapShortRecognizer);
RECOGNIZER_CLASS(DownSwipeShortRecognizer);
RECOGNIZER_CLASS(DownSwipeLongRecognizer);

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
