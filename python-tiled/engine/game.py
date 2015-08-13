#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import os
import math
import glob
import re

import pygame

try:
	import _path
except:
	pass

import tiledtmxloader

import actor
from hero import avatar_hero

#  -----------------------------------------------------------------------------

def start(screen, map_filename, hero, hero_layer, collision_layer):
	"""
	Use the paralax scrolling feature.
	"""

	# parser the map (it is done here to initialize the
	# window the same size as the map if it is small enough)
	world_map = tiledtmxloader.tmxreader.TileMapParser().parse_decode(map_filename)

	pygame.display.set_caption("Map viewer: " + map_filename + " ( " + str(world_map.pixel_width) +" x "+ str(world_map.pixel_height) + " )")

	screen_width, screen_height = screen.get_size()

	# load the images using pygame
	resources = tiledtmxloader.helperspygame.ResourceLoaderPygame()
	resources.load(world_map)

	# prepare map rendering
	assert world_map.orientation == "orthogonal"

	# renderer
	renderer = tiledtmxloader.helperspygame.RendererPygame()

	# get position of hero
	hero_pos_x = hero.rect.centerx
	hero_pos_y = hero.rect.bottom

	# dimensions of the hero for collision detection
	hero_width = hero.rect.width
	hero_height = 5

	# cam_offset is for scrolling
	cam_world_pos_x = hero.rect.centerx
	cam_world_pos_y = hero.rect.centery

	# set initial cam position and size
	renderer.set_camera_position_and_size(cam_world_pos_x, cam_world_pos_y, \
						screen_width, screen_height)

	# retrieve the layers
	sprite_layers = tiledtmxloader.helperspygame.get_layers_from_map(resources)

	# filter layers
	sprite_layers = [layer for layer in sprite_layers if not layer.is_object_group]

	# add the hero the the right layer, it can be changed using 0-9 keys
	sprite_layers[hero_layer].add_sprite(hero)

	# layer add/remove hero keys
	num_keys = [pygame.K_0, pygame.K_1, pygame.K_2, pygame.K_3, pygame.K_4, \
			pygame.K_5, pygame.K_6, pygame.K_7, pygame.K_8, pygame.K_9]

	# variables for the main loop
	clock = pygame.time.Clock()
	running = True
	speed = 0.075 * 2.
	# set up timer for fps printing
	pygame.time.set_timer(pygame.USEREVENT, 1000)

	speed_factor = 1.0
	action_type = actor.WALK

	# mainloop
	while running:
		dt = clock.tick()

		# event handling
		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				running = False
			elif event.type == pygame.USEREVENT:
				print("fps: ", clock.get_fps())
			elif event.type == pygame.KEYDOWN:
				if event.key == pygame.K_ESCAPE:
					running = False
				elif event.key in num_keys:
					# find out which layer to manipulate
					idx = num_keys.index(event.key)
					# make sure this layer exists
					if idx < len(world_map.layers):
						if sprite_layers[idx].contains_sprite(hero):
							sprite_layers[idx].remove_sprite(hero)
							print("removed hero sprite from layer", idx)
						else:
							sprite_layers[idx].add_sprite(hero)
							print("added hero sprite to layer", idx)
					else:
						print("no such layer or more than 10 layers: " + str(idx))
				elif event.key in (pygame.K_LSHIFT, pygame.K_RSHIFT):
					speed_factor = 2.0
					action_type = actor.RUN
			elif event.type == pygame.KEYUP:
				if event.key in (pygame.K_LSHIFT, pygame.K_RSHIFT):
					speed_factor = 1.0
					action_type = actor.WALK

		# find directions
		direction_x = pygame.key.get_pressed()[pygame.K_RIGHT] - \
						pygame.key.get_pressed()[pygame.K_LEFT]
		direction_y = pygame.key.get_pressed()[pygame.K_DOWN] - \
						pygame.key.get_pressed()[pygame.K_UP]

		# make sure the hero moves with same speed in all directions (diagonal!)
		dir_len = math.hypot(direction_x, direction_y)
		dir_len = dir_len if dir_len else 1.0

		# update position
		step_x = speed_factor * speed * dt * direction_x / dir_len
		step_y = speed_factor * speed * dt * direction_y / dir_len
		if collision_layer is not None:
			try:
				step_x, step_y = check_collision(hero_pos_x, hero_pos_y, step_x, step_y, hero_width, hero_height, sprite_layers[collision_layer])
			except:
				pass
		hero_pos_x += step_x
		hero_pos_y += step_y
		hero.rect.midbottom = (hero_pos_x, hero_pos_y)
		hero.update(step_x, step_y, action_type)

		# adjust camera according to the hero's position, follow him
		# (don't make the hero follow the cam, maybe later you want different
		#  objects to be followd by the cam)
		renderer.set_camera_position(hero.rect.centerx, hero.rect.centery)

		# clear screen, might be left out if every pixel is redrawn anyway
		screen.fill((0, 0, 0))

		# render the map
		for sprite_layer in sprite_layers:
			if sprite_layer.is_object_group:
				# we dont draw the object group layers
				# you should filter them out if not needed
				continue
			else:
				renderer.render_layer(screen, sprite_layer)

		pygame.display.flip()

