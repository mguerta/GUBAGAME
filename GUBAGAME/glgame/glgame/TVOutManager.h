//
//  TVOutManager.h

#import <Foundation/Foundation.h>


@interface TVOutManager : NSObject {
    
	UIWindow* deviceWindow;
	UIWindow* tvoutWindow;
	NSTimer *updateTimer;
	UIImage *image;
	UIImageView *mirrorView;
	BOOL done;
	BOOL tvSafeMode;
	CGAffineTransform startingTransform;
}

@property(assign) BOOL tvSafeMode;


+ (TVOutManager *)sharedInstance;

- (void) startTVOut;
- (void) stopTVOut;
- (void) updateTVOut;
- (void) updateLoop;
- (void) screenDidConnectNotification: (NSNotification*) notification;
- (void) screenDidDisconnectNotification: (NSNotification*) notification;
- (void) screenModeDidChangeNotification: (NSNotification*) notification;
- (void) deviceOrientationDidChange: (NSNotification*) notification;

@end
