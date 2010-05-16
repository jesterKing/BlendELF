#!/usr/bin/python

decls = """#if defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__)
	#ifndef ELF_PLAYER
		#define ELF_APIENTRY __stdcall
		#ifdef ELF_DLL
				#define ELF_API __declspec(dllexport)
		#else
				#define ELF_API __declspec(dllimport)
		#endif
		#include "windows.h"
		ELF_API bool ELF_APIENTRY elfInitWithHWND(int width, int height, const char* title, bool fullscreen, HWND hwnd);
		ELF_API HWND ELF_APIENTRY elfGetWindowHWND();
	#else
		#define ELF_API
		#define ELF_APIENTRY
	#endif
#else
	#define ELF_API
	#define ELF_APIENTRY
#endif
typedef struct elf_object		elf_object;
class ELF_API elf_handle {
	elf_object *obj;
public:
	elf_handle();
	elf_handle(elf_object *object);
	elf_handle(const elf_handle &handle);
	~elf_handle();

	elf_object* get() {return obj;}

	elf_handle& operator=(elf_object *object);
	elf_handle& operator=(elf_handle &handle);
	elf_handle& operator=(const elf_handle &handle);
};
#ifndef BLENDELF_H
extern "C" {
struct elf_vec2i {
	int x;
	int y;
};
struct elf_vec2f {
	float x;
	float y;
};
struct elf_vec3f {
	float x;
	float y;
	float z;
};
struct elf_vec4f {
	float x;
	float y;
	float z;
	float w;
};
struct elf_color {
	float r;
	float g;
	float b;
	float a;
};
}
#endif
ELF_API bool ELF_APIENTRY elfIsObject(elf_handle obj);
"""

