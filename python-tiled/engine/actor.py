#!/usr/bin/python
# -*- coding: utf-8 -*-

import tiledtmxloader
import sys
import os
import math
import glob
import re
import pygame

CENTER = -1
NORTH = 0
NORTHEAST = 1
EAST = 2
SOUTHEAST = 3
SOUTH = 4
SOUTHWEST = 5
WEST = 6
NORTHWEST = 7

STAND = 0
WALK = 1
RUN = 2
JUMP = 3
CLIMB = 4

def get_files_natural_order(path):
	file_list = []
	for infile in glob.glob( path ):
		if os.path.isfile(infile):
			file_list.append(infile)
	digits = re.compile(r'(\d+)')
	def tokenize(filename):
		return tuple(int(token) if match else token
			for token, match in
				((fragment, digits.search(fragment))
				for fragment in digits.split(filename)))
	file_list.sort(key=tokenize)
	return file_list

def get_images_from_files(file_list):
	image_list = []
	for filename in file_list:
		try:
			image = pygame.image.load(filename)
			print 'Image: ', filename
			image_list.append(image)
		except pygame.error, message:
			print 'Cannot load image: ', filename
	return image_list

def create_image(width, height, red, green, blue, alpha):
	image = pygame.Surface((width, height), pygame.SRCALPHA)
	image.fill((red, green, blue, alpha))
	return image

def get_images(directory, selection):
	if isinstance(selection, (list, tuple)):
		img = []
		for element in selection:
			img.extend(get_images(directory, element))
		return img
	else:
		return get_images_from_files(get_files_natural_order(os.path.join(directory, selection)))

def get_mirror_images(source_images, flip_horizontal, flip_vertical):
	destination_images = []
	for image in source_images:
		destination_images.append(pygame.transform.flip(image, flip_horizontal, flip_vertical))
	return destination_images

def fill_images(img):
	if not EAST in img and WEST in img:
		img[EAST] = get_mirror_images(img[WEST], True, False)
	if not WEST in img and not EAST in img and SOUTH in img:
		img[EAST] = img[SOUTH]
		img[WEST] = img[SOUTH]
	if not WEST in img and not EAST in img and NORTH in img:
		img[EAST] = img[NORTH]
		img[WEST] = img[NORTH]
	if not NORTH in img and SOUTH in img:
		img[NORTH] = img[SOUTH]
	if not SOUTH in img and NORTH in img:
		img[SOUTH] = img[NORTH]
	if not WEST in img and EAST in img:
		img[WEST] = get_mirror_images(img[EAST], True, False)
	if not SOUTHWEST in img and WEST in img:
		img[SOUTHWEST] = img[WEST]
	if not SOUTHEAST in img and EAST in img:
		img[SOUTHEAST] = img[EAST]
	if not NORTHWEST in img and WEST in img:
		img[NORTHWEST] = img[WEST]
	if not NORTHEAST in img and EAST in img:
		img[NORTHEAST] = img[EAST]
	for dir in [ NORTH, SOUTH, EAST, WEST, NORTHEAST, NORTHWEST, SOUTHEAST, SOUTHWEST ]:
		if not dir in img:
			if not 'fake_img' in locals():
				fake_img = create_image(25, 45, 255, 0, 0, 150)
			img[dir] = [ fake_img ]


class avatar(tiledtmxloader.helperspygame.SpriteLayer.Sprite):
	def __init__(self, directory, start_pos_x, start_pos_y):
		self.ch_speed = 100
		self.load(directory)
		try:
			self.images_standing
		except AttributeError:
			self.images_standing = { }
		fill_images(self.images_standing)
		try:
			fill_images(self.images_walking)
		except AttributeError:
			self.images_walking = self.images_standing
		try:
			fill_images(self.images_running)
		except AttributeError:
			self.images_running = self.images_walking

		self.images = self.images_standing
		try:
			image = self.images[self.direction][self.frame_num]
			rect = image.get_rect()
			rect.midbottom = (start_pos_x, start_pos_y)
		except:
			image = None
			rect = None
		super(avatar, self).__init__(image, rect)
	def load(self, directory):
		self.images_standing = { }
		self.images_walking = { }
		self.images_running = { }
	def lookat(self, direction):
		midbottom = self.rect.midbottom
		if (direction != CENTER):
			self.direction = direction
		if (self.frame_num >= len(self.images[self.direction])):
				self.frame_num=0;
		image = self.images[self.direction][self.frame_num]
		rect = image.get_rect()
		rect.midbottom = midbottom
		self.image = image
		self.rect = rect
	def setaction(self, action):
		self.images = self.images_standing
		if (action == WALK):
			self.images = self.images_walking
		elif (action == RUN):
			self.images = self.images_running
		if (self.frame_num >= len(self.images[self.direction])):
				self.frame_num=0;
	def update(self, inc_x, inc_y, action_type):
		direction = [NORTHWEST, NORTH, NORTHEAST, WEST, CENTER, EAST, SOUTHWEST, SOUTH, SOUTHEAST] \
				[4 + cmp(inc_x, 0) + cmp(inc_y, 0) * 3]
		self.ch_speed = 100
		if action_type == STAND or direction == CENTER:
			self.setaction(STAND)
		elif action_type == RUN:
			self.setaction(RUN)
			self.ch_speed = 75
		else:
			self.setaction(WALK)
		self.lookat(direction)
		tick = pygame.time.get_ticks()
		if (tick - self.last_tick > self.ch_speed):
			self.last_tick = tick;
			self.frame_num += 1;
			if (self.frame_num >= len(self.images[self.direction])):
				self.frame_num=0;
