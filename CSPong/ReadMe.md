**Overview**

This Chilli Source sample project showcases
a simple 3D pong game on iOS and Android and demonstrates the following
features:

•  
App
creation

•  
States

•  
Entities

•  
Components

•  
Resource
loading

•  
Lighting

•  
Shadows

•  
Pointer
input

•  
Accelerometer

•  
Sprites

•  
Static
meshes

•  
GUI

•  
Events

 

**Requirements**

Prior to running the sample the following
should be installed:

•  
Python
2.7.5+

•  
Xcode
5.1+

•  
Eclipse

•  
Android
SDK 21+

•  
Android
NDK r10c+

•  
Android
2.3+

•  
iOS
5.1+

 

**Projects**

** **The Eclipse and Xcode projects are in
CSPong/Projects/. The AndroidManifest.xml is auto-generated but its contents
can be changed from CSAndroidManifest.xml.

 

**Tour**

 Starting at the top of AppSource/CSPong.cpp
there is a commented tour that will lead you through the source code explaining
each main feature in more detail. By the end of the tour you should have an
understanding of how a standard CS app is structured and how to make use of the
most common features.

 

**Content Build Process**

 As well as the application a simple
“content build process” has been included in the sample. This process takes raw
assets such as .png and .dae files and converts them into CS ready resources
(i.e. meshes, textures, texture atlases, etc.). Although all conversions use
the CS tool suite, there is no standard pipeline for converting assets as this
can be tailored to each project’s needs. The raw assets are in
CSPong/Content/PrebuiltResources/ and the processed assets are in
CSPong/Content/AppResources/. The “BuildAll.command”, in the PrebuiltResources
folder, can be executed in order to build all the resource types (NOTE: This
will delete and overwrite matching folders from PrebuiltResources inside
AppResources). Assets that require no additional processing are placed directly
into AppResources (i.e. material and GUI files, fonts, etc.). More information
on the build process can be gleaned from reading the source in
CSPong/Content/PrebuiltResources/Scripts/
