#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <time.h>

#include "include/ui.hpp"
#include "include/keyinjector.hpp"
#include "include/gestures.hpp"
#include "include/gesture_definition.hpp"

enum Key
{
    Left = 105,
    Right = 106,
    Home = 102,
    Power = 116
};

static bool touch_enabled = false;
int f;
int w;
void injector_init()
{
    f = open(BUTTONS, O_WRONLY);
    if (!f)
    {
        fprintf(stderr, "cannot open buttons");
        exit(1);
    }

    /* w = open(WACOM, O_WRONLY); */
    /* if (!w){ */
    /*     fprintf(stderr, "cannot open digitizer"); */
    /*     exit(1); */
    /* } */
}

void press_button(int code);
void press_pen(int x, int y, long time);
void move_pen(int x, int y, long time);

void interpret_gesture(struct Gesture *g)
{
    if ((g->action == GestureAction::Enable) || ((g->action == GestureAction::ToggleEnabled) && !touch_enabled))
    {
        show("touch navigation enabled");
        printf("enabling\n");
        touch_enabled = true;
    }
    else if ((g->action == GestureAction::Disable) || ((g->action == GestureAction::ToggleEnabled) && touch_enabled))
    {
        show("touch navigation disabled");
        printf("disabling\n");
        touch_enabled = false;
    }

    time_t rawtime;
    time(&rawtime);
    switch (g->action)
    {
    case ShowClock:
    {
        struct tm *timeinfo = localtime(&rawtime);
        char buffer[100];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
        show(buffer);
    }
    break;

    default:
        break;
    }

    if (!touch_enabled)
        return;

    //require touch enabled
    switch (g->action)
    {
    case GestureAction::PreviousPage:
        press_button(Key::Left);
        break;
    case GestureAction::NextPage:
        press_button(Key::Right);
        break;
    case GestureAction::Suspend:
        printf("power\n");
        press_button(Key::Power);
        break;
    default:
        break;
    }
}

//todo: wip
void move_pen(int x, int y, long time)
{
    struct input_event key_input_event;

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = ABS_X;
    key_input_event.value = x;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event, sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = ABS_Y;
    key_input_event.value = y;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event, sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = ABS_DISTANCE;
    key_input_event.value = 16;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event, sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = 24;
    key_input_event.value = 3000;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event, sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_SYN;
    key_input_event.code = SYN_REPORT;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event, sizeof(key_input_event));
}
void press_pen(int x, int y, long time)
{
    struct input_event key_input_event;

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = 1;
    key_input_event.code = 320;
    key_input_event.value = 1;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event, sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = ABS_X;
    key_input_event.value = x;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event, sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = ABS_Y;
    key_input_event.value = y;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event, sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = ABS_DISTANCE;
    key_input_event.value = 16;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event, sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_SYN;
    key_input_event.code = SYN_REPORT;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event, sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = 1; //touch
    key_input_event.code = 330;
    key_input_event.value = 1;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event, sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = ABS_X;
    key_input_event.value = x;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event, sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = ABS_Y;
    key_input_event.value = y;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event, sizeof(key_input_event));
    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = 24;
    key_input_event.value = 3300;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event, sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = 25;
    key_input_event.value = 21;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event, sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = 26;
    key_input_event.value = 800;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event, sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = 27;
    key_input_event.value = -100;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event, sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_SYN;
    key_input_event.code = SYN_REPORT;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event, sizeof(key_input_event));
}

void press_button(int code)
{
    struct input_event key_input_event;
    memset(&key_input_event, 0, sizeof(key_input_event));

    debug_print("inject button %d \n", code);

    key_input_event.type = EV_KEY;
    key_input_event.code = code;
    key_input_event.value = 1;
    write(f, &key_input_event, sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_SYN;
    key_input_event.code = SYN_REPORT;
    write(f, &key_input_event, sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_KEY;
    key_input_event.code = code;
    write(f, &key_input_event, sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_SYN;
    key_input_event.code = SYN_REPORT;
    write(f, &key_input_event, sizeof(key_input_event));
}
