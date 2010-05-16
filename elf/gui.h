
const char* elf_get_gui_object_name(elf_gui_object *object)
{
	return object->name;
}

elf_vec2i elf_get_gui_object_position(elf_gui_object *object)
{
	return object->rel_pos;
}

elf_vec2i elf_get_gui_object_size(elf_gui_object *object)
{
	elf_vec2i size;

	size.x = object->width;
	size.y = object->height;

	return size;
}

elf_color elf_get_gui_object_color(elf_gui_object *object)
{
	return object->color;
}

unsigned char elf_get_gui_object_visible(elf_gui_object *object)
{
	return object->visible;
}

elf_script *elf_get_gui_object_script(elf_gui_object *object)
{
	return object->script;
}

int elf_get_gui_object_event(elf_gui_object *object)
{
	return object->event;
}

void elf_set_gui_object_position(elf_gui_object *object, float x, float y)
{
	object->rel_pos.x = x;
	object->rel_pos.y = y;
	elf_recalc_gui_object(object);
}

void elf_set_gui_object_color(elf_gui_object *object, float r, float g, float b, float a)
{
	object->color.r = r;
	object->color.g = g;
	object->color.b = b;
	object->color.a = a;
}

void elf_set_gui_object_visible(elf_gui_object *object, unsigned char visible)
{
	object->visible = !visible == ELF_FALSE;
}

void elf_set_gui_object_script(elf_gui_object *object, elf_script *script)
{
	if(object->script) elf_dec_ref((elf_object*)object->script);
	object->script = script;
	if(object->script) elf_inc_ref((elf_object*)object->script);
}

elf_label* elf_create_label(const char *name)
{
	elf_label *label;

	label = (elf_label*)malloc(sizeof(elf_label));
	memset(label, 0x0, sizeof(elf_label));
	label->type = ELF_LABEL;

	label->color.r = label->color.g = label->color.b = label->color.a = 1.0;
	label->visible = ELF_TRUE;

	if(name) label->name = elf_create_string(name);

	global_obj_count++;

	return label;
}

void elf_destroy_label(elf_label *label)
{
	if(label->name) elf_destroy_string(label->name);
	if(label->text) elf_destroy_string(label->text);

	if(label->font) elf_dec_ref((elf_object*)label->font);

	free(label);

	global_obj_count--;
}

void elf_draw_label(elf_label *label, gfx_shader_params *shader_params)
{
	if(!label->visible || !label->font || !label->text) return;

	gfx_set_color(&shader_params->material_params.color, label->color.r, label->color.g, label->color.b, label->color.a);
	elf_draw_string(label->font, label->text, label->pos.x, label->pos.y, shader_params);
	shader_params->texture_params[0].texture = NULL;
}

elf_font* elf_get_label_font(elf_label *label)
{
	return label->font;
}

const char* elf_get_label_text(elf_label *label)
{
	return label->text;
}

void elf_recalc_label(elf_label *label)
{
	if(label->font)
	{
		label->height = label->font->size;
		if(label->text)
		{
			label->width = elf_get_string_width(label->font, label->text);
		}
	}
	else
	{
		label->height = 0;
		label->width = 0;
	}
}

void elf_set_label_font(elf_label *label, elf_font *font)
{
	if(label->font) elf_dec_ref((elf_object*)label->font);
	label->font = font;
	elf_inc_ref((elf_object*)label->font);
	elf_recalc_gui_object((elf_gui_object*)label);
}

void elf_set_label_text(elf_label *label, const char *text)
{
	if(label->text) elf_destroy_string(label->text);
	label->text = elf_create_string(text);
	elf_recalc_gui_object((elf_gui_object*)label);
}

elf_button* elf_create_button(const char *name)
{
	elf_button *button;

	button = (elf_button*)malloc(sizeof(elf_button));
	memset(button, 0x0, sizeof(elf_button));
	button->type = ELF_BUTTON;

	button->color.r = button->color.g = button->color.b = button->color.a = 1.0;
	button->visible = ELF_TRUE;

	if(name) button->name = elf_create_string(name);

	global_obj_count++;

	return button;
}

void elf_destroy_button(elf_button *button)
{
	if(button->name) elf_destroy_string(button->name);

	if(button->off) elf_dec_ref((elf_object*)button->off);
	if(button->over) elf_dec_ref((elf_object*)button->over);
	if(button->on) elf_dec_ref((elf_object*)button->on);

	if(button->script) elf_dec_ref((elf_object*)button->script);

	free(button);

	global_obj_count--;
}

void elf_draw_button(elf_button *button, gfx_shader_params *shader_params)
{
	if(!button->visible) return;

	gfx_set_color(&shader_params->material_params.color, button->color.r, button->color.g, button->color.b, button->color.a);

	if(button->state == ELF_OFF)
	{
		if(button->off) shader_params->texture_params[0].texture = button->off->texture;
	}
	else if(button->state == ELF_OVER)
	{
		if(button->over) shader_params->texture_params[0].texture = button->over->texture;
	}
	else if(button->state == ELF_ON)
	{
		if(button->on) shader_params->texture_params[0].texture = button->on->texture;
	}

	if(shader_params->texture_params[0].texture)
	{
		gfx_set_shader_params(shader_params);
		gfx_draw_textured_2d_quad((float)button->pos.x, (float)button->pos.y, (float)button->width, (float)button->height);
		shader_params->texture_params[0].texture = NULL;
	}
}

unsigned char elf_get_button_state(elf_button *button)
{
	return button->state;
}

elf_texture* elf_get_button_off_texture(elf_button *button)
{
	return button->off;
}

elf_texture* elf_get_button_over_texture(elf_button *button)
{
	return button->over;
}

elf_texture* elf_get_button_on_texture(elf_button *button)
{
	return button->on;
}

void elf_recalc_button(elf_button *button)
{
	if(button->off)
	{
		button->width = elf_get_texture_width(button->off);
		button->height = elf_get_texture_height(button->off);
	}
	else
	{
		button->height = 0;
		button->width = 0;
	}
}

void elf_set_button_position(elf_button *button, int x, int y)
{
	button->rel_pos.x = x;
	button->rel_pos.y = y;
	elf_recalc_gui_object((elf_gui_object*)button);
}

void elf_set_button_off_texture(elf_button *button, elf_texture *off)
{
	if(button->off) elf_dec_ref((elf_object*)button->off);
	button->off = off;
	if(button->off) elf_inc_ref((elf_object*)button->off);
	elf_recalc_gui_object((elf_gui_object*)button);
}

void elf_set_button_over_texture(elf_button *button, elf_texture *over)
{
	if(button->over) elf_dec_ref((elf_object*)button->over);
	button->over = over;
	if(button->over) elf_inc_ref((elf_object*)button->over);
}

void elf_set_button_on_texture(elf_button *button, elf_texture *on)
{
	if(button->on) elf_dec_ref((elf_object*)button->on);
	button->on = on;
	if(button->on) elf_inc_ref((elf_object*)button->on);
}

elf_picture* elf_create_picture(const char *name)
{
	elf_picture *picture;

	picture = (elf_picture*)malloc(sizeof(elf_picture));
	memset(picture, 0x0, sizeof(elf_picture));
	picture->type = ELF_PICTURE;

	picture->color.r = picture->color.g = picture->color.b = picture->color.a = 1.0;
	picture->visible = ELF_TRUE;

	if(name) picture->name = elf_create_string(name);

	global_obj_count++;

	return picture;
}

void elf_destroy_picture(elf_picture *picture)
{
	if(picture->name) elf_destroy_string(picture->name);

	if(picture->texture) elf_dec_ref((elf_object*)picture->texture);

	free(picture);

	global_obj_count--;
}

void elf_draw_picture(elf_picture *picture, gfx_shader_params *shader_params)
{
	if(!picture->visible || !picture->texture) return;

	gfx_set_color(&shader_params->material_params.color, picture->color.r,
		picture->color.g, picture->color.b, picture->color.a);

	shader_params->texture_params[0].texture = picture->texture->texture;
	gfx_set_shader_params(shader_params);
	gfx_draw_textured_2d_quad((float)picture->pos.x, (float)picture->pos.y, (float)picture->width, (float)picture->height);
	shader_params->texture_params[0].texture = NULL;
}

