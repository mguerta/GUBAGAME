#include "graphic.h"
#include "sound.h"
#include "math.h"
#include "animate.h"
#include "parser.h"



#define NUMMAXROTES 5
#define NUMMAXACTIONS 10
#define NUMMAXACTORS 100

#define MOVERESOLUTION 0.5
#define VELOCITWALK 8
#define VELOCITTURN 100

#define MASKCOLOR 0xec26c9 //0x7098


typedef enum _actions
{
	STANDACTION = 0,			
	WALKACTION					
	
} actionType;

/*============================================================
 D: Data struct 
 =============================================================*/

typedef struct _pointRote
{
	int x,y;
	int color;
	
} pointRote;


typedef struct _Action
{
	Animate3D *ani;
	void (*control) (void *);
	char *fileSound;
	
	int channelSound;

} Action;

typedef struct _Actor
{
	int xo,yo,ago;
	char *name;
	void (*control) (void *,void *);
	int numActions;
	Action *actions[NUMMAXACTIONS];

	int actualPoint;
	int actualAction;
	int sel;
	int state;
	int xd,yd,agd;

	int pd,ps;
	char *rote;

	float xw,yw;
	int vel;
	

} Actor;

typedef struct _ActorPosition
{
	int p;
	Actor *ac;

} ActorPosition;


typedef struct _gameLavel
{
	char *fileBackground;
	char *fileMaskBackground;
	char *music;
	int xscore,yscore;
	pixel colorTransScore;
	char *fileScore;
	int xlives,ylives;
	pixel colorTransLives;
	char *fileLives;

	int xTime,yTime;
	pixel colorTime;

	Actor *actorMain;

	int numActors;
	ActorPosition listActorsPosition[NUMMAXACTORS];

	Actor *listActors[NUMMAXACTORS];

	BMPimage background;
	BMPimage maskBackground;
	BMPimage score;
	BMPimage lives;
	BMPimage frame;
	BMPimage view;

	int xview,yview;

	tmouse *mouse;
	int key;


} gameLevel;


typedef struct _Level
{
	char *fileBackground;
	char *fileMaskBackground;
	char *music;

	int numActors;
	ActorPosition *listActors[NUMMAXACTORS];

} level;

typedef struct _game
{
	int numActors;
	Actor *listActors[NUMMAXACTORS];

	BMPimage background;
	BMPimage maskBackground;
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
int gameInit(void);
gameLevel * setLevel(gameLevel *gl);
void resetLevel(gameLevel *gl);
void gameClose(void);
void calcViewCamera(gameLevel *gl,tmouse *mo);
void stopGame(void);

void getStringTime(long time,char *str);

void initActors(gameLevel *gl);
void runActors(gameLevel *gl,int draw);
void freeActors(gameLevel *gl);
void selActors(gameLevel *gl,Actor *ac);
void walkInLine(Actor *ac) ;
int turnActor(Actor *am);
int recalculateRote(gameLevel *gl,Actor *am,Actor *ac);
