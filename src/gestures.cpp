#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <math.h>

#include "include/ui.hpp"
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

// void RecognizeAll::recognize_gestures(struct TouchEvent *event)
// {
//     int slot = tap_down(event);

//     if (event->status == Up)
//     {
//         debug_print("slot %d up x:%d, y:%d segments:%d \n", slot, event->x, event->y, segment_count);
//         int x = event->x;
//         int y = event->y;

//         keys_down--;
//         segments[slot].end.x = x;
//         segments[slot].end.y = y;

//         if (keys_down == 0)
//         {
//             struct Gesture gesture;
//             struct Segment *p = segments;
//             int dx, dy, distance = 0;
//             switch (segment_count)
//             {
//             //single tap
//             case 1:
//                 printf("Tap  x:%d, y:%d  raw_x:%d, raw_y:%d\n", event->x, event->y, event->raw_position.x, event->raw_position.y);
//                 dx = p->end.x - p->start.x;
//                 dy = p->end.y - p->start.y;

//                 if (abs(dx) < JITTER &&
//                     abs(dy) < JITTER)
//                 {

//                     int nav_stripe = SCREEN_WIDTH / 3;
//                     if (y > 100)
//                     { //disable upper stripe
//                         if (x < nav_stripe)
//                         {
//                             if (y > SCREEN_HEIGHT - 150 && x < 150)
//                             {
//                                 printf("TOC\n");
//                             }
//                             else
//                             {

//                                 gesture.type = TapLeft;
//                                 interpret_gesture(&gesture);
//                             }
//                         }
//                         else if (x > nav_stripe * 2)
//                         {
//                             gesture.type = TapRight;
//                             interpret_gesture(&gesture);
//                         }
//                     }
//                 }
//                 else
//                 {
//                     //swipe
//                     if (abs(dx) > abs(dy))
//                     {
//                         //horizontal
//                         if (dx < 0)
//                         {
//                             printf("swipe left\n");
//                             gesture.type = SwipeLeft;
//                             interpret_gesture(&gesture);
//                         }
//                         else
//                         {
//                             printf("swipe right\n");
//                             gesture.type = SwipeRight;
//                             interpret_gesture(&gesture);
//                         }
//                     }
//                     else
//                     {
//                         //vertical
//                         if (dy > 0 && dy > 600)
//                         {
//                             gesture.type = SwipeDownLong;
//                             interpret_gesture(&gesture);
//                         }
//                         else if (dy < 0 && dy < -600)
//                         {
//                             gesture.type = SwipeUpLong;
//                             interpret_gesture(&gesture);
//                         }
//                     }
//                 }
//                 break;
//             case 2:
//                 dx = segments[0].end.x - segments[1].end.x;
//                 dy = segments[0].end.y - segments[1].end.y;
//                 distance = sqrt(dx * dx + dy * dy);
//                 if (distance > MULTITOUCH_DISTANCE)
//                 {
//                     gesture.type = TwoTapWide;
//                     interpret_gesture(&gesture);
//                 }
//                 break;
//             default:
//                 printf("%d finger tap\n", segment_count);
//                 break;
//             }
//             segment_count = 0;
//         }
//     }
// }

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

void LeftTapShortRecognizer::recognize_gestures(struct TouchEvent *event)
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
            struct Segment *p = segments;
            int dx, dy = 0;
            if (segment_count == 1)
            {
                printf("Tap  x:%d, y:%d  raw_x:%d, raw_y:%d\n", event->x, event->y, event->raw_position.x, event->raw_position.y);
                dx = p->end.x - p->start.x;
                dy = p->end.y - p->start.y;

                if (((abs(dx) < JITTER) && (abs(dy) < JITTER)) &&
                    (y > 100) &&
                    (x < (SCREEN_WIDTH / 3)) &&
                    ((y < (SCREEN_HEIGHT - 150)) ||
                     (x > 150)))
                {
                    printf("Left Tap Short");
                    gesture.type = TapLeft;
                    interpret_gesture(&gesture);
                }
            }
            segment_count = 0;
        }
    }
}

void RightTapShortRecognizer::recognize_gestures(struct TouchEvent *event)
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
            struct Segment *p = segments;
            int dx, dy = 0;
            if (segment_count == 1)
            {
                printf("Tap  x:%d, y:%d  raw_x:%d, raw_y:%d\n", event->x, event->y, event->raw_position.x, event->raw_position.y);
                dx = p->end.x - p->start.x;
                dy = p->end.y - p->start.y;

                if (((abs(dx) < JITTER) && (abs(dy) < JITTER)) &&
                    (y > 100) &&
                    (x > (SCREEN_WIDTH / 3 * 2)))
                {
                    printf("Right Tap Short");
                    gesture.type = TapRight;
                    interpret_gesture(&gesture);
                }
            }
            segment_count = 0;
        }
    }
}

