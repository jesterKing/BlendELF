#!/usr/bin/python

"""
Name: 'elf pak exporter'
Blender: 249
Group: 'Export'
Tooltip: 'Exports a resource library (.pak) for elf.'
"""

__author__ = 'None'
__version__ = '1.0'

__bpydoc__ = 'This python script exports a resource library for elf.'

import bpy
import Blender
from Blender.Mathutils import *
import struct
import math

def write_name_to_file(name, f):
	if len(name) > 63: name = name[0:63]
	if len(name) > 0: f.write(struct.pack('<'+str(len(name))+'s', name))
	i = len(name)
	while i < 64:
		f.write(struct.pack('<B', 0))
		i += 1

def trim_file_path(file_path):
	nfile_path = file_path
	if len(nfile_path) > 2:
		if nfile_path[0:2] == '//':
			nfile_path = nfile_path[2:]
	if nfile_path.rfind('\\') > -1:
		nfile_path = nfile_path[nfile_path.rfind('\\')+1:]
	if nfile_path.rfind('/') > -1:
		nfile_path = nfile_path[nfile_path.rfind('/')+1:]
	return nfile_path

class Texture:
	def __init__(self):
		self.name = ''
		self.file_path = ''
		self.type = ''
		self.length = 0
		self.data = None
		self.parallax_scale = 0.0
		self.size_bytes = 0

	def load(self, texture):
		self.size_bytes = 0
		tex = texture.tex

		self.name = tex.getName()

		img = tex.getImage()
		self.file_path = img.getFilename()
		if len(self.file_path) > 2:
			if self.file_path[0:2] == '//':
				self.file_path = self.file_path[2:]

		try:
			f = open(self.file_path, 'rb')

			self.data = f.read()
			self.length = len(self.data)

			self.file_path = trim_file_path(self.file_path)

			f.close()
		except:
			print 'error: could not load texture ' + self.name + ' from \"' + self.file_path + '\"'
			return False

		# magic
		self.size_bytes += struct.calcsize('<i')
		# name
		self.size_bytes += struct.calcsize('<64s')
		# image format
		self.size_bytes += struct.calcsize('<B')
		# length
		self.size_bytes += struct.calcsize('<i')
		# data
		self.size_bytes += self.length

		print 'Texture \"'+self.name+'\" converted'
		print '  size: '+str(self.size_bytes)+' bytes'

		return True

	def save(self, f):
		# write magic
		f.write(struct.pack('<i', 179532108))

		# write name
		write_name_to_file(self.name, f)

		# write format
		f.write(struct.pack('<B', 1))	# packed elf image

		# write length of data
		f.write(struct.pack('<i', self.length))

		# write data
		f.write(self.data)

		print 'Texture \"'+self.name+'\" saved'

class Material:
	def __init__(self):
		self.name = ''
		self.diffuse = [1.0, 1.0, 1.0, 1.0]
		self.ambient = [0.0, 0.0, 0.0, 1.0]
		self.specular = [0.0, 0.0, 0.0, 1.0]
		self.shininess = 32.0
		self.textures = []
		self.size_bytes = 0

	def load(self, mat):
		self.name = mat.getName()

		rgb = mat.getRGBCol()
		ref = mat.getRef()
		amb = mat.getAmb()
		specCol = mat.getSpecCol()
		spec = mat.getSpec()
		hardness = mat.getHardness()

		self.diffuse = [rgb[0]*ref, rgb[1]*ref, rgb[2]*ref, 1.0]
		self.ambient = [amb, amb, amb, 1.0]
		self.specular = [specCol[0]*spec, specCol[1]*spec, specCol[2]*spec, 1.0]
		self.shininess = float(hardness)/4.0

		for tex in mat.getTextures():
			if tex != None and tex.tex != None and tex.tex.getImage() != None:
				ntexture = Texture();

				ntexture.name = tex.tex.getName()
				ntexture.type = 1
				if tex.mtNor != 0: ntexture.type = 2
				elif tex.mtDisp != 0: ntexture.type = 4
				elif tex.mtSpec != 0: ntexture.type = 8
				elif tex.mtAmb != 0: ntexture.type = 128
				ntexture.parallax_scale = tex.dispfac

				self.textures.append(ntexture)

		self.size_bytes = 0

		# magic
		self.size_bytes += struct.calcsize('<i')
		# name
		self.size_bytes += struct.calcsize('<64s')
		# colors
		self.size_bytes += struct.calcsize('<ffff')*3
		# shininess
		self.size_bytes += struct.calcsize('<f')
		# texture count
		self.size_bytes += struct.calcsize('<B')
		# textures
		self.size_bytes += struct.calcsize('<64sfB')*len(self.textures)

		print 'Material \"'+self.name+'\" converted'
		print '  size: '+str(self.size_bytes)+' bytes'

	def save(self, f):
		# write magic
		f.write(struct.pack('<i', 179532109))

		# write name
		write_name_to_file(self.name, f)

		# write colors
		f.write(struct.pack('<ffff', self.diffuse[0], self.diffuse[1], self.diffuse[2], self.diffuse[3]))
		f.write(struct.pack('<ffff', self.ambient[0], self.ambient[1], self.ambient[2], self.ambient[3]))
		f.write(struct.pack('<ffff', self.specular[0], self.specular[1], self.specular[2], self.specular[3]))

		# write other specs
		f.write(struct.pack('<f', self.shininess))

		# write textures
		f.write(struct.pack('<B', len(self.textures)))
		for i in range(len(self.textures)):
			write_name_to_file(self.textures[i].name, f)
			f.write(struct.pack('<B', self.textures[i].type))
			f.write(struct.pack('<f', self.textures[i].parallax_scale))

		print 'Material \"'+self.name+'\" saved'

