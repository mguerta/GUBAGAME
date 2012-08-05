#include "graphic.h"
#include "sound.h"
#include "math.h"
#include "animate.h"
#include "parser.h"

#define PREVIEW 0
#define FULLSCREEN 1

#define NOTRANSITION 0
#define FADEOUT 1
#define RUNFADEOUT 2
#define FADEIN 3
#define RUNFADEIN 4
#define LOOPFADEIN 5
#define LOOPFADEOUT 6

#define RUNMENU 0
#define RUNPLAY 1
#define STANDBY 2

#define NUMMAXITENSMENUROLL 20



typedef struct _infoDir
{
	char *actualPath;
	boxArea *viewClip;

	int interval;


	int tamList;
	int posCursor;
	int posPage;
	int numListView;
	long timeRepetJoy;
	int timeRepeat;
	int flgScroll;
	int idxScroll;
	long timeScroll;
	unsigned int joyLast;
	int lastIdxExec;
	listDir listGames[NUMMAXLIST];


} infoDir;

typedef struct _itemMenuRoll
{
	char *texto;
	Animate2D *aniNormal;
	Animate2D *aniSelect;
	void *action;

} itemMenuRoll;

typedef struct _menuRoll
{
	int numItens;
	int numItensVisibles;
	int currentItem;
	itemMenuRoll *itens[NUMMAXITENSMENUROLL];

	int flgSel;

} menuRoll;

typedef struct _frameMenu
{
	int xview,yview;

	char *fileBackground;
	tsound *music;

	Animate2D *aniBackground;

	menuRoll *menuroll;

	BMPimage background;
	BMPimage frame;
	BMPimage view;

	int transition;
	int varTransition;
	long timeTransition;
	BMPimage frameTransition;

	tmouse *mouse;
	int key;

} frameMenu;
