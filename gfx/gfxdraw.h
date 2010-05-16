
void gfx_draw_2d_quad(float x, float y, float width, float height)
{
	float *vertex_buffer;

	vertex_buffer = (float*)gfx_get_vertex_data_buffer(driver->quad_vertex_data);

	vertex_buffer[0] = x;
	vertex_buffer[1] = y+height;
	vertex_buffer[2] = 0.0;
	vertex_buffer[3] = x;
	vertex_buffer[4] = y;
	vertex_buffer[5] = 0.0;
	vertex_buffer[6] = x+width;
	vertex_buffer[7] = y+height;
	vertex_buffer[8] = 0.0;
	vertex_buffer[9] = x+width;
	vertex_buffer[10] = y;
	vertex_buffer[11] = 0.0;

	gfx_update_vertex_data(driver->quad_vertex_data);
	gfx_draw_vertex_array(driver->quad_vertex_array, 4, GFX_TRIANGLE_STRIP);
}

void gfx_draw_textured_2d_quad(float x, float y, float width, float height)
{
	float *vertex_buffer;
	float *tex_coord_buffer;

	vertex_buffer = (float*)gfx_get_vertex_data_buffer(driver->quad_vertex_data);
	tex_coord_buffer = (float*)gfx_get_vertex_data_buffer(driver->quad_tex_coord_data);

	vertex_buffer[0] = x;
	vertex_buffer[1] = y+height;
	vertex_buffer[2] = 0.0;
	vertex_buffer[3] = x;
	vertex_buffer[4] = y;
	vertex_buffer[5] = 0.0;
	vertex_buffer[6] = x+width;
	vertex_buffer[7] = y+height;
	vertex_buffer[8] = 0.0;
	vertex_buffer[9] = x+width;
	vertex_buffer[10] = y;
	vertex_buffer[11] = 0.0;
	tex_coord_buffer[0] = 0.0;
	tex_coord_buffer[1] = 1.0;
	tex_coord_buffer[2] = 0.0;
	tex_coord_buffer[3] = 0.0;
	tex_coord_buffer[4] = 1.0;
	tex_coord_buffer[5] = 1.0;
	tex_coord_buffer[6] = 1.0;
	tex_coord_buffer[7] = 0.0;

	gfx_update_vertex_data(driver->quad_vertex_data);
	gfx_update_vertex_data(driver->quad_tex_coord_data);
	gfx_draw_vertex_array(driver->quad_vertex_array, 4, GFX_TRIANGLE_STRIP);
}

void gfx_draw_textured_2d_quad_region(float x, float y, float width, float height, float tx, float ty, float twidth, float theight)
{
	float *vertex_buffer;
	float *tex_coord_buffer;

	vertex_buffer = (float*)gfx_get_vertex_data_buffer(driver->quad_vertex_data);
	tex_coord_buffer = (float*)gfx_get_vertex_data_buffer(driver->quad_tex_coord_data);

	vertex_buffer[0] = x;
	vertex_buffer[1] = y+height;
	vertex_buffer[2] = 0.0;
	vertex_buffer[3] = x;
	vertex_buffer[4] = y;
	vertex_buffer[5] = 0.0;
	vertex_buffer[6] = x+width;
	vertex_buffer[7] = y+height;
	vertex_buffer[8] = 0.0;
	vertex_buffer[9] = x+width;
	vertex_buffer[10] = y;
	vertex_buffer[11] = 0.0;
	tex_coord_buffer[0] = tx;
	tex_coord_buffer[1] = ty+theight;
	tex_coord_buffer[2] = tx;
	tex_coord_buffer[3] = ty;
	tex_coord_buffer[4] = tx+twidth;
	tex_coord_buffer[5] = ty+theight;
	tex_coord_buffer[6] = tx+twidth;
	tex_coord_buffer[7] = ty;

	gfx_update_vertex_data(driver->quad_vertex_data);
	gfx_update_vertex_data(driver->quad_tex_coord_data);
	gfx_draw_vertex_array(driver->quad_vertex_array, 4, GFX_TRIANGLE_STRIP);
}

void gfx_draw_bounding_box(float min[3], float max[3])
{
	float *vertex_buffer;

	vertex_buffer = (float*)gfx_get_vertex_data_buffer(driver->bb_vertex_data);

	vertex_buffer[0] = min[0];
	vertex_buffer[1] = max[1];
	vertex_buffer[2] = max[2];
	vertex_buffer[3] = min[0];
	vertex_buffer[4] = max[1];
	vertex_buffer[5] = min[2];
	vertex_buffer[6] = min[0];
	vertex_buffer[7] = min[1];
	vertex_buffer[8] = max[2];
	vertex_buffer[9] = min[0];
	vertex_buffer[10] = min[1];
	vertex_buffer[11] = min[2];
	vertex_buffer[12] = max[0];
	vertex_buffer[13] = min[1];
	vertex_buffer[14] = max[2];
	vertex_buffer[15] = max[0];
	vertex_buffer[16] = min[1];
	vertex_buffer[17] = min[2];
	vertex_buffer[18] = max[0];
	vertex_buffer[19] = max[1];
	vertex_buffer[20] = max[2];
	vertex_buffer[21] = max[0];
	vertex_buffer[22] = max[1];
	vertex_buffer[23] = min[2];

	gfx_update_vertex_data(driver->bb_vertex_data);
	gfx_set_vertex_array(driver->bb_vertex_array);
	gfx_draw_vertex_index(driver->bb_vertex_index, GFX_TRIANGLES);
}

void gfx_draw_lines(int count, gfx_vertex_data *vertices)
{
	if(count < 2) return;
	if(count > gfx_get_vertex_data_count(vertices)/3) count -= count-(gfx_get_vertex_data_count(vertices)/3);

	gfx_set_vertex_array_data(driver->line_vertex_array, GFX_VERTEX, vertices);
	gfx_draw_vertex_array(driver->line_vertex_array, count, GFX_LINES);
}

float gfx_get_absolute_value(float val)
{
	if(val < 0.0) return -val;
	return val;
}

unsigned char gfx_is_about_same(float v1, float v2)
{
	if(gfx_get_absolute_value(v1-v2) < 0.0001) return GFX_TRUE;
	return GFX_FALSE;
}

void gfx_draw_circle(int vertices, float size)
{
	float step;
	int i;
	float *vertex_buffer;

	if(vertices < 3) return;
	if(vertices > GFX_MAX_CIRCLE_VERTICES) vertices = GFX_MAX_CIRCLE_VERTICES;

	if(vertices != driver->prev_circle_vertice_count || !gfx_is_about_same(size, driver->prev_circle_size))
	{
		step = (360.0/((float)vertices))*GFX_PI_DIV_180;

		vertex_buffer = gfx_get_vertex_data_buffer(driver->circle_vertex_data);

		vertex_buffer[0] = 0.0;
		vertex_buffer[1] = 0.0;
		vertex_buffer[2] = 0.0;

		for(i = 1; i < vertices+2; i++)
		{
			vertex_buffer[i*3] = -((float)sin((float)(step*(i-1))))*size;
			vertex_buffer[i*3+1] = ((float)cos((float)(step*(i-1))))*size;
			vertex_buffer[i*3+2] = 0.0;
		}
	}

	gfx_update_vertex_data(driver->circle_vertex_data);
	gfx_draw_vertex_array(driver->circle_vertex_array, vertices+2, GFX_TRIANGLE_FAN);

	driver->prev_circle_vertice_count = vertices;
}

