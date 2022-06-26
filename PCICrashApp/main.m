@import Foundation;
@import SystemExtensions;

@interface WDBSystemExtensionDelegate: NSObject<OSSystemExtensionRequestDelegate>
@end

@implementation WDBSystemExtensionDelegate
- (void)request:(OSSystemExtensionRequest *)request
didFinishWithResult:(OSSystemExtensionRequestResult)result {
    NSLog(@"didFinishWithResult: %ld", result);
    exit(0);
}
- (void)request:(OSSystemExtensionRequest *)request didFailWithError:(NSError *)error {
    NSLog(@"didFailWithError: %@", error);
    exit(1);
}
- (void)requestNeedsUserApproval:(OSSystemExtensionRequest *)request {
    NSLog(@"requestNeedsUserApproval");
}

- (OSSystemExtensionReplacementAction)request:(OSSystemExtensionRequest *)request
                  actionForReplacingExtension:(OSSystemExtensionProperties *)existing
                                withExtension:(OSSystemExtensionProperties *)ext {
    NSLog(@"actionForReplacingExtension %@ withExtension %@", existing, ext);
    return OSSystemExtensionReplacementActionReplace;
}
@end

static WDBSystemExtensionDelegate* gMainDelegate;

int main() {
    gMainDelegate = [WDBSystemExtensionDelegate new];
    NSString* bundleId = [[NSBundle mainBundle].bundleIdentifier stringByAppendingString:@".PCICrash"];
    OSSystemExtensionRequest* request = [OSSystemExtensionRequest activationRequestForExtension:bundleId queue:dispatch_get_main_queue()];
    request.delegate = gMainDelegate;
    [OSSystemExtensionManager.sharedManager submitRequest:request];
    CFRunLoopRun();
}
