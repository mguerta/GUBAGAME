#include "so.h"
#include "game.h"
#include "actor.h"
#include "math.h"

extern Actor bala;

/*============================================================
 F: Init Actors
 =============================================================*/

void initActors(gameLevel *gl)
{int i;


	if(!gl) return;

	gl->firstActor=0;
	gl->lastActor=0;
	gl->numActorsLive=0;
	for(i=0;i<gl->numActors;i++)
	{
		insertActor(gl,i,gl->listActorsPosition[i],ACTORSTATIC | ANIACTORSTATIC);
	}
}

/*============================================================
 F: Init Actors Open
 =============================================================*/

int initActorsOpen(gameLevel *gl)
{
    
    
	if(!gl) return -1;
    if(gl->idxLoadActor==0)
    {
        gl->firstActor=0;
        gl->lastActor=0;
        gl->numActorsLive=0;
    }
    if(gl->idxLoadActor>=gl->numActors-1) return 1;
    insertActor(gl,gl->idxLoadActor,gl->listActorsPosition[gl->idxLoadActor],ACTORSTATIC | ANIACTORSTATIC);
    gl->idxLoadActor++;
    return 0;
}


/*============================================================
 F: Remove Actor
 =============================================================*/

void removeActor(gameLevel *gl,Actor *ac)
{int i,previous,next;

	i=ac->idxActor;
	next=ac->nextActor;
	previous=ac->previousActor;

	if(previous>=0)
		gl->listActors[previous]->nextActor=next;		//refaz link
	if(next>=0)
		gl->listActors[next]->previousActor=previous;

	if(ac->flgActor==ACTORTEMP)
	{
		myfree(ac);
	}
	if(i>0)
		gl->listActors[i]=0;					//Libera posicao da lista	

	gl->numActorsLive--;					//Atualiza contabilidade

	if(i==gl->firstActor)					//refaz cabeça e calda da lista
		gl->firstActor=next;
	if(i==gl->lastActor)
		gl->lastActor=previous;

	if(i==gl->actualActor)
	{
		gl->actualActor=next;
		if(next==-1)
			gl->actualActor=previous;
	}
}

/*============================================================
 F: reset actual actor
 =============================================================*/

int resetActualActor(gameLevel *gl)
{
	gl->actualActor=gl->firstActor;

	return gl->actualActor;
}

/*============================================================
 F: get next actor
 =============================================================*/

int getNextActor(gameLevel *gl)
{int i;

	i=gl->actualActor;

	if(i==-1) 
		return 0;

	gl->actualActor=gl->listActors[i]->nextActor;

	return gl->actualActor;
}

/*============================================================
 F: Search free position
 =============================================================*/

int searchFreePositionActor(gameLevel *gl)
{int i;
	
	if(gl->numActorsLive>=NUMMAXACTORS)   // Ultrapassou o limite de numero de atores possiveis
		return -1;

	for(i=0;i<NUMMAXACTORS;i++)
	{
		if(gl->listActors[i]==0) break;  // procura por slot vazio na lista
	}
	if(i>=NUMMAXACTORS) 
		return -1;
	else
		return i;
}

/*============================================================
 F: Create Actor
 =============================================================*/

Actor * createActor(gameLevel *gl,int xo,int yo,int zo,int ago,Actor *ac,Actor *acParent,int FLAGLOADANI)
{int i;

 Actor *acNew=0;

	i=searchFreePositionActor(gl);
	if(i==-1)	return acNew;  // lista cheia

	acNew=mymalloc(sizeof(Actor));  //Cria novo objeto Actor
	if(acNew)
	{
		memcpy(acNew,ac,sizeof(Actor)); //Copia Actor de referencia

		acNew->ActorParent=acParent;
		acNew->xo=xo;
		acNew->yo=yo;
		acNew->zo=zo;
		acNew->ago=ago;
	
		insertActor(gl,i,acNew,FLAGLOADANI); //ACTORTEMP);
	}

	return acNew;
}

/*============================================================
 F: Insert Actors
 =============================================================*/

