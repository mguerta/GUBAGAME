//#ifndef WIN32_H
//#define WIN32_H
#include "stdio.h"
#include "string.h"

#define CACHE  1
#define NOCACHE 0

#define VIDEO_WIDTH  720 //600 //720 //320 //800
#define VIDEO_HEIGHT 480 //400 //480 //480 //240 //600

extern int VVIDEO_WIDTH;  
extern int VVIDEO_HEIGHT;

//#define PIXEL555(R,G,B) (((31*(R)/255)<<10)|((31*(G)/255)<<5)|(31*(B)/255))
//#define PIXEL565(R,G,B) (((31*(R)/255)<<11)|((31*(G)/255)<<6)|(31*(B)/255))
//#define PIXEL8888(R,G,B) ((R)<<16)|((G)<<8)|(B)
#ifdef BPP16
#ifdef WIN32
//555- nao tem 565 no windows
#define PIXEL(R,G,B) (((31*(R)/255)<<10)|((31*(G)/255)<<5)|(31*(B)/255))
#define RED(C)       ((((C)>>10)&0x1F))
#define GREEN(C)     ((((C)>>5)&0x1F))
#define BLUE(C)      (((C)&0x1F)) 
//565
#else
#define PIXEL(R,G,B) (((31*(R)/255)<<11)|((31*(G)/255)<<6)|(31*(B)/255))
#define RED(C)       ((((C)>>11)&0x1F))
#define GREEN(C)     ((((C)>>6)&0x1F))
#define BLUE(C)      (((C)&0x1F))
#endif
#else
#ifdef BPP32
#define PIXEL(R,G,B) (((R)<<16)|((G)<<8)|(B))
#define RED(C)       ((((C)>>16)&0xFF))
#define GREEN(C)     ((((C)>>8)&0xFF))
#define BLUE(C)      (((C)&0xFF)) 
#else
#define PIXEL(R,G,B) (((B)<<16)|((G)<<8)|(R))
#define BLUE(C)       ((((C)>>16)&0xFF))
#define GREEN(C)     ((((C)>>8)&0xFF))
#define RED(C)      (((C)&0xFF)) 
#endif
#endif

/*
#define RED(C)       (255*(((C)>>10)&0x1F)/31)
#define GREEN(C)     (255*(((C)>>5)&0x1F)/31)
#define BLUE(C)      (((C)&0x1F)) 
#define REDC(C)       ((((C)>>10)&0x1F))
#define GREENC(C)     ((((C)>>5)&0x1F))
#define BLUEC(C)      (((C)&0x1F)) 
#define RED8(C)       ((((C)>>16)&0xFF))
#define GREEN8(C)     ((((C)>>8)&0xFF))
#define BLUE8(C)      (((C)&0xFF)) 
#define PIXELC(R,G,B) ((R)<<10)|((G)<<5)|(B)
*/
/*#define RED(C)       (255*(((C)>>11)&0x1F)/31)
#define GREEN(C)     (255*(((C)>>6)&0x3F)/31)
#define BLUE(C)      (255*((C)&0x1F)/31) */


#define MOUSE_DOWN   0x0001
#define MOUSE_DOWNR  0x0002

#define BTN_LEFT     0x0008
#define BTN_RIGHT    0x0004
#define BTN_UP       0x0001
#define BTN_DOWN     0x0002
#define BTN_START    0x0080
#define BTN_EXIT     0x0080
#define BTN_A        0x0010
#define BTN_B        0x0020
#define BTN_C        0x0040
#define BTN_X        0x0100
#define BTN_Y        0x0200
#define BTN_Z        0x0400

#define MAXPATH 8192
#define NUMMAXLIST 8192
#define TYPE_FILE 0
#define TYPE_DIR 1
#define TYPE_BACKDIR 2


typedef enum _fileType
{
	TFARM = 0,				// arm executable
	TFGEN,					// genesis executable
	TFGEN2,					// genesis executable2 (para apenas 4 jogos)
	TFSMS,					// master system executable
	TFBMP,						// bit map image
	TFICON,						// Titan icon (48 colors bit map)
	TFLINK,					// Link simbolico
	TFMP3,					//MP3 file
	TFSDCARD,					//SDCARD
	TFNAND,						//NAND 
	TFMP4,						//VIDEO
	TFJPG,						//FOTOS 
	TFUSB,						//PASTA USB
	TFOLD,	
	TBACK,
	TFOTHER						// unknown

} fileType;

/*============================================================
E: Struct 
 =============================================================*/

typedef struct _listDir
{
	char *nameFile;
	int type;
	int subType;

} listDir;



