#include "crashreporter.h"

#import <Breakpad.h>
#include <QDebug>

CrashReporter::CrashReporter()
{
}

void CrashReporter::init()
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    BreakpadRef breakpad = NULL;
    NSDictionary* info = [[NSBundle mainBundle] infoDictionary];
    if(info){
        qDebug() << "Initing breakpad";
        breakpad = BreakpadCreate(info);
    }
    [pool release];
}
