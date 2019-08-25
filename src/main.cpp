#include <stdio.h>
#include <stdlib.h>

#include "include/ui.hpp"
#include "include/version.hpp"
#include "include/gestures.hpp"
#include "gesture_definition.hpp"
#include "include/eventreader.hpp"
#include "include/keyinjector.hpp"

int main(int argc, char **argv)
{
    printf("touchinjector %s\n", VERSION);

    injector_init();
    ui_init();

    GestureRecognizer *recognizers[] = {
        (GestureRecognizer *)new MultiTapShortRecognizer(),
        (GestureRecognizer *)new MultiTapWideRecognizer(),
        (GestureRecognizer *)new LeftTapShortRecognizer(),
        (GestureRecognizer *)new RightTapShortRecognizer(),
        (GestureRecognizer *)new UpTapShortRecognizer(),
        (GestureRecognizer *)new DownTapShortRecognizer(),
        (GestureRecognizer *)new LeftSwipeShortRecognizer(),
        (GestureRecognizer *)new RightSwipeShortRecognizer(),
        (GestureRecognizer *)new LeftSwipeLongRecognizer(),
        (GestureRecognizer *)new RightSwipeLongRecognizer(),
        (GestureRecognizer *)new UpSwipeShortRecognizer(),
        (GestureRecognizer *)new DownSwipeShortRecognizer(),
        (GestureRecognizer *)new UpSwipeLongRecognizer(),
        (GestureRecognizer *)new DownSwipeLongRecognizer(),
    };

    process_touch(recognizers, 14);

    return 0;
}