elf_texture* elf_get_picture_texture(elf_picture *picture)
{
	return picture->texture;
}

void elf_recalc_picture(elf_picture *picture)
{
	if(picture->texture)
	{
		picture->width = elf_get_texture_width(picture->texture);
		picture->height = elf_get_texture_height(picture->texture);
	}
	else
	{
		picture->height = 0;
		picture->width = 0;
	}
}

void elf_set_picture_texture(elf_picture *picture, elf_texture *texture)
{
	if(picture->texture) elf_dec_ref((elf_object*)picture->texture);
	picture->texture = texture;
	if(picture->texture) elf_inc_ref((elf_object*)picture->texture);
	elf_recalc_gui_object((elf_gui_object*)picture);
}

elf_text_field* elf_create_text_field(const char *name)
{
	elf_text_field *text_field;

	text_field = (elf_text_field*)malloc(sizeof(elf_text_field));
	memset(text_field, 0x0, sizeof(elf_text_field));
	text_field->type = ELF_TEXT_FIELD;

	text_field->color.r = text_field->color.g = text_field->color.b = text_field->color.a = 1.0;
	text_field->text_color.r = text_field->text_color.g = text_field->text_color.b = text_field->text_color.a = 1.0;
	text_field->visible = ELF_TRUE;
	text_field->text = elf_create_string("");

	if(name) text_field->name = elf_create_string(name);

	global_obj_count++;

	return text_field;
}

void elf_destroy_text_field(elf_text_field *text_field)
{
	if(text_field->name) elf_destroy_string(text_field->name);

	if(text_field->texture) elf_dec_ref((elf_object*)text_field->texture);
	if(text_field->font) elf_dec_ref((elf_object*)text_field->font);
	if(text_field->text) elf_destroy_string(text_field->text);
	if(text_field->script) elf_dec_ref((elf_object*)text_field->script);

	free(text_field);

	global_obj_count--;
}

void elf_draw_text_field(elf_text_field *text_field, elf_area *area, gfx_shader_params *shader_params)
{
	char *str;
	int x, y, width, height;

	if(!text_field->visible) return;

	x = text_field->pos.x;
	y = text_field->pos.y;
	width = text_field->width;
	height = text_field->height;
	if(text_field->parent)
	{
		if(x < area->pos.x)
		{
			width -= area->pos.x-x;
			x = area->pos.x;
		}
		if(y < area->pos.y)
		{
			height -= area->pos.y-y;
			y = area->pos.y;
		}
		if(x+width > area->pos.x+area->size.x)
			width -= (x+width)-(area->pos.x+area->size.x);
		if(y+height > area->pos.y+area->size.y)
			height -= (y+height)-(area->pos.y+area->size.y);
	}

	gfx_set_color(&shader_params->material_params.color, text_field->color.r,
		text_field->color.g, text_field->color.b, text_field->color.a);

	if(text_field->texture) shader_params->texture_params[0].texture = text_field->texture->texture;

	if(shader_params->texture_params[0].texture)
	{
		gfx_set_shader_params(shader_params);
		gfx_draw_textured_2d_quad((float)text_field->pos.x, (float)text_field->pos.y, (float)text_field->width, (float)text_field->height);
		shader_params->texture_params[0].texture = NULL;
	}

	if(text_field->font && text_field->text && strlen(text_field->text) > 0)
	{
		gfx_set_viewport(x+text_field->offset_x, y+text_field->offset_y,
			width-text_field->offset_x, height-text_field->offset_y);
		gfx_get_orthographic_projection_matrix((float)x+text_field->offset_x, (float)x+width-text_field->offset_x,
			(float)y+text_field->offset_y, (float)y+height-text_field->offset_y,
			-1.0f, 1.0f, shader_params->projection_matrix);

		gfx_set_color(&shader_params->material_params.color, text_field->text_color.r, text_field->text_color.g,
			text_field->text_color.b, text_field->text_color.a*text_field->color.a);

		str = elf_sub_string(text_field->text, text_field->draw_pos,
			strlen(text_field->text)-text_field->draw_pos);
		elf_draw_string(text_field->font, str, text_field->pos.x+text_field->offset_x, text_field->pos.y+text_field->offset_y, shader_params);
		elf_destroy_string(str);

		shader_params->texture_params[0].texture = NULL;
	}

	if(text_field->text && text_field->root && text_field->root->active_text_field == text_field)
	{
		gfx_set_shader_params(shader_params);

		str = elf_sub_string(text_field->text, text_field->draw_pos,
			text_field->cursor_pos-text_field->draw_pos);
		gfx_draw_2d_quad(text_field->pos.x+text_field->offset_x+elf_get_string_width(text_field->font, str),
			text_field->pos.y+text_field->offset_y, 1, text_field->height-text_field->offset_y*2);
		elf_destroy_string(str);
	}
}

elf_texture* elf_get_text_field_texture(elf_text_field *text_field)
{
	return text_field->texture;
}

elf_font* elf_get_text_field_font(elf_text_field *text_field)
{
	return text_field->font;
}

elf_color elf_get_text_field_text_color(elf_text_field *text_field)
{
	return text_field->text_color;
}

elf_vec2i elf_get_text_field_offset(elf_text_field *text_field)
{
	elf_vec2i offset;

	offset.x = text_field->offset_x;
	offset.y = text_field->offset_y;

	return offset;
}

const char* elf_get_text_field_text(elf_text_field *text_field)
{
	return text_field->text;
}

void elf_recalc_text_field(elf_text_field *text_field)
{
	if(text_field->texture)
	{
		text_field->width = elf_get_texture_width(text_field->texture);
		text_field->height = elf_get_texture_height(text_field->texture);
	}
	else
	{
		text_field->height = 0;
		text_field->width = 0;
	}
}

void elf_set_text_field_texture(elf_text_field *text_field, elf_texture *texture)
{
	if(text_field->texture) elf_dec_ref((elf_object*)text_field->texture);
	text_field->texture = texture;
	if(text_field->texture) elf_inc_ref((elf_object*)text_field->texture);
	elf_recalc_gui_object((elf_gui_object*)text_field);
}

void elf_set_text_field_font(elf_text_field *text_field, elf_font *font)
{
	if(text_field->font) elf_dec_ref((elf_object*)text_field->font);
	text_field->font = font;
	if(text_field->font) elf_inc_ref((elf_object*)text_field->font);
}

void elf_set_text_field_text_color(elf_text_field *text_field, float r, float g, float b, float a)
{
	text_field->text_color.r = r;
	text_field->text_color.g = g;
	text_field->text_color.b = b;
	text_field->text_color.a = a;
}

void elf_set_text_field_offset(elf_text_field *text_field, int offset_x, int offset_y)
{
	text_field->offset_x = offset_x;
	text_field->offset_y = offset_y;
}

void elf_set_text_field_text(elf_text_field *text_field, const char *text)
{
	if(text_field->text) elf_destroy_string(text_field->text);
	text_field->text = elf_create_string(text);
}

void elf_move_text_field_cursor_left(elf_text_field *text_field)
{
	if(text_field->cursor_pos == 0) return;

	text_field->cursor_pos--;

	if(text_field->cursor_pos == text_field->draw_pos &&
		text_field->draw_pos > 0)
	{
		text_field->draw_pos--;
	}
}

void elf_move_text_field_cursor_right(elf_text_field *text_field)
{
	char *str;

	if(text_field->cursor_pos >= (int)strlen(text_field->text)) return;

	text_field->cursor_pos++;

	if(!text_field->font) return;

	str = elf_sub_string(text_field->text, text_field->draw_pos,
		text_field->cursor_pos-text_field->draw_pos);
	if(elf_get_string_width(text_field->font, str)+2 > text_field->width-text_field->offset_x*2)
	{
		if(text_field->draw_pos+5 > text_field->cursor_pos)
			text_field->draw_pos += text_field->cursor_pos-1;
		else text_field->draw_pos += 5;
	}

	elf_destroy_string(str);
}