void insertActor(gameLevel *gl,int i,Actor *ac,int FLAG)
{int j;

	gl->listActors[i]=ac;
	gl->listActors[i]->flgActor=FLAG;
	gl->listActors[i]->actualAction=0;
	gl->listActors[i]->state=0;
	gl->listActors[i]->sel=0;
	
	gl->listActors[i]->xw=(float)gl->listActors[i]->xo;
	gl->listActors[i]->yw=(float)gl->listActors[i]->yo;
	gl->listActors[i]->agd=gl->listActors[i]->ago;

	if(FLAG & ANIACTORSTATIC)
	{
		for(j=0;j<gl->listActors[i]->numActions;j++)
		{
			loadAnimate3D(gl->listActors[i]->actions[j]->ani,&gl->view);
		}
	}
		
	gl->listActors[i]->previousActor=gl->lastActor; // refaz link
	gl->listActors[i]->nextActor=-1;
	gl->listActors[i]->idxActor=i;
	gl->listActors[gl->lastActor]->nextActor=i;
	gl->lastActor=i;

	gl->numActorsLive++;
}

/*============================================================
 F: Colision Actors
 M: Calcular a colicao pela distancia x,y,z entre os objetos.
 =============================================================*/

void colisionActorDetect(gameLevel *gl,Actor *act)
{int i;
 Actor *ac;
 int dx,dy;//,ag;

	i=gl->firstActor;
	while(i!=-1)
	{
		ac=gl->listActors[i];
		i=gl->listActors[i]->nextActor;  
		if((ac) && (ac!=act))
		{
			
			dx=act->xo-ac->xo;
			dy=act->yo-ac->yo;
			if(dx<0) dx=-dx;
			if(dy<0) dy=-dy;

			if( (dx<(ac->sizex+act->sizex)) && (dy<(ac->sizey+act->sizey)))
			{
				act->ActorColision=(void *)ac;
				ac->ActorColision=(void *)act;
				return ;
			}

		//	ag=get_vetor_arctan_table(dx,-dy);   // limitacao dx nao pode ser maior que 32768
			 
//			ag=ag+ANGLE_22_5;
//			if(ag>=ANGLE_360) ag=ag-ANGLE_360;

		}

	}
}

/*============================================================
 F: Run Actors
 =============================================================*/

void runActors(gameLevel *gl,int draw)
{int i,x,y;
 Actor *ac;
 void (*control) (void *,void *);


	if(!gl) return;

	i=resetActualActor(gl);
	while(i!=-1)
	{
		ac=gl->listActors[i];
		if(ac)
		{


			x=(ac->xo);//+TAMQUAD/2);
			y=(ac->yo);//+TAMQUAD/2);

			ac->xcel=((x<<PDD_FIX)+(gl->sceneLevel->realSizeTiledx*gl->sceneLevel->sizex/2))/gl->sceneLevel->realSizeTiledx;
			ac->ycel=((y<<PDD_FIX)+(gl->sceneLevel->realSizeTiledy*gl->sceneLevel->sizey/2))/gl->sceneLevel->realSizeTiledy;
			if(ac->xcel>=gl->sceneLevel->sizex) ac->xcel--;
			if(ac->ycel>=gl->sceneLevel->sizey) ac->ycel--;


			if(ac->control)   // inverti para evitar problema com delete actor
			{
				control=ac->control;

				(*control)((void *)gl,(void *)ac);	
			}
		}
		i=getNextActor(gl);  // aqui porque o next actor pode morrer na proxima interacao

	}
}

/*============================================================
 F: Draw Actors by cel
 =============================================================*/

void drawActorByCel(gameLevel *gl, int xc,int yc)
{
int i;
 Actor *ac;
 

	if(!gl) return;

	i=gl->firstActor;

	while(i!=-1)
	{
		ac=gl->listActors[i];

	
		if(ac)
		{
			if((ac->xcel==xc) && (ac->ycel==yc))
			{
				drawActor(gl,ac);
				//break;
			}
		}

		i=gl->listActors[i]->nextActor;  

	}
}



/*============================================================
 F: Draw Actors
 =============================================================*/

void drawActors(gameLevel *gl)
{
 int i;
 Actor *ac;

	if(!gl) return;

	i=gl->firstActor;
	while(i!=-1)
	{
		ac=gl->listActors[i];

		if(ac)
		{
			drawActor(gl,ac);
			
		}
		i=gl->listActors[i]->nextActor;  

	}
}

/*============================================================
 F: Draw Actors
 =============================================================*/

