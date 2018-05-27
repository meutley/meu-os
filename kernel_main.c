#include "defs.h"
#include "write.h"

int kernel_main()
{
    framebuffer_init(0);
    framebuffer_clear();
    framebuffer_set_foreground_color(COLOR_GREEN);
    framebuffer_write("Hello, World!");

    for (;;)
    {
        asm("hlt");
    }

    framebuffer_set_foreground_color(COLOR_WHITE);
    framebuffer_write("Exiting");
    
    return 0;
}