#
# Python Vector3 script
# 
# This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
# See "LICENSE.txt" for license information.
#

import math

class Vector3:
	'''This is the 3D vector class with components x, y and z'''
	x = y = z = None
	
	def __init__(self, x, y, z):
		self.x = x
		self.y = y
		self.z = z
	
	def __add__(self, other):
		return Vector3(self.x + other.x, self.y + other.y, self.z + other.z)
	
	def __iadd__(self, other):
		self.x += other.x
		self.y += other.y
		self.z += other.z
		return self
	
	def __sub__(self, other):
		return Vector3(self.x - other.x, self.y - other.y, self.z - other.z)
	
	def __isub__(self, other):
		self.x -= other.x
		self.y -= other.y
		self.z -= other.z
		return self
	
	def __mul__(self, other):
		return Vector3(self.x * other.x, self.y * other.y, self.z * other.z)
	
	def __imul__(self, other):
		self.x *= other.x
		self.y *= other.y
		self.z *= other.z
		return self
	
	def __div__(self, other):
		return Vector3(self.x / other.x, self.y / other.y, self.z / other.z)
	
	def __idiv__(self, other):
		self.x /= other.x
		self.y /= other.y
		self.z /= other.z
		return self
	
	def Dot(a, b):
		return a.x*b.x + a.y*b.y + a.z*b.z
	
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
		return self
	
	def Resize(self, size):
		self.Normalize()
		x *= size
		y *= size
		z *= size
		
	def Cross(a, b):
		return Vector3(
			a.y*b.z - b.y*a.z,
			b.x*a.z - a.x*b.z,
			a.x*b.y - b.x*a.y
		)

