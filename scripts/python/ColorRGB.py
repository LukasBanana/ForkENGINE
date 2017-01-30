#
# Python ColorRGB script
# 
# This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
# See "LICENSE.txt" for license information.
#

import math

class ColorRGB:
	'''This is the RGB color class with components r, g and b'''
	r = g = b = None
	
	def __init__(self, r, g, b):
		self.r = r
		self.g = g
		self.b = b
	
	def __add__(self, other):
		return ColorRGB(self.r + other.r, self.g + other.g, self.b + other.b)
	
	def __iadd__(self, other):
		self.r += other.r
		self.g += other.g
		self.b += other.b
		return self
	
	def __sub__(self, other):
		return ColorRGB(self.r - other.r, self.g - other.g, self.b - other.b)
	
	def __isub__(self, other):
		self.r -= other.r
		self.g -= other.g
		self.b -= other.b
		return self
	
	def __mul__(self, other):
		return ColorRGB(self.r * other.r, self.g * other.g, self.b * other.b)
	
	def __imul__(self, other):
		self.r *= other.r
		self.g *= other.g
		self.b *= other.b
		return self
	
	def __div__(self, other):
		return ColorRGB(self.r / other.r, self.g / other.g, self.b / other.b)
	
	def __idiv__(self, other):
		self.r /= other.r
		self.g /= other.g
		self.b /= other.b
		return self