elf_slider* elf_create_slider(const char *name)
{
	elf_slider *slider;

	slider = (elf_slider*)malloc(sizeof(elf_slider));
	memset(slider, 0x0, sizeof(elf_slider));
	slider->type = ELF_SLIDER;

	slider->color.r = slider->color.g = slider->color.b = slider->color.a = 1.0; 
	slider->visible = ELF_TRUE;
	slider->value = 1.0f;

	if(name) slider->name = elf_create_string(name);

	global_obj_count++;

	return slider;
}

void elf_destroy_slider(elf_slider *slider)
{
	if(slider->name) elf_destroy_string(slider->name);

	if(slider->background) elf_dec_ref((elf_object*)slider->background);
	if(slider->slider) elf_dec_ref((elf_object*)slider->slider);

	if(slider->script) elf_dec_ref((elf_object*)slider->script);

	free(slider);

	global_obj_count--;
}

void elf_draw_slider(elf_slider *slider, gfx_shader_params *shader_params)
{
	if(!slider->visible) return;

	if(slider->background)
	{
		gfx_set_color(&shader_params->material_params.color, slider->color.r, slider->color.g, slider->color.b, slider->color.a);
		shader_params->texture_params[0].texture = slider->background->texture;
		gfx_set_shader_params(shader_params);
		gfx_draw_textured_2d_quad((float)slider->pos.x, (float)slider->pos.y, (float)slider->width, (float)slider->height);
		shader_params->texture_params[0].texture = NULL;
	}

	if(slider->slider)
	{
		gfx_set_color(&shader_params->material_params.color, slider->color.r, slider->color.g, slider->color.b, slider->color.a);
		shader_params->texture_params[0].texture = slider->slider->texture;
		gfx_set_shader_params(shader_params);
		if(slider->width > slider->height)
		{
			gfx_draw_textured_2d_quad_region((float)slider->pos.x, (float)slider->pos.y, (float)slider->width*slider->value, (float)slider->height, 0.0, 0.0, slider->value, 1.0);
		}
		else
		{
			gfx_draw_textured_2d_quad_region((float)slider->pos.x, (float)slider->pos.y, (float)slider->width, (float)slider->height*slider->value, 0.0, 0.0, 1.0, slider->value);	
		}
		shader_params->texture_params[0].texture = NULL;
	}
}

elf_texture* elf_get_slider_background_texture(elf_slider *slider)
{
	return slider->background;
}

elf_texture* elf_get_slider_slider_texture(elf_slider *slider)
{
	return slider->slider;
}

float elf_get_slider_value(elf_slider *slider)
{
	return slider->value;
}

void elf_recalc_slider(elf_slider *slider)
{
	if(slider->background)
	{
		slider->width = elf_get_texture_width(slider->background);
		slider->height = elf_get_texture_height(slider->background);
	}
	else
	{
		slider->height = 0;
		slider->width = 0;
	}
}

void elf_set_slider_background_texture(elf_slider *slider, elf_texture *background)
{
	if(slider->background) elf_dec_ref((elf_object*)slider->background);
	slider->background = background;
	if(slider->background) elf_inc_ref((elf_object*)slider->background);
	elf_recalc_gui_object((elf_gui_object*)slider);
}

void elf_set_slider_slider_texture(elf_slider *slider, elf_texture *slider_texture)
{
	if(slider->slider) elf_dec_ref((elf_object*)slider->slider);
	slider->slider = slider_texture;
	if(slider->slider) elf_inc_ref((elf_object*)slider->slider);
}

void elf_set_slider_value(elf_slider *slider, float value)
{
	slider->value = value;
	if(slider->value < 0.0f) slider->value = 0.0f;
	if(slider->value > 1.0f) slider->value = 1.0f;
}

elf_screen* elf_create_screen(const char *name)
{
	elf_screen *screen;

	screen = (elf_screen*)malloc(sizeof(elf_screen));
	memset(screen, 0x0, sizeof(elf_screen));
	screen->type = ELF_SCREEN;

	screen->color.r = screen->color.g = screen->color.b = screen->color.a = 1.0;
	screen->visible = ELF_TRUE;

	screen->children = elf_create_list();
	screen->screens = elf_create_list();

	elf_inc_ref((elf_object*)screen->children);
	elf_inc_ref((elf_object*)screen->screens);

	if(name) screen->name = elf_create_string(name);

	global_obj_count++;

	return screen;
}

void elf_destroy_screen(elf_screen *screen)
{
	if(screen->name) elf_destroy_string(screen->name);

	if(screen->texture) elf_dec_ref((elf_object*)screen->texture);

	elf_dec_ref((elf_object*)screen->children);
	elf_dec_ref((elf_object*)screen->screens);

	free(screen);

	global_obj_count--;
}

void elf_draw_screen(elf_screen *screen, elf_area *area, gfx_shader_params *shader_params)
{
	elf_gui_object *object;
	int x, y, width, height;

	if(!screen->visible || !screen->texture) return;

	x = screen->pos.x;
	y = screen->pos.y;
	width = screen->width;
	height = screen->height;
	if(screen->parent)
	{
		if(x < area->pos.x)
		{
			width -= area->pos.x-x;
			x = area->pos.x;
		}
		if(y < area->pos.y)
		{
			height -= area->pos.y-y;
			y = area->pos.y;
		}
		if(x+width > area->pos.x+area->size.x)
			width -= (x+width)-(area->pos.x+area->size.x);
		if(y+height > area->pos.y+area->size.y)
			height -= (y+height)-(area->pos.y+area->size.y);
	}

	gfx_set_color(&shader_params->material_params.color, 1.0, 1.0, 1.0, 1.0);

	shader_params->texture_params[0].texture = screen->texture->texture;
	gfx_set_shader_params(shader_params);
	gfx_draw_textured_2d_quad((float)screen->pos.x, (float)screen->pos.y, (float)screen->width, (float)screen->height);
	shader_params->texture_params[0].texture = NULL;

	gfx_set_viewport(x, y, width, height);
	gfx_get_orthographic_projection_matrix((float)x, (float)x+width, (float)y, (float)y+height,
		-1.0f, 1.0f, shader_params->projection_matrix);

	for(object = (elf_gui_object*)elf_begin_list(screen->children); object;
		object = (elf_gui_object*)elf_next_in_list(screen->children))
	{
		if(object->type == ELF_LABEL) elf_draw_label((elf_label*)object, shader_params);
		else if(object->type == ELF_BUTTON) elf_draw_button((elf_button*)object, shader_params);
		else if(object->type == ELF_PICTURE) elf_draw_picture((elf_picture*)object, shader_params);
		else if(object->type == ELF_TEXT_FIELD)
		{
			elf_draw_text_field((elf_text_field*)object, area, shader_params);
			gfx_set_viewport(x, y, width, height);
			gfx_get_orthographic_projection_matrix((float)x, (float)x+width, (float)y, (float)y+height,
				-1.0f, 1.0f, shader_params->projection_matrix);
		}
		else if(object->type == ELF_TEXT_LIST)
		{
			elf_draw_text_list((elf_text_list*)object, area, shader_params);
			gfx_set_viewport(x, y, width, height);
			gfx_get_orthographic_projection_matrix((float)x, (float)x+width, (float)y, (float)y+height,
				-1.0f, 1.0f, shader_params->projection_matrix);
		}
		else if(object->type == ELF_SLIDER) elf_draw_slider((elf_slider*)object, shader_params);
		else if(object->type == ELF_CHECK_BOX) elf_draw_check_box((elf_check_box*)object, shader_params);
	}

	for(object = (elf_gui_object*)elf_begin_list(screen->screens); object;
		object = (elf_gui_object*)elf_next_in_list(screen->screens))
	{
		area->pos.x = x; area->pos.y = y; area->size.x = width; area->size.y = height;
		elf_draw_screen((elf_screen*)object, area, shader_params);
		gfx_set_viewport(x, y, width, height);
		gfx_get_orthographic_projection_matrix((float)x, (float)x+width, (float)y, (float)y+height,
			-1.0f, 1.0f, shader_params->projection_matrix);
	}
}

