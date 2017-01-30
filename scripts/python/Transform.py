#
# Python Transform script
# 
# This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
# See "LICENSE.txt" for license information.
#

from Vector3 import *
from Quaternion import *

class Transform:
	'''This is the transform class with components position, rotation and scale'''
	position = Vector3(0.0, 0.0, 0.0)
	rotation = Quaternion(0.0, 0.0, 0.0, 1.0)
	scale = Vector3(0.0, 0.0, 0.0)
	
	def Rotate(self, pitch, yaw, roll):
		self.rotation.EulerRotation(pitch, yaw, roll)