void drawActor(gameLevel *gl,Actor *ac)
{int x,y;
 Animate3D *ani;
 int ag;


	projOrtho3D(ac->xo,ac->yo,ac->zo,&ac->xp,&ac->yp);

	ag=ac->ago-ANGLE_45;
	if(ag<0) ag+=ANGLE_360;


	ani=ac->actions[ac->actualAction]->ani;
	calcPosView(gl,ac->xp,ac->yp,&x,&y);
	printAnimate3D(x,y,ag,ani,&gl->view,1);

//	projectActor(gl,ac,COLOR_GREEN);
//	printImageTextClip(x,y,ac->name,gl->fonts,COLOR_WHITE,&gl->view);


	//selActors(gl,ac);
}


/*============================================================
 F: Pos view camera by actor
 =============================================================*/

void posViewCameraByActor (gameLevel *gl,Actor *ac)
{
	projOrtho3D(ac->xo,ac->yo,ac->zo,&ac->xp,&ac->yp);
	//if(ac->camera)
	{
		calcViewCamera(gl,ac); 
		ajustActorCamera(gl,ac);
	}
}


/*============================================================
 F: Sel Actors
 =============================================================*/

void selActors(gameLevel *gl,Actor *ac)
{int x,y,x1,y1,x2,y2;
 Animate3D *ani;
 BMPimage *fr;

	if((!gl) || (!ac)) return;

	x=gl->mouseView.x; //+gl->xview;
	y=gl->mouseView.y;

	ani=ac->actions[ac->actualAction]->ani;
	fr=ani->frame[ani->actualFrame];

	x1=ac->xo-fr->resx/2;y1=ac->yo-fr->resy/2;
	x2=x1+fr->resx;y2=y1+fr->resy;

	if((x>x1) && (x<x2) && (y>y1) && (y<y2))
	{

		calcPosView(gl,ac->xp,ac->yp,&x1,&y1);
		x2=x1+fr->resx;y2=y1-fr->resy;
		drawRetangle(x1,y1,x2,y2,COLOR_GREEN,&gl->view); 
		ac->sel=1;
	}
	else
	{
		ac->sel=0;
	}

}


/*============================================================
 F: free Actors
 =============================================================*/

void freeActors(gameLevel *gl)
{int i,j;

	if(!gl) return;

	for(i=0;i<gl->numActors;i++)
	{
		for(j=0;j<gl->listActors[i]->numActions;j++)
		{
			freeAnimate3D(gl->listActors[i]->actions[j]->ani);
		}
	}
}

/*============================================================
 F: walk in line
 =============================================================*/

int walkInLine(gameLevel *gl,Actor *ac,float pass) 
{int dx,dy,ag;
 //float x,y;
 //int i;
   int ret=0;

	dx=ac->xd-ac->xo;
	dy=ac->yd-ac->yo;

	ag=get_vetor_arctan_table(dx,-dy);
			 
	ac->ago=ag+ANGLE_22_5;
	if(ac->ago>=ANGLE_360) ac->ago=ac->ago-ANGLE_360;

	ret= walkPass(gl,ac,pass); 

	if((ac->xo>=ac->xd) && (ac->yo>=ac->yd)) ret=1;

	return ret;

/*	pass/=MOVERESOLUTION;
	for(i=0;i<pass;i++)
	{
		if((ac->xo==ac->xd) && (ac->yo==ac->yd)) return 1;

		x=(float)(fcos(ag)*MOVERESOLUTION);
		y=(float)(fsen(ag)*MOVERESOLUTION);

	
		ret=moveInScene(gl,ac,x,y);
		//if(ret==1) break;

	} */

	return ret;


	
}

/*============================================================
 F: walk in line
 =============================================================*/

int walkPass(gameLevel *gl,Actor *ac,float pass) 
{int ag,ret=0;
 float x,y;

	ag=ac->ago;

	x=(float)(fcos(ag)*pass);
	y=(float)(fsen(ag)*pass);

	ret=moveInScene(gl,ac,x,y);

	return ret;
}

/*============================================================
 F: Interagem com cenario	
 =============================================================*/