class Vertice:
	def __init__(self):
		self.co = [0.0, 0.0, 0.0]
		self.no = [0.0, 0.0, 0.0]
		self.uv = [0.0, 0.0]
		self.weights = [0.0, 0.0, 0.0, 0.0]
		self.boneids = [-1, -1, -1, -1]
	def __eq__(self, v):
		return self.co == v.co and self.no == v.no and self.uv == v.uv

class Area:
	def __init__(self):
		self.index = []

class Model:
	def __init__(self):
		self.name = ''
		self.verts = []
		self.areas = []
		self.indice_count = 0
		self.size_bytes = 0
		self.weights_and_boneids = False

	def load(self, obj):
		mesh = obj.getData()

		self.name = mesh.name

		max_frame = 1
		if obj.getParent() != None and obj.getParent().getAction() != None:
				action = obj.getParent().getAction()
				if len(action.getFrameNumbers()) > 0:
					frame_numbers = action.getFrameNumbers()
					for num in frame_numbers:
						if num > max_frame: max_frame = num
		
		groups = mesh.getVertGroupNames()
		if len(groups) > 0:
			self.weights_and_boneids = True
			if obj.getParent() != None:
				parent = obj.getParent()
				if parent.getType() == 'Armature':
					armdata = parent.getData()
					groups = armdata.bones.keys()

		Blender.Set("curframe", 1)
		Blender.Window.Redraw()

		for face in mesh.faces:
			while len(self.areas) < face.materialIndex+1: self.areas.append(Area())
			face_verts = [None, None, None, None]
			face_indexes = [0, 0, 0, 0]
			for i in range(len(face.v)):
				vert = Vertice()
				vert.co = [face.v[i].co.x, face.v[i].co.y, face.v[i].co.z]
				if face.smooth == 1: vert.no = [face.v[i].no.x, face.v[i].no.y, face.v[i].no.z]
				else: vert.no = [face.normal[0], face.normal[1], face.normal[2]]
				if len(groups) > 0:
					vi = mesh.getVertexInfluences(face.v[i].index)
					for j in range(len(vi)):
						if j > 3: break
						vert.weights[j] = vi[j][1]
						vert.boneids[j] = groups.index(vi[j][0])
				if mesh.hasFaceUV() is 1: vert.uv = [face.uv[i][0], face.uv[i][1]]
				face_verts[i] = vert
				if i == 3:
					#try:
					#	n = self.verts.index(vert)
					#	self.areas[face.materialIndex].index.append(face_indexes[0])
					#	self.areas[face.materialIndex].index.append(face_indexes[2])
					#	self.areas[face.materialIndex].index.append(n)
					#except:
					self.areas[face.materialIndex].index.append(face_indexes[0])
					self.areas[face.materialIndex].index.append(face_indexes[2])
					self.areas[face.materialIndex].index.append(len(self.verts))
					self.verts.append(vert)
				else:
					#try:
					#	n = self.verts.index(vert)
					#	self.areas[face.materialIndex].index.append(n)
					#except:
					self.areas[face.materialIndex].index.append(len(self.verts))
					face_indexes[i] = len(self.verts)
					face_verts[i] = vert
					self.verts.append(vert)

		if len(self.verts) < 3:
			return False

		self.size_bytes = 0

		# magic
		self.size_bytes += struct.calcsize('<i')
		# name
		self.size_bytes += struct.calcsize('<64s')
		# header
		self.size_bytes += struct.calcsize('<iiiiBBBB')
		# frames
		self.size_bytes += struct.calcsize('<fff')*len(self.verts)*1
		# areas
		self.indice_count = 0
		for area in self.areas:
			self.size_bytes += struct.calcsize('<i')
			self.size_bytes += struct.calcsize('<I')*len(area.index)
			self.indice_count += len(area.index)
		# normals
		self.size_bytes += struct.calcsize('<fff')*len(self.verts)
		# tex coords
		self.size_bytes += struct.calcsize('<ff')*len(self.verts)
		# weights and boneids
		if self.weights_and_boneids is True:
			self.size_bytes += struct.calcsize('<ffff')*len(self.verts)
			self.size_bytes += struct.calcsize('<hhhh')*len(self.verts)

		print 'Mesh \"'+self.name+'\" converted'
		print '  vertices: '+str(len(self.verts))
		print '  indicles: '+str(self.indice_count)
		print '  areas: '+str(len(self.areas))
		print '  size: '+str(self.size_bytes)+' bytes'

		return True

	def save(self, f):
		if len(self.verts) < 3: return

		# write magic
		f.write(struct.pack('<i', 179532110))

		# write name
		write_name_to_file(self.name, f)

		# write header
		f.write(struct.pack('<i', len(self.verts)))
		f.write(struct.pack('<i', 1))
		f.write(struct.pack('<i', self.indice_count))
		f.write(struct.pack('<i', len(self.areas)))
		f.write(struct.pack('<B', 255)) # normals
		f.write(struct.pack('<B', 255)) # texcoords
		if self.weights_and_boneids is True:
			f.write(struct.pack('<B', 255)) # weights and bone ids
		else: f.write(struct.pack('<B', 0))
		f.write(struct.pack('<B', 255)) # junk...

		# write the frames
		for v in self.verts:
			f.write(struct.pack('<fff', v.co[0], v.co[1], v.co[2]))

		# write the index
		for area in self.areas:
			f.write(struct.pack('<i', len(area.index)))
			for idx in area.index:
				f.write(struct.pack('<I', idx))

		# write normals
		for v in self.verts:
			f.write(struct.pack('<fff', v.no[0], v.no[1], v.no[2]))

		# write tex coords
		for v in self.verts:
			f.write(struct.pack('<ff', v.uv[0], v.uv[1]))
			
		# write weights and boneids
		if self.weights_and_boneids is True:
			for v in self.verts:
				f.write(struct.pack('<ffff', v.weights[0], v.weights[1], v.weights[2], v.weights[3]))
			for v in self.verts:
				f.write(struct.pack('<hhhh', v.boneids[0], v.boneids[1], v.boneids[2], v.boneids[3]))

		print 'Mesh \"'+self.name+'\" saved'

