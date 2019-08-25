#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <math.h>

#include "include/gestures.hpp"
#include "include/keyinjector.hpp"
#include "include/gesture_definition.hpp"

GestureRecognizer::GestureRecognizer()
{
    this->segments = (struct Segment *)calloc(MAX_SLOTS + 1, sizeof(Segment));
}

int GestureRecognizer::tap_down(struct TouchEvent *event)
{
    int slot = event->slot;
    if (event->status == Down)
    {
        debug_print("slot %d down x:%d, y:%d  \n", slot, event->x, event->y);

        if (keys_down < 0)
            keys_down = 0; //todo: fixit
        segments[slot].start.x = event->x;
        segments[slot].start.y = event->y;

        keys_down++;
        segment_count++;
    }
    return slot;
}

void MultiTapShortRecognizer::recognize_gestures(struct TouchEvent *event)
{
    int slot = tap_down(event);

    if (event->status == Up)
    {
        debug_print("slot %d up x:%d, y:%d segments:%d \n", slot, event->x, event->y, segment_count);
        int x = event->x;
        int y = event->y;

        keys_down--;
        segments[slot].end.x = x;
        segments[slot].end.y = y;

        if (keys_down == 0)
        {
            struct Gesture gesture;
            if (segment_count > 1)
            {
                switch (segment_count)
                {
                case 2:
                    printf("%d finger tap\n", segment_count);
                    gesture.type = TwoTap;
                    interpret_gesture(&gesture);
                    break;
                default:
                    printf("%d finger tap (ignored)\n", segment_count);
                    break;
                }
            }
            segment_count = 0;
        }
    }
}

void MultiTapWideRecognizer::recognize_gestures(struct TouchEvent *event)
{
    int slot = tap_down(event);

    if (event->status == Up)
    {
        debug_print("slot %d up x:%d, y:%d segments:%d \n", slot, event->x, event->y, segment_count);
        int x = event->x;
        int y = event->y;

        keys_down--;
        segments[slot].end.x = x;
        segments[slot].end.y = y;

        if (keys_down == 0)
        {
            struct Gesture gesture;
            int dx, dy, distance = 0;
            if (segment_count > 1)
            {
                switch (segment_count)
                {
                case 2:
                    dx = segments[0].end.x - segments[1].end.x;
                    dy = segments[0].end.y - segments[1].end.y;
                    distance = sqrt(dx * dx + dy * dy);
                    if (distance > MULTITOUCH_DISTANCE)
                    {
                        printf("%d wide finger tap\n", segment_count);
                        gesture.type = TwoTapWide;
                        interpret_gesture(&gesture);
                    }
                    break;
                default:
                    printf("%d finger tap (wide) ignored\n", segment_count);
                    break;
                }
            }
            segment_count = 0;
        }
    }
}

bool GestureRecognizer::single_digit_recognizer(struct TouchEvent *event)
{
    bool ret = false;
    int slot = event->slot;
    if (event->status == Down)
    {
        debug_print("slot %d down x:%d, y:%d  \n", slot, event->x, event->y);

        if (keys_down < 0)
        {
            keys_down = 0; //todo: fixit
        }

        segments[slot].start.x = event->x;
        segments[slot].start.y = event->y;

        keys_down++;
        segment_count++;
    }
    else if (event->status == Up)
    {
        debug_print("slot %d up x:%d, y:%d segments:%d \n", slot, event->x, event->y, segment_count);
        int x = event->x;
        int y = event->y;

        keys_down--;
        segments[slot].end.x = x;
        segments[slot].end.y = y;

        if (keys_down == 0)
        {
            struct Gesture gesture;
            struct Segment *p = segments;
            int dx, dy = 0;
            if (segment_count == 1)
            {
                // printf("Tap  x:%d, y:%d  raw_x:%d, raw_y:%d\n", event->x, event->y, event->raw_position.x, event->raw_position.y);
                dx = p->end.x - p->start.x;
                dy = p->end.y - p->start.y;

                if (cond(x, y, dx, dy))
                {
                    ret = true;
                    printf("Recognized gesture %s\n", gesture_string);
                    gesture.type = type;
                    printf("%d\n", type);
                    gesture.action = action;
                    interpret_gesture(&gesture);
                }
            }
            segment_count = 0;
        }
    }
    return ret;
}