void UpTapShortRecognizer::recognize_gestures(struct TouchEvent *event)
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
            struct Segment *p = segments;
            int dx, dy = 0;
            if (segment_count == 1)
            {
                printf("Tap  x:%d, y:%d  raw_x:%d, raw_y:%d\n", event->x, event->y, event->raw_position.x, event->raw_position.y);
                dx = p->end.x - p->start.x;
                dy = p->end.y - p->start.y;

                if (((abs(dx) < JITTER) && (abs(dy) < JITTER)) &&
                    (x > 100) &&
                    (y < (SCREEN_WIDTH / 3)))
                {
                    printf("Up Tap Short");
                    gesture.type = TapUp;
                    interpret_gesture(&gesture);
                }
            }
            segment_count = 0;
        }
    }
}

void DownTapShortRecognizer::recognize_gestures(struct TouchEvent *event)
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
            struct Segment *p = segments;
            int dx, dy = 0;
            if (segment_count == 1)
            {
                printf("Tap  x:%d, y:%d  raw_x:%d, raw_y:%d\n", event->x, event->y, event->raw_position.x, event->raw_position.y);
                dx = p->end.x - p->start.x;
                dy = p->end.y - p->start.y;

                if (((abs(dx) < JITTER) && (abs(dy) < JITTER)) &&
                    (x > 100) &&
                    (y > (SCREEN_WIDTH / 3 * 2)))
                {
                    printf("Down Tap Short");
                    gesture.type = TapDown;
                    interpret_gesture(&gesture);
                }
            }
            segment_count = 0;
        }
    }
}

void LeftSwipeShortRecognizer::recognize_gestures(struct TouchEvent *event)
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
            struct Segment *p = segments;
            int dx, dy = 0;

            if (segment_count == 1)
            {
                printf("Tap  x:%d, y:%d  raw_x:%d, raw_y:%d\n", event->x, event->y, event->raw_position.x, event->raw_position.y);
                dx = p->end.x - p->start.x;
                dy = p->end.y - p->start.y;

                if (((abs(dx) > JITTER) || (abs(dy) < JITTER)) &&
                    (abs(dx) > abs(dy)) &&
                    (dx < 0) &&
                    (dx > -LONG_HORIZONTAL_SWIPE_LENGTH))
                {
                    printf("Left Swipe Short\n");
                    gesture.type = SwipeLeft;
                    interpret_gesture(&gesture);
                }
            }
            segment_count = 0;
        }
    }
}

void LeftSwipeLongRecognizer::recognize_gestures(struct TouchEvent *event)
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
            struct Segment *p = segments;
            int dx, dy = 0;

            if (segment_count == 1)
            {
                printf("Tap  x:%d, y:%d  raw_x:%d, raw_y:%d\n", event->x, event->y, event->raw_position.x, event->raw_position.y);
                dx = p->end.x - p->start.x;
                dy = p->end.y - p->start.y;

                if (((abs(dx) > JITTER) || (abs(dy) < JITTER)) &&
                    (abs(dx) > abs(dy)) &&
                    (dx < 0) &&
                    (dx < -LONG_HORIZONTAL_SWIPE_LENGTH))
                {
                    printf("Left Swipe Long\n");
                    gesture.type = SwipeLeftLong;
                    interpret_gesture(&gesture);
                }
            }
            segment_count = 0;
        }
    }
}

void RightSwipeShortRecognizer::recognize_gestures(struct TouchEvent *event)
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
            struct Segment *p = segments;
            int dx, dy = 0;

            if (segment_count == 1)
            {
                printf("Tap  x:%d, y:%d  raw_x:%d, raw_y:%d\n", event->x, event->y, event->raw_position.x, event->raw_position.y);
                dx = p->end.x - p->start.x;
                dy = p->end.y - p->start.y;

                if (((abs(dx) > JITTER) || (abs(dy) < JITTER)) &&
                    (abs(dx) > abs(dy)) &&
                    (dx > 0) &&
                    (dx < LONG_HORIZONTAL_SWIPE_LENGTH))
                {
                    printf("Right Swipe Short\n");
                    gesture.type = SwipeRight;
                    interpret_gesture(&gesture);
                }
            }
            segment_count = 0;
        }
    }
}

