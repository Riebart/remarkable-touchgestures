#include <stdio.h>
#include <stdlib.h>

#include "include/ui.h"
#include "include/version.h"
#include "include/gestures.h"
#include "include/eventreader.h"
#include "include/keyinjector.h"

int main()
{
    printf("touchinjector %s\n", VERSION);

    injector_init();
    ui_init();

    process_touch(&recognize_gestures);

    return 0;
}
