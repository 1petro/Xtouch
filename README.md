# Xtouch
Linux touch driver using xdotool focused Xwayland may works for xorg
too not tested this wont work on wayland!

# Dependinces 
xdotool

# How it works
it takes five arguments 
1) /dev/input/eventX (x is number of ur touch device)
2) max x,y that is the actual resolution of touch screen
3) desired x,y that u want depend on your X display size
4) mode for x,y (0-7) 8 different modes for x,y depends on ur orientation mode (landscape,portrait)
   
mode           function
0) x,y are used the same (default mode)
1) x inversed and y same
2) x same but y inversed
3) both x,y inversed
4) x,y are reversed (ex x is y and y is x)

5,6,7,8) same for mode (0-4) while x,y reversed

# Mouse functions
Emulated mouse functions supported by xdotool
1) left click
2) right click ( need hold touch and release 1sec or more)
3) moving cursor 
4) moving cursor while left click (holding,graping it uses multitouch requires two fingers one fixed
   other is moving the cursor

# Running and Testing
simply compile it with gcc
 1)          gcc xtouch.c -o xtouch 
then run 
ex

 2)          ./xtouch /dev/input/event4 1080 2246 1080 2246 4
 if it works u will notice the clicks being detected in the output always sure u choosed the correct event
 for your touchscreen 

# compatibilty 
 this driver uses xdotool it won't work without it
 Not supported by wayland maybe in feature it will
 It focused Xwayland tested on phosh,gnome not tested for xorg but it should work normally
 It fixes right click issue and works fine with wine
 And at last Enjoy ,just feel free to open an issue or report a problem !






    


            
