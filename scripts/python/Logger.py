#
# Python Logger (redirect) script
# 
# This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
# See "LICENSE.txt" for license information.
#

import sys, Log

class LoggerMessage:
	def write(self, msg):
		Log.Message(msg)
	def flush(self):
		pass

class LoggerError:
	def write(self, msg):
		Log.Message(msg)
	def flush(self):
		pass

sys.stdout = LoggerMessage()
sys.stderr = LoggerError()