defs = """#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <AL/alc.h>
#include <AL/al.h>

#include "gfx.h"
#include "blendelf.h"
#include "binds.h"

#if defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__)
	#ifndef ELF_PLAYER
ELF_API bool ELF_APIENTRY elfInitWithHWND(int width, int height, const char* title, bool fullscreen, HWND hwnd)
{
	return elf_init_with_hwnd(width, height, title, fullscreen, hwnd);
}
ELF_API HWND ELF_APIENTRY elfGetWindowHWND()
{
	return elf_get_window_hwnd();
}
	#endif
#endif

ELF_API elf_handle::elf_handle()
{
	obj = NULL;
}
elf_handle::elf_handle(elf_object *object)
{
	if(object) elf_inc_ref(object);
	obj = object;
}
ELF_API elf_handle::elf_handle(const elf_handle &handle)
{
	if(handle.obj) elf_inc_ref(handle.obj);
	obj = handle.obj;
}
ELF_API elf_handle::~elf_handle()
{
	if(obj) elf_dec_ref(obj);
}
elf_handle& elf_handle::operator=(elf_object *object)
{
	if(obj)
	{
		elf_dec_ref(obj);
		obj = NULL;
	}

	if(object) elf_inc_ref(object);
	obj = object;

	return *this;
}
ELF_API elf_handle& elf_handle::operator=(elf_handle &handle)
{
	if(obj)
	{
		elf_dec_ref(obj);
		obj = NULL;
	}

	if(handle.obj) elf_inc_ref(handle.obj);
	obj = handle.obj;

	return *this;
}
ELF_API elf_handle& elf_handle::operator=(const elf_handle &handle)
{
	if(obj)
	{
		elf_dec_ref(obj);
		obj = NULL;
	}

	if(handle.obj) elf_inc_ref(handle.obj);
	obj = handle.obj;

	return *this;
}\n
ELF_API bool ELF_APIENTRY elfIsObject(elf_handle obj)
{
	if(obj.get()) return true;
	return false;
}
"""
apiheader = """<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
<title>BlendELF, Lua API doc</title>
<link rel="stylesheet" type="text/css" href="style.css" />
<meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
<meta name="description" content="BlendELF, Compact and powerful 3D Game Engine" />
</head>
<body>

<div class="images">
<img src="images/topbanner.jpg" alt="Top Banner" />
</div>

<div class="links">
<a href="index.html">Home</a> | 
<a href="http://forum.blendelf.com">Forums</a> | 
<a href="http://blendelf.com/index.html#Demos">Demos</a> | 
<a href="http://blendelf.com/index.html#Features">Features</a> | 
<a href="http://blendelf.com/index.html#Devkit">Get Development Kit</a> | 
<a href="http://blendelf.com/index.html#Documentation">Documentation</a> | 
<a href="http://blendelf.com/index.html#License">License</a> | 
<a href="http://blendelf.com/index.html#IRC">IRC</a> | 
<a href="http://blendelf.com/index.html#Contact">Contact</a>
</div>

<div class="topic">BlendELF Lua API doc</div>
<div class="info">
This document contains all of the avaialable functions for the BlendELF Lua API.
</div>

<div class="apitopic">BlendELF TYPES</div>
<div class="apiinfo">
<span class="apikeytype">float</span> - a floating point value, like 0.25 or 7.24
<br /><span class="apikeytype">double</span> - same as a floating poit value but can store more decimals
<br /><span class="apikeytype">int</span> - a positive or negative integer value, like 40 or -2500
<br /><span class="apikeytype">unsigned int</span> - a positive integer value, like 21 or 60
<br /><span class="apikeytype">bool</span> - a truth value, like true or false
<br /><span class="apikeytype">string</span> - a string of characters, like "cat" or "MyEntity"
<br /><span class="apikeytype">elf_vec2i</span> - contains the integer attributes [x|y] which can be accessed by vec.x and vec.y
<br /><span class="apikeytype">elf_vec2f</span> - contains the floating point attributes [x|y] which can be accessed by vec.x and vec.y
<br /><span class="apikeytype">elf_vec3f</span> - contains the floating point attributes [x|y|z] which can be accessed by vec.x, vec.y and vec.z
<br /><span class="apikeytype">elf_vec4f</span> - contains the floating point attributes [x|y|z|w] which can be accessed by vec.x, vec.y, vec.z and vec.w
<br /><span class="apikeytype">elf_color</span> - contains the floating point attributes [r|g|b|a] which can be accessed by col.r, col.g, col.b and col.a
<br /><span class="apiobjtype">object</span> - any BlendELF object other than the types listed above
</div>

<div class="apitopic">BUILT IN VARIABLES</div>
<div class="apiinfo">
The variables <b>scn</b>, <b>gui</b> and <b>event</b> are used for internal operations and you can freely access from them, but <b>NOT ASSIGN</b> anything to them.
</div>
"""

apifooter = """<div class="copyright">Copyright &copy; 2009-2010 Samuel Anjam</div>

<script type="text/javascript">
var gaJsHost = (("https:" == document.location.protocol) ? "https://ssl." : "http://www.");
document.write(unescape("%3Cscript src='" + gaJsHost + "google-analytics.com/ga.js' type='text/javascript'%3E%3C/script%3E"));
</script>
<script type="text/javascript">
try {
var pageTracker = _gat._getTracker("UA-4880379-2");
pageTracker._trackPageview();
} catch(err) {}</script>

</body>
</html>"""

pretypes = ['unsigned',
	'signed',
	'const']

types = ['void',
	'char',
	'int',
	'float',
	'double',
	'unsigned int',
	'const char*',
	'bool']

elftypes = ['elf_vec2i',
	'elf_vec2f',
	'elf_vec3f',
	'elf_vec4f',
	'elf_color']

elfgenobjs = ['elf_object',
	'elf_actor',
	'elf_gui_object']

