
gfx_query *gfx_create_query()
{
	gfx_query *query;

	query = (gfx_query*)malloc(sizeof(gfx_query));
	memset(query, 0x0, sizeof(gfx_query));

	glGenQueries(1, &query->id);

	return query;
}

void gfx_destroy_query(gfx_query *query)
{
	glDeleteQueries(1, &query->id);
	free(query);
}

void gfx_begin_query(gfx_query *query)
{
	glBeginQuery(GL_SAMPLES_PASSED, query->id);
}

void gfx_end_query(gfx_query *query)
{
	glEndQuery(GL_SAMPLES_PASSED);
}

unsigned char gfx_is_query_result(gfx_query *query)
{
	int result;

	result = 0;
	glGetQueryObjectiv(query->id, GL_QUERY_RESULT_AVAILABLE, &result);

	return result == GL_TRUE;
}

int gfx_get_query_result(gfx_query *query)
{
	int result;

	result = 0;
	glGetQueryObjectiv(query->id, GL_QUERY_RESULT, &result);

	return result;
}