int moveInScene(gameLevel *gl,Actor *ac,float dx,float dy)
{int sx,sy,x,y,xc,yc,ret=0,xca,yca;
 scene *sc;
 unsigned short *bp;

	sc = gl->sceneLevel;

	sx=(sc->sizex*sc->realSizeTiledx)>>(PDD_FIX+1);
	sy=(sc->sizey*sc->realSizeTiledy)>>(PDD_FIX+1);


	ac->xw+=dx;
	ac->yw-=dy;		

	ac->xo=(int)ac->xw;
	ac->yo=(int)ac->yw;


	if((ac->yo-ac->sizey)<=-sy) 
	{
		ac->yo=-sy+ac->sizey;
		ac->yw=(float)ac->yo;

		ac->agNormalColision=ANGLE_270;

		ret=2;
	}

	if((ac->yo+ac->sizey)>=sy) 
	{
		ac->yo=(sy-ac->sizey)+1;
		ac->yw=(float)ac->yo;

		ac->agNormalColision=ANGLE_270;
		ret=2;
	}

	if((ac->xo-ac->sizex)<=-sx) 
	{
		ac->xo=-sx+ac->sizex;
		ac->xw=(float)ac->xo;

		ac->agNormalColision=ANGLE_180;

		ret=2;
	}

	if((ac->xo+ac->sizex)>=sx) 
	{
		ac->xo=(sx-ac->sizex)+0;
		ac->xw=(float)ac->xo;

		ac->agNormalColision=ANGLE_180;

		ret=2;
	} 

	x=ac->xo+((sc->sizex*sc->realSizeTiledx)>>(PDD_FIX+1));
	y=ac->yo+((sc->sizey*sc->realSizeTiledy)>>(PDD_FIX+1));
	
	xca=(x-ac->sizex)+1;
	yca=(y-ac->sizey)+1;
	xc=(xca<<(PDD_FIX))/sc->realSizeTiledx;
	yc=(yca<<(PDD_FIX))/sc->realSizeTiledy;
	bp=sc->map+yc*sc->sizex+xc;
	if(sc->listTiled[*bp]->sizez>0) 
	{

		ac->xw-=dx;
		ac->yw+=dy;		

		ac->xo=(int)ac->xw;
		ac->yo=(int)ac->yw;


		xc=((xc*sc->realSizeTiledx+sc->realSizeTiledx)>>PDD_FIX);
		yc=((yc*sc->realSizeTiledy+sc->realSizeTiledy)>>PDD_FIX);

		x=ac->xo+((sc->sizex*sc->realSizeTiledx)>>(PDD_FIX+1));
		y=ac->yo+((sc->sizey*sc->realSizeTiledy)>>(PDD_FIX+1));
		xca=x-ac->sizex;
		yca=y-ac->sizey;


		if(yca<yc)
		{
			dx=(float)(xc-xca);
		}
		else
		{
			if((yca==yc) && (xc==xca))  // super gambi..
				dy=dy;
			else
				dy=(float)(yca-yc);
		}

		ac->xw+=dx;
		ac->yw-=dy;		

		ac->xo=(int)ac->xw;
		ac->yo=(int)ac->yw; 

		x=ac->xo+((sc->sizex*sc->realSizeTiledx)>>(PDD_FIX+1));
		y=ac->yo+((sc->sizey*sc->realSizeTiledy)>>(PDD_FIX+1));
	

		ret=1;

	}

	xca=x+ac->sizex;
	yca=(y-ac->sizey)+1;
	xc=(xca<<(PDD_FIX))/sc->realSizeTiledx;
	yc=(yca<<(PDD_FIX))/sc->realSizeTiledy;
	bp=sc->map+yc*sc->sizex+xc;
	if(sc->listTiled[*bp]->sizez>0) 
	{

		ac->xw-=dx;
		ac->yw+=dy;		

		ac->xo=(int)ac->xw;
		ac->yo=(int)ac->yw;


		xc=((xc*sc->realSizeTiledx)>>PDD_FIX);
		yc=((yc*sc->realSizeTiledy+sc->realSizeTiledy)>>PDD_FIX);

		x=ac->xo+((sc->sizex*sc->realSizeTiledx)>>(PDD_FIX+1));
		y=ac->yo+((sc->sizey*sc->realSizeTiledy)>>(PDD_FIX+1));
		xca=x+ac->sizex;
		yca=y-ac->sizey;


		if(yca<yc)
		{
			dx=(float)(xc-xca);
		}
		else
		{	if((yca==yc) && (xc==xca))  // super gambi..
				dy=dy;
			else
				dy=(float)(yca-yc);
		}

		ac->xw+=dx;
		ac->yw-=dy;		

		ac->xo=(int)ac->xw;
		ac->yo=(int)ac->yw; 

		x=ac->xo+((sc->sizex*sc->realSizeTiledx)>>(PDD_FIX+1));
		y=ac->yo+((sc->sizey*sc->realSizeTiledy)>>(PDD_FIX+1));

		ret=1;


	}

	xca=(x-ac->sizex)+1;
	yca=y+ac->sizey;
	xc=(xca<<(PDD_FIX))/sc->realSizeTiledx;
	yc=(yca<<(PDD_FIX))/sc->realSizeTiledy;
	bp=sc->map+yc*sc->sizex+xc;
	if(sc->listTiled[*bp]->sizez>0) 
	{

		ac->xw-=dx;
		ac->yw+=dy;		

		ac->xo=(int)ac->xw;
		ac->yo=(int)ac->yw;

		xc=((xc*sc->realSizeTiledx+sc->realSizeTiledx)>>PDD_FIX);
		yc=((yc*sc->realSizeTiledy)>>PDD_FIX);

		x=ac->xo+((sc->sizex*sc->realSizeTiledx)>>(PDD_FIX+1));
		y=ac->yo+((sc->sizey*sc->realSizeTiledy)>>(PDD_FIX+1));
		xca=x-ac->sizex;
		yca=y+ac->sizey;


		if(yca>yc)
		{
			dx=(float)(xc-xca);
		}
		else
		{

			dy=(float)(yca-yc);
		}

		ac->xw+=dx;
		ac->yw-=dy;		

		ac->xo=(int)ac->xw;
		ac->yo=(int)ac->yw; 

		x=ac->xo+((sc->sizex*sc->realSizeTiledx)>>(PDD_FIX+1));
		y=ac->yo+((sc->sizey*sc->realSizeTiledy)>>(PDD_FIX+1));

		ret=1;


	}

	xca=x+ac->sizex;
	yca=y+ac->sizey;
	xc=(xca<<(PDD_FIX))/sc->realSizeTiledx;
	yc=(yca<<(PDD_FIX))/sc->realSizeTiledy;
	bp=sc->map+yc*sc->sizex+xc;
	if(sc->listTiled[*bp]->sizez>0) 
	{

		ac->xw-=dx;
		ac->yw+=dy;		

		ac->xo=(int)ac->xw;
		ac->yo=(int)ac->yw;

		xc=((xc*sc->realSizeTiledx)>>PDD_FIX);
		yc=((yc*sc->realSizeTiledy)>>PDD_FIX);

		x=ac->xo+((sc->sizex*sc->realSizeTiledx)>>(PDD_FIX+1));
		y=ac->yo+((sc->sizey*sc->realSizeTiledy)>>(PDD_FIX+1));
		xca=x+ac->sizex;
		yca=y+ac->sizey;


		if(yca>yc)
		{
			dx=(float)(xc-xca);
		}
		else
		{

			dy=(float)(yca-yc);
		}

		ac->xw+=dx;
		ac->yw-=dy;		

		ac->xo=(int)ac->xw;
		ac->yo=(int)ac->yw; 

		x=ac->xo+((sc->sizex*sc->realSizeTiledx)>>(PDD_FIX+1));
		y=ac->yo+((sc->sizey*sc->realSizeTiledy)>>(PDD_FIX+1));

		ret=1;


	}

	return ret;
}

