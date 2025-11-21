NAME = 2E

# List all source directories
AUTO_SRCS = $(shell find srcs -name '*.cpp' -or -name '*.c')

# Find all .c files in the specific framework directory
# (This matches your original inclusion of 'frameworks/kissFFT')
FRAMEWORK_SRCS = $(wildcard frameworks/kissFFT/*.cpp)

# Combine the lists
SRCS = $(AUTO_SRCS) $(FRAMEWORK_SRCS)

# Create a list of object files by replacing .cpp and .c extensions with .o
OBJ = $(SRCS:.cpp=.o)
OBJ := $(OBJ:.c=.o)

# Create a list of dependency files
DEP = $(OBJ:.o=.d)

# Header directories
AUTO_HDR_DIRS = $(shell find hdr -type d)

# Keep the manually specified framework headers
FRAMEWORK_HDR_FLAGS = -I frameworks/libtess2/Include \
					  -I frameworks/imgui \
					  -I frameworks/SDL2 \
					  -I frameworks/kissFFT

# Combine them
HDR = $(addprefix -I,$(AUTO_HDR_DIRS)) $(FRAMEWORK_HDR_FLAGS)

# Compilation flags
FLAGS = -std=c++17 -I/opt/homebrew/Cellar/glm/1.0.1/include -g -DGL_SILENCE_DEPRECATION -fsanitize=address

# Framework and library includes
INCLUDES = -I./frameworks/SDL2.framework/Versions/A/Headers \
		   -I./frameworks/SDL2_image.framework/Versions/A/Headers \
		   -I./frameworks/SDL2_mixer.framework/Versions/A/Headers \
		   -I./frameworks/SDL2_ttf.framework/Versions/A/Headers \
		   -F./frameworks/

FRAMEWORKS = -F./frameworks \
			 -rpath ./frameworks \
			 -framework SDL2 -framework SDL2_image \
			 -framework SDL2_mixer \
			 -framework SDL2_ttf \
			 -framework OpenGL 

# Path to external library
LIBTESS_PATH = frameworks/libtess2/Source/libtess2.a frameworks/imgui/libimgui.a

# Main target
all: $(NAME)

# Include the dependency files
-include $(DEP)

# Link the program
$(NAME): $(OBJ)
	@g++ $(FLAGS) $(FRAMEWORKS) $(OBJ) $(LIBTESS_PATH) -lm -lz -o $(NAME)

# Compile C++ files to object files
%.o: %.cpp
	@g++ $(FLAGS) $(INCLUDES) $(HDR) -MMD -MP -MT $@ -c $< -o $@

# Compile C files to object files
%.o: %.c
	@gcc $(FLAGS) $(HDR) -MMD -MP -MT $@ -c $< -o $@

# Clean up object and dependency files
clean:
	@rm -rf $(OBJ) $(DEP)

# Full clean
fclean: clean
	@rm -rf $(NAME)

# Rebuild everything
re: fclean all

# Sanitize build
fsan: fclean
fsan: FLAGS += -fsanitize=address
fsan: $(NAME)