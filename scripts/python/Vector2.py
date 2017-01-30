#
# Python Vector2 script
# 
# This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
# See "LICENSE.txt" for license information.
#

import math

class Vector2:
	'''This is the 2D vector class with components x and y'''
	x = y = None
	
	def __init__(self, x, y):
		self.x = x
		self.y = y
	
	def __add__(self, other):
		return Vector2(self.x + other.x, self.y + other.y)
	
	def __iadd__(self, other):
		self.x += other.x
		self.y += other.y
		return self
	
	def __sub__(self, other):
		return Vector2(self.x - other.x, self.y - other.y)
	
	def __isub__(self, other):
		self.x -= other.x
		self.y -= other.y
		return self
	
	def __mul__(self, other):
		return Vector2(self.x * other.x, self.y * other.y)
	
	def __imul__(self, other):
		self.x *= other.x
		self.y *= other.y
		return self
	
	def __div__(self, other):
		return Vector2(self.x / other.x, self.y / other.y)
	
	def __idiv__(self, other):
		self.x /= other.x
		self.y /= other.y
		return self
	
	def Dot(a, b):
		return a.x*b.x + a.y*b.y
		
	def LengthSq(self):
		return Vector2.Dot(self, self)
	
	def Length(self):
		return math.sqrt(self.LengthSq())
	
	def Normalize(self):
		len = self.Length()
		if len != 0.0 and len != 1.0:
			invLen = 1.0 / len
			self.x *= invLen
			self.y *= invLen
		return self
	
	def Resize(self, size):
		self.Normalize()
		x *= size
		y *= size

