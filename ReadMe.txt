-------------------------------------------------------------------------

This is the Readme for the "Commander Keen Galaxy Text Editor" v. 1.1

Written by John314

-------------------------------------------------------------------------

This program is ment to be used on Galaxy text files such as "CK4_txt_0000.txt" or

Use:

KeenTxtEd [OPTIONS]
Options 
      -h -help     : Print the help
      -file [name] : Use a specific file to edit
      -bmp [name]  : The filename to use for BMPs [ex. CKD_pic_ ]
      -extended    : Allow for Atroxian Realms text additions

--------------------------------------------------------------------------
  More info on the options

  -h -help --h --help :

       All this does is print the help

  -file [name] :

       This argument is used to load a file on boot. however, you can also load 
       files in the application

       example: KeenTxtEd -file CKD_txt_0014.txt

  -bmp [name] :

       This argument is used to set the graphics prefix name.
       It does not contain any numbers or extention.

       example: KeenTxtEd -bmp 4BMP
       example: KeenTxtEd -bmp CKD_bmp_

  -extended

       This argument allows for the program to parse some extra commands correctly.
       However, I'm not sure any of them are ever used in Atroxian Realms.

       example: KeenTxtEd -extended

----------------------------------------------------------------------------

   Bitmap Directory

----------------------------------------------------------------------------

  This program relys on extracted keen bitmaps.

  Please put the graphical bitmaps in a folder called BMP.

  This program can be put in your mod folder so you don't have to copy the
  graphics every time.
  Just make sure to copy the needed files:

  	colors.bmp
  	font.bmp
  	KeenEdit.bmp
  	KeenTextEd (exe)


----------------------------------------------------------------------------

   In Editor Controls

----------------------------------------------------------------------------

 There aren't many controls, but there are a few.

 ESC -> Moves the mouse to 0,0 thus opening the editor menu.
 		Closes most menus.

 LEFT -> Moves the mouse left when the editor menu is open.
         Changes the current page on.
         Changes the picture when in sticker mode.

 RIGHT -> Moves the mouse right when the editor menu is open.
         Changes the current page on.
         Changes the picture when in sticker mode.

 ENTER -> Closes the typing feild when in the loading / saving menu
          Otherwise, adds a new line break when editing text.

 DELETE -> Deletes the character in front of the selector or deletes all
           selected text.

 BACKSPACE -> Removes a character from behind the selector.

 TAB   -> Toggles graphical animations.

 HOME  -> Sends the text cursor to the start of the text feild.

 END   -> Sends the text cursor to the end of the text feild.

 +     -> Increases the animation time of animated stickers.

 -     -> Decreases the animation time of animated stickers.

 Move the mouse to the top to open the editor menu.

 Menu:

 [ Load / ][ Edit ][Sticker][Animated][  Add ][Remove]
 [  Save  ][ Text ][ Mode  ][ Sticker][ Page ][ Page ]


===========================
 Loading / Saving
===========================
 Click on the text feild to edit it.

  _____________________
 |                     |
 | [ SAVE ]   [ LOAD ] |
 |                     |
 | [ TEXT FEILD HERE ] |
 |_____________________|

===========================
 Text Editing
===========================
 When in the text editor, press ESC to leave.

 You can select text by holding shift and moving the cursor around.
 You can select all the text by pressing CTRL + A

 You can then copy the text selected to the clipboard by pressing CTRL + C

 You can paste text from your clipboard by pressing CTRL + P 
   (note this can be any text from any text editor)

 A row of colors at the bottom show the hex value for each EGA color for refrence
 when using the ^C command.

 Please note, that you will have to type most text commands by hand.

 Here is a list of the current commands:

	 TEXT FORMATTING COMMANDS
	------------------------
	^C<hex digit>  			Change text color
	^E[enter]				End of layout (all pages)
	^G<y>,<x>,<pic>[ENTER]	Draw a graphic and push margins
	^P[enter]				start new page, must be the first chars in a layout
	^L<x>,<y>[ENTER]		Locate to a specific spot, x in pixels, y in lines

	^B<y>,<x>,<w>,<h>,<col>[ENTER]    Draw a bar 
	^; coment goes here[ENTER]        Comment (Doesn't show up)
	^T<y>,<x>,<pic>,<ticks>[ENTER]    Waits for a number of tics, then draws pic
	^>                                Indent halfway

	Atroxian Realms additions (Most likely not used):
	These only work with the -extended flag

	^M<x>                   Play Music for level x
	^S<x>                   Play sound number x
	^W                      Wait until sound is done
	^D<ticks>               Waits for a number of tics
	^Q                      Stop sound and music


===========================
 Stickers
===========================

 ** New in version 1.1 **
   Graphics are now placed from back to front, so if you place an image on top of another, 
    it will be placed on top of that image.

 Stickers (Also known as graphics), are placed by clicking the left mouse button.

 Animated stickers have a little number next to the image, telling you how long it will
 take before the image is show when animated.
 
 Use the + and - keys to change the time.

===========================
 Bugs
===========================

 If you encounter any bugs, such as text commands being written to the screen (^),
 please make sure it is not because it is an usupported command.

 Any other bugs, please report to me.

 Email: John314TheGeekyGenius@gmail.com


