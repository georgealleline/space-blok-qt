Space Blok Qt
=============

Space Blok Qt is a Qt port of the Space Blok XNA application. 

Space Blok is a 1-4 player game where the goal is to gather as many points as 
possible by destroying game bloks. The game utilises the open-source Bullet 
Physics library to provide 3D simulation and realistic collision handling for 
the game objects. The 3D graphics are drawn by using Qt3D.

This Nokia Developer example is hosted at: 
- http://projects.developer.nokia.com/spaceblokqt/

For more information about the implementation and the porting, see the project 
wiki pages:
- http://projects.developer.nokia.com/spaceblokqt/wiki
- http://projects.developer.nokia.com/spaceblokqt/wiki/porting


PREREQUISITES
-------------------------------------------------------------------------------

- Qt basics
- Qt OpenGL basics
- Qt Quick basics


REQUIRED CAPABILITIES
-------------------------------------------------------------------------------

None.


KNOWN ISSUES
-------------------------------------------------------------------------------

Performance is poor on Symbian devices at the moment.


BUILD & INSTALLATION INSTRUCTIONS
-------------------------------------------------------------------------------

Preparations
~~~~~~~~~~~~

Check that you have 
- the latest Qt/Qt SDK installed in the development environment and on the
  device.
- the Qt3D library installed into the SDK and on the device. See the section
  'Setting up the Qt3D' in the project wiki pages:
  http://projects.developer.nokia.com/spaceblokqt/wiki


Build & installation instructions using Qt SDK
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. Open the Qt PRO file.
   File > Open File or Project, select the PRO file

2. Select target(s), for example 'MeeGo 1.2 Harmattan (Qt SDK)', and press the 
   Finish button.

3. Press the Run button to build the project and to create the installation
   package. The application is installed on the device. 


RUNNING THE APPLICATION
-------------------------------------------------------------------------------

Launch Space Blok on your device. Select New Game in the main menu to start 
the game.

Place the phone on the table between the players so that one corner of the
phone is towards each player. The balls can be shot by swiping on top of each 
platform. The direction and the velocity of the shot depends on the direction 
and the length of the swipe. Each shot will have a slight trajectory towards 
the camera to make it possible to hit the closest bloks. The bloks closest to 
the center of the compound are stronger and need severals hits before they are 
destroyed. The outer bloks only require a single hit.

You may pause the game by clicking the Pause button at the bottom of the
screen. The game is resumed by tapping Resume. You may also exit to the main
menu by tapping Main menu, but note that this also ends the current game.

The game ends when all of the bloks have been destroyed. The Results view is 
then shown with scores for each player. Returning to the main menu is done by 
clicking the Back button.

You may open the info view, containing some tips about the game, by tapping 
Info in the main menu.
   
   
COMPATIBILITY
-------------------------------------------------------------------------------

MeeGo 1.2 Harmattan, Symbian and Qt desktop (Windows and Linux).

Tested on:
- Nokia C7-00
- Nokia 701
- Nokia N9
- Nokia N950
- Windows 7

Developed with:
- Qt SDK 1.1


LICENSE INFORMATION
-------------------------------------------------------------------------------

See the license text file delivered with this project. The license file is 
also available online at:
http://projects.developer.nokia.com/spaceblokqt/browser/trunk/Licence.txt

The Bullet Physics Library is hosted at http://bulletphysics.org/ and licensed 
under zlib. Portions of the library content is licensed under BSD license.
The Bullet license file, containing also the additional licenses, is delivered
with this project and is also available online at:
http://projects.developer.nokia.com/spaceblokqt/browser/trunk/src/bullet/BulletLicense.txt

Assimp, Open Asset Import Library, is licensed under a liberal BSD license.
For more information about this library, please visit
http://assimp.sourceforge.net/


CHANGE HISTORY
-------------------------------------------------------------------------------

1.2   More optimizations.
1.1   Performance optimizations, Symbian support.
1.0   First version.