class BezierPoint:
	def __init__(self):
		self.c1 = [0.0, 0.0, 0.0]
		self.p = [0.0, 0.0, 0.0]
		self.c2 = [0.0, 0.0, 0.0]

class Curve:
	def __init__(self):
		self.type = 0
		self.interpolation = 0
		self.points = []

class Ipo:
	def __init__(self):
		self.name = ''
		self.curves = []

def get_ipo_from_ipo(ipo):
	nipo = Ipo()
	for cur in ipo.getCurves():
		curve = Curve()
		if cur.name == "LocX": curve.type = 0
		elif cur.name == "LocY": curve.type = 1
		elif cur.name == "LocZ": curve.type = 2
		elif cur.name == "RotX": curve.type = 3
		elif cur.name == "RotY": curve.type = 4
		elif cur.name == "RotZ": curve.type = 5
		elif cur.name == "ScaleX": curve.type = 6
		elif cur.name == "ScaleY": curve.type = 7
		elif cur.name == "ScaleZ": curve.type = 8
		elif cur.name == "QuatX": curve.type = 9
		elif cur.name == "QuatY": curve.type = 10
		elif cur.name == "QuatZ": curve.type = 11
		elif cur.name == "QuatW": curve.type = 12
		for pnt in cur.getPoints():
			point = BezierPoint()
			point.p[0] = pnt.pt[0]
			point.p[1] = pnt.pt[1]
			if curve.type >= 3 and curve.type <= 5:
				point.p[1] = point.p[1]*10.0
			curve.points.append(point)
		nipo.curves.append(curve)
	return nipo