/*============================================================
 F: Turn actor
 =============================================================*/

int turnActor(Actor *am) 
{int dag,dag1,dag2;

	if(am->ago!=am->agd) 
	{

		dag1=am->ago-am->agd;
		if(dag1<0) dag1=-dag1;
		dag2=ANGLE_360-dag1;
		if(dag2<0) dag2=ANGLE_360+dag2;

		if(dag1<=dag2)
		{
			dag=am->ago-am->agd;
			if(dag<0)
			{
				if(dag>-VELOCITTURN ) am->ago-=dag;
				else		 am->ago+=VELOCITTURN ;		
			}
			else
			{
				if(dag<VELOCITTURN )  am->ago-=dag;
				else		 am->ago-=VELOCITTURN ;
			} 
		}
		else
		{
			dag=am->ago-am->agd;
			if(dag<0)
			{
				dag=ANGLE_360+dag;
				if(dag<VELOCITTURN ) am->ago-=dag;
				else		 am->ago-=VELOCITTURN ;	
				if(am->ago<0) am->ago=ANGLE_360+am->ago;
			}
			else
			{
				dag=ANGLE_360-dag;
				if(dag<VELOCITTURN )  am->ago+=dag;
				else		 am->ago+=VELOCITTURN ;
				if(am->ago>=ANGLE_360) am->ago=am->ago-ANGLE_360;
			} 

		}
	}
	if(am->ago==am->agd) 
		return 1;
	else
		return 0;
}

