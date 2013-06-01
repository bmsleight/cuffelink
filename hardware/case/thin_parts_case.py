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
min_embossed_detail = 0.3

pcb_diameter = 14.5
pcb_thickness = 0.4
pcb_gap =0.25
tooth_depth = 0.35

#top_inside_diameter = 15.8
top_inside_diameter = pcb_diameter + 2*tooth_depth + 2*pcb_gap
top_outside_diameter = 15.8 + (2 * min_wall_free)
top_height = 8.1
led_hole_dia = 1.4
led_gap = 3

touch_pads_height = 1.75
touch_pads_cube_height = 1.4
touch_pads_offset = 5.5
touch_pads_diameter = 1.23;

plastic_connector_dia = top_inside_diameter - 2*tooth_depth
plastic_top_screw_height=1.8

battery_diameter = 9.5
wire_diameter = 1.23
wiggle_room = 2
plastic_battery_hole_diameter = battery_diameter + wire_diameter + wiggle_room


def triangle(depth, a_len, o_len):
    return linear_extrude(height=depth) ( polygon(points=[[0,0],[a_len,0],[0,o_len]], paths=[[0,1,2]]) )



def pcb():

    p = cylinder(h=pcb_thickness, r=pcb_diameter/2)
    return p


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

def boarder():
    width = 5
    height = 11.0
    b = cube([width, height, min_wall_free], center=True)
    b = b - cube([width - 2*min_wall_free, height - 2*min_wall_free, min_wall_free], center=True)
    return b

def pads():
    c = cylinder(h=touch_pads_height, r=touch_pads_diameter/2, center=True);
    b = translate([0,0,(touch_pads_height/2)-touch_pads_cube_height/2])(cube([touch_pads_cube_height*2,touch_pads_diameter,touch_pads_cube_height], center=True))
    return c + b

def cuff_top():
    top = cylinder(r=top_outside_diameter/2, h=min_wall_free)
    top = top - led_hole() - forward(led_gap)(led_hole()) - back(led_gap)(led_hole())
    hoods = hood() + forward(led_gap)(hood()) + back(led_gap)(hood())
    h = up(min_wall_free)(hoods)
    b = up(min_wall_free*1.5)(boarder())
    
    p = up(-min_wall_free)( forward(touch_pads_offset)(pads()) +  back(touch_pads_offset)(pads()) )
    return top + h + b + p

def top():
    return cuff_side() + up(top_height-min_wall_free)(cuff_top())


def plastic_connector_top(gap=0, hole=True):
    SEGMENTS = 120
    inner_rad = plastic_connector_dia/2
    section = screw_thread.default_thread_section( tooth_height=0.7, tooth_depth=tooth_depth)   
    s = screw_thread.thread( outline_pts=section, inner_rad = inner_rad,
                            pitch= 0.7, length=plastic_top_screw_height, segments_per_rot=SEGMENTS
                            , neck_in_degrees=30, neck_out_degrees=30)
                        
    c = cylinder( r=inner_rad + .05, h=plastic_top_screw_height )
    if hole==False:
        top=s+c
    else:
        top=cylinder(r=top_inside_diameter/2, h=plastic_top_screw_height ) - (s+c)

    top = down(plastic_top_screw_height/2+pcb_thickness)(top)
    return up(top_height-min_wall_free-touch_pads_height-pcb_thickness)(top)





def assembly():
   cutaway = cube([20,20,20])
   t = top()
   c = plastic_connector_top()
   p = color(Green)(up(5.75)(pcb()))
#   stork = stork()
#   connector = connector()

#  return top + stork + connector
   return t + p + c - cutaway



if __name__ == '__main__':    
    out_dir = sys.argv[1] if len(sys.argv) > 1 else os.curdir
    file_out = os.path.join( out_dir, 'parts.scad')
    
    a = assembly()
    
    print "%(__file__)s: SCAD file written to: \n%(file_out)s \n"%vars()
    
    scad_render_to_file( a, file_out, include_orig_code=True)
