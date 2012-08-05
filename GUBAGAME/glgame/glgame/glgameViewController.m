//
//  glgameViewController.m
//  glgame
//
//  Created by Valeria Thome Izar on 14/07/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import <AVFoundation/AVFoundation.h>

#import "glgameViewController.h"
#import "EAGLView.h"

#import "so.h"
#import "graphic.h"
#import "animate.h"
#import "math.h"



#define ITOXX(i)						((i) << 16)

int VRESX; // =768; //320  //especifico para cada device
int VRESY; // =1024; //480 

#define PRESX VIDEO_WIDTH //512 //640
#define PRESY VIDEO_HEIGHT //480

#define PCRESX VIDEO_WIDTH 
#define PCRESY VIDEO_HEIGHT 



long firstCurrent;

unsigned char *fb;
int cor;
CFAbsoluteTime myCurrentTime,delta;
int fps;

int xp,yp,xpi,ypi;
int joyVirtual;

tmouse mTouch;

BMPimage im1,fbimg;

//SystemSoundID sounds[10];
AVAudioPlayer *player[100];


float gColorRed,gColorBlue,gColorGree,gColorAlfa;


// Uniform index.
enum {
    UNIFORM_TRANSLATE,
    NUM_UNIFORMS
};
GLint uniforms[NUM_UNIFORMS];

// Attribute index.
enum {
    ATTRIB_VERTEX,
    ATTRIB_COLOR,
    NUM_ATTRIBUTES
};

@interface glgameViewController ()
@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) CADisplayLink *displayLink;
@end

@implementation glgameViewController

@synthesize animating, context, displayLink;

fontText fontsGl[]={"./res/fontGRBBold17.tga",29,32,4};

/*
void setThread:(NSString *)urlBase
      NSInteger:(NSInteger)dispatchPeriod
{
    period=dispatchPeriod;
    sUrlBase = [[NSString alloc] initWithString:urlBase];
    [NSThread detachNewThreadSelector:@selector(run) 
                             toTarget:self 
                           withObject:nil];   
} 
*/


glgameViewController *me;

int loadAVSound(const char *name,int idx,int loop)
{
    
    NSString *tmp = [NSString stringWithUTF8String:name];
    NSString *soundPath = [[NSBundle mainBundle] pathForResource:tmp ofType:nil];
    
    player[idx] =[[AVAudioPlayer alloc] initWithContentsOfURL:[NSURL fileURLWithPath: soundPath] error:nil];
    
    player[idx].volume = 1.0;
    
    player[idx].numberOfLoops = loop; // play once
    
   // player[idx].meteringEnabled=true;
    
    [player[idx] prepareToPlay];
    
    return idx;
    
}

void playAVSound(int idx,int loop)
{
    player[idx].currentTime=0;
    player[idx].numberOfLoops = loop;
    [player[idx] play];
    
}

unsigned long getTimeCurrent(void)
{unsigned long tempo;
    
    float t=CFAbsoluteTimeGetCurrent()-firstCurrent;
    
    tempo=(unsigned long)(t*1000);
    
    return tempo;
}


long long getTimeMicro(void)
{long long tempo;
    
    float t=CFAbsoluteTimeGetCurrent()-firstCurrent;
    
    tempo=(long long)(t*1000000);
    
    return tempo;
}


char * getFileName (void)
{
    
    //  NSString *path = [[NSBundle mainBundle] pathForResource: @"lives.tga" ofType:nil];
    
    NSString *path = [[NSBundle mainBundle] pathForResource: nil ofType:nil inDirectory:nil];
    
    
    return [path UTF8String];
    
}

int getJoyIphone(void)
{
    
    return joyVirtual;
}



