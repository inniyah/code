#!/usr/bin/python
# -*- coding: utf-8 -*-

from . import game
__all__ = ["game"]

try:
	from . import actor
	__all__.append("actor")
except:
	pass

try:
	from . import hero
	__all__.append("hero")
except:
	pass

__author__ = 'Miriam Ruiz <miriam@debian.org>'