bool LeftTapShortRecognizer::cond(int x, int y, int dx, int dy)
{
    return ((abs(dx) < JITTER) && (abs(dy) < JITTER)) &&
           (y > 100) &&
           (x < (SCREEN_WIDTH / 3)) &&
           ((y < (SCREEN_HEIGHT - 150)) ||
            (x > 150));
}

bool LeftSwipeShortRecognizer::cond(int, int, int dx, int dy)
{
    return ((abs(dx) > JITTER) || (abs(dy) > JITTER)) &&
           (abs(dx) > abs(dy)) &&
           (dx < 0) &&
           (dx > -LONG_HORIZONTAL_SWIPE_LENGTH);
}

bool LeftSwipeLongRecognizer::cond(int, int, int dx, int dy)
{
    return ((abs(dx) > JITTER) || (abs(dy) > JITTER)) &&
           (abs(dx) > abs(dy)) &&
           (dx < 0) &&
           (dx < -LONG_HORIZONTAL_SWIPE_LENGTH);
}

bool RightTapShortRecognizer::cond(int x, int y, int dx, int dy)
{
    return ((abs(dx) < JITTER) && (abs(dy) < JITTER)) &&
           (y > 100) &&
           (x > (SCREEN_WIDTH / 3 * 2));
}

bool RightSwipeShortRecognizer::cond(int, int, int dx, int dy)
{
    return ((abs(dx) > JITTER) || (abs(dy) > JITTER)) &&
           (abs(dx) > abs(dy)) &&
           (dx > 0) &&
           (dx < LONG_HORIZONTAL_SWIPE_LENGTH);
}

bool RightSwipeLongRecognizer::cond(int, int, int dx, int dy)
{
    return ((abs(dx) > JITTER) || (abs(dy) > JITTER)) &&
           (abs(dx) > abs(dy)) &&
           (dx > 0) &&
           (dx > LONG_HORIZONTAL_SWIPE_LENGTH);
}

bool UpTapShortRecognizer::cond(int x, int y, int dx, int dy)
{
    return ((abs(dx) < JITTER) && (abs(dy) < JITTER)) &&
           (x > 100) &&
           (y < (SCREEN_HEIGHT / 3));
}

bool UpSwipeShortRecognizer::cond(int, int, int dx, int dy)
{
    return ((abs(dx) > JITTER) || (abs(dy) > JITTER)) &&
           (abs(dy) > abs(dx)) &&
           (dy < 0) &&
           (dy > -LONG_VERTICAL_SWIPE_LENGTH);
}

bool UpSwipeLongRecognizer::cond(int, int, int dx, int dy)
{
    return ((abs(dx) > JITTER) || (abs(dy) > JITTER)) &&
           (abs(dy) > abs(dx)) &&
           (dy < 0) &&
           (dy < -LONG_VERTICAL_SWIPE_LENGTH);
}

bool DownTapShortRecognizer::cond(int x, int y, int dx, int dy)
{
    return ((abs(dx) < JITTER) && (abs(dy) < JITTER)) &&
           (x > 100) &&
           (y > (SCREEN_HEIGHT / 3 * 2));
}

bool DownSwipeShortRecognizer::cond(int, int, int dx, int dy)
{
    return ((abs(dx) > JITTER) || (abs(dy) > JITTER)) &&
           (abs(dy) > abs(dx)) &&
           (dy > 0) &&
           (dy < LONG_VERTICAL_SWIPE_LENGTH);
}

bool DownSwipeLongRecognizer::cond(int, int, int dx, int dy)
{
    return ((abs(dx) > JITTER) || (abs(dy) > JITTER)) &&
           (abs(dy) > abs(dx)) &&
           (dy > 0) &&
           (dy > LONG_VERTICAL_SWIPE_LENGTH);
}