class Actor(object):
	def __init__(self):
		super(Actor, self).__init__()

		self.name = ''
		self.parent = ''
		self.script = ''

		self.position = [0.0, 0.0, 0.0]
		self.rotation = [0.0, 0.0, 0.0]

		self.bounding_lengths = [0.0, 0.0, 0.0]
		self.bounding_offset = [0.0, 0.0, 0.0]
		self.shape = 0
		self.mass = 0.0
		self.lin_damp = 0.0
		self.ang_damp = 0.0
		self.lin_sleep = 0.8
		self.ang_sleep = 1.0
		self.restitution = 0.0
		self.anis_fric = [1.0, 1.0, 1.0]
		self.lin_factor = [1.0, 1.0, 1.0]
		self.ang_factor = [1.0, 1.0, 1.0]

		self.ipo = None

		self.size_bytes = 0

def get_actor_header_from_object(obj, eobj):
	Blender.Set("curframe", 1)
	Blender.Window.Redraw()
	
	eobj.name = obj.getName()

	if obj.getParent() != None:
		eobj.parent = obj.getParent().getName()

	try:
		prop = obj.getProperty('script')
		if prop.getType() == 'STRING':
			eobj.script = prop.getData()
	except: pass

	data = obj.getData()
	loc = obj.getLocation()
	rot = obj.getEuler()

	eobj.position[0] = loc[0]
	eobj.position[1] = loc[1]
	eobj.position[2] = loc[2]

	eobj.rotation[0] = rot[0]*(180.0/3.1415926535)
	eobj.rotation[1] = rot[1]*(180.0/3.1415926535)
	eobj.rotation[2] = rot[2]*(180.0/3.1415926535)
	
	if obj.getIpo() != None:
		eobj.ipo = get_ipo_from_ipo(obj.getIpo())

	try:
		prop = obj.getProperty('shape')
		if prop.getType() == 'STRING':
			if prop.getData() == 'CAPSULE': eobj.shape = 4
			if prop.getData() == 'MESH': eobj.shape = 3
			if prop.getData() == 'SPHERE': eobj.shape = 2
			if prop.getData() == 'BOX': eobj.shape = 1
	except: pass
	try:
		prop = obj.getProperty('mass')
		if prop.getType() == 'FLOAT':
			eobj.mass = prop.getData()
	except: pass
	try:
		prop = obj.getProperty('lin_damp')
		if prop.getType() == 'STRING':
			eobj.lin_damp = prop.getData()
	except: pass
	try:
		prop = obj.getProperty('ang_damp')
		if prop.getType() == 'STRING':
			eobj.ang_damp = prop.getData()
	except: pass

def write_actor_header(eobj, f):
	# write name
	write_name_to_file(eobj.name, f)
	write_name_to_file(eobj.parent, f)
	write_name_to_file(eobj.script, f)

	# write transformations
	f.write(struct.pack('<fff', eobj.position[0], eobj.position[1], eobj.position[2]))
	f.write(struct.pack('<fff', eobj.rotation[0], eobj.rotation[1], eobj.rotation[2]))
	
	# write ipos
	if eobj.ipo == None:
		f.write(struct.pack('<B', 0))
	else:
		f.write(struct.pack('<B', len(eobj.ipo.curves)))
		for curve in eobj.ipo.curves:
			f.write(struct.pack('<BBi', curve.type, curve.interpolation, len(curve.points)))
			for point in curve.points:
				f.write(struct.pack('<ffffff', point.c1[0], point.c1[1], point.p[0], point.p[1], point.c2[0], point.c2[0]))

	f.write(struct.pack('<fff', eobj.bounding_lengths[0], eobj.bounding_lengths[1], eobj.bounding_lengths[2]))
	f.write(struct.pack('<fff', eobj.bounding_offset[0], eobj.bounding_offset[1], eobj.bounding_offset[2]))
	f.write(struct.pack('<B', eobj.shape))
	f.write(struct.pack('<f', eobj.mass))
	f.write(struct.pack('<f', eobj.lin_damp))
	f.write(struct.pack('<f', eobj.ang_damp))
	f.write(struct.pack('<f', eobj.lin_sleep))
	f.write(struct.pack('<f', eobj.ang_sleep))
	f.write(struct.pack('<f', eobj.restitution))
	f.write(struct.pack('<fff', eobj.anis_fric[0], eobj.anis_fric[1], eobj.anis_fric[2]))
	f.write(struct.pack('<fff', eobj.lin_factor[0], eobj.lin_factor[1], eobj.lin_factor[2]))
	f.write(struct.pack('<fff', eobj.ang_factor[0], eobj.ang_factor[1], eobj.ang_factor[2]))

