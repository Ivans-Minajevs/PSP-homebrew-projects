// Graphics Tester

/*
          This program was created by (Ivans Minajevs (introsent)) on (10.08.24)
*/

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspgum.h>

PSP_MODULE_INFO("Graphics tester", 0, 1, 1);
/* Exit callback */
int exit_callback(int arg1, int arg2, void *common) {
          sceKernelExitGame();
          return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp) {
          int cbid;

          cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
          sceKernelRegisterExitCallback(cbid);

          sceKernelSleepThreadCB();

          return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void) {
          int thid = 0;

          thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
          if(thid >= 0) {
                    sceKernelStartThread(thid, 0, 0);
          }

          return thid;
}

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272
#define BUFFER_WIDTH 512
unsigned int list[64*1024];

struct Vertex
{
    unsigned int color;
    float x;
    float y;
    float z;
};

void Initialize()
{

    void* videoRam = 0;
    void* displayRam = videoRam;
    void* drawRam = displayRam + BUFFER_WIDTH * SCREEN_HEIGHT * 4;

    sceGuInit();

    sceGuStart(GU_DIRECT, list);
    sceGuDispBuffer(SCREEN_WIDTH, SCREEN_HEIGHT, displayRam, BUFFER_WIDTH);
    sceGuDrawBuffer(GU_PSM_8888, drawRam, BUFFER_WIDTH);

    sceGuOffset(0, 0);
    sceGuViewport(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT);

    sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    sceGuEnable(GU_SCISSOR_TEST);

    sceGuFrontFace(GU_CW);
    sceGuShadeModel(GU_SMOOTH);
    sceGuDisplay(GU_TRUE);

    sceGuFinish();
    sceGuSync(0,0);

    sceDisplayWaitVblank();
    sceGuSwapBuffers();
}

void Display(struct Vertex* vertices, int num_vertices)
{
    sceGuStart(GU_DIRECT, list);

    sceGuClearDepth(0);
    sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);

    sceGumMatrixMode(GU_VIEW);
    sceGumLoadIdentity();

    sceGumMatrixMode(GU_PROJECTION);
    sceGumLoadIdentity();
    sceGumPerspective(90, 16.0 / 9.0, 1, 100);

    sceGumMatrixMode(GU_MODEL);
    sceGumLoadIdentity();

    sceGumDrawArray(GU_TRIANGLES, GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D, num_vertices, 0, vertices);

    sceGuFinish();
    sceGuSync(0,0);

    sceDisplayWaitVblank();
    sceGuSwapBuffers();

};

int main() {
	//pspDebugScreenInit();
	SetupCallbacks();

    Initialize();

    struct Vertex vertices[] = { 
        {0xFF0000ff, 0,  1, -1},
        {0xFF00ff00, 1,  -1, -1},
        {0xFFff0000, -1, -1, -1},
    };
    int num_vertices = sizeof(vertices) / sizeof(vertices[0]);

	while(1)
    {
        Display(vertices, num_vertices);
    }

	sceKernelSleepThread();
	
	return 0;
}