elf_texture* elf_get_screen_texture(elf_screen *screen)
{
	return screen->texture;
}

void elf_recalc_screen(elf_screen *screen)
{
	if(screen->texture)
	{
		screen->width = elf_get_texture_width(screen->texture);
		screen->height = elf_get_texture_height(screen->texture);
	}
	else
	{
		screen->height = 0;
		screen->width = 0;
	}
}

void elf_set_screen_texture(elf_screen *screen, elf_texture *texture)
{
	if(screen->texture) elf_dec_ref((elf_object*)screen->texture);
	screen->texture = texture;
	if(screen->texture) elf_inc_ref((elf_object*)screen->texture);
	elf_recalc_gui_object((elf_gui_object*)screen);
}

elf_text_list* elf_create_text_list(const char *name)
{
	elf_text_list *text_list;

	text_list = (elf_text_list*)malloc(sizeof(elf_text_list));
	memset(text_list, 0x0, sizeof(elf_text_list));
	text_list->type = ELF_TEXT_LIST;

	text_list->color.r = text_list->color.g = text_list->color.b = text_list->color.a = 1.0;
	text_list->selection_color.r = text_list->selection_color.g = text_list->selection_color.b = 0.5;
	text_list->selection_color.a = 0.5;
	text_list->visible = ELF_TRUE;
	text_list->rows = 16;
	text_list->list_width = 256;
	text_list->selection = -1;

	text_list->items = elf_create_list();
	elf_inc_ref((elf_object*)text_list->items);

	if(name) text_list->name = elf_create_string(name);

	global_obj_count++;

	return text_list;
}

void elf_destroy_text_list(elf_text_list *text_list)
{
	if(text_list->name) elf_destroy_string(text_list->name);
	if(text_list->font) elf_dec_ref((elf_object*)text_list->font);

	elf_dec_ref((elf_object*)text_list->items);

	free(text_list);

	global_obj_count--;
}

void elf_draw_text_list(elf_text_list *text_list, elf_area *area, gfx_shader_params *shader_params)
{
	int x, y, width, height;
	int offset;
	int i;
	elf_string *str_obj;

	if(!text_list->visible || !text_list->font || elf_get_list_length(text_list->items) < 1) return;

	x = text_list->pos.x;
	y = text_list->pos.y;
	width = text_list->width;
	height = text_list->height;
	if(text_list->parent)
	{
		if(x < area->pos.x)
		{
			width -= area->pos.x-x;
			x = area->pos.x;
		}
		if(y < area->pos.y)
		{
			height -= area->pos.y-y;
			y = area->pos.y;
		}
		if(x+width > area->pos.x+area->size.x)
			width -= (x+width)-(area->pos.x+area->size.x);
		if(y+height > area->pos.y+area->size.y)
			height -= (y+height)-(area->pos.y+area->size.y);
	}

	gfx_set_viewport(x, y, width, height);
	gfx_get_orthographic_projection_matrix((float)x, (float)x+width, (float)y, (float)y+height,
		-1.0f, 1.0f, shader_params->projection_matrix);
	gfx_set_color(&shader_params->material_params.color, text_list->color.r,
		text_list->color.g, text_list->color.b, text_list->color.a);

	offset = text_list->font->size+text_list->font->offset_y;
	for(i = 0, str_obj = (elf_string*)elf_begin_list(text_list->items); str_obj;
		str_obj = (elf_string*)elf_next_in_list(text_list->items), i++)
	{
		if(i < text_list->offset) continue;
		if(i-text_list->offset > text_list->rows-1) break;

		if(i == text_list->selection)
		{
			shader_params->texture_params[0].texture = NULL;
			gfx_set_color(&shader_params->material_params.color, text_list->selection_color.r,
				text_list->selection_color.g, text_list->selection_color.b, text_list->selection_color.a);
			gfx_set_shader_params(shader_params);	
			gfx_draw_2d_quad((float)text_list->pos.x, (float)text_list->pos.y+text_list->height-offset,
				(float)text_list->list_width, (float)text_list->font->size+text_list->font->offset_y);
		}

		gfx_set_color(&shader_params->material_params.color, text_list->color.r,
			text_list->color.g, text_list->color.b, text_list->color.a);
		elf_draw_string(text_list->font, str_obj->str, text_list->pos.x, text_list->pos.y+text_list->height-offset, shader_params);

		offset += text_list->font->size+text_list->font->offset_y;
	}

	shader_params->texture_params[0].texture = NULL;
}

elf_font* elf_get_text_list_font(elf_text_list *text_list)
{
	return text_list->font;
}

elf_color elf_get_text_list_selection_color(elf_text_list *text_list)
{
	return text_list->selection_color;
}

int elf_get_text_list_row_count(elf_text_list *text_list)
{
	return text_list->rows;
}

int elf_get_text_list_item_count(elf_text_list *text_list)
{
	return elf_get_list_length(text_list->items);
}

int elf_get_text_list_selection_index(elf_text_list *text_list)
{
	return text_list->selection;
}

int elf_get_text_list_offset(elf_text_list *text_list)
{
	return text_list->offset;
}

const char* elf_get_text_list_item(elf_text_list *text_list, int idx)
{
	int i;
	elf_string *str_obj;

	if(idx < 0 || idx > elf_get_list_length(text_list->items)-1) return "";

	for(i = 0, str_obj = (elf_string*)elf_begin_list(text_list->items); str_obj;
		str_obj = (elf_string*)elf_next_in_list(text_list->items), i++)
	{
		if(idx == i) return str_obj->str;
	}

	return "";
}

const char* elf_get_text_list_selected_item(elf_text_list *text_list)
{
	int i;
	elf_string *str_obj;

	if(text_list->selection < 0 || text_list->selection > elf_get_list_length(text_list->items)-1) return "";

	for(i = 0, str_obj = (elf_string*)elf_begin_list(text_list->items); str_obj;
		str_obj = (elf_string*)elf_next_in_list(text_list->items), i++)
	{
		if(text_list->selection == i) return str_obj->str;
	}

	return "";
}

void elf_recalc_text_list(elf_text_list *text_list)
{
	if(text_list->font)
	{
		text_list->height = (text_list->font->size+text_list->font->offset_y)*text_list->rows;
		text_list->width = text_list->list_width;
	}
	else
	{
		text_list->height = 0;
		text_list->width = 0;
	}
}

void elf_set_text_list_font(elf_text_list *text_list, elf_font *font)
{
	if(text_list->font) elf_dec_ref((elf_object*)text_list->font);
	text_list->font = font;
	if(text_list->font) elf_inc_ref((elf_object*)text_list->font);
	elf_recalc_gui_object((elf_gui_object*)text_list);
}

void elf_set_text_list_selection_color(elf_text_list *text_list, float r, float g, float b, float a)
{
	text_list->selection_color.r = r;
	text_list->selection_color.g = g;
	text_list->selection_color.b = b;
	text_list->selection_color.a = a;
}

void elf_set_text_list_size(elf_text_list *text_list, int rows, int width)
{
	text_list->rows = rows;
	text_list->list_width = width;
	if(text_list->rows < 1) text_list->rows = 1;
	if(text_list->list_width < 1) text_list->width = 1;
	elf_recalc_gui_object((elf_gui_object*)text_list);
}

void elf_add_text_list_item(elf_text_list *text_list, const char *text)
{
	elf_string *str_obj;

	if(!text) return;

	str_obj = elf_create_string_object();
	str_obj->str = elf_create_string(text);

	elf_append_to_list(text_list->items, (elf_object*)str_obj);
}

unsigned char elf_remove_text_list_item(elf_text_list *text_list, int idx)
{
	int i;
	elf_string *str_obj;

	if(idx < 0 || idx > elf_get_list_length(text_list->items)-1) return ELF_FALSE;

	for(i = 0, str_obj = (elf_string*)elf_begin_list(text_list->items); str_obj;
		str_obj = (elf_string*)elf_next_in_list(text_list->items), i++)
	{
		if(idx == i)
		{
			if(idx == text_list->selection) text_list->selection = -1;
			elf_remove_from_list(text_list->items, (elf_object*)str_obj);
			return ELF_TRUE;
		}
	}

	return ELF_FALSE;
}

