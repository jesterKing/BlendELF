
void mouse_button_callback(int button, int state);
void mouse_position_callback(int x, int y);
void mouse_wheel_callback(int wheel);
void key_callback(int key, int state);
void char_callback(int code, int state);

elf_key_event* elf_create_key_event()
{
	elf_key_event *key_event;

	key_event = (elf_key_event*)malloc(sizeof(elf_key_event));
	memset(key_event, 0x0, sizeof(elf_key_event));
	key_event->type = ELF_KEY_EVENT;

	global_obj_count++;

	return key_event;
}

void elf_destroy_key_event(elf_key_event *key_event)
{
	free(key_event);

	global_obj_count--;
}

elf_char_event* elf_create_char_event()
{
	elf_char_event *char_event;

	char_event = (elf_char_event*)malloc(sizeof(elf_char_event));
	memset(char_event, 0x0, sizeof(elf_char_event));
	char_event->type = ELF_CHAR_EVENT;

	global_obj_count++;

	return char_event;
}

void elf_destroy_char_event(elf_char_event *char_event)
{
	free(char_event);

	global_obj_count--;
}

elf_context* elf_create_context()
{
	elf_context *context;

	context = (elf_context*)malloc(sizeof(elf_context));
	memset(context, 0x0, sizeof(elf_context));
	context->type = ELF_CONTEXT;

	context->events = elf_create_list();

	global_obj_count++;

	return context;
}

void elf_destroy_context(elf_context *context)
{
	if(context->title) elf_destroy_string(context->title);
	if(context->events) elf_destroy_list(context->events);

	free(context);

	global_obj_count--;
}

unsigned char elf_init_context(int width, int height,
	const char *title, unsigned char fullscreen)
{
	if(ctx)
	{
		elf_write_to_log("warning: can not open window twice\n");
		return ELF_FALSE;
	}

	if(width < 1 || height < 1)
	{
		elf_write_to_log("error: invalid dimensions (%d, %d) for window\n", width, height);
		return ELF_FALSE;
	}

	ctx = elf_create_context();
	elf_inc_ref((elf_object*)ctx);

	ctx->width = width;
	ctx->height = height;
	ctx->fullscreen = (fullscreen == ELF_FALSE) ? ELF_FALSE : ELF_TRUE;
	ctx->title = elf_create_string(title);

	glfwInit();
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);

	if(!glfwOpenWindow(width, height, 8, 8, 8, 0, 24, 0,
		(fullscreen == ELF_FALSE) ? GLFW_WINDOW : GLFW_FULLSCREEN))
	{
		elf_dec_ref((elf_object*)ctx);
		return ELF_FALSE;
	}

	glfwEnable(GLFW_MOUSE_CURSOR);

	glfwSetWindowTitle(title);
	glfwPollEvents();
	glfwSwapInterval(0);

	glfwSetMouseButtonCallback(mouse_button_callback);
	glfwSetMousePosCallback(mouse_position_callback);
	glfwSetMouseWheelCallback(mouse_wheel_callback);
	glfwSetKeyCallback(key_callback);
	glfwSetCharCallback(char_callback);

	return ELF_TRUE;
}

void elf_deinit_context()
{
	if(!ctx) return;

	glfwTerminate();

	elf_dec_ref((elf_object*)ctx);
	ctx = NULL;
}

#if defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__)
	#ifndef ELF_PLAYER
unsigned char elf_init_context_with_hwnd(int width, int height,
	const char *title, unsigned char fullscreen, HWND hwnd)
{
	if(ctx)
	{
		elf_write_to_log("warning: can not open window twice");
		return ELF_FALSE;
	}

	if(width < 1 || height < 1)
	{
		elf_write_to_log("error: invalid dimensions (%d, %d) for window\n", width, height);
		return ELF_FALSE;
	}

	ctx = elf_create_context();
	elf_inc_ref((elf_object*)ctx);

	ctx->width = width;
	ctx->height = height;
	ctx->fullscreen = (fullscreen == ELF_FALSE) ? ELF_FALSE : ELF_TRUE;
	ctx->title = elf_create_string(title);

	glfwInit();
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);

	if(!glfwOpenWindowWithHWND(width, height, 8, 8, 8, 0, 24, 0,
		(fullscreen == ELF_FALSE) ? GLFW_WINDOW : GLFW_FULLSCREEN, hwnd))
	{
		elf_dec_ref((elf_object*)ctx);
		return ELF_FALSE;
	}

	glfwEnable(GLFW_MOUSE_CURSOR);

	glfwSetWindowTitle(title);
	glfwPollEvents();
	glfwSwapInterval(0);

	glfwSetMouseButtonCallback(mouse_button_callback);
	glfwSetMousePosCallback(mouse_position_callback);
	glfwSetMouseWheelCallback(mouse_wheel_callback);
	glfwSetKeyCallback(key_callback);
	glfwSetCharCallback(char_callback);

	return ELF_TRUE;
}