#  -----------------------------------------------------------------------------

# unused in this demo, just here to show how you could check for collision!
def is_walkable(pos_x, pos_y, coll_layer):
	"""
	Just checks if a position in world coordinates is walkable.
	"""
	tile_x = int(pos_x // coll_layer.tilewidth)
	tile_y = int(pos_y // coll_layer.tileheight)

	if coll_layer.content2D[tile_y][tile_x] is None:
		return True
	return False

#  -----------------------------------------------------------------------------

def check_collision(hero_pos_x, hero_pos_y, step_x, step_y, \
					hero_width, hero_height, coll_layer):
	"""
	Checks collision of the hero against the world. Its not the best way to
	handle collision detection but for this demo it is good enough.

	:Returns: steps to add to heros current position.
	"""
	# create hero rect
	hero_rect = pygame.Rect(0, 0, hero_width, hero_height)
	hero_rect.midbottom = (hero_pos_x, hero_pos_y)

	# find the tile location of the hero
	tile_x = int((hero_pos_x) // coll_layer.tilewidth)
	tile_y = int((hero_pos_y) // coll_layer.tileheight)

	# find the tiles around the hero and extract their rects for collision
	tile_rects = []
	for diry in (-1, 0 , 1):
		for dirx in (-1, 0, 1):
			if coll_layer.content2D[tile_y + diry][tile_x + dirx] is not None:
				tile_rects.append(coll_layer.content2D[tile_y + diry][tile_x + dirx].rect)

	# save the original steps and return them if not canceled
	res_step_x = step_x
	res_step_y = step_y

	# x direction, floor or ceil depending on the sign of the step
	step_x = special_round(step_x)

	# detect a collision and dont move in x direction if colliding
	if hero_rect.move(step_x, 0).collidelist(tile_rects) > -1:
		res_step_x = 0

	# y direction, floor or ceil depending on the sign of the step
	step_y = special_round(step_y)

	# detect a collision and dont move in y direction if colliding
	if hero_rect.move(0, step_y).collidelist(tile_rects) > -1:
		res_step_y = 0

	# return the step the hero should do
	return res_step_x, res_step_y

#  -----------------------------------------------------------------------------

def special_round(value):
	"""
	For negative numbers it returns the value floored,
	for positive numbers it returns the value ceiled.
	"""
	# same as:  math.copysign(math.ceil(abs(x)), x)
	# OR:
	# ## versus this, which could save many function calls
	# import math
	# ceil_or_floor = { True : math.ceil, False : math.floor, }
	# # usage
	# x = floor_or_ceil[val<0.0](val)

	if value < 0:
		return math.floor(value)
	return math.ceil(value)