void elf_remove_text_list_items(elf_text_list *text_list)
{
	elf_dec_ref((elf_object*)text_list->items);
	text_list->items = elf_create_list();
	elf_inc_ref((elf_object*)text_list->items);
	text_list->offset = 0;
	text_list->selection = 0;
}

void elf_set_text_list_offset(elf_text_list *text_list, int offset)
{
	text_list->offset = offset;
	if(text_list->offset < 0) text_list->offset = 0;
}

void elf_set_text_list_selection(elf_text_list *text_list, int selection)
{
	text_list->selection = selection;
	if(text_list->selection < 0) text_list->selection = 0;
	if(text_list->selection > elf_get_list_length(text_list->items)-1)
		text_list->selection = elf_get_list_length(text_list->items)-1;
	if(elf_get_list_length(text_list->items) == 0) text_list->selection = -1;
}

elf_check_box* elf_create_check_box(const char *name)
{
	elf_check_box *check_box;

	check_box = (elf_check_box*)malloc(sizeof(elf_check_box));
	memset(check_box, 0x0, sizeof(elf_check_box));
	check_box->type = ELF_CHECK_BOX;

	check_box->color.r = check_box->color.g = check_box->color.b = check_box->color.a = 1.0;
	check_box->visible = ELF_TRUE;

	if(name) check_box->name = elf_create_string(name);

	global_obj_count++;

	return check_box;
}

void elf_destroy_check_box(elf_check_box *check_box)
{
	if(check_box->name) elf_destroy_string(check_box->name);

	if(check_box->off) elf_dec_ref((elf_object*)check_box->off);
	if(check_box->on) elf_dec_ref((elf_object*)check_box->on);

	if(check_box->script) elf_dec_ref((elf_object*)check_box->script);

	free(check_box);

	global_obj_count--;
}

void elf_draw_check_box(elf_check_box *check_box, gfx_shader_params *shader_params)
{
	if(!check_box->visible) return;

	gfx_set_color(&shader_params->material_params.color, check_box->color.r,
		check_box->color.g, check_box->color.b, check_box->color.a);

	if(check_box->state == ELF_OFF)
	{
		if(check_box->off) shader_params->texture_params[0].texture = check_box->off->texture;
	}
	else if(check_box->state == ELF_ON)
	{
		if(check_box->on) shader_params->texture_params[0].texture = check_box->on->texture;
	}

	if(shader_params->texture_params[0].texture)
	{
		gfx_set_shader_params(shader_params);
		gfx_draw_textured_2d_quad((float)check_box->pos.x, (float)check_box->pos.y, (float)check_box->width, (float)check_box->height);
		shader_params->texture_params[0].texture = NULL;
	}
}

unsigned char elf_get_check_box_state(elf_check_box *check_box)
{
	return check_box->state;
}

elf_texture* elf_get_check_box_off_texture(elf_check_box *check_box)
{
	return check_box->off;
}

elf_texture* elf_get_check_box_on_texture(elf_check_box *check_box)
{
	return check_box->on;
}

void elf_recalc_check_box(elf_check_box *check_box)
{
	if(check_box->off)
	{
		check_box->width = elf_get_texture_width(check_box->off);
		check_box->height = elf_get_texture_height(check_box->off);
	}
	else
	{
		check_box->height = 0;
		check_box->width = 0;
	}
}

void elf_set_check_box_off_texture(elf_check_box *check_box, elf_texture *off)
{
	if(check_box->off) elf_dec_ref((elf_object*)check_box->off);
	check_box->off = off;
	if(check_box->off) elf_inc_ref((elf_object*)check_box->off);
	elf_recalc_gui_object((elf_gui_object*)check_box);
}

void elf_set_check_box_on_texture(elf_check_box *check_box, elf_texture *on)
{
	if(check_box->on) elf_dec_ref((elf_object*)check_box->on);
	check_box->on = on;
	if(check_box->on) elf_inc_ref((elf_object*)check_box->on);
}

void elf_set_check_box_state(elf_check_box *check_box, unsigned char state)
{
	check_box->state = !state == ELF_OFF;
}

void elf_recalc_gui_object(elf_gui_object *object)
{
	elf_gui_object *obj;

	if(object->type == ELF_LABEL) elf_recalc_label((elf_label*)object);
	else if(object->type == ELF_BUTTON) elf_recalc_button((elf_button*)object);
	else if(object->type == ELF_PICTURE) elf_recalc_picture((elf_picture*)object);
	else if(object->type == ELF_TEXT_FIELD) elf_recalc_text_field((elf_text_field*)object);
	else if(object->type == ELF_SLIDER) elf_recalc_slider((elf_slider*)object);
	else if(object->type == ELF_SCREEN) elf_recalc_screen((elf_screen*)object);
	else if(object->type == ELF_TEXT_LIST) elf_recalc_text_list((elf_text_list*)object);
	else if(object->type == ELF_CHECK_BOX) elf_recalc_check_box((elf_check_box*)object);

	if(object->parent)
	{
		object->pos.x = object->parent->pos.x+object->rel_pos.x;
		object->pos.y = object->parent->pos.y+object->parent->height-object->rel_pos.y-object->height;
	}
	else
	{
		object->pos.x = object->rel_pos.x;
		object->pos.y = elf_get_window_height()-object->rel_pos.y-object->height;
	}

	if(object->children)
	{
		for(obj = (elf_gui_object*)elf_begin_list(object->children); obj;
			obj = (elf_gui_object*)elf_next_in_list(object->children))
		{
			elf_recalc_gui_object(obj);
		}
	}
}

void elf_clear_gui_object_root(elf_gui_object *object)
{
	elf_gui_object *cobject;

	if(object->children)
	{
		for(cobject = (elf_gui_object*)elf_begin_list(object->children); cobject;
			cobject = (elf_gui_object*)elf_next_in_list(object->children))
		{
			elf_clear_gui_object_root(cobject);
		}
	}

	object->root = NULL;
}

void elf_set_gui_object_root(elf_gui_object *object, elf_gui *root)
{
	elf_gui_object *cobject;

	if(object->children)
	{
		for(cobject = (elf_gui_object*)elf_begin_list(object->children); cobject;
			cobject = (elf_gui_object*)elf_next_in_list(object->children))
		{
			elf_set_gui_object_root(cobject, root);
		}
	}

	object->root = root;
}

elf_gui* elf_create_gui()
{
	elf_gui *gui;

	gui = (elf_gui*)malloc(sizeof(elf_gui));
	memset(gui, 0x0, sizeof(elf_gui));
	gui->type = ELF_GUI;

	gui->children = elf_create_list();
	gui->screens = elf_create_list();

	elf_inc_ref((elf_object*)gui->children);
	elf_inc_ref((elf_object*)gui->screens);

	gui->root = gui;

	gui->pos.x = gui->pos.y = 0;
	gui->width = elf_get_window_width();
	gui->height = elf_get_window_height();

	global_obj_count++;

	return gui;
}

void elf_destroy_gui(elf_gui *gui)
{
	elf_gui_object *object;

	if(gui->name) elf_destroy_string(gui->name);

	for(object = (elf_gui_object*)elf_begin_list(gui->children); object;
		object = (elf_gui_object*)elf_next_in_list(gui->children))
	{
		elf_clear_gui_object_root(object);
		object->parent = NULL;
	}

	elf_dec_ref((elf_object*)gui->children);
	elf_dec_ref((elf_object*)gui->screens);

	free(gui);

	global_obj_count--;
}