def get_actor_header_size(eobj):
	size_bytes = 0
	# name, parent, script
	size_bytes += struct.calcsize('<64s')*3
	# transformations
	size_bytes += struct.calcsize('<fff')*2
	# ipo
	size_bytes += struct.calcsize('<B')
	if eobj.ipo != None:
		for curve in eobj.ipo.curves:
			size_bytes += struct.calcsize('<BBi')
			size_bytes += struct.calcsize('<ffffff')*len(curve.points)

	size_bytes += struct.calcsize('<fff')	# bounding lengths
	size_bytes += struct.calcsize('<fff')	# bounding offsets
	size_bytes += struct.calcsize('<B')	# shape
	size_bytes += struct.calcsize('<f')	# mass
	size_bytes += struct.calcsize('<f')	# linear damp
	size_bytes += struct.calcsize('<f')	# angular damp
	size_bytes += struct.calcsize('<f')	# linear sleep threshold
	size_bytes += struct.calcsize('<f')	# angular sleep threshold
	size_bytes += struct.calcsize('<f')	# restitution
	size_bytes += struct.calcsize('<fff')	# anisotropic friction
	size_bytes += struct.calcsize('<fff')	# linear factor
	size_bytes += struct.calcsize('<fff')	# angular factor

	return size_bytes


class Camera(Actor):
	def __init__(self):
		super(Camera, self).__init__()

		self.fov = 35.0
		self.clip_near = 1.0
		self.clip_far = 1000.0

	def load(self, obj):
		get_actor_header_from_object(obj, self)
		
		data = obj.data

		self.fov = data.getLens()
		self.clip_near = data.getClipStart()
		self.clip_far = data.getClipEnd()
		
		# magic
		self.size_bytes += struct.calcsize('<i')
		# actor specs
		self.size_bytes += get_actor_header_size(self)
		# field of view, clip
		self.size_bytes += struct.calcsize('<fff')

		print 'Camera \"'+self.name+'\" converted'
		print '  size: '+str(self.size_bytes)+' bytes'

	def save(self, f):
		# write magic
		f.write(struct.pack('<i', 179532111))

		write_actor_header(self, f)

		# write filed of view
		f.write(struct.pack('<f', self.fov))

		# write clip
		f.write(struct.pack('<ff', self.clip_near, self.clip_far))

		print 'Camera \"'+self.name+'\" saved'

class Entity(Actor):
	def __init__(self):
		super(Entity, self).__init__()

		self.scale = [1.0, 1.0, 1.0]
		self.model = ''
		self.armature = ''
		self.materials = []

	def load(self, obj):
		get_actor_header_from_object(obj, self)

		if obj.getParent() != None:
			if obj.getParent().getType() == 'Armature':
				self.parent = ''
				self.armature = obj.getParent().getName()

		data = obj.getData()
		scl = obj.getSize()
		ipo = obj.getIpo()

		self.scale = scl

		self.model = data.name

		for mat in data.materials:
			self.materials.append(mat.getName())

		self.size_bytes += struct.calcsize('<i')	# magic
		self.size_bytes += get_actor_header_size(self)	# actor header
		self.size_bytes += struct.calcsize('<fff')	# scale
		self.size_bytes += struct.calcsize('<64s')	# model
		self.size_bytes += struct.calcsize('<64s')	# armature
		
		self.size_bytes += struct.calcsize('<i')	# material count
		self.size_bytes += struct.calcsize('<64s')*len(self.materials)	# material names

		print 'Entity \"'+self.name+"\" converted"
		print '  model: '+self.model
		print '  materials: '+str(len(self.materials))
		print '  size: '+str(self.size_bytes)+' bytes'

	def save(self, f):
		# write magic
		f.write(struct.pack('<i', 179532112))

		# write actor header
		write_actor_header(self, f)
		
		# write scale
		f.write(struct.pack('<fff', self.scale[0], self.scale[1], self.scale[2]))
		# write model
		write_name_to_file(self.model, f)
		# write armature
		write_name_to_file(self.armature, f)
		# write materials
		f.write(struct.pack('<i', len(self.materials)))
		for mat in self.materials:
			write_name_to_file(mat, f)

		print 'Entity \"'+self.name+"\" saved"

