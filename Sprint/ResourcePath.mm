//
//  ResourcePath.cpp
//  Sprint
//
//  Created by Thomas Redding on 11/17/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#include "ResourcePath.hpp"
#include <Cocoa/Cocoa.h>

std::string desktopPath() {
	NSArray * paths = NSSearchPathForDirectoriesInDomains (NSDesktopDirectory, NSUserDomainMask, YES);
	NSString * desktopPath = [paths objectAtIndex:0];
	std::string rtn = [desktopPath UTF8String];
	return rtn;
}