elf_gui_object* elf_trace_top_object(elf_gui_object *object, unsigned char click)
{
	elf_gui_object *cobject;
	elf_gui_object *result;
	elf_vec2i mouse_pos;

	mouse_pos = elf_get_mouse_position();
	mouse_pos.y = elf_get_window_height()-mouse_pos.y;

	result = NULL;
	if(object->screens)
	{
		for(cobject = (elf_gui_object*)elf_rbegin_list(object->screens); cobject;
			cobject = (elf_gui_object*)elf_rnext_in_list(object->screens))
		{
			if(mouse_pos.x >= cobject->pos.x && mouse_pos.x <= cobject->pos.x+cobject->width &&
				mouse_pos.y >= cobject->pos.y && mouse_pos.y <= cobject->pos.y+cobject->height)
			{
				if(click)
				{
					elf_inc_ref((elf_object*)cobject);
					elf_remove_from_list(object->screens, (elf_object*)cobject);
					elf_append_to_list(object->screens, (elf_object*)cobject);
					elf_dec_ref((elf_object*)cobject);
				}
				result = elf_trace_top_object(cobject, click);
				if(result) return result;
				return cobject;
			}
		}
	}

	result = NULL;
	if(object->children)
	{
		for(cobject = (elf_gui_object*)elf_rbegin_list(object->children); cobject;
			cobject = (elf_gui_object*)elf_rnext_in_list(object->children))
		{
			if(mouse_pos.x >= cobject->pos.x && mouse_pos.x <= cobject->pos.x+cobject->width &&
				mouse_pos.y >= cobject->pos.y && mouse_pos.y <= cobject->pos.y+cobject->height)
			{
				result = elf_trace_top_object(cobject, click);
				if(result) return result;
				return cobject;
			}
		}
	}

	if(mouse_pos.x >= object->pos.x && mouse_pos.x <= object->pos.x+object->width &&
		mouse_pos.y >= object->pos.y && mouse_pos.y <= object->pos.x+object->height)
	{
		return object;
	}

	return NULL;
}

void elf_send_gui_char_event(elf_gui *gui, char chr)
{
	char *str;

	if(chr < 32 || chr > 126) return;

	if(gui->active_text_field)
	{
		str = elf_insert_char_to_string(gui->active_text_field->text,
			gui->active_text_field->cursor_pos, chr);
		elf_destroy_string(gui->active_text_field->text);
		gui->active_text_field->text = str;
		elf_move_text_field_cursor_right(gui->active_text_field);

		gui->active_text_field->event = ELF_CHAR_INPUT;
		if(gui->active_text_field->script)
		{
			eng->actor = (elf_object*)gui->active_text_field;
			elf_inc_ref((elf_object*)gui->active_text_field);

			elf_run_string("me = elf.GetActor(); event = elf.CHAR_INPUT");
			elf_run_script(gui->active_text_field->script);
			elf_run_string("me = nil; event = 0");

			elf_dec_ref((elf_object*)gui->active_text_field);
			eng->actor = NULL;
		}
	}
}

void elf_send_gui_key_event(elf_gui *gui, int key)
{
	char *str;

	if(gui->active_text_field)
	{
		if(key == ELF_KEY_LEFT)
		{
			elf_move_text_field_cursor_left(gui->active_text_field);
		}
		else if(key == ELF_KEY_RIGHT)
		{
			elf_move_text_field_cursor_right(gui->active_text_field);
		}
		else if(key == ELF_KEY_DEL)
		{
			if(gui->active_text_field->cursor_pos < (int)strlen(gui->active_text_field->text))
			{
				str = elf_remove_char_from_string(gui->active_text_field->text,
					gui->active_text_field->cursor_pos);
				elf_destroy_string(gui->active_text_field->text);
				gui->active_text_field->text = str;
			}
		}
		else if(key == ELF_KEY_BACKSPACE)
		{
			if(gui->active_text_field->cursor_pos > 0 &&
				strlen(gui->active_text_field->text) > 0)
			{
				str = elf_remove_char_from_string(gui->active_text_field->text,
					gui->active_text_field->cursor_pos-1);
				elf_destroy_string(gui->active_text_field->text);
				gui->active_text_field->text = str;
				elf_move_text_field_cursor_left(gui->active_text_field);
			}
		}
		else if(key == ELF_KEY_ENTER)
		{
			gui->active_text_field->event = ELF_LOSE_FOCUS;
			if(gui->active_text_field->script)
			{
				eng->actor = (elf_object*)gui->active_text_field;
				elf_inc_ref((elf_object*)gui->active_text_field);

				elf_run_string("me = elf.GetActor(); event = elf.LOSE_FOCUS");
				elf_run_script(gui->active_text_field->script);
				elf_run_string("me = nil; event = 0");

				elf_dec_ref((elf_object*)gui->active_text_field);
				eng->actor = NULL;
			}

			gui->active_text_field = NULL;
		}
	}
}

void elf_reset_gui_object_events(elf_gui_object *object)
{
	elf_gui_object *obj;

	object->event = 0;

	if(object->children)
	{
		for(obj = (elf_gui_object*)elf_begin_list(object->children); obj;
			obj = (elf_gui_object*)elf_next_in_list(object->children))
		{
			elf_reset_gui_object_events(obj);
		}
	}

	if(object->screens)
	{
		for(obj = (elf_gui_object*)elf_begin_list(object->screens); obj;
			obj = (elf_gui_object*)elf_next_in_list(object->screens))
		{
			elf_reset_gui_object_events(obj);
		}
	}
}

