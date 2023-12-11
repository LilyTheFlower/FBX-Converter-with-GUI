# FBX-Converter-with-GUI
A GUI program to convert between binary and ascii FBX using the FBX sdk and Qt

## Featues:
-multiple and single file selection and conversion
-filter files based on format for ascii or binary
-displays file format
-optionally log actions taken on files
-workspace settings are saved after the program closes and reloaded when it opens

## How to use:
1.When the program is opened, select a source directory where your original fbx file is
2.Select a destination directoy where you want the converted fbx file to be placed
3.The table will fill with fbx files located in the source directory and label them as ascii or binary
4.Select a number of files, holding ctrl will allow you to select multiple or use the selection tool
5.To choose the format to convert to use the drop down at the bottom of the window to choose ascii or binary
6.Push the convert button and the conversion will begin, new files will be placed in the destination directory with an altered name if replace originals is not selected.

## Build Requirements:
To build this project you need to have: 
-Qt 1.0.18 - 6.6.0 or greater and Qt creator
-AutoDesk FBX sdk: https://aps.autodesk.com/developer/overview/fbx-sdk

If you dont want to build I will include a pre-built version for windows x64 users using the latest code in the directory

## Platforms:
As of 12/10/23:
Windows: Working
Linux: Should be fine, but I havent checked yet, I will update this when I have

## Notes:
This project uses three classes I will soon be putting on my github as libraries. 
-fileSaver: Allows any kind of data in C++ to be saved and recalled from a file, simply pass the file to store is, the pointer, the size of the data and give it a label. To recall the data pass the label and the file where the data is stored
-logger: Write messages to a log file labeled with the time of its creation and each log message is pre-fixed with the time its added
-fbxconverter: If you are in the very niche circumstance of wanting to write your own program that does fbx conversion you can use this to interface with the sdk a little more simply.

## Why did I make this?:
There are many tools like this one, but all I have seen use a command line
interface. That is fine for people who are used to such an interface, like
programmers, but I wanted to create an interface friendly to artists and
designers who werent comfortable with that interface. I also feel this interface 
facilitates simpler directory selection.

Personally I had an issue where Blender does not import ascii fbx, and I wanted to learn
Qt. I also like working on projects and I wanted to show my ability with Qt on my resume, so this
was perfect. My personal style of programming tends to be more procedural than straight object
oriented, and I wanted to exemplify that as well. This was also a great oppurtunity to make
something that didnt just work and get the job done, but goes above and beyond with quality of
life featues and air-tight error checking and handling.

## New Release:
I will continue to run tests as I devise new scenarios for this program. I will also be testing this program on other platforms soon, if you have an issue, go ahead and shoot me an email so I can get it resolved!
