<img width="512" src="./mak3do_logo.svg">

 # mak3do 2.0 - Scene graph-based engine

 mak3do is a C++17 game engine that abstracts an underlying scene graph implementation like 
 SceneKit from Apple or SceneForm from Android

 ## Features supported today
 * glTF loaded scenes
 * Scenekit (.scn) scenes (only on iOS, watchOS and macOS)
 * GameObject API with 2D (box2D) and 3D (bullet3) physics
 * Actions for node animations (not SceneKit-native's ones)
 * Materials (partial)

 ## Plans
* Filament-based SceneGraph

## How to build

### iOS, tvOS, macOS

```
#1 Clone the repo
#2 update modules
$ git submodule update --init --recursive
#3 Open XCode 11
# Build the targets mak3do-sandbox, mak3do-sandbox-tv or mak3do-sandbox-mac for iOS, tvOS and macOS respectively to run the tests
```

