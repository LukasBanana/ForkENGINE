#
# Python platform info script
# 
# This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
# See "LICENSE.txt" for license information.
#

import platform

def PrintPlatformInfo():
	print("Platform:  " + platform.platform())
	print("Processor: " + platform.processor())
	print("Machine:   " + platform.machine())
