#include "crashreporter.h"

#import <Breakpad.h>
#include <QDebug>

void CrashReporter::init()
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    BreakpadRef breakpad = NULL;
    NSDictionary* info = [[NSBundle mainBundle] infoDictionary];
    if(info){
        breakpad = BreakpadCreate(info);
    }
    [pool release];
}
