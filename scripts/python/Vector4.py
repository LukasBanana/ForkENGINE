#
# Python Vector4 script
# 
# This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
# See "LICENSE.txt" for license information.
#

import math

class Vector4:
	'''This is the 4D vector class with components x, y, z and w'''
	x = y = z = w = None
	
	def __init__(self, x, y, z, w):
		self.x = x
		self.y = y
		self.z = z
		self.w = w
	
	def __add__(self, other):
		return Vector4(self.x + other.x, self.y + other.y, self.z + other.z, self.w + other.w)
	
	def __iadd__(self, other):
		self.x += other.x
		self.y += other.y
		self.z += other.z
		self.w += other.w
		return self
	
	def __sub__(self, other):
		return Vector4(self.x - other.x, self.y - other.y, self.z - other.z, self.w - other.w)
	
	def __isub__(self, other):
		self.x -= other.x
		self.y -= other.y
		self.z -= other.z
		self.w -= other.w
		return self
	
	def __mul__(self, other):
		return Vector4(self.x * other.x, self.y * other.y, self.z * other.z, self.w * other.w)
	
	def __imul__(self, other):
		self.x *= other.x
		self.y *= other.y
		self.z *= other.z
		self.w *= other.w
		return self
	
	def __div__(self, other):
		return Vector4(self.x / other.x, self.y / other.y, self.z / other.z, self.w / other.w)
	
	def __idiv__(self, other):
		self.x /= other.x
		self.y /= other.y
		self.z /= other.z
		self.w /= other.w
		return self
	
	def Dot(a, b):
		return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w
	
	def LengthSq(self):
		return Dot(self, self)
	
	def Length(self):
		return math.sqrt(self.LengthSq())
	
	def Normalize(self):
		len = self.Length()
		if len != 0.0 and len != 1.0:
			invLen = 1.0 / len
			self.x *= invLen
			self.y *= invLen
			self.z *= invLen
			self.w *= invLen
		return self
	
	def Resize(self, size):
		self.Normalize()
		x *= size
		y *= size
		z *= size
		w *= size