elfobjs = ['elf_object',
	'elf_list',
	'elf_handle',
	'elf_script',
	'elf_actor',
	'elf_camera',
	'elf_texture',
	'elf_material',
	'elf_model',
	'elf_entity',
	'elf_light',
	'elf_bone',
	'elf_armature',
	'elf_scene',
	'elf_collision',
	'elf_joint',
	'elf_font',
	'elf_gui_object',
	'elf_gui',
	'elf_label',
	'elf_button',
	'elf_picture',
	'elf_text_field',
	'elf_slider',
	'elf_screen',
	'elf_text_list',
	'elf_check_box',
	'elf_timer',
	'elf_sst',
	'elf_resources',
	'elf_particle',
	'elf_particles',
	'elf_game_config',
	'elf_bezier_point',
	'elf_bezier_curve',
	'elf_ipo',
	'elf_frame_player',
	'elf_sound',
	'elf_audio_source',
	'elf_property']

defines = []
functions = []

class Define:
	def __init__(self):
		self.name = ''
		self.value = ''
		self.doc = ''
		self.mdoc = ''
		self.mdocc = ''

class Parameter:
	def __init__(self):
		self.type = ''
		self.orig_type = ''
		self.name = ''

class Function:
	def __init__(self):
		self.type = ''
		self.orig_type = ''
		self.name = ''
		self.orig_name = ''
		self.params = []
		self.doc = ''
		self.mdoc = ''
		self.mdocc = ''
	def dump(self):
		dstr = self.type+'['+self.orig_type+'] '+self.name+'['+self.orig_name+']('
		for param in self.params:
			dstr += param.type+'['+param.orig_type+'] '+param.name+', '
		dstr += ')'
		print dstr

def get_parts(line):
	tstr = ''
	parts = []
	for c in line:
		if c in ' \t\n#();*,':
			if len(tstr) > 0: parts.append(tstr)
			tstr = ''
			continue
		tstr += c
	return parts

def get_name_in_caps(name):
	nname = ''
	start = True
	for c in name:
		if start == True:
			nname += c.upper()
			start = False
		else:
			if c == '_':
				start = True
				continue
			else:
				nname += c
	return nname

def get_doc(obj, parts):
	next = 0
	readdoc = False
	readmdoc = False
	readmdocc = False

	while next < len(parts):
		if parts[next] == '<doc>': readdoc = True; readmdoc = False; readmdocc = False
		elif parts[next] == '<mdoc>': readmdoc = True; readdoc = False; readmdocc = False
		elif parts[next] == '<mdocc>': readmdocc = True; readdoc = False; readmdoc = False
		else:
			if readdoc:
				if len(obj.doc) > 0: obj.doc += ' '+parts[next]
				else: obj.doc = parts[next]
			elif readmdoc:
				if len(obj.mdoc) > 0: obj.mdoc += ' '+parts[next]
				else: obj.mdoc = parts[next]
			elif readmdocc:
				if len(obj.mdocc) > 0: obj.mdocc += ' '+parts[next]
				else: obj.mdocc = parts[next]
		next += 1

def get_define(parts):
	define = Define()
	define.name = parts[1][4:]
	define.value = parts[2]

	get_doc(define, parts)

	return define

def get_function(parts):
	func = Function()

	next = 0;
	if parts[0] in pretypes:
		func.type = parts[0] + ' ' + parts[1]
		if func.type == 'const char': func.type += '*'
		if func.type == 'unsigned char': func.type = 'bool'
		func.orig_type = parts[0] + ' ' + parts[1]
		next = 2
	elif parts[0] in types or parts[0] in elftypes:
		func.type = parts[0]
		func.orig_type = parts[0]
		next = 1
	elif parts[0] in elfobjs:
		func.type = 'elf_handle'
		func.orig_type = parts[0]
		next = 1
	else: return None

	func.name = get_name_in_caps(parts[next])[3:]
	func.orig_name = parts[next]
	next += 1

	while next < len(parts):
		param = Parameter()
		if parts[next] in pretypes:
			param.type = parts[next] + ' ' + parts[next+1]
			if param.type == 'const char': param.type += '*'
			if param.type == 'unsigned char': param.type = 'bool'
			param.orig_type = parts[next] + ' ' + parts[next+1]
			next += 2
		elif parts[next] in types or parts[next] in elftypes:
			param.type = parts[next]
			param.orig_type = parts[next]
			next += 1
		elif parts[next] in elfobjs:
			param.type = 'elf_handle'
			param.orig_type = parts[next]
			next += 1
		elif parts[next] == '//': break;
		else: return None

		param.name = parts[next]
		next += 1

		func.params.append(param)

	get_doc(func, parts)

	return func