void elf_update_gui(elf_gui *gui, float step)
{
	elf_vec2i mouse_force;
	elf_vec2i mouse_position;
	unsigned moved;
	elf_gui_object *prev_trace;
	elf_object *event;
	elf_slider *slider;
	elf_text_list *text_list;
	int i;

	elf_reset_gui_object_events((elf_gui_object*)gui);

	mouse_force = elf_get_mouse_force();
	mouse_position = elf_get_mouse_position();

	moved = ELF_FALSE;
	if(mouse_force.x || mouse_force.y) moved = ELF_TRUE;

	prev_trace = gui->trace;
	gui->trace = elf_trace_top_object((elf_gui_object*)gui, elf_get_mouse_button_state(ELF_BUTTON_LEFT) == ELF_PRESSED);

	if(gui->trace)
	{
		if(gui->trace->type == ELF_BUTTON)
		{
			if(!gui->target)
			{
				((elf_button*)gui->trace)->state = ELF_OVER;
			}
			else if(gui->target == gui->trace)
			{
				((elf_button*)gui->trace)->state = ELF_ON;
			}
		}
	}

	if(prev_trace && prev_trace != gui->trace)
	{
		if(prev_trace->type == ELF_BUTTON)
		{
			((elf_button*)prev_trace)->state = ELF_OFF;
		}
	}

	if(elf_get_mouse_button_state(ELF_BUTTON_LEFT) == ELF_PRESSED)
	{
		gui->active_text_field = NULL;
		gui->target = gui->trace;
		if(gui->target)
		{
			if(gui->target->type == ELF_BUTTON)
			{
				((elf_button*)gui->target)->state = ELF_ON;
			}
			else if(gui->target->type == ELF_TEXT_FIELD)
			{
				gui->active_text_field = (elf_text_field*)gui->target;

				gui->active_text_field->event = ELF_GAIN_FOCUS;
				if(gui->active_text_field->script)
				{
					eng->actor = (elf_object*)gui->active_text_field;
					elf_inc_ref((elf_object*)gui->active_text_field);

					elf_run_string("me = elf.GetActor(); event = elf.GAIN_FOCUS");
					elf_run_script(gui->active_text_field->script);
					elf_run_string("me = nil; event = 0");

					elf_dec_ref((elf_object*)gui->active_text_field);
					eng->actor = NULL;
				}
			}
			else if(gui->target->type == ELF_SLIDER)
			{
				slider = (elf_slider*)gui->target;
				if(slider->width > slider->height)
				{
					slider->value = (float)(mouse_position.x-slider->pos.x)/((float)slider->width);
					if(slider->value < 0.0f) slider->value = 0.0f;
					if(slider->value > 1.0f) slider->value = 1.0f;
				}
				else
				{
					slider->value = (float)((elf_get_window_height()-mouse_position.y)-slider->pos.y)/((float)slider->height);
					if(slider->value < 0.0f) slider->value = 0.0f;
					if(slider->value > 1.0f) slider->value = 1.0f;
				}

				slider->event = ELF_VALUE_CHANGED;
				if(slider->script)
				{
					eng->actor = (elf_object*)gui->target;
					elf_inc_ref((elf_object*)gui->target);

					elf_run_string("me = elf.GetActor(); event = elf.VALUE_CHANGED");
					elf_run_script(slider->script);
					elf_run_string("me = nil; event = 0");

					elf_dec_ref((elf_object*)gui->target);
					eng->actor = NULL;
				}
			}
			else if(gui->target->type == ELF_TEXT_LIST)
			{
				text_list = (elf_text_list*)gui->target;
				if(text_list->font && elf_get_list_length(text_list->items) > 0)
				{
					text_list->selection = text_list->rows-1
						-(elf_get_window_height()-mouse_position.y-text_list->pos.y)/
							(text_list->font->size+text_list->font->offset_y)
						+text_list->offset;
					if(text_list->selection > elf_get_list_length(text_list->items)-1) text_list->selection = -1;

					text_list->event = ELF_SELECTION_CHANGED;
					if(text_list->script)
					{
						eng->actor = (elf_object*)gui->target;
						elf_inc_ref((elf_object*)gui->target);

						elf_run_string("me = elf.GetActor(); event = elf.SELECTION_CHANGED");
						elf_run_script(text_list->script);
						elf_run_string("me = nil; event = 0");

						elf_dec_ref((elf_object*)gui->target);
						eng->actor = NULL;
					}
				}
			}
			else if(gui->target->type == ELF_CHECK_BOX)
			{
				((elf_check_box*)gui->target)->state = !((elf_check_box*)gui->target)->state;

				((elf_check_box*)gui->target)->event = ELF_STATE_CHANGED;
				if(((elf_check_box*)gui->target)->script)
				{
					eng->actor = (elf_object*)gui->target;
					elf_inc_ref((elf_object*)gui->target);

					elf_run_string("me = elf.GetActor(); event = elf.STATE_CHANGED");
					elf_run_script(((elf_check_box*)gui->target)->script);
					elf_run_string("me = nil; event = 0");

					elf_dec_ref((elf_object*)gui->target);
					eng->actor = NULL;
				}
			}
		}
	}
	else if(elf_get_mouse_button_state(ELF_BUTTON_LEFT) == ELF_RELEASED)
	{
		if(gui->target)
		{
			if(gui->target->type == ELF_BUTTON)
			{
				if(gui->target == gui->trace)
				{
					((elf_button*)gui->target)->event = ELF_CLICKED;
					if(((elf_button*)gui->target)->script)
					{
						eng->actor = (elf_object*)gui->target;
						elf_inc_ref((elf_object*)gui->target);

						((elf_button*)gui->target)->state = ELF_OFF;

						elf_run_string("me = elf.GetActor(); event = elf.CLICKED");
						elf_run_script(((elf_button*)gui->target)->script);
						elf_run_string("me = nil; event = 0");

						elf_dec_ref((elf_object*)gui->target);
						eng->actor = NULL;
					}
				}
			}
		}
		gui->target = NULL;
	}
	else if(elf_get_mouse_button_state(ELF_BUTTON_LEFT) == ELF_DOWN && moved)
	{
		if(gui->target)
		{
			if(gui->target->type == ELF_SLIDER)
			{
				slider = (elf_slider*)gui->target;
				if(slider->width > slider->height)
				{
					slider->value = (float)(mouse_position.x-slider->pos.x)/((float)slider->width);
					if(slider->value < 0.0f) slider->value = 0.0f;
					if(slider->value > 1.0f) slider->value = 1.0f;
				}
				else
				{
					slider->value = (float)((elf_get_window_height()-mouse_position.y)-slider->pos.y)/((float)slider->height);
					if(slider->value < 0.0f) slider->value = 0.0f;
					if(slider->value > 1.0f) slider->value = 1.0f;
				}

				slider->event = ELF_VALUE_CHANGED;
				if(slider->script)
				{
					eng->actor = (elf_object*)gui->target;
					elf_inc_ref((elf_object*)gui->target);

					elf_run_string("me = elf.GetActor(); event = elf.VALUE_CHANGED");
					elf_run_script(slider->script);
					elf_run_string("me = nil; event = 0");

					elf_dec_ref((elf_object*)gui->target);
					eng->actor = NULL;
				}
			}
		}
	}

	if(gui->cur_key)
	{
		gui->key_step += step;
		if(gui->key_repeat)
		{
			if(gui->key_step >= 0.05)
			{
				elf_send_gui_key_event(gui, gui->cur_key);
				gui->key_step = gui->key_step-0.05;
			}
		}
		else
		{
			if(gui->key_step >= 0.35)
			{
				elf_send_gui_key_event(gui, gui->cur_key);
				gui->key_step = gui->key_step-0.35;
				gui->key_repeat = ELF_TRUE;
			}
		}
	}

	if(gui->cur_char)
	{
		gui->char_step += step;
		if(gui->char_repeat)
		{
			if(gui->char_step >= 0.05)
			{
				elf_send_gui_char_event(gui, gui->cur_char);
				gui->char_step = gui->char_step-0.05;
			}
		}
		else
		{
			if(gui->char_step >= 0.35)
			{
				elf_send_gui_char_event(gui, gui->cur_char);
				gui->char_step = gui->char_step-0.35;
				gui->char_repeat = ELF_TRUE;
			}
		}
	}

	for(i = 0; i < elf_get_event_count(); i++)
	{
		event = elf_get_event(i);
		if(event->type == ELF_CHAR_EVENT)
		{
			if(((elf_char_event*)event)->state == ELF_TRUE)
			{
				elf_send_gui_char_event(gui, (char)((elf_char_event*)event)->code);

				gui->cur_char = (char)((elf_char_event*)event)->code;
				gui->char_step = 0.0;
			}
			else
			{
				gui->cur_char = 0;
				gui->char_step = 0.0;
				gui->char_repeat = ELF_FALSE;
			}
		}
		if(event->type == ELF_KEY_EVENT)
		{
			if(((elf_key_event*)event)->state == ELF_TRUE)
			{
				elf_send_gui_key_event(gui, ((elf_key_event*)event)->key);

				gui->cur_key = ((elf_key_event*)event)->key;
				gui->key_step = 0.0;
			}
			else
			{
				gui->cur_key = 0;
				gui->key_step = 0.0;
				gui->key_repeat = ELF_FALSE;
			}
		}
	}
}

void elf_draw_gui(elf_gui *gui)
{
	elf_gui_object *object;
	elf_area area;

	area.pos.x = gui->pos.x;
	area.pos.y = gui->pos.y;
	area.size.x = gui->width;
	area.size.y = gui->height;

	gfx_set_shader_params_default(&gui->shader_params);
	gui->shader_params.render_params.depth_write = GFX_FALSE;
	gui->shader_params.render_params.depth_test = GFX_FALSE;
	gui->shader_params.render_params.blend_mode = GFX_TRANSPARENT;
	gfx_set_viewport(gui->pos.x, gui->pos.y, gui->width, gui->height);
	gfx_get_orthographic_projection_matrix((float)gui->pos.x, (float)gui->width, (float)gui->pos.x, (float)gui->height,
		-1.0f, 1.0f, gui->shader_params.projection_matrix);

	for(object = (elf_gui_object*)elf_begin_list(gui->children); object;
		object = (elf_gui_object*)elf_next_in_list(gui->children))
	{
		if(object->type == ELF_LABEL) elf_draw_label((elf_label*)object, &gui->shader_params);
		else if(object->type == ELF_BUTTON) elf_draw_button((elf_button*)object, &gui->shader_params);
		else if(object->type == ELF_PICTURE) elf_draw_picture((elf_picture*)object, &gui->shader_params);
		else if(object->type == ELF_TEXT_FIELD)
		{
			elf_draw_text_field((elf_text_field*)object, &area, &gui->shader_params);
			gfx_set_viewport(gui->pos.x, gui->pos.y, gui->width, gui->height);
			gfx_get_orthographic_projection_matrix((float)gui->pos.x, (float)gui->pos.x+gui->width, (float)gui->pos.y, (float)gui->pos.y+gui->height,
				-1.0f, 1.0f, gui->shader_params.projection_matrix);
		}
		else if(object->type == ELF_TEXT_LIST)
		{
			elf_draw_text_list((elf_text_list*)object, &area, &gui->shader_params);
			gfx_set_viewport(gui->pos.x, gui->pos.y, gui->width, gui->height);
			gfx_get_orthographic_projection_matrix((float)gui->pos.x, (float)gui->pos.x+gui->width, (float)gui->pos.y, (float)gui->pos.y+gui->height,
				-1.0f, 1.0f, gui->shader_params.projection_matrix);
		}
		else if(object->type == ELF_SLIDER) elf_draw_slider((elf_slider*)object, &gui->shader_params);
		else if(object->type == ELF_CHECK_BOX) elf_draw_check_box((elf_check_box*)object, &gui->shader_params);
	}

	for(object = (elf_gui_object*)elf_begin_list(gui->screens); object;
		object = (elf_gui_object*)elf_next_in_list(gui->screens))
	{
		area.pos.x = gui->pos.x; area.pos.y = gui->pos.y; area.size.x = gui->width; area.size.y = gui->height;
		elf_draw_screen((elf_screen*)object, &area, &gui->shader_params);
		gfx_set_viewport(gui->pos.x, gui->pos.y, gui->width, gui->height);
		gfx_get_orthographic_projection_matrix((float)gui->pos.x, (float)gui->pos.x+gui->width, (float)gui->pos.y, (float)gui->pos.y+gui->height,
			-1.0f, 1.0f, gui->shader_params.projection_matrix);
	}

	// reset state just to be sure...
	gfx_set_shader_params_default(&gui->shader_params);
	gfx_set_shader_params(&gui->shader_params);
}