HWND elf_get_window_hwnd()
{
	return glfwGetWindowHWND();
}
	#endif
#endif

void elf_set_title(const char *title)
{
	if(!title || !strlen(title)) return;

	if(ctx->title) elf_destroy_string(ctx->title);

	ctx->title = elf_create_string(title);

	glfwSetWindowTitle(title);
	glfwPollEvents();
}

int elf_get_window_width()
{
	return ctx->width;
}

int elf_get_window_height()
{
	return ctx->height;
}

unsigned char elf_is_fullscreen()
{
	return ctx->fullscreen;
}

const char* elf_get_title()
{
	return ctx->title;
}

int elf_get_multisamples()
{
	return ctx->multisamples;
}

double elf_get_time()
{
	return glfwGetTime();
}

unsigned char elf_is_window_opened()
{
	return glfwGetWindowParam(GLFW_OPENED);
}

void elf_swap_buffers()
{
	int i;

	elf_destroy_list(ctx->events);
	ctx->events = elf_create_list();

	memcpy(ctx->prv_mbuts, ctx->cur_mbuts, sizeof(unsigned char)*3);
	memcpy(ctx->prv_keys, ctx->cur_keys, sizeof(unsigned char)*256);
	memcpy(ctx->prv_mouse_position, ctx->mouse_position, sizeof(int)*2);

	for(i = 0; i < 16; i++)
	{
		if(ctx->joysticks[i].present)
		{
			memcpy(ctx->joysticks[i].prv_buts, ctx->joysticks[i].cur_buts, sizeof(unsigned char)*16);
		}
	}

	glfwSwapBuffers();

	for(i = 0; i < 16; i++)
	{
		ctx->joysticks[i].present = glfwGetJoystickParam(GLFW_JOYSTICK_1+i, GLFW_PRESENT) == GL_TRUE;
		if(ctx->joysticks[i].present)
		{
			glfwGetJoystickPos(GLFW_JOYSTICK_1+i, ctx->joysticks[i].axis_pos, 2);
			glfwGetJoystickButtons(GLFW_JOYSTICK_1+i, ctx->joysticks[i].cur_buts, 16);
		}
	}
}

void mouse_button_callback(int button, int state)
{
	unsigned int elf_button;

	switch(button)
	{
		case GLFW_MOUSE_BUTTON_LEFT: elf_button = ELF_BUTTON_LEFT; break;
		case GLFW_MOUSE_BUTTON_MIDDLE: elf_button = ELF_BUTTON_MIDDLE; break;
		case GLFW_MOUSE_BUTTON_RIGHT: elf_button = ELF_BUTTON_RIGHT; break;
		default: return;
	}

	ctx->cur_mbuts[elf_button] = (state == GLFW_PRESS) ? ELF_TRUE : ELF_FALSE;
}

void mouse_position_callback(int x, int y)
{
	ctx->mouse_position[0] = x;
	ctx->mouse_position[1] = y;
}

void mouse_wheel_callback(int wheel)
{
	ctx->mouse_wheel = wheel;
}

