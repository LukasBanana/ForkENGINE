Render-to-texture loop with custom mipmap generation.
        The depth renderbuffer is not a mipmap.

        glGenFramebuffers(1, &fb);
        glGenTextures(1, &color_tex);
        glGenRenderbuffers(1, &depth_rb);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb);

        // initialize color texture and establish mipmap chain
        glBindTexture(GL_TEXTURE_2D, color_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 512, 512, 0,
                     GL_RGB, GL_INT, NULL);
        glGenerateMipmap(GL_TEXTURE_2D);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
                                  GL_COLOR_ATTACHMENT0,
                                  GL_TEXTURE_2D, color_tex, 0);

        // initialize depth renderbuffer
        glBindRenderbuffer(GL_RENDERBUFFER, depth_rb);
        glRenderbufferStorage(GL_RENDERBUFFER,
                                 GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,
                                     GL_DEPTH_ATTACHMENT,
                                     GL_RENDERBUFFER, depth_rb);

        // Check framebuffer completeness at the end of initialization.
        CHECK_FRAMEBUFFER_STATUS();

        loop {
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb);
            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
                                      GL_COLOR_ATTACHMENT0,
                                      GL_TEXTURE_2D, color_tex, 0);
            glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,
                                         GL_DEPTH_ATTACHMENT,
                                         GL_RENDERBUFFER, depth_rb);

            <draw to the base level of the color texture>

            // custom-generate successive mipmap levels
            glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER,
                                         GL_DEPTH_ATTACHMENT,
                                         GL_RENDERBUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, color_tex);
            foreach (level > 0, in order of increasing values of level) {
                glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
                                          GL_COLOR_ATTACHMENT0,
                                          GL_TEXTURE_2D, color_tex, level);
                glTexParameteri(TEXTURE_2D, TEXTURE_BASE_LEVEL, level-1);
                glTexParameteri(TEXTURE_2D, TEXTURE_MAX_LEVEL, level-1);

                <draw to level>
            }
            glTexParameteri(TEXTURE_2D, TEXTURE_BASE_LEVEL, 0);
            glTexParameteri(TEXTURE_2D, TEXTURE_MAX_LEVEL, max);

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            <draw to the window, reading from the color texture>
        }