unsigned char elf_add_gui_object(elf_gui_object *parent, elf_gui_object *object)
{
	if(parent->type != ELF_GUI && parent->type != ELF_SCREEN) return ELF_FALSE;
	if(object->parent) elf_remove_gui_object_by_object(object->parent, object);
	elf_set_gui_object_root(object, parent->root);
	object->parent = parent;
	elf_recalc_gui_object(object);
	if(object->type != ELF_SCREEN) elf_append_to_list(parent->children, (elf_object*)object);
	else elf_append_to_list(parent->screens, (elf_object*)object);
	return ELF_TRUE;
}

elf_gui_object* elf_get_gui_object_by_name(elf_gui_object *parent, const char *name)
{
	elf_gui_object *object;

	if(!strcmp(parent->name, name)) return parent;

	if(parent->children)
	{
		for(object = (elf_gui_object*)elf_begin_list(parent->children); object;
			object = (elf_gui_object*)elf_next_in_list(parent->children))
		{
			elf_get_gui_object_by_name(object, name);
		}
	}

	if(parent->screens)
	{
		for(object = (elf_gui_object*)elf_begin_list(parent->screens); object;
			object = (elf_gui_object*)elf_next_in_list(parent->screens))
		{
			elf_get_gui_object_by_name(object, name);
		}
	}

	return NULL;
}

elf_gui_object* elf_get_gui_object_by_index(elf_gui_object *parent, int idx)
{
	elf_gui_object *object;
	int i;

	if(parent->type != ELF_GUI && parent->type == ELF_SCREEN) return ELF_FALSE;

	if(idx < 0 || idx > elf_get_list_length(parent->children)+elf_get_list_length(parent->screens)-1) return ELF_FALSE;

	if(idx < elf_get_list_length(parent->children))
	{
		for(i = 0, object = (elf_gui_object*)elf_begin_list(parent->children); object;
			i++, object = (elf_gui_object*)elf_next_in_list(parent->children))
		{
			if(i == idx) return object;
		}
	}
	else
	{
		idx -= elf_get_list_length(parent->children);
		for(i = 0, object = (elf_gui_object*)elf_begin_list(parent->screens); object;
			i++, object = (elf_gui_object*)elf_next_in_list(parent->screens))
		{
			if(i == idx) return object;
		}
	}

	return ELF_FALSE;
}

unsigned char elf_remove_gui_object_by_name(elf_gui_object *parent, const char *name)
{
	elf_gui_object *object;

	if(parent->type != ELF_GUI && parent->type != ELF_SCREEN) return ELF_FALSE;

	for(object = (elf_gui_object*)elf_begin_list(parent->children); object;
		object = (elf_gui_object*)elf_next_in_list(parent->children))
	{
		if(!strcmp(object->name, name))
		{
			if(object->root)
			{
				if(object->root->trace == object) object->root->trace = NULL;
				if(object->root->target == object) object->root->target = NULL;
			}
			object->parent = NULL;
			elf_clear_gui_object_root(object);
			elf_remove_from_list(parent->children, (elf_object*)object);
			return ELF_TRUE;
		}
	}

	for(object = (elf_gui_object*)elf_begin_list(parent->screens); object;
		object = (elf_gui_object*)elf_next_in_list(parent->screens))
	{
		if(!strcmp(object->name, name))
		{
			if(object->root)
			{
				if(object->root->trace == object) object->root->trace = NULL;
				if(object->root->target == object) object->root->target = NULL;
			}
			object->parent = NULL;
			elf_clear_gui_object_root(object);
			elf_remove_from_list(parent->children, (elf_object*)object);
			return ELF_TRUE;
		}
	}

	return ELF_FALSE;
}

unsigned char elf_remove_gui_object_by_index(elf_gui_object *parent, int idx)
{
	elf_gui_object *object;
	int i;

	if(parent->type != ELF_GUI && parent->type == ELF_SCREEN) return ELF_FALSE;

	if(idx < 0 || idx > elf_get_list_length(parent->children)+elf_get_list_length(parent->screens)-1) return ELF_FALSE;

	if(idx < elf_get_list_length(parent->children))
	{
		for(i = 0, object = (elf_gui_object*)elf_begin_list(parent->children); object;
			i++, object = (elf_gui_object*)elf_next_in_list(parent->children))
		{
			if(i == idx)
			{
				if(object->root)
				{
					if(object->root->trace == object) object->root->trace = NULL;
					if(object->root->target == object) object->root->target = NULL;
					if(object->root->active_text_field == (elf_text_field*)object)
						object->root->active_text_field = NULL;
				}
				object->parent = NULL;
				elf_clear_gui_object_root(object);
				elf_remove_from_list(parent->children, (elf_object*)object);
				return ELF_TRUE;
			}
		}
	}
	else
	{
		idx -= elf_get_list_length(parent->children);
		for(i = 0, object = (elf_gui_object*)elf_begin_list(parent->screens); object;
			i++, object = (elf_gui_object*)elf_next_in_list(parent->screens))
		{
			if(i == idx)
			{
				if(object->root)
				{
					if(object->root->trace == object) object->root->trace = NULL;
					if(object->root->target == object) object->root->target = NULL;
					if(object->root->active_text_field == (elf_text_field*)object)
						object->root->active_text_field = NULL;
				}
				object->parent = NULL;
				elf_clear_gui_object_root(object);
				elf_remove_from_list(parent->children, (elf_object*)object);
				return ELF_TRUE;
			}
		}
	}

	return ELF_FALSE;
}

unsigned char elf_remove_gui_object_by_object(elf_gui_object *parent, elf_gui_object *object)
{
	if(parent->type != ELF_GUI && parent->type != ELF_SCREEN) return ELF_FALSE;
	
	if(object->root)
	{
		if(object->root->trace == object) object->root->trace = NULL;
		if(object->root->target == object) object->root->target = NULL;
		if(object->root->active_text_field == (elf_text_field*)object)
			object->root->active_text_field = NULL;
	}
	object->parent = NULL;
	elf_clear_gui_object_root(object);
	if(object->type != ELF_SCREEN) return elf_remove_from_list(parent->children, (elf_object*)object);
	else return elf_remove_from_list(parent->screens, (elf_object*)object);
}

void elf_empty_gui(elf_gui *gui)
{
	elf_dec_ref((elf_object*)gui->children);
	elf_dec_ref((elf_object*)gui->screens);
	gui->children = elf_create_list();
	gui->screens = elf_create_list();
	elf_inc_ref((elf_object*)gui->children);
	elf_inc_ref((elf_object*)gui->screens);
}