src = open('elf/blendelf.h')

skip = False
for line in src:
	parts = get_parts(line)
	if '<!>' in parts: continue
	if '<!!' in parts: skip = True
	if '!!>' in parts: skip = False
	if skip == True: continue
	if len(parts) > 0:
		if parts[0] == 'define':
			define = get_define(parts)
			defines.append(define)
		if parts[0] in pretypes or parts[0] in types or parts[0] in elftypes or parts[0] in elfobjs:
			func = get_function(parts)
			if func != None: functions.append(func)

src.close()

bindsh = open('elf/binds.h', 'w')
bindsh.write('#ifndef ELF_BINDS_H\n#define ELF_BINDS_H\n')

for define in defines:
	bindsh.write('#define ELF_'+define.name+' '+define.value+'\n')

bindsh.write(decls)

for func in functions:
	bindsh.write('ELF_API '+func.type+' ELF_APIENTRY elf'+func.name+'(')
	for i in range(len(func.params)):
		bindsh.write(func.params[i].type+' '+func.params[i].name)
		if(i < len(func.params)-1): bindsh.write(', ')
	bindsh.write(');\n')

bindsh.write('#endif\n')
bindsh.close()

bindsh = open('elf/binds.h')
bindshtext = bindsh.read()
bindsh.close()
blendelfh = open('cppsdk/blendelf.h', 'w')
blendelfh.write(bindshtext)
blendelfh.close()

bindscpp = open('elf/binds.cpp', 'w')

bindscpp.write(defs)

for func in functions:
	bindscpp.write('ELF_API '+func.type+' ELF_APIENTRY elf'+func.name+'(')
	for i in range(len(func.params)):
		bindscpp.write(func.params[i].type+' '+func.params[i].name)
		if(i < len(func.params)-1): bindscpp.write(', ')
	bindscpp.write(')\n')

	bindscpp.write('{\n')

	if len(func.params) < 1:
		if func.type == 'void':
			bindscpp.write('\t'+func.orig_name+'();\n')
		elif func.type in types or func.type in elftypes:
			if func.orig_type == 'unsigned char':
				bindscpp.write('\treturn (bool)'+func.orig_name+'();\n')
			else:
				bindscpp.write('\treturn '+func.orig_name+'();\n')
		elif func.type in elfobjs:
			bindscpp.write('\telf_handle handle;\n')
			bindscpp.write('\thandle = (elf_object*)'+func.orig_name+'();\n')
			bindscpp.write('\treturn handle;\n')
	else:
		if func.type == 'elf_handle':
			bindscpp.write('\telf_handle handle;\n');
		elif func.type in elftypes:
			bindscpp.write('\t'+func.type+' _e_type;\n');
			bindscpp.write('\tmemset(&_e_type, 0x0, sizeof('+func.type+'));\n')

		for param in func.params:
			if param.orig_type in elfgenobjs or param.orig_type in elfobjs:
				if param.orig_type == 'elf_object': bindscpp.write('\tif(!'+param.name+'.get())\n')
				elif param.orig_type in elfgenobjs: bindscpp.write('\tif(!'+param.name+'.get() || !elf_is_'+param.orig_type[4:]+'('+param.name+'.get()))\n')
				elif param.orig_type in elfobjs: bindscpp.write('\tif(!'+param.name+'.get() || elf_get_object_type('+param.name+'.get()) != '+param.orig_type.upper()+')\n')
				bindscpp.write('\t{\n')
				bindscpp.write('\t\tprintf(\"'+func.name+'() -> invalid handle\\n\");\n')
				if func.type in elfobjs:
					bindscpp.write('\t\treturn handle;\n')
				elif func.type in elftypes:
					bindscpp.write('\t\treturn _e_type;\n')
				elif func.type != 'void':
					if func.type == 'unsigned int' or func.type == 'int' or func.type == 'float' or func.type == 'double':
						bindscpp.write('\t\treturn 0;\n')
					elif func.type == 'bool':
						bindscpp.write('\t\treturn false;\n')
					elif func.type == 'const char*':
						bindscpp.write('\t\treturn \"\";\n')
				else: bindscpp.write('\t\treturn;\n')
				bindscpp.write('\t}\n')

		if func.type in elfobjs:
			bindscpp.write('\thandle = (elf_object*)')
		elif func.type in elftypes:
			bindscpp.write('\t_e_type = ')
		elif func.type != 'void':
			if func.orig_type == 'unsigned char':
				bindscpp.write('\treturn (bool)')
			else:
				bindscpp.write('\treturn ')
		else: bindscpp.write('\t')

		bindscpp.write(func.orig_name+'(')
		for i in range(len(func.params)):
			if func.params[i].orig_type in elfobjs:
				bindscpp.write('('+func.params[i].orig_type+'*)'+func.params[i].name+'.get()')
			else: bindscpp.write(func.params[i].name)
			if(i < len(func.params)-1): bindscpp.write(', ')
		bindscpp.write(');\n')

		if func.type in elfobjs:
			bindscpp.write('\treturn handle;\n')
		elif func.type in elftypes:
			bindscpp.write('\treturn _e_type;\n')			

	bindscpp.write('}\n')

