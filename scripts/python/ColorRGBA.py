#
# Python ColorRGBA script
# 
# This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
# See "LICENSE.txt" for license information.
#

import math

class ColorRGBA:
	'''This is the RGBA color class with components r, g, b and a'''
	r = g = b = a = None
	
	def __init__(self, r, g, b, a):
		self.r = r
		self.g = g
		self.b = b
		self.a = a
	
	def __add__(self, other):
		return ColorRGBA(self.r + other.r, self.g + other.g, self.b + other.b, self.a + other.a)
	
	def __iadd__(self, other):
		self.r += other.r
		self.g += other.g
		self.b += other.b
		self.a += other.a
		return self
	
	def __sub__(self, other):
		return ColorRGBA(self.r - other.r, self.g - other.g, self.b - other.b, self.a - other.a)
	
	def __isub__(self, other):
		self.r -= other.r
		self.g -= other.g
		self.b -= other.b
		self.a -= other.a
		return self
	
	def __mul__(self, other):
		return ColorRGBA(self.r * other.r, self.g * other.g, self.b * other.b, self.a * other.a)
	
	def __imul__(self, other):
		self.r *= other.r
		self.g *= other.g
		self.b *= other.b
		self.a *= other.a
		return self
	
	def __div__(self, other):
		return ColorRGBA(self.r / other.r, self.g / other.g, self.b / other.b, self.a / other.a)
	
	def __idiv__(self, other):
		self.r /= other.r
		self.g /= other.g
		self.b /= other.b
		self.a /= other.a
		return self

