//
//  ui.h
//  glgame
//
//  Created by Valeria Thome Izar on 23/04/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//



typedef enum _wipes
{
    IDLE = 0,
	FADEIN ,			
	FADEOUT

} wipeType;

typedef struct _wipe
{
	wipeType type;

    float r,g,b,a;
    long time;
    int state;
    BMPimage *view;
    
} wipe;

int wipeView(wipe *gl);
int setWipe(wipe *gl,wipeType type);
int fadeColorScreen(wipe *gl);



