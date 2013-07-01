//BEGIN(ALL)
//BEGIN(COGL_HEADER)
#include <cogl/cogl.h>
//END(COGL_HEADER)
//BEGIN(GLIB_HEADER)
#include <glib.h>
//END(GLIB_HEADER)

//BEGIN(DATA)
typedef struct _Data
{
    CoglContext *ctx;
    CoglFramebuffer *fb;
    CoglPrimitive *triangle;
    CoglPipeline *pipeline;
} Data;
//END(DATA)

//BEGIN(PAINT_CALLBACK_PROTO)
static CoglBool
paint_cb (void *user_data)
{
    Data *data = user_data;
//END(PAINT_CALLBACK_PROTO)

    //BEGIN(DRAW_COMMANDS)
    cogl_framebuffer_clear4f (data->fb, COGL_BUFFER_BIT_COLOR, 0, 0, 0, 1);
    cogl_framebuffer_draw_primitive (data->fb, data->pipeline, data->triangle);
    cogl_onscreen_swap_buffers (COGL_ONSCREEN (data->fb));
    //END(DRAW_COMMANDS)

//BEGIN(UNREGISTER_IDLE)
    return FALSE; /* un-register idle callback */
}
//END(UNREGISTER_IDLE)

//BEGIN(FRAME_EVENT_CALLBACK)
static void
frame_event_cb (CoglOnscreen *onscreen,
                CoglFrameEvent event,
                CoglFrameInfo *info,
                void *user_data)
{
    if (event == COGL_FRAME_EVENT_SYNC)
        paint_cb (user_data);
}
//END(FRAME_EVENT_CALLBACK)

int
main (int argc, char **argv)
{
    Data data;
    CoglOnscreen *onscreen;
    CoglError *error = NULL;
    //BEGIN(VERTEX_DATA)
    CoglVertexP2C4 triangle_vertices[] = {
        {0, 0.7, 0xff, 0x00, 0x00, 0x80},
        {-0.7, -0.7, 0x00, 0xff, 0x00, 0xff},
        {0.7, -0.7, 0x00, 0x00, 0xff, 0xff}
    };
    //END(VERTEX_DATA)
    GSource *cogl_source;
    GMainLoop *loop;

    //BEGIN(NEW_CONTEXT)
    data.ctx = cogl_context_new (NULL, &error);
    if (!data.ctx) {
        fprintf (stderr, "Failed to create context: %s\n", error->message);
        return 1;
    }
    //END(NEW_CONTEXT)

    //BEGIN(NEW_ONSCREEN)
    onscreen = cogl_onscreen_new (data.ctx, 640, 480);
    //END(NEW_ONSCREEN)
    //BEGIN(ONSCREEN_SHOW)
    cogl_onscreen_show (onscreen);
    //END(ONSCREEN_SHOW)
    //BEGIN(CAST_FRAMEBUFFER)
    data.fb = COGL_FRAMEBUFFER (onscreen);
    //END(CAST_FRAMEBUFFER)

    //BEGIN(PRIMITIVE_NEW)
    data.triangle = cogl_primitive_new_p2c4 (data.ctx,
                                             COGL_VERTICES_MODE_TRIANGLES,
                                             3, triangle_vertices);
    //END(PRIMITIVE_NEW)
    //BEGIN(PIPELINE_NEW)
    data.pipeline = cogl_pipeline_new (data.ctx);
    //END(PIPELINE_NEW)

    //BEGIN(GLIB_SOURCE_NEW)
    cogl_source = cogl_glib_source_new (data.ctx, G_PRIORITY_DEFAULT);

    g_source_attach (cogl_source, NULL);
    //END(GLIB_SOURCE_NEW)

    //BEGIN(ADD_FRAME_CALLBACK)
    cogl_onscreen_add_frame_callback (COGL_ONSCREEN (data.fb),
                                      frame_event_cb,
                                      &data,
                                      NULL /* destroy callback */);
    //END(ADD_FRAME_CALLBACK)

    //BEGIN(ADD_PAINT_IDLE)
    g_idle_add (paint_cb, &data);
    //END(ADD_PAINT_IDLE)

    //BEGIN(RUN_MAINLOOP)
    loop = g_main_loop_new (NULL, TRUE);
    g_main_loop_run (loop);
    //END(RUN_MAINLOOP)

    return 0;
}
//END(ALL)
