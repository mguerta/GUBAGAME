// MPTVOutWindow.h
//
// Copyright 2009 Stinkbot LLC. All rights reserved.
// Copyright 2009 RedSky IT. All rights reserved.
//


@class UIImage;

@interface MPTVOutWindow : UIWindow
- (id)initWithVideoView:(id)fp8;
+ (void) startTvOut:(int)fps;
+ (void) stopTvOut;
@end

@interface MPTVOutWindow (extended)
+ (void) createAndMakeKeyWindow;
+ (void) outputScreenToTv;
@end

@interface MPVideoView : UIView
- (id)initWithFrame:(struct CGRect)fp8;
@end

