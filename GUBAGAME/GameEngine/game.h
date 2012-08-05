#include "graphic.h"
#include "sound.h"
//#include "math.h"
#include "animate.h"
#include "parser.h"
#include "ui.h"




#define MASKCOLOR 0xec26c9 //0x7098



#define NUMMAXROTES 5
#define NUMMAXACTIONS 10
#define NUMMAXACTORS 200

#define MOVERESOLUTION 0.5
#define VELOCITWALK 8
#define VELOCITTURN ANGLE_10

#define PASSWALK 5.0 //6.0

#define NUMMAXTILEDMAP 1024

#define TAMQUAD 113
#define FTAMQUAD 113.15
#define ITAMQUAD (int)(FTAMQUAD*KDD_FIX)

#define CAMERAOFF 0
#define CAMERAON 1

#define TILEDNORMAL 0
#define TILEDANI 1

#define ACTORTEMP 0x0
#define ACTORSTATIC 0x1
#define ANIACTORSTATIC 0x2

#define VIEWTOUCHDISABLE 0
#define VIEWTOUCHENABLE 1


/*============================================================
 D: Data struct 
 =============================================================*/

typedef enum _actions
{
	STANDACTION = 0,			
	WALKACTION,
	PUNCH
	
} actionType;

typedef enum _typesActor
{
	ACTORMAIN=0,
	ACTORBAD,
	ACTORGOD,
	ACTORFIRE

} typesActor;

typedef struct _pointRote
{
	int x,y;
	int color;
	
} pointRote;


typedef struct _Action
{
	Animate3D *ani;
	void (*control) (void *);
//	char *fileSound;
	
//	int channelSound;

	int soundID;

} Action;

typedef struct _Actor
{
	int type;
	int xo,yo,zo,ago;
	const char *name;
	int camera;
	int sizex;
	int sizey;
	int sizez;
	void (*control) (void *,void *);
	int numActions;
	Action *actions[NUMMAXACTIONS];

	int actualPoint;
	int actualAction;
	int sel;
	int state;
	int xd,yd,agd;
	int xcel,ycel;
	int xp,yp;

	int pd,ps;
	

	float xw,yw;
	int vel;

	int agNormalColision;

	int nextActor;
	int previousActor;
	int idxActor;
	int flgActor;

	void *ActorColision;
	void *ActorParent;

	int control0;

} Actor;



/*============================================================
 D: Data struct 
 =============================================================*/

typedef struct _tiled
{
	int type;
	unsigned char *nameFile;
	int sizez;
	//Animate3D *ani;
	Animate2D *ani;
	BMPimage bmp;

} tiled;

typedef struct _scene
{
	int sizex,sizey;
	int sizeTiledx,sizeTiledy;
	int realSizeTiledx,realSizeTiledy;
	unsigned short *map;
	tiled *listTiled[NUMMAXTILEDMAP];

//	int x1,y1;
//	int x2,y2;
} scene;

typedef struct _camera
{
	int x,y,z,ag;

} camera;


typedef struct _tmouse3D
{
	int x;
	int y;
	int z;
	int status;

} tmouse3D;

typedef struct _listSound
{
	int size;
	char *nameSounds[100];
} listSound;

typedef struct _viewGame
{
	int x,y;
	int tamx,tamy;
	int celx,cely;
	int flagAlpha;
	void (*control) (void *,void *,void *);
    int flgView;
	camera cam;
	tmouse mouse;

	int flgTouch;

	int flgTopView;
	int xTopView,yTopView;

} viewGame;



typedef struct _gameLevel
{
	scene *sceneLevel;
	fontText *fonts;
	char *fileBackground;
	listSound *listSounds;
	int xscore,yscore;
	int colorTransScore;
	char *fileScore;
	int xlives,ylives;
	int colorTransLives;
	char *fileLives;

	int xTime,yTime;
	int colorTime;

	int numViewGame;
	viewGame *viewList[5];

	Actor *actorMain;

	int numActors;
	Actor * listActorsPosition[NUMMAXACTORS];

	int numActorsLive;
	int firstActor;
	int lastActor;
	int actualActor;
	Actor *listActors[NUMMAXACTORS];

	BMPimage background;
	BMPimage score;
	BMPimage lives;
	BMPimage frame;
	BMPimage view;

	int xview,yview;

	int flgTopView;
	int xTopView,yTopView;

	camera cam;

	tmouse *mouse;

	tmouse3D mouseView;

	int buttonFire;

	int key;
    
    int stateGame;
    int idxLoadActor;

    wipe transiction;

} gameLevel;



typedef struct _game
{
	int numActors;
	Actor *listActors[NUMMAXACTORS];

	BMPimage background;
	BMPimage frame;
	BMPimage view;

	int xview,yview;

	tmouse *mouse;
	int key;

} game;



typedef struct _coord3D
{
	int x,y,z;

} coord3D;


/*============================================================
 F:  Functions
 =============================================================*/

void gameMain(void);
int gameMainOpen(void);
int gameInit(void);
gameLevel * setLevel(gameLevel *gl);
void resetLevel(gameLevel *gl);
void gameClose(void);
void calcViewCamera(gameLevel *gl,Actor *ac);
void stopGame(void);
void ajustActorCamera(gameLevel *gl,Actor *ac);
void getStringTime(long time,char *str);
void calcPosView(gameLevel *gl,int x,int y,int *xp, int *yp);
void projectQUAD(int xt,int yt,int zt,int lenx, int leny, gameLevel *gl,BMPimage *view,pixel COLOR);
void projectActor(gameLevel *gl,Actor *ac,pixel color);