- (void)awakeFromNib
{
    
    [super awakeFromNib];
           
    
    EAGLContext *aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1]; //2];
    
    if (!aContext) {
        aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    }
    
    if (!aContext)
        NSLog(@"Failed to create ES context");
    else if (![EAGLContext setCurrentContext:aContext])
        NSLog(@"Failed to set ES context current");
    
	self.context = aContext;
	[aContext release];
	
    [(EAGLView *)self.view setContext:context];
    [(EAGLView *)self.view setFramebuffer];
    
    [(EAGLView *)self.view frame];
    
    
    EAGLView * t=(EAGLView *)self.view;   // screen size ipad or iphone (full screen)
    VRESX=t->framebufferWidth;
    VRESY=t->framebufferHeight;
    
    
    if ([context API] == kEAGLRenderingAPIOpenGLES2)
        [self loadShaders];
    
    animating = FALSE;
    animationFrameInterval = 1;  // 1 60 fps  2 30 fps
    self.displayLink = nil;
    
    me=self;

    fb=malloc(VIDEO_WIDTH*VIDEO_HEIGHT*4); //alloca buffer textura 
    
    

    mainIphone();
    
    fbimg.buf=(pixel *)fb;
    fbimg.resx=VIDEO_WIDTH;
    fbimg.resy=VIDEO_HEIGHT;
    
    setActualView(VRESX,VRESY);
    
    
    gameInit();
    
    
    firstCurrent=CFAbsoluteTimeGetCurrent();
    
 
}


- (void)dealloc
{
    if (program) {
        glDeleteProgram(program);
        program = 0;
    }
    
    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
    
    [context release];
    
    [super dealloc];
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewWillAppear:(BOOL)animated
{
    [self startAnimation];
    
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [self stopAnimation];
    
    [super viewWillDisappear:animated];
}

- (void) viewDidLoad
{
    [super viewDidLoad];
        
    [self.view setBackgroundColor:[UIColor blackColor]];
    UIImage *image;
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)    
        image = [UIImage imageNamed:@"Default-Portrait~ipad.png"];
    else
        image = [UIImage imageNamed:@"Default.png"];
    
    UIImageView *defaultImage = [[UIImageView alloc] initWithImage:image];
   
    [self.view addSubview:defaultImage];
    
    
    [UIView beginAnimations:@"fadeout" context:NULL];
    [UIView setAnimationDuration:3];
    defaultImage.alpha=0;
    [UIView commitAnimations];
    
    
    [defaultImage release]; 
    
   }