/*============================================================
 F: Control Actor Main
 =============================================================*/

void controlActorMain(gameLevel *gl,Actor *ac)
{
 float xwa,ywa;
 int xoa,yoa;

 int sx,sy;
 int i=0;


//	colisionActorDetect(gl,ac);

//	if(ac->ActorColision)
	{
		//if(ac->ActorColision!=&bala)
		//{
			//SstartIDMusic(1,0);
			//removeActor(gl,ac->ActorColision);
		//}
//		ac->ActorColision=0; 
	}

	if(ac->actualAction==PUNCH)
	{
		if(ac->actions[ac->actualAction]->ani->actualFrame==0)
			ac->actualAction=STANDACTION;
	}
	else
	{

		ac->actualAction=STANDACTION;

		sx=(gl->sceneLevel->sizex*gl->sceneLevel->realSizeTiledx)>>(PDD_FIX+1);
		sy=(gl->sceneLevel->sizey*gl->sceneLevel->realSizeTiledy)>>(PDD_FIX+1);

		xwa=ac->xw;ywa=ac->yw;xoa=ac->xo;yoa=ac->yo;


		if(gl->key & BTN_RIGHT)
		{	ac->agd+=ANGLE_10;
  			if(ac->agd>=ANGLE_360) ac->agd=ac->agd-ANGLE_360;
			ac->ago=(ac->agd/ANGLE_45);
			ac->ago*=ANGLE_45;
		}

		if(gl->key & BTN_LEFT)
		{
			ac->agd-=ANGLE_10;
  			if(ac->agd<0) ac->agd=ac->agd+ANGLE_360;
			ac->ago=(ac->agd/ANGLE_45);
			ac->ago*=ANGLE_45;
		}

		if(gl->key&BTN_A)
		{
			ac->zo+=5;
		}
       
		if(gl->key&BTN_B)
		{
			ac->zo-=5;
		}

		if(gl->key&BTN_C)
		{
			ac->actualAction=PUNCH;
		}
        
        

		if(gl->key & BTN_UP)
		{
			ac->actualAction=WALKACTION;
					
			walkPass(gl,ac,PASSWALK); 

		}

		
		if (gl->key & BTN_DOWN)
		{
			ac->actualAction=WALKACTION;

			walkPass(gl,ac,-PASSWALK); 
		}

		if(gl->mouseView.status & MOUSE_DOWN)
		{
			ac->actualAction=WALKACTION;
			ac->xd=gl->mouseView.x;
			ac->yd=gl->mouseView.y; 
			walkInLine(gl,ac,PASSWALK); 

			ac->ago=(ac->ago/ANGLE_45);  // para garantir movimento em angulos de 45 graus
			ac->ago*=ANGLE_45;

			ac->state=1;

			
		/*	if(getCurrentTime()-ac->control0>200)
			{
				ac->control0=getCurrentTime();
				createActor(gl,ac->xo,ac->yo,ac->zo-150,ac->ago,&bala,ac,ACTORTEMP);
			} */
		}
		else
		{
			ac->actualAction=WALKACTION;

			ac->state=0;

			i=walkPass(gl,ac,PASSWALK);

			if(i)
			{
		
				ac->ago-=myrand()&0xfff;
				if(ac->ago<0) ac->ago+=ANGLE_360;
	
				ac->ago=(ac->ago/ANGLE_45); // para garantir movimento em angulos de 45 graus
				ac->ago*=ANGLE_45;
		
			} 
		} 

		if(gl->buttonFire)
		{
			if(getCurrentTime()-ac->control0>200)
			{
				ac->control0=getCurrentTime();
				createActor(gl,ac->xo,ac->yo,ac->zo-150,ac->ago,&bala,ac,ACTORTEMP);
			}
		}

	}
		
}