void key_callback(int key, int state)
{
	unsigned int elf_key;
	elf_key_event *key_event;

	if(key >= 32 && key < 127)
	{
		elf_key = key;
	}
	else
	{
		switch(key)
		{
			case GLFW_KEY_ESC: elf_key = ELF_KEY_ESC; break;
			case GLFW_KEY_F1: elf_key = ELF_KEY_F1; break;
			case GLFW_KEY_F2: elf_key = ELF_KEY_F2; break;
			case GLFW_KEY_F3: elf_key = ELF_KEY_F3; break;
			case GLFW_KEY_F4: elf_key = ELF_KEY_F4; break;
			case GLFW_KEY_F5: elf_key = ELF_KEY_F5; break;
			case GLFW_KEY_F6: elf_key = ELF_KEY_F6; break;
			case GLFW_KEY_F7: elf_key = ELF_KEY_F7; break;
			case GLFW_KEY_F8: elf_key = ELF_KEY_F8; break;
			case GLFW_KEY_F9: elf_key = ELF_KEY_F9; break;
			case GLFW_KEY_F10: elf_key = ELF_KEY_F10; break;
			case GLFW_KEY_F11: elf_key = ELF_KEY_F11; break;
			case GLFW_KEY_F12: elf_key = ELF_KEY_F12; break;
			case GLFW_KEY_UP: elf_key = ELF_KEY_UP; break;
			case GLFW_KEY_DOWN: elf_key = ELF_KEY_DOWN; break;
			case GLFW_KEY_LEFT: elf_key = ELF_KEY_LEFT; break;
			case GLFW_KEY_RIGHT: elf_key = ELF_KEY_RIGHT; break;
			case GLFW_KEY_LSHIFT: elf_key = ELF_KEY_LSHIFT; break;
			case GLFW_KEY_RSHIFT: elf_key = ELF_KEY_RSHIFT; break;
			case GLFW_KEY_LCTRL: elf_key = ELF_KEY_LCTRL; break;
			case GLFW_KEY_RCTRL: elf_key = ELF_KEY_RCTRL; break;
			case GLFW_KEY_LALT: elf_key = ELF_KEY_LALT; break;
			case GLFW_KEY_RALT: elf_key = ELF_KEY_RALT; break;
			case GLFW_KEY_TAB: elf_key = ELF_KEY_TAB; break;
			case GLFW_KEY_ENTER: elf_key = ELF_KEY_ENTER; break;
			case GLFW_KEY_BACKSPACE: elf_key = ELF_KEY_BACKSPACE; break;
			case GLFW_KEY_INSERT: elf_key = ELF_KEY_INSERT; break;
			case GLFW_KEY_DEL: elf_key = ELF_KEY_DEL; break;
			case GLFW_KEY_PAGEUP: elf_key = ELF_KEY_PAGEUP; break;
			case GLFW_KEY_PAGEDOWN: elf_key = ELF_KEY_PAGEDOWN; break;
			case GLFW_KEY_HOME: elf_key = ELF_KEY_HOME; break;
			case GLFW_KEY_END: elf_key = ELF_KEY_END; break;
			case GLFW_KEY_KP_0: elf_key = ELF_KEY_KP_0; break;
			case GLFW_KEY_KP_1: elf_key = ELF_KEY_KP_1; break;
			case GLFW_KEY_KP_2: elf_key = ELF_KEY_KP_2; break;
			case GLFW_KEY_KP_3: elf_key = ELF_KEY_KP_3; break;
			case GLFW_KEY_KP_4: elf_key = ELF_KEY_KP_4; break;
			case GLFW_KEY_KP_5: elf_key = ELF_KEY_KP_5; break;
			case GLFW_KEY_KP_6: elf_key = ELF_KEY_KP_6; break;
			case GLFW_KEY_KP_7: elf_key = ELF_KEY_KP_7; break;
			case GLFW_KEY_KP_8: elf_key = ELF_KEY_KP_8; break;
			case GLFW_KEY_KP_9: elf_key = ELF_KEY_KP_9; break;
			case GLFW_KEY_KP_DIVIDE: elf_key = ELF_KEY_KP_DIVIDE; break;
			case GLFW_KEY_KP_MULTIPLY: elf_key = ELF_KEY_KP_MULTIPLY; break;
			case GLFW_KEY_KP_SUBTRACT: elf_key = ELF_KEY_KP_SUBTRACT; break;
			case GLFW_KEY_KP_ADD: elf_key = ELF_KEY_KP_ADD; break;
			case GLFW_KEY_KP_DECIMAL: elf_key = ELF_KEY_KP_DECIMAL; break;
			case GLFW_KEY_KP_EQUAL: elf_key = ELF_KEY_KP_EQUAL; break;
			case GLFW_KEY_KP_ENTER: elf_key = ELF_KEY_KP_ENTER; break;
			case GLFW_KEY_SPACE: elf_key = ELF_KEY_SPACE; break;
			default: return;
		}
	}

	ctx->cur_keys[elf_key] = (state == GLFW_PRESS) ? ELF_TRUE : ELF_FALSE;

	key_event = elf_create_key_event();
	key_event->key = elf_key;
	key_event->state = (state == GLFW_PRESS) ? ELF_TRUE : ELF_FALSE;

	elf_append_to_list(ctx->events, (elf_object*)key_event);
}

