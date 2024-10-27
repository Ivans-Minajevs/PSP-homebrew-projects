
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>

#define TRIANGLE "Triangle"  // Define TRIANGLE as the triangle symbol (▲)
#define CIRCLE   "Circle"  // Define CIRCLE as the circle symbol (○)
#define SQUARE   "Square" 
#define CROSS    "Cross"

PSP_MODULE_INFO("Button tester", 0, 1, 1);


/* While loop */

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


int main() {
    pspDebugScreenInit();
    SetupCallbacks();

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

  
    while (1)
    {
        SceCtrlData pad;

        sceDisplayWaitVblankStart();
        //pspDebugScreenClear();
        pspDebugScreenSetXY(0, 0);
        pspDebugScreenPrintf("Press [any] to test button\n");

        sceCtrlPeekBufferPositive(&pad, 1);

        if (pad.Buttons & PSP_CTRL_CROSS)  pspDebugScreenPrintf("[%s] pressed ", CROSS);
        if (pad.Buttons & PSP_CTRL_TRIANGLE) pspDebugScreenPrintf("[%s] pressed ", TRIANGLE);
        if (pad.Buttons & PSP_CTRL_CIRCLE)  pspDebugScreenPrintf("[%s] pressed ", CIRCLE);
        if (pad.Buttons & PSP_CTRL_SQUARE)  pspDebugScreenPrintf("[%s] pressed ", SQUARE);
        
        if (pad.Buttons & PSP_CTRL_LEFT) pspDebugScreenPrintf("[Left] pressed ");
        if (pad.Buttons & PSP_CTRL_RIGHT) pspDebugScreenPrintf("[Right] pressed ");
        if (pad.Buttons & PSP_CTRL_DOWN) pspDebugScreenPrintf("[Down] pressed ");
        if (pad.Buttons & PSP_CTRL_UP) pspDebugScreenPrintf("[Up] pressed ");

        if (pad.Buttons & PSP_CTRL_LTRIGGER) pspDebugScreenPrintf("[L] pressed ");
        if (pad.Buttons & PSP_CTRL_RTRIGGER) pspDebugScreenPrintf("[R] pressed ");

        if (pad.Lx | pad.Ly)
        {
            pspDebugScreenPrintf("\nAxes %d %d", pad.Lx, pad.Ly);
        }

        if (pad.Buttons & PSP_CTRL_START) {
            pspDebugScreenPrintf("Exiting...\n");
            break;
        }

    }

    
	sceKernelExitGame();

	return 0;
}