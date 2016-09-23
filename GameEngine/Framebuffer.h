#pragma once
#include <GL\glew.h>
#include "ScreenQuad.h"

namespace GameEngine
{
  class Framebuffer
  {
  public:
    Framebuffer();
    ~Framebuffer();

    /** Sets the SW/SH values and creates an ID for the fbo.*/
    void Init(int _screenWidth, int _screenHeight);

    /** Deletes the fbo.*/
    void Destroy();

    /** Binds this fbo to be used.*/
    void Bind(GLenum _target);

    /** Unbinds the fbo and the default framebuffer is used until another one is bound*/
    void Unbind(GLenum _target);

    void Blit(GLbitfield _mask, GLenum _filter);

    /** Attaches a texture to the framebuffer which can be a RGB/depth/stencil/depth + stencil buffer.*/
    void AttachTexture2D(GLboolean _depth, GLboolean _stencil, GLboolean _multisampled, GLint _samples = 4);

    /** Attaches a renderbuffer to the framebuffer which can be a RGB/depth/stencil/depth + stencil buffer.
        The difference betweehn a renderbuffer and a 2D texture attachment is that the renderbuffer cannot be sampled/read.
         Therefore, this can be used when you don't want to sample from a certain part of the framebuffer*/
    void AttachRenderbuffer(GLboolean _depth, GLboolean _stencil, GLboolean _multisampled, GLint _samples = 4);

    void Render();
     
    /** Checks if the framebuffer is valid
      \return true if the framebuffer status is complete
      \return false if the renderbuffer status isn't complete */
    bool CheckFramebufferStatus();

    /** Accessors */
    GLuint GetFBO()           const noexcept { return m_fboID; }
    GLuint GetTextureBuffer() const noexcept { return m_textureBuffer; }
    int GetScreenWidth()      const noexcept { return m_screenWidth; }
    int GetScreenHeight()     const noexcept { return m_screenHeight; }
  private:
    int m_screenWidth, m_screenHeight;
    GLuint m_fboID, m_rboID, m_textureBuffer;
    ScreenQuad m_quad;
  };
}


