// MPTVOutWindow.m
//
// Copyright 2009 Stinkbot LLC. All rights reserved.
// Copyright 2009 RedSky IT. All rights reserved.
//


#ifndef DEBUG
#  error trying to include TV Out code in a non-debug build
#endif

#import "MPTVOutWindow.h"
#import <QuartzCore/QuartzCore.h>

#define degreesToRadian(x) (M_PI * (x) / 180.0)

static MPTVOutWindow *tvout = nil;
static UIImageView *imageView = nil;
static BOOL done = NO;
static int FPS = 10;

CGAffineTransform tnormal;

@implementation MPTVOutWindow (extended)

+ (void) createAndMakeKeyWindow {
    MPVideoView *vidView = [[MPVideoView alloc] initWithFrame:CGRectZero];
    tvout = [[MPTVOutWindow alloc] initWithVideoView:vidView];  
    
    imageView = [[UIImageView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    tnormal = imageView.transform;
    imageView.center = vidView.center;
    
    [vidView addSubview:imageView];
    [imageView release];
    [vidView release];
    
    [tvout makeKeyAndVisible];
}

+ (void) outputScreenToTv {
    CGImageRef UIGetScreenImage();
    CGImageRef screen = UIGetScreenImage();		// Auto-release (supposedly)
    if (screen) {
        UIImage *image = [[UIImage alloc] initWithCGImage:screen];
        CFRelease(screen);		// BUT! client crashes after a few fraees if we DONT release this
        if ([UIApplication sharedApplication].statusBarOrientation == UIInterfaceOrientationPortrait) {
            imageView.transform = tnormal;
        } else if ([UIApplication sharedApplication].statusBarOrientation == UIInterfaceOrientationLandscapeLeft) {
            imageView.transform = CGAffineTransformRotate(tnormal, degreesToRadian(90));;
        } else if ([UIApplication sharedApplication].statusBarOrientation == UIInterfaceOrientationLandscapeRight) {
            imageView.transform = CGAffineTransformRotate(tnormal, degreesToRadian(-90));
        } else if ([UIApplication sharedApplication].statusBarOrientation == UIInterfaceOrientationPortraitUpsideDown) {
            imageView.transform = CGAffineTransformRotate(tnormal, degreesToRadian(180));
        }
        imageView.image = image;
        [image release];
    }
}

+ (void) updateLoop
{
    done = NO;
    while (!done) {
        NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
        [self performSelectorOnMainThread:@selector(outputScreenToTv) withObject:nil waitUntilDone:NO];
        [NSThread sleepForTimeInterval: (1.0/FPS) ];
        [pool drain];
    }
}

+ (void) startTvOut:(int)fps {
    FPS = fps;
    [NSThread detachNewThreadSelector:@selector(updateLoop) toTarget:self withObject:nil];
}

+ (void) stopTvOut {
    done = YES;
}

@end

@implementation MPVideoView (extended)
- (void) addSubview: (UIView *) aView
{
    [super addSubview:aView];
}
@end 

