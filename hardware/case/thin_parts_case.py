#! /usr/bin/python
# -*- coding: UTF-8 -*-
from __future__ import division
import os, sys, re

from solid import *
from solid.utils import *
from solid import screw_thread 

SEGMENTS = 60

min_wall_free = 0.7
min_wire_free = 1.0
top_inside_diameter = 15.8
top_outside_diameter = 15.8 + (2 * min_wall_free)
top_height = 8.1
led_hole_dia = 1.5
led_gap = 3


def triangle(depth, a_len, o_len):
    return linear_extrude(height=depth) ( polygon(points=[[0,0],[a_len,0],[0,o_len]], paths=[[0,1,2]]) )

def cuff_side():
    outer = cylinder(r=top_outside_diameter/2, h=top_height)
    inner = cylinder(r=top_inside_diameter/2, h=top_height)
    side = outer - inner
    return side

def led_hole():
    return cylinder(r=led_hole_dia/2, h=min_wall_free)

def hood():
    hood_d = led_hole_dia + (min_wall_free*2)
    hood_r = hood_d/2
    c = cylinder( r=hood_r, h=min_wall_free)
    c = c - cylinder( r=led_hole_dia/2, h=min_wall_free)
    t = translate([-hood_r,-hood_r,min_wall_free])(rotate([0, 90, 0])(triangle(hood_d,min_wall_free,hood_d)))
    return c - t


def cuff_top():
    top = cylinder(r=top_outside_diameter/2, h=min_wall_free)
    top = top - led_hole() - forward(led_gap)(led_hole()) - back(led_gap)(led_hole())
    hoods = hood() + forward(led_gap)(hood()) + back(led_gap)(hood())
    h = up(min_wall_free)(hoods)
    return top + h

def top():
    return cuff_side() + up(top_height-min_wall_free)(cuff_top())


def assembly():
   t = top()
#   stork = stork()
#   connector = connector()

#  return top + stork + connector
   return t



if __name__ == '__main__':    
    out_dir = sys.argv[1] if len(sys.argv) > 1 else os.curdir
    file_out = os.path.join( out_dir, 'parts.scad')
    
    a = assembly()
    
    print "%(__file__)s: SCAD file written to: \n%(file_out)s \n"%vars()
    
    scad_render_to_file( a, file_out, include_orig_code=True)