class Light(Actor):
	def __init__(self):
		super(Light, self).__init__()

		self.type = 'point'
		self.color = [1.0, 1.0, 1.0, 1.0]
		self.distance = 0.0
		self.fade_speed = 0.0
		self.inner_cone = 180.0
		self.outer_cone = 0.0
		self.shadow_map_size = 512
		self.shadow_caster = 1

	def load(self, obj):
		get_actor_header_from_object(obj, self)

		data = obj.getData()
		col = data.getCol()
		energy = data.getEnergy()

		if data.getType() is 1: self.type = 'sun'
		elif data.getType() is 2: self.type = 'spot'
		else: self.type = 'point'

		self.color = [col[0]*energy*0.75, col[1]*energy*0.75, col[2]*energy*0.75, 1.0]

		self.distance = data.getDist()
		self.fade_speed = 1.0/self.distance;

		self.inner_cone = data.getSpotSize()/2.0*(1.0-data.getSpotBlend())
		self.outer_cone = data.getSpotSize()/2.0-self.inner_cone
		
		self.shadow_map_size = data.getBufferSize()
		
		if data.getMode() is 1: self.shadow_caster = 1
		else: self.shadow_caster = 0

		# magic
		self.size_bytes += struct.calcsize('<i')
		# actor header
		self.size_bytes += get_actor_header_size(self)
		# type
		self.size_bytes += struct.calcsize('<B')
		# color
		self.size_bytes += struct.calcsize('<ffff')
		# attenuation
		self.size_bytes += struct.calcsize('<ff')
		# spot light specs
		self.size_bytes += struct.calcsize('<ff')
		# shadow specs
		self.size_bytes += struct.calcsize('<IB')
		
		print 'Light \"'+self.name+'\" converted'
		print '  type: '+self.type
		print '  size: '+str(self.size_bytes)+' bytes'

	def save(self, f):
		# write magic
		f.write(struct.pack('<i', 179532113))

		# write actor header
		write_actor_header(self, f)

		# write type
		if self.type is 'sun':
			f.write(struct.pack('<B', 2))
		elif self.type is 'spot':
			f.write(struct.pack('<B', 3))
		else:
			f.write(struct.pack('<B', 1))

		# write colors
		f.write(struct.pack('<ffff', self.color[0], self.color[1], self.color[2], self.color[3]))

		# write attenuation
		f.write(struct.pack('<ff', self.distance, self.fade_speed))

		# write spot light specs
		f.write(struct.pack('<ff', self.inner_cone, self.outer_cone))
		
		# write shadow specs
		
		f.write(struct.pack('<IB', self.shadow_map_size, self.shadow_caster))

		print 'Light \"'+self.name+'\" saved'

"""
def matrix4_to_eulers(mat):
	eul = [0.0, 0.0, 0.0]
	if mat[4] > 0.998:
		eul[0] = math.atan2(mat[2],mat[10])*(180.0/math.pi);
		eul[1] = math.pi/2*(180.0/math.pi);
		eul[2] = 0;
		return eul
	if mat[4] < -0.998:
		eul[0] = math.atan2(mat[2],mat[10])*(180.0/math.pi);
		eul[1] = -math.pi/2*(180.0/math.pi);
		eul[2] = 0;
		return eul
	eul[0] = math.atan2(-mat[8],mat[0])*(180.0/math.pi);
	eul[1] = math.atan2(-mat[6],mat[5])*(180.0/math.pi);
	eul[2] = math.asin(mat[4])*(180.0/math.pi);
	return eul
"""

class BoneFrame:
	def __init__(self):
		self.pos = [0.0, 0.0, 0.0]
		self.qua = [0.0, 0.0, 0.0, 1.0]

class Bone:
	def __init__(self):
		self.name = ''
		self.parent = ''
		self.id = 0
		self.pos = [0.0, 0.0, 0.0]
		self.qua = [0.0, 0.0, 0.0, 1.0]
		self.frames = []
		self.size_bytes = 0

