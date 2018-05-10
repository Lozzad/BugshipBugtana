# Bugship Bugtana #

This is my Uni coursework for my 'Introduction to games development' module, based off this code: https://github.com/AlmasB/StarshipFontana

All modifications are my own, and the complicated parts are adapted from the SDL2 tutorials found on [Lazyfoo Productions](http://lazyfoo.net/tutorials/SDL/), specifically this one on [SDL_ttf](http://lazyfoo.net/tutorials/SDL/16_true_type_fonts/index.php) and this one on [SDL_mixer](http://lazyfoo.net/tutorials/SDL/21_sound_effects_and_music/index.php).  

## Installation ##
On Fedora (64bit) you can install the SDL2 library as follows: 

```bash
$ sudo dnf install SDL2-devel.x86_64 SDL2_image-devel.x86_64 SDL2.x86_64 SDL2_image.x86_64
```
make sure you have SDL_ttf and SDL_mixer too

```bash
$ sudo dnf install SDL2_ttf-devel
$ sudo dnf install SDL2_mixer-devel
```
either use ./Makefile or

```bash
$ g++ -c src/*.cpp -I /usr/include/SDL2/
$ g++ -o bugship *.o -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
```

which will produce an executable file called "bugship" in the
top-level directory.  To execute this file do the following

`$ ./bugship`
 
from the top-level directory.  The game will expect to find the
`assets` directory under its current working directory.

## Credits ##
The majority of sprites in this game are original 'works' by me (Laurie Dibble), however the projectile sprite is a modified version of the original projectile sprite, taken from
[SpriteLib](http://www.widgetworx.com/widgetworx/portfolio/spritelib.html) and used
under the terms of the [CPL 1.0](http://opensource.org/licenses/cpl1.0.php).

The music has been provided by Ren Brosche, so send all the hatemail to him.

The singular sound effect is provided courtesy of George Cartwright.

Tested by Charlie Gadd and Fin Tungate.
