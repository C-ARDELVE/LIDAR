# LIDAR
LIDAR development

Building the utility
++++++++++++++++++++

The development activity is currently happening on a Apple OSX platform using xcode. TO build the app use the followign command.

xcodebuild

Or build from within the IDE.

If you use xcodebuild the application executable will appear in the build/Release directory and is currently named Lidar_data

I've also provided a make file that will also generate an executable, which may be useful if you are building on an alternative unix platform, the executable is just placed under build, and has the same name, to make using teh Makefile just type

make

Running the command
+++++++++++++++++++

If you are running the utility for the XCode IDE, then add schemes as required to adjust the arguments applied to the utility, current there's a sample scheme which will need to be edited to match your specific location of the utility source code.

If you want to run from your terminal then you can do so from the location of the executable do...

build/Lidar_data --file <location of lidar data file>

and example can be....

build/Lidar_data --file /Users/dimambro/Lidar/Sample_data/lidar_data  


