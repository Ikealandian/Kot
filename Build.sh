# Make output folder
mkdir out

# Build examples/VulkanTest.cpp
g++ -Wall -Wextra -fsanitize=leak -I src/ -I platform/ -L/usr/X11R6/lib -lX11 -L/usr/vulkan/lib -lvulkan src/Window.cpp src/Vulkan.cpp platform/X11/X11Window.cpp platform/X11/X11Vulkan.cpp examples/VulkanTest.cpp -o out/Linux.o