typedef struct _tmouse
{
	int x;
	int y;
	int status;

} tmouse;

typedef struct _colorType
{
    float r,g,b,a;
} colorType;

//** pixel **********************************************************
//** In our code, Windows pixels are 16bit each.                   **
//*******************************************************************

#ifdef BPP16
typedef unsigned short pixel; //RGB555 
#else
typedef unsigned long pixel; //RGB8888 
#endif

//** sample *********************************************************
//** Our audio samples are 8bit signed values.                    **
//*******************************************************************
typedef signed short sample;  // 8bits data wav

//** InitSound() ****************************************************
//** Initialize sound. Returns rate (Hz) on success, 0 otherwise.  **
//** Rate=0 to skip initialization (will be silent).               **
//*******************************************************************
sample *GetAudio(void);

//** GetTotalAudio() ************************************************
//** Get the total length of the audio buffer, in samples.         **
//*******************************************************************
unsigned int GetTotalAudio(void);

//** GetFreeAudio() *************************************************
//** Get the amount of free space in the audio buffer, in samples. **
//*******************************************************************
unsigned int GetFreeAudio(void);

//** WriteAudio() ***************************************************
//** Write up to a given number of samples into the audio buffer.  **
//** Returns the number of samples written.                        **
//*******************************************************************
unsigned int WriteAudio(sample *Data,unsigned int Length);


//** AskFilename() **************************************************
//** Shows an open-file dialog and makes the user select a file.   **
//** Returns 0 on failure. ATTENTION: This function returns a      **
//** pointer to the internal buffer!                               **
//*******************************************************************
const char *AskFilename(const char *Title,const char *Types);

//** ProcessMsgs() **************************************************
//** Call this function periodically to process Windows messages.  **
//** Returns number of messages processed.                         **
//*******************************************************************
int ProcessMsgs(void);

//** Application() **************************************************
//** This is the main application function that you implement. It  **
//** is called from WinMain() after performing initialization.     **
//****************************************** TO BE WRITTEN BY USER **
int Application(char *cmdLine);

//** Application() **************************************************
//** This is the main application function that you implement. It  **
//** is called from WinMain() after performing initialization.     **
//****************************************** TO BE WRITTEN BY USER **
int stopApplication(void);

//** WaitSyncTimer() ************************************************
//** Wait for the timer to become ready.                           **
//*******************************************************************
void WaitSyncTimer(void);

//** SetSyncTimer() *************************************************
//** Set synchronization timer to a given frequency in Hz.         **
//*******************************************************************
int SetSyncTimer(int Hz);


pixel *GetVideo(void); 
pixel *GetFrontVideo(void);
pixel *GetBackVideo(void);

int GetVideoWidth(void); 
int GetVideoHeight(void); 

//** FlipVideo() ****************************************************
//** Switch displayed video buffer from VBUF0 to VBUF1 and back.   **
//** Returns the address of the INACTIVE video buffer that can be  **
//** modified by your program. The ACTIVE video buffer cannot be   **
//** written to.                                                   **
//*******************************************************************
pixel *FlipVideo(void);
unsigned int GetJoystick(void);
long readFile(char *nameFile, char **buf);
long readFileChunk(char *nameFile, char **buf,int seek,int size);
int testFileExist(char *nameFile);

long  getSizeMalloc(void);
void * mymalloc(long size);
void myfree(void *p);
tmouse * getMouseInf(void);
void setMousePosition(tmouse * mo);
long getCurrentTime(void);

int getFPS(int * FPSG,int * FPS);
int getDrawFrame(void);
void watchdog(long time);

int getRenderAudio(void);

char * getRootPath(void);
char * getDEMOPath(void);
int getListFileDir(char *path,listDir *list);
void stopSound(void);
void resumeSound(void);
void resumeVideo(void);
void saveVideo(void);

void tvOff(void);
void tvOn(void);

//void mute(void);
void mute(int i);
void resumeMute(void);

int execFile(char *nameExec, char *nameFile);

int execFileNoKill(char *nameExec, char *nameFile);

pixel *GetBufferPreview(void);
void RenderAudioPreview(void);

void waitVBLANK(void);

int checkStandBy(void);
int runStandBy(void);

void setLowCPUConsume(void);
void setNormalCPUConsume(void);

fileType getTypeFile(char *name);
int checkPENDRIVE(void);
int checkJOY(void);

int getPosMPlayer(void);
int getLenMPlayer(void);
int getEndMPlayer(void);
void setPause(int flag);
void resetVarMPLayer(void);

int getIndexLastBar(char *name);

int myrand(void);

void nativeFadeColorScreen(float red,float green,float blue,float alpha);





//#endif // WIN32_H
