
#ifndef FBO_H
# define FBO_H

# include <OpenGL/gl3.h>

class FBO
{
	private:
		GLuint fbo;
		GLuint colorTexture;
		int width, height;
		void InitFBO();
	public:
		FBO(int width, int height);
		~FBO();
		void Bind() const;
		GLuint GetTexture() const {return colorTexture;};
};

void BindScreenForUse();

#endif
