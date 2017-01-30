#
# Python Quaternion script
# 
# This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
# See "LICENSE.txt" for license information.
#

import math

class Quaternion:
	'''This is the quaternion class with components x, y, z and w'''
	x = y = z = w = None
	
	def __init__(self, x, y, z, w):
		self.x = x
		self.y = y
		self.z = z
		self.w = w
	
	def __iadd__(self, other):
		self.x += other.x
		self.y += other.y
		self.z += other.z
		self.w += other.w
		return self
	
	def __add__(self, other):
		return Quaternion(self.x + other.x, self.y + other.y, self.z + other.z)
	
	def __isub__(self, other):
		self.x -= other.x
		self.y -= other.y
		self.z -= other.z
		self.w -= other.w
		return self
	
	def __sub__(self, other):
		return Quaternion(self.x - other.x, self.y - other.y, self.z - other.z)
	
	def __imul__(self, other):
		x = self.x
		y = self.y
		z = self.z
		w = self.w
		self.x = (other.w * x) + (other.x * w) + (other.y * z) - (other.z * y)
		self.y = (other.w * y) - (other.x * z) + (other.y * w) + (other.z * x)
		self.z = (other.w * z) + (other.x * y) - (other.y * x) + (other.z * w)
		self.w = (other.w * w) - (other.x * x) - (other.y * y) - (other.z * z)
		return self
	
	def __mul__(self, other):
		result = Quaternion(self.x, self.y, self.z, self.w)
		result *= other
		return result
	
	def Normalize(self):
		len = math.sqrt(self.x*self.x + self.y*self.y + self.z*self.z + self.w*self.w)
		if len != 0.0 and len != 1.0:
			invLen = 1.0 / len
			self.x *= invLen
			self.y *= invLen
			self.z *= invLen
			self.w *= invLen
		return self
	
	def Inverse(self):
		return Quaternion(-self.x, -self.y, -self.z, self.w)
	
	def EulerRotation(self, pitch, yaw, roll):
		pitch *= 0.5
		yaw *= 0.5
		roll *= 0.5
		
		cr = math.cos(pitch)
		cp = math.cos(yaw)
		cy = math.cos(roll)
		
		sr = math.sin(pitch)
		sp = math.sin(yaw)
		sy = math.sin(roll)
		
		cpcy = cp * cy
		spsy = sp * sy
		cpsy = cp * sy
		spcy = sp * cy
		
		self.x = sr * cpcy - cr * spsy;
		self.y = cr * spcy + sr * cpsy;
		self.z = cr * cpsy - sr * spcy;
		self.w = cr * cpcy + sr * spsy;
		
		self.Normalize()

