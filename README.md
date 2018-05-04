# Starship Fontana #

This is an example C++ application using the SDL2 library.

## Installation ##
On Fedora (64bit) you can install the SDL2 library as follows:

```bash
$ sudo dnf install SDL2-devel.x86_64 SDL2_image-devel.x86_64 SDL2.x86_64 SDL2_image.x86_64
```

The easiest way to compile is to use a command-line
(tested using g++ (GCC) 6.3.1 20161221 (Red Hat 6.3.1-1))

```bash
$ g++ -c src/*.cpp -I /usr/include/SDL2/
$ g++ -o starship *.o -lSDL2 -lSDL2_image -lSDL2_mixer
```

which will produce an executable file called "starship" in the
top-level directory.  To execute this file do the following

`$ ./starship`
 
from the top-level directory.  The game will expect to find the
`assets` directory under its current working directory.

## Credits ##
The sprites in this game come directly from 
[SpriteLib](http://www.widgetworx.com/widgetworx/portfolio/spritelib.html) and are used
under the terms of the [CPL 1.0](http://opensource.org/licenses/cpl1.0.php).