class Armature:
	def __init__(self):
		self.name = ''
		self.bones = []
		self.frame_count = 0
		self.ipo = None
		self.size_bytes = 0

	def load(self, obj):
		self.name = obj.name

		data = obj.data

		action = obj.getAction()
		if action != None and len(action.getFrameNumbers()) > 0:
			frame_numbers = action.getFrameNumbers()
			for num in frame_numbers:
				if num > self.frame_count: self.frame_count = num
		
		bone_id = 0
		for name, bone in data.bones.items():
			nbone = Bone()
			nbone.name = name
			if bone.hasParent():
				nbone.parent = bone.parent.name
			nbone.id = bone_id

			mat = Matrix(bone.matrix['ARMATURESPACE'])
			qua = mat.toQuat()
			nbone.pos = [mat[3][0], mat[3][1], mat[3][2]]
			nbone.qua = [qua[1], qua[2], qua[3], qua[0]]
			
			# name and parent
			nbone.size_bytes += struct.calcsize('<64s')*2
			# id
			nbone.size_bytes += struct.calcsize('<i')
			# transformations
			nbone.size_bytes += struct.calcsize('<7f')
			# frames
			nbone.size_bytes += struct.calcsize('<7f')*self.frame_count
			
			self.bones.append(nbone)
			bone_id += 1

		for i in range(self.frame_count):
			Blender.Set("curframe", i+1)
			Blender.Window.Redraw()
			pose = obj.getPose()
			for bone in self.bones:
				mat = Matrix(pose.bones[bone.name].poseMatrix)
				qua = mat.toQuat()
				frame = BoneFrame()
				frame.pos = [mat[3][0], mat[3][1], mat[3][2]]
				frame.qua = [qua[1], qua[2], qua[3], qua[0]]
				bone.frames.append(frame)
		
		# magic
		self.size_bytes += struct.calcsize('<i')
		# name
		self.size_bytes += struct.calcsize('<64s')
		# frame_count
		self.size_bytes += struct.calcsize('<i')
		# bone count
		self.size_bytes += struct.calcsize('<i')
		# bones
		for bone in self.bones:
			self.size_bytes += nbone.size_bytes
		
		print 'Armature \"'+self.name+'\" converted'
		print '  Bones: '+str(len(self.bones))
		print '  size: '+str(self.size_bytes)+' bytes'

	def save(self, f):
		# write magic
		f.write(struct.pack('<i', 179532122))

		# write actor header
		write_name_to_file(self.name, f)
		# write frame count
		f.write(struct.pack('<i', self.frame_count))
		
		# write bones
		f.write(struct.pack('<i', len(self.bones)))
		for bone in self.bones:
			# name and parent
			write_name_to_file(bone.name, f)
			write_name_to_file(bone.parent, f)
			# id
			f.write(struct.pack('<i', bone.id))
			# transformations
			f.write(struct.pack('<3f', bone.pos[0], bone.pos[1], bone.pos[2]))
			f.write(struct.pack('<4f', bone.qua[0], bone.qua[1], bone.qua[2], bone.qua[3]))
			# frames
			for frame in bone.frames:
				f.write(struct.pack('<3f', frame.pos[0], frame.pos[1], frame.pos[2]))
				f.write(struct.pack('<4f', frame.qua[0], frame.qua[1], frame.qua[2], frame.qua[3]))
		
		print 'Armature \"'+self.name+'\" saved'

class Script:
	def __init__(self):
		self.name = ''
		self.text = ''
		self.size_bytes = 0
	
	def load(self, text):
		self.name = text.name
		
		while 1:
			line = text.readline()
			if len(line) < 1: break
			self.text += line
		
		# magic, name
		self.size_bytes = struct.calcsize('<i')
		self.size_bytes += struct.calcsize('<64s')
		# text length, text
		self.size_bytes += struct.calcsize('<i')
		self.size_bytes += len(self.text)
		
		print 'Script: '+text.name+' converted'
	
	def save(self, f):
		# write magic
		f.write(struct.pack('<i', 179532121))
		# write name
		write_name_to_file(self.name, f)
		# write text
		f.write(struct.pack('<i', len(self.text)))
		if len(self.text) > 0: f.write(struct.pack('<'+str(len(self.text))+'s', self.text))
		
		print 'Script: '+self.name+' saved'

class Scene:
	def __init__(self):
		self.name = ''
		self.ambient = [0.0, 0.0, 0.0, 1.0]
		self.size_bytes = 0

	def load(self, scn):
		self.name = scn.getName()

		amb = scn.world.getAmb()
		self.ambient[0] = amb[0]
		self.ambient[1] = amb[1]
		self.ambient[2] = amb[2]

		# magic
		self.size_bytes += struct.calcsize('<i', )
		#name
		self.size_bytes += struct.calcsize('<64s')
		# ambience
		self.size_bytes += struct.calcsize('<ffff')

	def save(self, f):
		# write magic
		f.write(struct.pack('<i', 179532120))
		# write name
		write_name_to_file(self.name, f)
		# write ambience
		f.write(struct.pack('<ffff', self.ambient[0], self.ambient[1], self.ambient[2], self.ambient[3]))

