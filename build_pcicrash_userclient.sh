set -e
clang -fmodules pcicrash_userclient.m -target arm64-apple-macos12 -o pcicrash_userclient
#codesign -s - -f --entitlements pcicrash_userclient.entitlements pcicrash_userclient
