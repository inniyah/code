#!/usr/bin/python
# -*- coding: utf-8 -*-

from engine.actor import *

class avatar_hero(avatar):
	def __init__(self, directory, start_pos_x, start_pos_y):
		super(avatar_hero, self).__init__(directory, start_pos_x, start_pos_y)
	def load(self, directory):
		self.images_standing = {
				NORTH: get_images(directory, 'back.gif' ),
				SOUTH: get_images(directory, 'front.gif'),
				WEST: get_images(directory, 'left.gif'),
			}
		self.images_walking = {
				NORTH: get_images(directory, 'back_walk.*.gif'),
				SOUTH: get_images(directory, 'front_walk.*.gif'),
				WEST: get_images(directory, 'left_walk.*.gif'),
			}
		self.images_running = {
				NORTH: get_images(directory, 'back_run.*.gif'),
				SOUTH: get_images(directory, 'front_run.*.gif'),
				WEST: get_images(directory, 'left_run.*.gif'),
			}
		self.direction = SOUTH
		self.frame_num = 0
		self.last_tick = pygame.time.get_ticks()
