//
//  PCICrash.cpp
//  PCICrash
//
//  Created by Zhuowei Zhang on 2022-06-19.
//

#include <os/log.h>

#include <DriverKit/IOUserServer.h>
#include <DriverKit/IOLib.h>

#include "PCICrash.h"

kern_return_t
IMPL(PCICrash, Start)
{
    kern_return_t ret;
    ret = Start(provider, SUPERDISPATCH);
    os_log(OS_LOG_DEFAULT, "Hello World");
    return ret;
}
