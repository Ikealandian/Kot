# mkbld -f build.mk -c g++
# 	g++ -Wall -Wextra -fsanitize=leak -I src/ -I platform/ -L/usr/X11R6/lib -lX11 src/Window.cpp platform/X11/X11Window.cpp Main.cpp -o out/Linux.o
# mkbld -run
#	./out/Linux.o

type = Executable
output_directory = "out/"
output_name = "Linux.o"

# WLevel#1-4
# WLevelAll
# WLevelNone
# WLevelAllExtra
warning_level = WLevelAllExtra

include_directories =
{
	"src/",
	"plaform/"
}

source_files = 
{
	"Main.cpp",
	"src/Window.cpp",
	"plaform/X11/X11Window.cpp"
}

library_directories = 
{
	"/usr/X11R6/lib"
}

library_files = 
{
	"X11"
}