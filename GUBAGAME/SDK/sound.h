#define NORMALMUSIC 0
#define REPEATMUSIC 1
#define NUMMAXSOUNDS 15
#ifdef WIN32
#define MAXCACHESOUND  1024
#else
#ifdef ANDROID_NDK
#define MAXCACHESOUND  256
#else
#define MAXCACHESOUND 256 // 4096
#endif
#endif


#define HEADERWAVFILE 256

typedef struct _tsound
{
	char *fileName;
	int statusRepeat;
	int cache;

	sample *buf;
	long size;
	long idx;
	long idxTotal;


} tsound;

int RenderAudio(void);
int setMusic(char *nameFile,int flgRepeat);
int setSoundChannel(char *nameFile,int channel,int flgRepeat);
int freeAudio(void);
int startSound(tsound *snd);
int startSoundBuffer(sample *buf,long size);
void startIDMusic(int id,int flgRepeat);

//----------------------------------------------------------
// Sound interface
//---------------------------------------------------------

int SRenderAudio(void);
int SsetMusic(char *nameFile,int flgRepeat);
int SsetSoundChannel(char *nameFile,int channel,int flgRepeat);
int SfreeAudio(void);
int SstartSound(tsound *snd);
int SstartSoundBuffer(sample *buf,long size);
void SstartIDMusic(int id,int flgRepeat);
