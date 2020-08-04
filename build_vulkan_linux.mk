# mkbld -f build.mk -c g++
# 	g++ -Wall -Wextra -fsanitize=leak -I src/ -I platform/ -L/usr/X11R6/lib -lX11 -L/usr/vulkan/lib -lvulkan src/Window.cpp src/Vulkan.cpp platform/X11/X11Window.cpp platform/X11/X11Vulkan.cpp Examples/VulkanTest.cpp -o out/Linux.o
# mkbld -run
#	./out/Linux.o

# TODO
# 	Build as library

# NevaMasq Build File

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
	"Examples/VulkanTest.cpp",
	"src/Window.cpp",
	"src/Vulkan.cpp",
	"platform/X11/X11Window.cpp",
	"platform/X11/X11Vulkan.cpp"
}

library_directories = 
{
	"/usr/X11R6/lib",
	"/usr/vulkan/lib"
}

library_files = 
{
	"X11",
	"vulkan"
}