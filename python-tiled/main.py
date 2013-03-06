#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import os
import engine
import pygame

from hero import avatar_hero

MAP = "maps/map.tmx"
HERO_DIR = "avatars/crono"

SCREEN_WIDTH = 640
SCREEN_HEIGHT = 480

HERO_START_X = 1024
HERO_START_Y = 768

HERO_LAYER = 1
COLLISION_LAYER = 3

# Este es el programa principal, invocado desde más abajo
def main():
	"""
	Main method.
	"""
	args = sys.argv[1:]
	if len(args) < 1:
		path_to_map = os.path.join(os.curdir, MAP)
		print(("usage: python %s your_map.tmx\n\nUsing default map '%s'\n" % \
			(os.path.basename(__file__), path_to_map)))
	else:
		path_to_map = args[0]

	# Inicializa pygame y abre una ventana
	pygame.init()
	pygame.display.set_caption("Map viewer")
	screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
	# Crea el avatar del heroe
	hero = create_hero(HERO_DIR, HERO_START_X, HERO_START_Y)
	# Muestra el mundo en la ventana de forma interactiva
	engine.game.start(screen, path_to_map, hero, HERO_LAYER, COLLISION_LAYER)

def create_hero(directory, start_pos_x, start_pos_y):
	"""
	Creates the hero sprite.
	"""
	hero = avatar_hero(directory, start_pos_x, start_pos_y)
	return hero

# Este es el punto en el que empieza el programa
if __name__ == '__main__':
	# Cambia el directorio activo al directorio en el que está el programa
	abspath = os.path.abspath(__file__)
	dname = os.path.dirname(abspath)
	os.chdir(dname)
	# Ejecuta el programa principal
	main()

