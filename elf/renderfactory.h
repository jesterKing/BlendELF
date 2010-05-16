
const char *gbuffer_vert =
"attribute vec3 elf_VertexAttr;\n"
"attribute vec3 elf_NormalAttr;\n"
"uniform mat4 elf_ProjectionMatrix;\n"
"uniform mat4 elf_ModelviewMatrix;\n"
"varying vec4 elf_Vertex;\n"
"varying vec3 elf_Normal;\n"
"void main()\n"
"{\n"
"\tvec4 vertex = elf_ModelviewMatrix*vec4(elf_VertexAttr, 1.0);"
"\telf_Vertex = vertex;"
"\telf_Normal = vec3(elf_ModelviewMatrix*vec4(elf_NormalAttr, 0.0));"
"\tgl_Position = elf_ProjectionMatrix*vertex;\n"
"}\n";

const char *gbuffer_frag =
"varying vec4 elf_Vertex;\n"
"varying vec3 elf_Normal;\n"
"void main()\n"
"{\n"
"\tgl_FragData[0] = elf_Vertex;\n"
"\tgl_FragData[1] = vec4(elf_Normal, 1.0);\n"
"}\n";

const char *screen_quad_vert =
"attribute vec3 elf_VertexAttr;\n"
"attribute vec3 elf_TexCoordAttr;\n"
"uniform mat4 elf_ProjectionMatrix;\n"
"uniform mat4 elf_ModelviewMatrix;\n"
"varying vec2 elf_TexCoord;\n"
"void main()\n"
"{\n"
"\telf_TexCoord = elf_TexCoordAttr;\n"
"\tgl_Position = elf_ProjectionMatrix*elf_ModelviewMatrix*vec4(elf_VertexAttr, 1.0);\n"
"}\n";

const char *screen_quad_frag =
"uniform sampler2D elf_ColorMap;\n"
"varying vec4 elf_Vertex;\n"
"varying vec3 elf_Normal;\n"
"varying vec2 elf_TexCoord;\n"
"void main()\n"
"{\n"
"\tgl_FragColor = texture2D(elf_ColorMap, elf_TexCoord);\n"
"}\n";

