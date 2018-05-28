#include "defs.h"
#include "write.h"
#include "descriptor_table.h"

int kernel_main()
{
    framebuffer_init(0);
    cls();
    framebuffer_set_foreground_color(COLOR_GREEN);
    framebuffer_write("\t\t\t\t\t _ __ ___   ___ _   _        ___  ___\n");
    framebuffer_write("\t\t\t\t\t| '_ ` _ \\ / _ | | | |_____ / _ \\/ __|\n");
    framebuffer_write("\t\t\t\t\t| | | | | |  __| |_| |_____| (_) \\__ \\\n");
    framebuffer_write("\t\t\t\t\t|_| |_| |_|\\___|\\__,_|      \\___/|___/\n\n\n");

    // framebuffer_set_foreground_color(COLOR_WHITE);
    // framebuffer_write("Barebones kernel, capable of writing text in ");
    // framebuffer_set_foreground_color(COLOR_RED);
    // framebuffer_write("multiple colors.\n");
    // framebuffer_set_foreground_color(COLOR_WHITE);
    // framebuffer_write("And handling\ttabs (\\t)\n");
    // framebuffer_write("And handling\nnewlines (\\n)\n");
    // framebuffer_write("And handling b\backspaces (\\b)\n");

    init_idt();

    for (;;)
    {
        asm("hlt");
    }
    
    return 0;
}