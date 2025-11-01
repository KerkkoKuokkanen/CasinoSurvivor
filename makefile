NAME = 2E

# List all source directories
SRC_DIRS =	srcs srcs/ImagePipeline/GL_Stuff srcs/ImagePipeline/Rendering srcs/Tools srcs/Tools/input \
			frameworks/kissFFT \
			srcs/ImagePipeline/RenderPipeline srcs/ImagePipeline/RenderPipeline/ImageHandling \
			srcs/systemObj srcs/systemObj/systemMemory srcs/Tools/common srcs/systemObj/sysEnv \
			srcs/systemObj/transform srcs/engineMode srcs/engineMode/shapes srcs/engineMode/saveSystem \
			srcs/ImagePipeline/RenderPipeline/MultiSprite srcs/engineMode/shapes/atlasTool srcs/engineMode/objBar \
			srcs/engineMode/components srcs/audioSystem srcs/physics srcs/physics/hitbox srcs/physics/rigidBody \
			srcs/objects srcs/objects/environment srcs/objects/player

# Find all .cpp and .c files in the listed source directories
SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp $(dir)/*.c))

# Create a list of object files by replacing .cpp and .c extensions with .o
OBJ = $(SRCS:.cpp=.o)
OBJ := $(OBJ:.c=.o)

# Create a list of dependency files
DEP = $(OBJ:.o=.d)

# Header directories
HDR =	-I hdr/ImagePipeline/GL_Stuff -I hdr/ -I hdr/ImagePipeline/Rendering -I hdr/Tools -I frameworks/libtess2/Include \
		-I hdr/Tools/input -I hdr/ImagePipeline/RenderPipeline -I hdr/ImagePipeline/RenderPipeline/ImageHandling \
		-I frameworks/imgui -I hdr/systemObj -I hdr/systemObj/systemMemory -I hdr/Tools/common -I hdr/systemObj/sysEnv \
		-I hdr/systemObj/transform -I hdr/engineMode/imageEditing -I hdr/engineMode/shapes -I hdr/engineMode/saveSystem \
		-I hdr/ImagePipeline/RenderPipeline/MultiSprite -I hdr/engineMode/shapes/atlasTool -I hdr/engineMode/objBar \
		-I hdr/engineMode/components -I hdr/audioSystem -I frameworks/SDL2 -I hdr/physics -I hdr/physics/hitbox \
		-I hdr/physics/rigidBody -I hdr/objects -I frameworks/kissFFT -I hdr/objects/environment -I hdr/objects/player

# Compilation flags
FLAGS = -std=c++17 -I/opt/homebrew/Cellar/glm/1.0.1/include -g -DGL_SILENCE_DEPRECATION -fsanitize=address
CGFLAGS = 

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
	@g++ $(FLAGS) $(CGFLAGS) $(FRAMEWORKS) $(OBJ) $(LIBTESS_PATH) -lm -lz -o $(NAME)

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