
unsigned char gfx_check_render_target()
{
	GLenum status;

	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

	switch(status) {
		case GL_FRAMEBUFFER_COMPLETE_EXT: break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			printf("fbo: incomplete attachment\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			printf("fbo: missing attachment\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			printf("fbo: incomplete dimensions\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			printf("fbo: incomplete formats\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			//printf("fbo: incomplete draw buffer\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			printf("fbo: incomplete read buffer\n");
			break;
	}

	if(status != GL_FRAMEBUFFER_COMPLETE_EXT &&
		status != GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT) return GFX_FALSE;

	return GFX_TRUE;
}

gfx_render_target* gfx_create_render_target(unsigned int width, unsigned int height)
{
	gfx_render_target *render_target;

	if(!width || !height) return NULL;

	render_target = (gfx_render_target*)malloc(sizeof(gfx_render_target));
	memset(render_target, 0x0, sizeof(gfx_render_target));

	glGenFramebuffersEXT(1, &render_target->fb);

	return render_target;
}

void gfx_destroy_render_target(gfx_render_target *render_target)
{
	if(render_target->fb) glDeleteFramebuffersEXT(1, &render_target->fb);

	free(render_target);
}

gfx_render_target* gfx_get_cur_render_target()
{
	return driver->render_target;
}

unsigned char gfx_set_render_target_color_texture(gfx_render_target *render_target, unsigned int n, gfx_texture *color)
{
	unsigned char status;
	gfx_render_target *rt;

	if((int)n > driver->max_draw_buffers-1) return GFX_FALSE;

	if(driver->render_target != render_target)
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, render_target->fb);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+n,
		GL_TEXTURE_2D, color->id, 0);

	status = gfx_check_render_target();

	if(status) render_target->targets[n] = GFX_TRUE;

	if(!driver->render_target)
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
	else if(driver->render_target != render_target)
	{
		rt = driver->render_target;
		driver->render_target = NULL;
		gfx_set_render_target(rt);
	}

	return status;
}

unsigned char gfx_set_render_target_depth_texture(gfx_render_target *render_target, gfx_texture *depth)
{
	unsigned char status;
	gfx_render_target *rt;

	if(driver->render_target != render_target)
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, render_target->fb);

	if(depth)
	{
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
			GL_TEXTURE_2D, depth->id, 0);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
			GL_TEXTURE_2D, depth->id);
	}
	else
	{
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
			GL_TEXTURE_2D, 0, 0);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
			GL_TEXTURE_2D, 0);
	}

	status = gfx_check_render_target();

	if(!driver->render_target)
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
	else if(driver->render_target != render_target)
	{
		rt = driver->render_target;
		driver->render_target = NULL;
		gfx_set_render_target(rt);
	}

	return status;
}

unsigned char gfx_set_render_target(gfx_render_target *render_target)
{
	int i, j;
	GLenum draw_buffers[16];

	if(driver->render_target == render_target) return GFX_TRUE;

	for(i = 0, j = 0; i < driver->max_draw_buffers; i++)
	{
		if(render_target->targets[i])
		{
			draw_buffers[j] = GL_COLOR_ATTACHMENT0_EXT+i;
			j++;
		}
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, render_target->fb);

	if(j) glDrawBuffers(j, draw_buffers);
	else
	{
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	if(!gfx_check_render_target())
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		return GFX_FALSE;
	}

	driver->render_target = render_target;

	return GFX_TRUE;
}

void gfx_disable_render_target()
{
	int i, j;

	if(!driver->render_target) return;

	for(i = 0, j = 0; i < driver->max_draw_buffers; i++)
	{
		if(driver->render_target->targets[i])
		{
			j++;
		}
	}

	glDrawBuffer(GL_BACK);
	glReadBuffer(GL_BACK);

	driver->render_target = NULL;

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

