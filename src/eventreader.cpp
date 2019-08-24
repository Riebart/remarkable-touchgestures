#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <linux/input.h>

#include "include/gestures.hpp"
#include "include/eventreader.hpp"
#include "include/ui.hpp"

struct Finger
{
    int x;
    int y;
    int raw_x;
    int raw_y;
    enum FingerStatus status;
    int track_id;
};

void process_touch(class GestureRecognizer **recognizers, int num_recognizers)
{
    struct Finger fingers[MAX_SLOTS + 1];

    struct input_event evt;

    int slot = 0;
    int x, y = 0;
    int scr_width = SCREEN_WIDTH;
    int scr_height = SCREEN_HEIGHT;
    int width = TOUCH_WIDTH;
    int height = TOUCH_HEIGHT;

    memset(fingers, 0, sizeof(fingers));

    int touchscreen = open(TOUCHSCREEN, O_RDONLY);
    if (!touchscreen)
    {
        fprintf(stderr, "cannot open touchscreen");
        exit(1);
    }

    while (read(touchscreen, &evt, sizeof(evt)))
    {
        if (evt.type == EV_ABS)
        {
            if (evt.code == ABS_MT_SLOT)
            {
                slot = evt.value;
                if (slot >= MAX_SLOTS)
                {
                    slot = MAX_SLOTS; //sink
                }
                if (fingers[slot].status)
                {
                    fingers[slot].status = Move;
                }
                else
                {
                    fingers[slot].status = Down;
                }
            }
            else if (evt.code == ABS_MT_POSITION_X)
            {
                float pos = width - 1 - evt.value;
                x = pos / width * scr_width;
                fingers[slot].x = x;
                fingers[slot].raw_x = evt.value;
            }
            else if (evt.code == ABS_MT_POSITION_Y)
            {

                float pos = height - 1 - evt.value;
                y = pos / height * scr_height;
                fingers[slot].y = y;
                fingers[slot].raw_y = evt.value;
            }
            else if (evt.code == ABS_MT_TRACKING_ID && evt.value == -1)
            {
                /* printf("slot %d tracking %d\n",slot, evt.value); */
                if (fingers[slot].status)
                {
                    fingers[slot].status = Up;
                }
            }
            else if (evt.code == ABS_MT_TRACKING_ID)
            {
                if (slot == 0)
                {
                    /* printf("TRACK\n"); */
                    fingers[slot].status = Down;
                }
                if (fingers[slot].status)
                {
                    fingers[slot].track_id = evt.value;
                }
            }
            else
            {
                /* printf("Uknown %d %d\n", evt.code, evt.value); */
            }
        }
        else if (evt.type == EV_SYN && evt.code == SYN_REPORT)
        {
            /* printf("SYN slot %d\n", slot); */
            struct Finger *f;
            for (int i = 0; i < MAX_SLOTS; i++)
            {
                f = &fingers[i];
                if (f->status)
                {

                    struct TouchEvent event;
                    event.time = evt.time.tv_sec;
                    event.slot = i; //enumerating slots
                    event.x = f->x;
                    event.y = f->y;
                    event.raw_position.x = f->raw_x;
                    event.raw_position.y = f->raw_y;
                    event.status = f->status;

                    for (int r_i = 0; r_i < num_recognizers; r_i++)
                    {
                        recognizers[r_i]->recognize_gestures(&event);
                    }

                    if (f->status == Down)
                    {
                        f->status = Move;
                    }

                    if (f->status == Up)
                    {
                        f->status = Untracked;
                        ;
                    }
                }
            }
        }
    }
    close(touchscreen);
}
