set -e
path="/Users/zhuowei/Library/Developer/Xcode/DerivedData/PCICrash-ckzcqoivusefroeboxjynqceoipz/Build/Products/Debug/PCICrashApp.app"
dpath="$path/Contents/Library/SystemExtensions/com.worthdoingbadly.PCICrashApp.PCICrash.dext"
codesign -s - -f --entitlements "PCICrash/PCICrash.entitlements" "$dpath"
codesign -s - -f --entitlements "PCICrashApp/PCICrashApp.entitlements" "$path"
echo "rm -r PCICrashApp.app
put -r $path" | sftp zhuowei-mini.local
