#XR_Helper
This tool was created to more easily view and modfiy information stored in the game's weapons LTX files.

What this does:
 - Loads in LTX files in the gamedata folder and displays relevant information.
 - Inventory icon tweaking (x, y, width, and height) with the respective icon shown.
 - Attachment tweaking, with checkboxes to toggle them on and off.
 - Ability to select gamedata folder.

What this will do:
 - Save LTX files after modification.
 - Load/Export icon to PNG.
 - More features soon.

Building:

1. Dependencies:
 - Qt-5: Qt Creator, so I highly recommend installing that also. www.qt.io
 - libsquish: https://code.google.com/p/libsquish/
 - Both of these can be installed manually, or from the Arch User Repository.
2. Pull the repo into a directory, and make a new project in Qt Creator.
3. Build the project.
4. ???
5. Profit!

Usage:
 - It is assumed that you have an installation of S.T.A.L.K.E.R.: Shadow of Chernobyl and/or some mod already.
  - TODO: Test on Clear Sky and Call of Pripyat.
 - The executable can be ran from any directory. For convenience though, can be ran from the same directory as gamedata.
 - Browse to the game folder and select Choose.
 - In the File menu:
  - New Gamedata will prompt a selection for a new gamedata folder.
  - Reload Content will reload all LTX files in the currently selected gamedata path.
  - Exit exits the program.
 - A list of LTX file entries will be populated on the left, click on one to view its properties.
  - If it has an inventory icon, it will be displayed on the right.
  - Weapons that have toggleable attachments such as a silencer, scope, or launcher can be activated by clicking on the respective check box.
  - Adjustments can be made to the displayed icon by changing the applicable lines.
