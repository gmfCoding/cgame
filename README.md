## Cgame a test game
Built using C with glfw, glad, glmc, STC, openGL.

## Compilation
Download using `git clone git@github.com:gmfCoding/cgame.git --recursive`

For the most part running `make` should work.
The Makefile will build the dependencies.
> NOTE: glfw requires cmake to be built and some packages 
>
> see: offcial glfw compilations options
>
> sudo apt install libwayland-dev libxkbcommon-dev xorg-dev

## Troubleshooting
### Where are the minimise buttons?

The OS may have launched using wayland.

Export the following environment variables to use x11
 
`QT_QPA_PLATFORM=x11`
`XDG_SESSION_TYPE=x11`
