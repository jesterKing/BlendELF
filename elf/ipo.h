
elf_bezier_point* elf_create_bezier_point()
{
	elf_bezier_point *point;

	point = (elf_bezier_point*)malloc(sizeof(elf_bezier_point));
	memset(point, 0x0, sizeof(elf_bezier_point));
	point->type = ELF_BEZIER_POINT;

	elf_inc_obj_count();

	return point;
}

void elf_destroy_bezier_point(elf_bezier_point *point)
{
	free(point);

	elf_dec_obj_count();
}

elf_bezier_curve* elf_create_bezier_curve()
{
	elf_bezier_curve *curve;

	curve = (elf_bezier_curve*)malloc(sizeof(elf_bezier_curve));
	memset(curve, 0x0, sizeof(elf_bezier_curve));
	curve->type = ELF_BEZIER_CURVE;

	curve->points = elf_create_list();
	elf_inc_ref((elf_object*)curve->points);

	elf_inc_obj_count();

	return curve;
}

void elf_destroy_bezier_curve(elf_bezier_curve *curve)
{
	elf_dec_ref((elf_object*)curve->points);

	free(curve);

	elf_dec_obj_count();
}

void elf_add_point_to_bezier_curve(elf_bezier_curve *curve, elf_bezier_point *point)
{
	int i;
	elf_bezier_point *pnt;

	for(i = 0, pnt = (elf_bezier_point*)elf_begin_list(curve->points); pnt;
		pnt = (elf_bezier_point*)elf_next_in_list(curve->points), i++)
	{
		if(pnt->p.x > point->p.x)
		{
			elf_insert_to_list(curve->points, i, (elf_object*)point);
			return;
		}
	}

	elf_append_to_list(curve->points, (elf_object*)point);
}

float elf_get_bezier_curve_value(elf_bezier_curve *curve, float x)
{
	elf_bezier_point *pnt;
	elf_bezier_point *point1 = NULL;
	elf_bezier_point *point2 = NULL;
	float t;

	for(pnt = (elf_bezier_point*)elf_begin_list(curve->points); pnt;
		pnt = (elf_bezier_point*)elf_next_in_list(curve->points))
	{
		if(pnt->p.x > x)
		{
			point2 = pnt;
			break;
		}
		point1 = pnt;
	}

	if(!point1 && !point2) return 0.0;
	if(!point2) return point1->p.y;

	t = (x-point1->p.x)/(point2->p.x-point1->p.x);
	return point1->p.y+(point2->p.y-point1->p.y)*t;
}

elf_ipo* elf_create_ipo()
{
	elf_ipo *ipo;

	ipo = (elf_ipo*)malloc(sizeof(elf_ipo));
	memset(ipo, 0x0, sizeof(elf_ipo));
	ipo->type = ELF_IPO;

	ipo->curves = elf_create_list();
	elf_inc_ref((elf_object*)ipo->curves);

	elf_inc_obj_count();

	return ipo;
}

void elf_destroy_ipo(elf_ipo *ipo)
{
	elf_dec_ref((elf_object*)ipo->curves);

	free(ipo);

	elf_dec_obj_count();
}

unsigned char elf_add_curve_to_ipo(elf_ipo *ipo, elf_bezier_curve *curve)
{
	elf_bezier_curve *cur;

	for(cur = (elf_bezier_curve*)elf_begin_list(ipo->curves); cur;
		cur = (elf_bezier_curve*)elf_next_in_list(ipo->curves))
	{
		if(cur->curve_type == curve->curve_type) return ELF_FALSE;
	}

	elf_append_to_list(ipo->curves, (elf_object*)curve);

	if(curve->curve_type >= ELF_LOC_X && curve->curve_type <= ELF_LOC_Z) ipo->loc = ELF_TRUE;
	if(curve->curve_type >= ELF_ROT_X && curve->curve_type <= ELF_ROT_Z) ipo->rot = ELF_TRUE;
	if(curve->curve_type >= ELF_SCALE_X && curve->curve_type <= ELF_SCALE_Z) ipo->scale = ELF_TRUE;
	if(curve->curve_type >= ELF_QUA_X && curve->curve_type <= ELF_QUA_W) ipo->qua = ELF_TRUE;

	return ELF_TRUE;
}

elf_vec3f elf_get_ipo_loc(elf_ipo *ipo, float x)
{
	elf_bezier_curve *curve;
	elf_vec3f result;

	memset(&result, 0x0, sizeof(elf_vec3f));

	for(curve = (elf_bezier_curve*)elf_begin_list(ipo->curves); curve;
		curve = (elf_bezier_curve*)elf_next_in_list(ipo->curves))
	{
		if(curve->curve_type == ELF_LOC_X) result.x = elf_get_bezier_curve_value(curve, x);
		else if(curve->curve_type == ELF_LOC_Y) result.y = elf_get_bezier_curve_value(curve, x);
		else if(curve->curve_type == ELF_LOC_Z) result.z = elf_get_bezier_curve_value(curve, x);
	}

	return result;
}

elf_vec3f elf_get_ipo_rot(elf_ipo *ipo, float x)
{
	elf_bezier_curve *curve;
	elf_vec3f result;

	memset(&result, 0x0, sizeof(elf_vec3f));

	for(curve = (elf_bezier_curve*)elf_begin_list(ipo->curves); curve;
		curve = (elf_bezier_curve*)elf_next_in_list(ipo->curves))
	{
		if(curve->curve_type == ELF_ROT_X) result.x = elf_get_bezier_curve_value(curve, x);
		else if(curve->curve_type == ELF_ROT_Y) result.y = elf_get_bezier_curve_value(curve, x);
		else if(curve->curve_type == ELF_ROT_Z) result.z = elf_get_bezier_curve_value(curve, x);
	}

	return result;
}

elf_vec3f elf_get_ipo_scale(elf_ipo *ipo, float x)
{
	elf_bezier_curve *curve;
	elf_vec3f result;

	memset(&result, 0x0, sizeof(elf_vec3f));

	for(curve = (elf_bezier_curve*)elf_begin_list(ipo->curves); curve;
		curve = (elf_bezier_curve*)elf_next_in_list(ipo->curves))
	{
		if(curve->curve_type == ELF_SCALE_X) result.x = elf_get_bezier_curve_value(curve, x);
		else if(curve->curve_type == ELF_SCALE_Y) result.y = elf_get_bezier_curve_value(curve, x);
		else if(curve->curve_type == ELF_SCALE_Z) result.z = elf_get_bezier_curve_value(curve, x);
	}

	return result;
}

elf_vec4f elf_get_ipo_qua(elf_ipo *ipo, float x)
{
	elf_bezier_curve *curve;
	elf_vec4f result;

	memset(&result, 0x0, sizeof(elf_vec4f));

	for(curve = (elf_bezier_curve*)elf_begin_list(ipo->curves); curve;
		curve = (elf_bezier_curve*)elf_next_in_list(ipo->curves))
	{
		if(curve->curve_type == ELF_QUA_X) result.x = elf_get_bezier_curve_value(curve, x);
		else if(curve->curve_type == ELF_QUA_Y) result.y = elf_get_bezier_curve_value(curve, x);
		else if(curve->curve_type == ELF_QUA_Z) result.z = elf_get_bezier_curve_value(curve, x);
		else if(curve->curve_type == ELF_QUA_W) result.w = elf_get_bezier_curve_value(curve, x);
	}

	return result;
}

