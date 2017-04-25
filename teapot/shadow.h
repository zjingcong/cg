# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <math.h>
# include <fstream>

# define WINDOW_HEIGHT 1024
# define WINDOW_WIDTH 1280


void set_shadowuniform(int p)
{
    int location;
    location = glGetUniformLocation(p,"myshadow");
    glUniform1i(location,6);
}

void build_shadowmap()
{
    // Set properties of texture id #1.
    glBindTexture(GL_TEXTURE_2D,1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    // Declare size and type of texture; it has no data initially (last arg 0).
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    // Back to default.
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebufferEXT(GL_FRAMEBUFFER,1);
    glDrawBuffer(GL_NONE); // No color buffers will be written.
    // Attach this framebuffer (id #1 above) to texture (id #1 is penultimate arg),
    // so that we can perform an offscreen render-to-texture.
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,1,0);
    // Back to default.
    glBindFramebufferEXT(GL_FRAMEBUFFER,0);
}