/*============================================================
 F: Control Actor Test
 =============================================================*/

void controlActorBala(gameLevel *gl,Actor *ac)
{Actor *acColision;
 int ret;

	colisionActorDetect(gl,ac);

	if(ac->ActorColision)
	{
		acColision=(Actor *)ac->ActorColision;
		if((ac->ActorColision!=ac->ActorParent) && (acColision->type!=ACTORFIRE))
		{
			SstartIDMusic(1,0);
			removeActor(gl,ac->ActorColision);
			removeActor(gl,ac);

			return;
		}
		ac->ActorColision=0; 
	}


	ac->vel=(int)(5*PASSWALK);
	if(ret=walkPass(gl,ac,(float)ac->vel))
			removeActor(gl,ac);
}

/*============================================================
 F: Control Actor Test
 =============================================================*/

void controlActorTest(gameLevel *gl,Actor *ac)
{Actor *acR=0;
 int i;
	
	//acR=ac->ActorColision;

	//if(acR)
	{
		ac->vel=(int)(1*PASSWALK);
		ac->ActorColision=0;

	//	ac->ago=acR->ago;

	}
    
    if(gl->mouseView.status & MOUSE_DOWN)
    {
        int dx=gl->mouseView.x-ac->xo;
        int dy=gl->mouseView.y-ac->yo;
        if(dx<0) dx=-dx;
        if(dy<0) dy=-dy;
        
       // printf("dx=%d dy=%d\n",dx,dy);
        
        if((dx<ac->sizex*2) && (dy<ac->sizey*2))           
        {
            removeActor(gl,ac);
           // SstartIDMusic(1,0);
            return;
            
        }
	}	

	if(ac->vel)
	{
	/*	if(ac->control0==0) ac->control0=-6;
		ac->zo+=ac->control0;
		if(ac->zo<-80) ac->control0=-ac->control0;
		if(ac->zo>=0) ac->control0=-ac->control0; */
		
		i=walkPass(gl,ac,(float)ac->vel);
		if(ac->vel>PASSWALK)
		ac->vel--;

	
		if(getCurrentTime()-ac->control0>500)
		{
			ac->control0=getCurrentTime();
			
			colisionActorDetect(gl,ac);
            
            acR=ac->ActorColision;

			if(acR)
			{

                ac->ago=(acR->ago-ac->ago);
                if(ac->ago<0) ac->ago+=ANGLE_360; 
                
                
				ac->ActorColision=0;
				//ac->ago-=myrand()&0xfff;
				//if(ac->ago<0) ac->ago+=ANGLE_360; 
	
				ac->ago=(ac->ago/ANGLE_45);  // para garantir movimento em angulos de 45 graus
				ac->ago*=ANGLE_45;
				
			} 
		} 

		//if(i=walkPass(gl,ac,PASSWALK))
		if(i)
		{
			//ac->ago=ac->agNormalColision-ac->ago;
  			//if(ac->ago<0) 
			//	ac->ago=ac->ago+ANGLE_360;

			if(ac->state==0)
			{
				ac->state++;
				if(gl->numActorsLive<100)
				{
					acR=createActor(gl,ac->xo,ac->yo,ac->zo,ac->ago,ac,ac,ACTORTEMP);

					if(acR!=0)
					{
						acR->ago+=ANGLE_180;
						if(acR->ago>=ANGLE_360) acR->ago-=ANGLE_360;

						acR->ago-=myrand()&0xfff;
						if(acR->ago<0) acR->ago+=ANGLE_360;

						//SstartIDMusic(1,0);
					}
				}
			}
			ac->ago-=myrand()&0xfff;
			if(ac->ago<0) ac->ago+=ANGLE_360;

			ac->ago=(ac->ago/ANGLE_45);  // para garantir movimento em angulos de 45 graus
			ac->ago*=ANGLE_45;


		//	if(i==2)
		//		removeActor(gl,ac);
		} 
	}

}