bindscpp.write('\n')
bindscpp.close()

docs = open('docs/api_doc.html', 'w')

docs.write(apiheader)

for define in defines:
	if len(define.mdoc) > 0:
		docs.write('<div class=\"apitopic\">'+define.mdoc+'</div>\n')
	if len(define.mdocc) > 0:
		docs.write('<div class=\"apiinfo\">'+define.mdocc+'</div>\n')
	docs.write('<div class=\"apidefine\">elf.'+define.name+'</div>\n')

for func in functions:
	if len(func.mdoc) > 0:
		docs.write('<div class=\"apitopic\">'+func.mdoc+'</div>\n')
	if len(func.mdocc) > 0:
		docs.write('<div class=\"apiinfo\">'+func.mdocc+'</div>\n')
	docs.write('<div class=\"apifunc\">')
	if func.type == 'elf_handle':
		docs.write('<span class=\"apiobjtype\">object</span> ')
	elif func.type == 'const char*':
		docs.write('<span class=\"apikeytype\">string</span> ')
	elif func.type == 'void':
		pass
	else: docs.write('<span class=\"apikeytype\">'+func.type+'</span> ')
	docs.write('elf.'+func.name+'( ')
	for i in range(len(func.params)):
		if func.params[i].type == 'elf_handle':
			docs.write('<span class=\"apiobjtype\">object</span> '+func.params[i].name)
		elif func.params[i].type == 'const char*':
			docs.write('<span class=\"apikeytype\">string</span> '+func.params[i].name)
		else: docs.write('<span class="apikeytype">'+func.params[i].type+'</span> '+func.params[i].name)
		if(i < len(func.params)-1): docs.write(', ')
	docs.write(' )</div>\n')

docs.write(apifooter)

docs.close()

import os
os.system('swig -lua -c++ elf/blendelf.i')

realbindslines = []

realbinds = open('elf/blendelf_wrap.cxx')
for line in realbinds:
	realbindslines.append(line)
realbinds.close()

realbinds = open('elf/blendelf_wrap.cxx', 'w')
for line in realbindslines:
	if "SWIG_name" not in line:
		line = line.replace('\"elf_', '\"')
		line = line.replace('\"ELF_', '\"')
		line = line.replace('\"elf', '\"')
	realbinds.write(line)
realbinds.close()

