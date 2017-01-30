
# Test Python Script 1
# 28/09/2014

import Log

from Vector3 import *

for i in range(0, 10):
	print("Hello, World (" + str(i) + ")")

Log.Error("Some error")
Log.IncIndent()
Log.Warning("Some warning")
Log.Success("Some success")

def fib(n):
	return 1 if n <= 2 else fib(n-1) + fib(n-2)

a = Vector3(1.0, 2.0, -0.5) + Vector3(3.0, 0.5, 2.0)
a.Normalize()

b = Vector3(1.5, 2.0, 0.5)

print("a.Length() = " + str(a.Length()))

print("Dot(a, b) = " + str(Vector3.Dot(a, b)))

c = Vector3.Cross(Vector3(0, 0, 1), Vector3(1, 0, 0))

print("Cross(a, b) = { " + str(c.x) + ", " + str(c.y) + ", " + str(c.z) + " }")
	
