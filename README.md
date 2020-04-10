# MegaXor
Xor for the MegaDrive

This is a port of the game XOR for the MegaDrive (Genesis).

![](https://www.dropbox.com/s/xhgy1d3bfkpoqmc/rom_009.png?raw=1)

As taken from the original Xor packaging:
"
There are no random events in XOR's labyrinthine palace, and there is no premium on hand–eye coordination. You have all the time in the world to work out the precise nature of the hazards you encounter. Logical thinking, strategy and tactical problem solving are part of the XOR experience. And XOR is not simply about finding solutions. If you can solve the entire puzzle (15 levels plus an encrypted anagram), you qualify to be a member of the Order of XOR
"

The player controls two shields, Magus and Questor, which can be moved horizontally and vertically though the maze, in order to collect masks. The object of each level is to collect all the masks, then reach the exit. In all levels, use of both shields are required for completion, and in some, one shield must be sacrificed (walled in or destroyed) in order to complete the level. 

Currrently the first three levels have been implemented

![](https://www.dropbox.com/s/tfj7yxnppl3ard2/rom_007.png?raw=1)
![](https://www.dropbox.com/s/cfbbm3qvjglg2ua/rom_008.png?raw=1)
![](https://www.dropbox.com/s/n8zfkb8hz4xci31/rom_010.png?raw=1)

To replay a completed solution press C when selecting a level.

Find a download in the releases page.




## How to play ##
Move the shield around with the D-Pad.
Switch between the 2 Shields with A.


## How to Win ##
Collect all the tiles that look like the mask on the side then find the exit door.





## Version History ##

### V0.5 ###
* Added Teleporter
* Added all the rest of the levels.
* Further fixes to the order that fish, chicken and bombs move
* Fixed off screen explositions showing on screen
* Correctly kill other objects when exploded
* Refactored graphics loading code
* better handling of animated tiles
* Added Options Menu
* Added Sonic tile set
* Added YouTuber tile set
* Added Settings, score saving and loading
* Added option to clear save
* Fixed up teleport to handle blocked squares correctly
* fixed broken player death
* fixed issue with going off the map when drawing near the bottom right hand corner.
* fixed bug with sprites showing up multiple times.


### V0.4 ###
* Implement Switch tile type
* Fix ordering of chicken's and fish dependent on what moved last
* Level 8 added
* Level 9 added
* Fixed chicken and fish not correctly being killed


### V0.3 ###
* Level 5 added
* Level 6 added
* Level 7 added
* Doll logic added
* Fixed BombV being pushed up and down
* Fixed graphical glitch on title screen
* Added Music
* Fixed glitch on title screen
* Implement Reset
* backing on level select now animated
* Implemented Pause menu
* Add user initated replays 


### V0.2 ###
* Level 4 added.
* Horiztonal and Vertical Bombs added.
* Map drawing speed improved.
* Issue with moving tiles looping off screen fixed.
* Added animated tiles for the horiztonal and vertical force fields.
* Speed up game logic.
* Fixed issue with main menu not looping.
* Mini-map generated from the level and updated when masks are collected.


### V0.1 ###
* Added first 3 levels.
* Added main menu.
