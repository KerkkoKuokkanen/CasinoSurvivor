
#ifndef FBO_RENDER_H
# define FBO_RENDER_H

# include "mesh.h"
# include "ShaderClass.h"

class FBORender
{
	private:
		GLuint texture;
		Mesh mesh;
		Shader *shader;
		GLuint uniform;
	public:
		FBORender(Shader *shader);
		~FBORender();
		void SetTexture(GLuint texture) {mesh.texture = texture;};
		void Draw();
};

#endif
