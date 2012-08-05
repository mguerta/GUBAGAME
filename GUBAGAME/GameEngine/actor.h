/*============================================================
 F:  Functions
 =============================================================*/

void initActors(gameLevel *gl);
void runActors(gameLevel *gl,int draw);
void drawActors(gameLevel *gl);
void drawActor(gameLevel *gl,Actor *ac);
void drawActorByCel(gameLevel *gl, int xc,int yc);
void freeActors(gameLevel *gl);
void selActors(gameLevel *gl,Actor *ac);
int walkInLine(gameLevel *gl,Actor *ac,float pass) ;
int walkPass(gameLevel *gl,Actor *ac,float pass);
int turnActor(Actor *am);
int recalculateRote(gameLevel *gl,Actor *am,Actor *ac);

void controlActorTest(gameLevel *gl,Actor *ac);
void controlActorObject(gameLevel *gl,Actor *ac);
void controlActorMain(gameLevel *gl,Actor *ac);

void controlActorBala(gameLevel *gl,Actor *ac);

int moveInScene(gameLevel *gl,Actor *ac,float dx,float dy);

void posViewCameraByActor (gameLevel *gl,Actor *ac);

Actor * createActor(gameLevel *gl,int xo,int yo,int zo,int ago,Actor *ac,Actor *acParent,int FLAGLOADANI);

void insertActor(gameLevel *gl,int i,Actor *ac,int FLAGLOADANI);
int searchFreePositionActor(gameLevel *gl);
void removeActor(gameLevel *gl,Actor *ac);

int resetActualActor(gameLevel *gl);
int getNextActor(gameLevel *gl);

int initActorsOpen(gameLevel *gl);