void RightSwipeLongRecognizer::recognize_gestures(struct TouchEvent *event)
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
            struct Segment *p = segments;
            int dx, dy = 0;

            if (segment_count == 1)
            {
                printf("Tap  x:%d, y:%d  raw_x:%d, raw_y:%d\n", event->x, event->y, event->raw_position.x, event->raw_position.y);
                dx = p->end.x - p->start.x;
                dy = p->end.y - p->start.y;

                if (((abs(dx) > JITTER) || (abs(dy) < JITTER)) &&
                    (abs(dx) > abs(dy)) &&
                    (dx > 0) &&
                    (dx > LONG_HORIZONTAL_SWIPE_LENGTH))
                {
                    printf("Right Swipe Long\n");
                    gesture.type = SwipeRightLong;
                    interpret_gesture(&gesture);
                }
            }
            segment_count = 0;
        }
    }
}

void UpSwipeShortRecognizer::recognize_gestures(struct TouchEvent *event)
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
            struct Segment *p = segments;
            int dx, dy = 0;

            if (segment_count == 1)
            {
                printf("Tap  x:%d, y:%d  raw_x:%d, raw_y:%d\n", event->x, event->y, event->raw_position.x, event->raw_position.y);
                dx = p->end.x - p->start.x;
                dy = p->end.y - p->start.y;

                if (((abs(dx) > JITTER) || (abs(dy) < JITTER)) &&
                    (abs(dy) > abs(dx)) &&
                    (dy < 0) &&
                    (dy > -LONG_VERTICAL_SWIPE_LENGTH))
                {
                    printf("Up Swipe Short\n");
                    gesture.type = SwipeUp;
                    interpret_gesture(&gesture);
                }
            }
            segment_count = 0;
        }
    }
}

void UpSwipeLongRecognizer::recognize_gestures(struct TouchEvent *event)
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
            struct Segment *p = segments;
            int dx, dy = 0;

            if (segment_count == 1)
            {
                printf("Tap  x:%d, y:%d  raw_x:%d, raw_y:%d\n", event->x, event->y, event->raw_position.x, event->raw_position.y);
                dx = p->end.x - p->start.x;
                dy = p->end.y - p->start.y;

                if (((abs(dx) > JITTER) || (abs(dy) < JITTER)) &&
                    (abs(dy) > abs(dx)) &&
                    (dy < 0) &&
                    (dy < -LONG_VERTICAL_SWIPE_LENGTH))
                {
                    printf("Up Swipe Long\n");
                    gesture.type = SwipeUpLong;
                    interpret_gesture(&gesture);
                }
            }
            segment_count = 0;
        }
    }
}

void DownSwipeShortRecognizer::recognize_gestures(struct TouchEvent *event)
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
            struct Segment *p = segments;
            int dx, dy = 0;

            if (segment_count == 1)
            {
                printf("Tap  x:%d, y:%d  raw_x:%d, raw_y:%d\n", event->x, event->y, event->raw_position.x, event->raw_position.y);
                dx = p->end.x - p->start.x;
                dy = p->end.y - p->start.y;

                if (((abs(dx) > JITTER) || (abs(dy) < JITTER)) &&
                    (abs(dy) > abs(dx)) &&
                    (dy > 0) &&
                    (dy < LONG_VERTICAL_SWIPE_LENGTH))
                {
                    printf("Down Swipe Short\n");
                    gesture.type = SwipeDown;
                    interpret_gesture(&gesture);
                }
            }
            segment_count = 0;
        }
    }
}

void DownSwipeLongRecognizer::recognize_gestures(struct TouchEvent *event)
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
            struct Segment *p = segments;
            int dx, dy = 0;

            if (segment_count == 1)
            {
                printf("Tap  x:%d, y:%d  raw_x:%d, raw_y:%d\n", event->x, event->y, event->raw_position.x, event->raw_position.y);
                dx = p->end.x - p->start.x;
                dy = p->end.y - p->start.y;

                if (((abs(dx) > JITTER) || (abs(dy) < JITTER)) &&
                    (abs(dy) > abs(dx)) &&
                    (dy > 0) &&
                    (dy > LONG_VERTICAL_SWIPE_LENGTH))
                {
                    printf("Down Swipe Long\n");
                    gesture.type = SwipeDownLong;
                    interpret_gesture(&gesture);
                }
            }
            segment_count = 0;
        }
    }
}
