#include <stdio.h>
#include <libdragon.h>
#include <rdpq.h>
#include "game.h"

int main(void)
{
    game_init();

    while (1)
    {
        surface_t *disp = NULL;

        /* Wait for a video frame and grab a render buffer */
        while (!(disp = display_get()))
        {
            // This ensures we only proceed when we have a valid display context
        }

        /* Attach RDPQ to the framebuffer */
        rdpq_attach_clear(disp, NULL);

        game_update();
        game_draw();

        /* Detach RDPQ and trigger the graphics display */
        rdpq_detach_show();

        /* RDPQ detach handles vsync, no need for wait_ms */
        
        game_update_input();
    }
}