def export(path):
	print
	print '##############################'
	print '#### elf pak exporter 1.0 ####'
	print '##############################'
	print

	scripts = []
	script_names = []
	textures = []
	texture_names = []
	materials = []
	material_names = []
	models = []
	model_names = []
	cameras = []
	entities = []
	armatures = []
	lights = []
	scenes = []

	sce = bpy.data.scenes.active

	cur_scene = Scene()
	cur_scene.load(sce)
	scenes.append(cur_scene)
	
	for text in bpy.data.texts:
		if '.lua' in text.name:
			script = Script()
			script.load(text)
			scripts.append(script)

	for obj in sce.objects:
		if obj.getName()[0] == '.': continue
		if obj.getType() == 'Mesh':
			entity = Entity()
			entity.load(obj)
			entities.append(entity)
			data = obj.getData()
			if data.name not in model_names:
				model = Model()
				if model.load(obj) == False: continue
				models.append(model)
				model_names.append(model.name)
				for mat in data.materials:
					if mat.getName() not in material_names:
						material_names.append(mat.getName())
						material = Material()
						material.load(mat)
						materials.append(material)
						for tex in mat.getTextures():
							if tex != None and tex.tex != None and tex.tex.getImage() != None and tex.tex.getName() not in texture_names:
								texture = Texture()
								if texture.load(tex) == True:
									textures.append(texture)
									texture_names.append(texture.name)
		elif obj.getType() == 'Lamp':
			light = Light()
			light.load(obj)
			lights.append(light)
		elif obj.getType() == 'Camera':
			camera = Camera()
			camera.load(obj)
			cameras.append(camera)
		elif obj.getType() == 'Armature':
			armature = Armature()
			armature.load(obj)
			armatures.append(armature)

	print
	print

	offset = 0

	# calculate the index offset
	offset += struct.calcsize('<B')
	offset += struct.calcsize('<64s')
	offset += struct.calcsize('<i')
	offset *= len(scenes)+len(scripts)+len(textures)+len(materials)+len(models)+len(cameras)+len(entities)+len(lights)+len(armatures)
	# magic and number of indexes
	offset += struct.calcsize('<i')
	offset += struct.calcsize('<i')

	f = open(path, 'wb')

	# magic
	f.write(struct.pack('<i', 179532100))

	# index count
	f.write(struct.pack('<i', len(scenes)+len(scripts)+len(textures)+len(materials)+len(models)+len(cameras)+len(entities)+len(lights)+len(armatures)))

	# write the index
	for scene in scenes:
		f.write(struct.pack('<B', 6))
		write_name_to_file(scene.name, f)
		f.write(struct.pack('<i', offset))
		offset += scene.size_bytes
	for script in scripts:
		f.write(struct.pack('<B', 17))
		write_name_to_file(script.name, f)
		f.write(struct.pack('<i', offset))
		offset += script.size_bytes
	for texture in textures:
		f.write(struct.pack('<B', 0))
		write_name_to_file(texture.name, f)
		f.write(struct.pack('<i', offset))
		offset += texture.size_bytes
	for material in materials:
		f.write(struct.pack('<B', 1))
		write_name_to_file(material.name, f)
		f.write(struct.pack('<i', offset))
		offset += material.size_bytes
	for model in models:
		f.write(struct.pack('<B', 2))
		write_name_to_file(model.name, f)
		f.write(struct.pack('<i', offset))
		offset += model.size_bytes
	for camera in cameras:
		f.write(struct.pack('<B', 3))
		write_name_to_file(camera.name, f)
		f.write(struct.pack('<i', offset))
		offset += camera.size_bytes
	for entity in entities:
		f.write(struct.pack('<B', 4))
		write_name_to_file(entity.name, f)
		f.write(struct.pack('<i', offset))
		offset += entity.size_bytes
	for light in lights:
		f.write(struct.pack('<B', 5))
		write_name_to_file(light.name, f)
		f.write(struct.pack('<i', offset))
		offset += light.size_bytes
	for armature in armatures:
		f.write(struct.pack('<B', 22))
		write_name_to_file(armature.name, f)
		f.write(struct.pack('<i', offset))
		offset += armature.size_bytes

	# write the data
	for scene in scenes:
		scene.save(f)
	for script in scripts:
		script.save(f)
	for texture in textures:
		texture.save(f)
	for material in materials:
		material.save(f)
	for model in models:
		model.save(f)
	for camera in cameras:
		camera.save(f)
	for entity in entities:
		entity.save(f)
	for light in lights:
		light.save(f)
	for armature in armatures:
		armature.save(f)

	f.close()

	print
	print '######### EXPORT DONE #########'
	print
	print

if __name__ == '__main__':
	Blender.Window.FileSelector(export, 'Export .pak')

