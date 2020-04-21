# firebase_on_mac

The project works fine on :

```
Xcode : 11.3.1
MacOS: 10.15.3 Catalina
Firebase cpp sdk : 6.12
```

Installation
---------------
1.`cd` to `proj.ios_mac` folder

```
pod install
```

2.Add libs below to your Other Linker Flags in Xcode.
```
"[firebase_cpp_sdk_6.12 path on your computer]/firebase_cpp_sdk/libs/darwin/universal/libfirebase_auth.a"
"[firebase_cpp_sdk_6.12 path on your computer]/firebase_cpp_sdk/libs/darwin/universal/libfirebase_analytics.a"
"[firebase_cpp_sdk_6.12 path on your computer]/firebase_cpp_sdk/libs/darwin/universal/libfirebase_app.a"
"[firebase_cpp_sdk_6.12 path on your computer]/firebase_cpp_sdk/libs/darwin/universal/libfirebase_database.a"
```

3. If you want to reproduce bug , please take a look at the `"Signing & Capabilities"` tab. If we
+ remove the `"App Sanbox"` , all codes work correctly -> the helloword.png image will appear immediately in the center of the screen.
+ add the `"App Sanbox"` -> The query takes a long time to respond (1 to 3 minutes) to return a response. You can see the code in HelloWord.cpp file   
