//
//  glgameAppDelegate.h
//  glgame
//
//  Created by Valeria Thome Izar on 14/07/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class glgameViewController;

@interface glgameAppDelegate : NSObject <UIApplicationDelegate> {

}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@property (nonatomic, retain) IBOutlet glgameViewController *viewController;


@end
