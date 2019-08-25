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
        // (GestureRecognizer *)new MultiTapNarrowRecognizer(),
        (GestureRecognizer *)new MultiTapWideRecognizer(GestureAction::ToggleEnabled),
        // (GestureRecognizer *)new LeftTapShortRecognizer(),
        // (GestureRecognizer *)new RightTapShortRecognizer(),
        // (GestureRecognizer *)new UpTapShortRecognizer(),
        // (GestureRecognizer *)new DownTapShortRecognizer(),
        (GestureRecognizer *)new LeftSwipeShortRecognizer(GestureAction::NextPage),
        (GestureRecognizer *)new RightSwipeShortRecognizer(GestureAction::PreviousPage),
        (GestureRecognizer *)new LeftSwipeLongRecognizer(GestureAction::NextPage),
        (GestureRecognizer *)new RightSwipeLongRecognizer(GestureAction::PreviousPage),
        // (GestureRecognizer *)new UpSwipeShortRecognizer(),
        (GestureRecognizer *)new DownSwipeShortRecognizer(GestureAction::ShowClock),
        // (GestureRecognizer *)new UpSwipeLongRecognizer(),
        (GestureRecognizer *)new DownSwipeLongRecognizer(GestureAction::ShowClock),
    };

    process_touch(recognizers, 7);

    return 0;
}
