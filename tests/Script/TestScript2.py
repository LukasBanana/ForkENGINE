
# Test Python Script 2
# 29/09/2014

from Keyboard import *
from Mouse import *

if KeyHit(KEY_LEFT):
	print("Left")
if KeyHit(KEY_UP):
	print("Up")
if KeyHit(KEY_RIGHT):
	print("Right")
if KeyHit(KEY_DOWN):
	print("Down")

if ButtonHit(MOUSE_LEFT):
	print("Left Mouse Button Hit")
if ButtonDoubleClicked(MOUSE_LEFT):
	print("Left Mouse Button Double Clicked")