- (void)viewDidUnload
{
	[super viewDidUnload];
	
    if (program) {
        glDeleteProgram(program);
        program = 0;
    }

    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
	self.context = nil;	
}

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    
    
    /*
	 Frame interval defines how many display frames must pass between each time the display link fires.
	 The display link will only fire 30 times a second when the frame internal is two on a display that refreshes 60 times a second. The default frame interval setting of one will fire 60 times a second when the display refreshes at 60 times a second. A frame interval setting of less than one results in undefined behavior.
	 */
    if (frameInterval >= 1) {
        animationFrameInterval = frameInterval;
        
        if (animating) {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

int numFrames;
- (void)startAnimation
{

    
    if (!animating) {
        
        numFrames=0;
       // numFrames=1;
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
            numFrames=8;
        
        CADisplayLink *aDisplayLink = [[UIScreen mainScreen] displayLinkWithTarget:self selector:@selector(drawFrame)];

        [aDisplayLink setFrameInterval:animationFrameInterval];
        [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        self.displayLink = aDisplayLink;
        
        animating = TRUE;
    } 
    //comentar para desativar
    
   }

- (void)stopAnimation
{
    if (animating) {
        [self.displayLink invalidate];
        self.displayLink = nil;
        animating = FALSE;
    }
}

int flgRunGame=0;
- (void) runGame
{
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];   
    do {
        if(flgRunGame&0x80)
        {
          gameMainOpen();
            flgRunGame^=0x80;
        }
   
    } while (1);
   
    
       
  [pool release];
    
}


- (void)drawFrame
{int frames=0;
    
    do {
        
        if(!gameMainOpen()) 
            continue;   
        
    } while (frames++<numFrames);
    
    
}

void iphoneFadeColorScreen(colorType *c)
{

    gColorRed=c->r;
    gColorBlue=c->b;
    gColorGree=c->g;
    gColorAlfa=c->a;

}

void iphoneFlipVideo(void)
{
    
    // Replace the implementation of this method to do your own custom drawing.
    
    float texCoords1[] = {
        0, (float)((float)PRESY/(float)PCRESY),				
		(float)((float)PRESX/(float)PCRESX),(float)((float)PRESY/(float)PCRESY),
        (float)((float)PRESX/(float)PCRESX), 0,
        0,0
	}; 
    
    int squareX[12]; 
    int h,w;
    

    
    EAGLView * t=(EAGLView *)me.view;   // screen size ipad or iphone (full screen)
    
    VRESX=t->framebufferWidth;
    VRESY=t->framebufferHeight;
    setActualView(VRESX,VRESY);
    

        
        [(EAGLView *)me.view setFramebuffer];
        
        delta = CFAbsoluteTimeGetCurrent()-myCurrentTime;
        
        
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glColor4f(gColorRed,gColorBlue,gColorGree,gColorAlfa);

        
        glBindTexture(GL_TEXTURE_2D, 0);
        glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
#ifdef BPP16
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, PCRESX, PCRESY, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, fb); 
#else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, PCRESX, PCRESY, 0, GL_RGBA, GL_UNSIGNED_BYTE, fb);      
#endif
        
        //----------------------
        
        h=PRESY;
        w=PRESX;
        
        glViewport (0, 0, VRESX, VRESY);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity ();
        
        glOrthof(0, PRESX, 0, PRESY, -3*PRESX, 3*PRESY);
        
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
		glDisable (GL_DEPTH_TEST);
        glDisable (GL_BLEND);
        
        
		glEnable(GL_TEXTURE_2D);
        
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, 0);
        
		glPushMatrix();
		glLoadIdentity();
        
        
        squareX[0]=ITOXX(w);squareX[1]=0;squareX[2]=0;          //tombado
        squareX[3]=ITOXX(w);squareX[4]=ITOXX(h);squareX[5]=0;
        squareX[6]=0;squareX[7]=ITOXX(h);squareX[8]=0;
        squareX[9]=0;squareX[10]=0;squareX[11]=0;
    
    
           
/*     squareX[0]=0;squareX[1]=0;squareX[2]=0;                 //normal
     squareX[3]=ITOXX(w);squareX[4]=0;squareX[5]=0;
     squareX[6]=ITOXX(w);squareX[7]=ITOXX(h);squareX[8]=0;
     squareX[9]=0;squareX[10]=ITOXX(h);squareX[11]=0;
 */    

        
	    
		glVertexPointer(3, GL_FIXED, 0, squareX);	
		glTexCoordPointer(2, GL_FLOAT, 0, texCoords1);
        
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        
	    glPopMatrix();
        
        
        [(EAGLView *)me.view presentFramebuffer];
    
}


- (void)motionBegan:(UIEventSubtype)motion withEvent:(UIEvent *)event
{
    
   // printf("motion began\n");
}

- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event
{
	if (motion == UIEventSubtypeMotionShake )
	{
		// User was shaking the device. Post a notification named "shake".
		[[NSNotificationCenter defaultCenter] postNotificationName:@"shake" object:self];
	}
}

- (void)motionCancelled:(UIEventSubtype)motion withEvent:(UIEvent *)event
{	
}




// Handles the start of a touch
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
  //  printf("tocou began\n");
    UITouch *touch = [touches anyObject];
    
	
    CGPoint location = touch->_locationInWindow;
    
   
    mTouch.x=location.x;
    mTouch.y=location.y;
    mTouch.status=1;
    
    setMousePosition(&mTouch);
    
 
    
}

// Handles the continuation of a touch.
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{  
    UITouch *touch = [touches anyObject];

	
    CGPoint location = touch->_locationInWindow;
    

    mTouch.x=location.x;
    mTouch.y=location.y;
    mTouch.status=1;
    
    setMousePosition(&mTouch);
    
    

    
}

// Handles the end of a touch event when the touch is a tap.
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    
    
    
	
    CGPoint location = touch->_locationInWindow;
    
    
    mTouch.x=location.x;
    mTouch.y=location.y;
    mTouch.status=0;
    
    setMousePosition(&mTouch);
    
  //  printf("tocou End\n");
    joyVirtual=0;

    
    
    
}

// Handles the end of a touch event.
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	// If appropriate, add code necessary to save the state of the application.
	// This application is not saving state.
    
    
    
}

-(BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
   // return YES;
     return NO;
} 



@end