void char_callback(int code, int state)
{
	elf_char_event *char_event;

	char_event = elf_create_char_event();
	char_event->code = code;
	char_event->state = (state == GLFW_PRESS) ? ELF_TRUE : ELF_FALSE;

	elf_append_to_list(ctx->events, (elf_object*)char_event);
}

elf_vec2i elf_get_mouse_position()
{
	elf_vec2i pos;

	pos.x = ctx->mouse_position[0];
	pos.y = ctx->mouse_position[1];

	return pos;
}

elf_vec2i elf_get_mouse_force()
{
	elf_vec2i force;

	force.x = ctx->mouse_position[0]-ctx->prv_mouse_position[0];
	force.y = ctx->mouse_position[1]-ctx->prv_mouse_position[1];

	return force;
}

void elf_set_mouse_position(int x, int y)
{
	glfwSetMousePos(x, y);

	ctx->mouse_position[0] = x;
	ctx->mouse_position[1] = y;

	if(ctx->mouse_position[0] < 0) ctx->mouse_position[0] = 0;
	else if(ctx->mouse_position[0] > elf_get_window_width()) ctx->mouse_position[0] = elf_get_window_width();
	if(ctx->mouse_position[1] < 0) ctx->mouse_position[1] = 0;
	else if(ctx->mouse_position[1] > elf_get_window_width()) ctx->mouse_position[1] = elf_get_window_height();
}

void elf_hide_mouse(unsigned char hide)
{
	if(hide)
	{
		glfwDisable(GLFW_MOUSE_CURSOR);
		ctx->hide_mouse = ELF_TRUE;
	}
	else
	{
		glfwEnable(GLFW_MOUSE_CURSOR);
		ctx->hide_mouse = ELF_FALSE;
	}
}

unsigned char elf_is_mouse_hidden()
{
	return ctx->hide_mouse;
}

int elf_get_mouse_wheel()
{
	return ctx->mouse_wheel;
}

int elf_get_mouse_button_state(int button)
{
	if(button < 0 || button > 2) return 0;
	if(ctx->prv_mbuts[button])
	{
		if(ctx->cur_mbuts[button]) return ELF_DOWN;
		else return ELF_RELEASED;
	}
	else
	{
		if(ctx->cur_mbuts[button]) return ELF_PRESSED;
		else return ELF_UP;
	}
}

int elf_get_key_state(int key)
{
	if(key < 0 || key > 256) return 0;
	if(ctx->prv_keys[key])
	{
		if(ctx->cur_keys[key]) return ELF_DOWN;
		else return ELF_RELEASED;
	}
	else
	{
		if(ctx->cur_keys[key]) return ELF_PRESSED;
		else return ELF_UP;
	}
}

unsigned char elf_get_joystick_present(int joy)
{
	if(joy < 0 || joy > 15) return ELF_FALSE;
	return ctx->joysticks[joy].present;
}

elf_vec2f elf_get_joystick_axis(int joy)
{
	elf_vec2f pos;
	memset(&pos, 0x0, sizeof(elf_vec2f));
	if(joy < 0 || joy > 15) return pos;
	pos.x = ctx->joysticks[joy].axis_pos[0];
	pos.y = ctx->joysticks[joy].axis_pos[1];
	return pos;
}

int elf_get_joystick_button_state(int joy, int but)
{
	if(joy < 0 || joy > 15) return ELF_UP;
	if(but < 0 || but > 15) return ELF_UP;
	if(ctx->joysticks[joy].prv_buts[but])
	{
		if(ctx->joysticks[joy].cur_buts[but]) return ELF_DOWN;
		else return ELF_RELEASED;
	}
	else
	{
		if(ctx->joysticks[joy].cur_buts[but]) return ELF_PRESSED;
		else return ELF_UP;
	}
}

int elf_get_event_count()
{
	return elf_get_list_length(ctx->events);
}

elf_object* elf_get_event(int idx)
{
	elf_object *obj;
	int i;

	if(idx < 0 || idx > elf_get_list_length(ctx->events)-1) return NULL;

	for(i = 0, obj = elf_begin_list(ctx->events); obj; obj = elf_next_in_list(ctx->events), i++)
	{
		if(i == idx) return obj;
	}

	return NULL;
}

int elf_get_key_event_key(elf_key_event *key_event)
{
	return key_event->key;
}

int elf_get_key_event_state(elf_key_event *key_event)
{
	return key_event->state;
}

