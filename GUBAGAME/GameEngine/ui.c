//
//  ui.c
//  glgame
//
//  Created by Valeria Thome Izar on 23/04/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include "so.h"
#include "graphic.h"
#include "math.h"
#include "ui.h"

/*============================================================
 F: transiction view
 =============================================================*/

int wipeView(wipe *gl)
{
    
    switch(gl->type)
    {
        case IDLE:
            break;
        case FADEIN:
        case FADEOUT:
            if(fadeColorScreen(gl))
                gl->type=IDLE;
            break;
            
    }
    return 0;
}

/*============================================================
 F: setWipe
 =============================================================*/

int setWipe(wipe *gl,wipeType type)
{
    gl->type=type;
    gl->state=0;
    
    return 0;
    
    
}

/*============================================================
 F: Fade color prototipo
 =============================================================*/

int fadeColorScreen(wipe *gl)
{
    float pass;
    
    
    if(gl->state==0)
    {
        if(gl->type==FADEOUT)
        {
            gl->r=0;
            gl->g=0;
            gl->b=0;
            
        }
        else
        {
            gl->r=1.0f;
            gl->g=1.0f;
            gl->b=1.0f;
        }
        gl->state=1;
    }
    
    if(gl->type==FADEOUT) pass=0.1f;
    else             pass=-0.1f;
    
    if(getCurrentTime()-gl->time>50)
    {
        gl->time=getCurrentTime();
        
        
        gl->r+=pass;
        gl->g+=pass;
        gl->b+=pass;
        
        
        if((gl->r<=0.0f) || (gl->r>=1.0f))
        {    
            gl->state=0;
            return 1;
        }
        
        nativeFadeColorScreen(gl->r,gl->g,gl->b,1.0f);   //usando funcao nativa pela primeira vez
        
    }
    
    if(gl->state==0) 
        return 1;
    else 
        return 0;
    
    return 0;
    
}

