#define NUMMAXFRAMES 60
#define ANGLES3D   360/45  // 8 graus
#define ANGLES3D_0 0/45  
#define ANGLES3D_45 45/45  
#define ANGLES3D_90 90/45  
#define ANGLES3D_135 135/45  
#define ANGLES3D_180 180/45 
#define ANGLES3D_225 225/45  
#define ANGLES3D_270 270/45  
#define ANGLES3D_315 315/45  
#define ANGLES3D_360 0/45  

#define ANGLERESOLUTION ANGLE_45

#define ANINORMAL 0
#define ANIPINGPONG 1



/*============================================================
 D: Data struct 
 =============================================================*/


typedef struct _Animate2D
{
	char *fileFrame;
	int colorTrans;
	long delayFrame;

	int numFrame;
	int flgCache;
	int flgOpt;

	int actualFrame;
	long lastCurrentTime;
	BMPimage frame[NUMMAXFRAMES];

} Animate2D;


typedef struct _Animate3D
{
	char *fileFrame;
	int  colorTrans;
	long delayFrame;
	int type;
	int cache;
	int passFrame;
	int flgOpt;


	int direction;
	int numFrame;
	int actualFrame;
	long lastCurrentTime;

	BMPimage frame[ANGLES3D][NUMMAXFRAMES];

} Animate3D;


/*============================================================
 F:  Functions
 =============================================================*/

void loadAnimate2D(Animate2D *ani,BMPimage *view);
void printAnimate2D(int x,int y,Animate2D *ani,BMPimage *view);
void freeAnimate2D(Animate2D *ani);

void loadAnimate3D(Animate3D *ani,BMPimage *view);
void printAnimate3D(int x,int y,int angle,Animate3D *ani,BMPimage *view,int ctrAni);
void printMaskAnimate3D(int x,int y,int angle,Animate3D *ani,BMPimage *view,BMPimage *mask,pixel colorMask);
void freeAnimate3D(Animate3D *ani);