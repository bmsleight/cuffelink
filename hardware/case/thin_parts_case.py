#! /usr/bin/python
# -*- coding: UTF-8 -*-
from __future__ import division
import os, sys, re, argparse

from solid import *
from solid.utils import *
from solid import screw_thread 

SEGMENTS = 120


min_wall_free = 0.7
min_wire_free = 1.0
min_embossed_detail = 0.3

pcb_diameter = 14.5
pcb_thickness = 0.4
pcb_gap =0.25
tooth_depth = 0.7
tooth_height = 1.4
tooth_pitch = 1.4

stork_length = 14
lower_bar_length = 12

top_height = 8.1 - 0.6 # Thin waller mean think cuffelinks
led_hole_dia = 1.4
led_gap = 3


#top_inside_diameter = 15.8
top_inside_diameter = pcb_diameter + 2*tooth_depth + 2*pcb_gap
top_outside_diameter = top_inside_diameter + (2 * min_wall_free)

touch_pads_height = 1.75
touch_pads_cube_height = 1.4
touch_pads_offset = 5.5
touch_pads_diameter = 1.23;

plastic_connector_dia = top_inside_diameter - 2*tooth_depth
plastic_top_screw_height=1.8

battery_diameter = 9.5
wire_diameter = 1.23
wiggle_room = 1
plastic_battery_hole_diameter = battery_diameter + wire_diameter + wiggle_room


def triangle(depth, a_len, o_len):
    return linear_extrude(height=depth) ( polygon(points=[[0,0],[a_len,0],[0,o_len]], paths=[[0,1,2]]) )



def pcb():
    p = cylinder(h=pcb_thickness, r=pcb_diameter/2)
    return p + battery()


def battery():
#        translate([0,0,0.6/2]) cylinder(h=2.1, r=battery_diameter/2, center=true);
#        translate([0,0,-2.1/2]) cylinder(h=0.6, r=7.4/2, center=true);
#battery_thickness = 2.7;
    battery_diameter_top = 9.5
    battery_h_top = 1.4
    battery_diameter_bottom = 8.0
    battery_h_bottom = 2.7

    b = up(battery_h_bottom - battery_h_top)(cylinder(r=battery_diameter/2, h=battery_h_top))

    b = b + cylinder(r=battery_diameter_bottom/2, h=battery_h_bottom)
    return down(battery_h_bottom)(b)


def cuff_side():
    outer = cylinder(r=top_outside_diameter/2, h=top_height)
    inner = cylinder(r=top_inside_diameter/2, h=top_height)
    side = outer - inner
    return side

def led_hole():
    return cylinder(r=led_hole_dia/2, h=min_wall_free)

def hood():
    hood_d = led_hole_dia + (min_wall_free)
    hood_r = hood_d/2
    c = cylinder( r=hood_r, h=min_wall_free/2)
    c = c - cylinder( r=led_hole_dia/2, h=min_wall_free/2)
    t = translate([-hood_r,-hood_r,min_wall_free/2])(rotate([0, 90, 0])(triangle(hood_d,min_wall_free/2,hood_d)))
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
    return cuff_side() + up(top_height-min_wall_free)(cuff_top()) + plastic_connector_top(gap=0, reverse=True)


def plastic_connector_top(gap=0, reverse=False):
    inner_rad = plastic_connector_dia/2 - gap
    section = screw_thread.default_thread_section( tooth_height=tooth_height, tooth_depth=tooth_depth)   
    s = screw_thread.thread( outline_pts=section, inner_rad = inner_rad,
                            pitch=tooth_pitch, length=plastic_top_screw_height, segments_per_rot=SEGMENTS, 
                            neck_in_degrees=30, neck_out_degrees=30)
                        
    c = cylinder( r=inner_rad + .05, h=plastic_top_screw_height )
    if reverse==False:
        top=s+c    
    else:
        top = up(pcb_gap/2)(cylinder(r=top_outside_diameter/2, h=plastic_top_screw_height-pcb_gap ))
        top = top - (s+c)
    top = down(plastic_top_screw_height/2+pcb_thickness)(top)
    return up(top_height-min_wall_free-touch_pads_height-pcb_thickness)(top)

def plastic_connector_bottom(gap=pcb_gap):
# top_height
    bottom_height = top_height-min_wall_free-touch_pads_height-pcb_thickness-plastic_top_screw_height
    bottom_height_extra = bottom_height + min_wall_free
    c = cylinder( r=top_inside_diameter/2 - gap, h=bottom_height )
    c = c + cylinder( r=plastic_connector_dia/2 - gap, h=bottom_height_extra )
    return c

def plastic_battery_hole():
    height = top_height
    return cylinder( r=plastic_battery_hole_diameter/2, h=top_height)

def plastic_connector():
    return plastic_connector_bottom() + plastic_connector_top(0.35) - plastic_battery_hole() - cuff_bottom_disc()


def cuff_bottom_disc(gap=0, reverse=False):
    # plastic_battery_hole_diameter/2

    inner_rad = plastic_battery_hole_diameter/2 - pcb_gap
    section = screw_thread.default_thread_section( tooth_height=tooth_height, tooth_depth=tooth_depth)   
    s = screw_thread.thread( outline_pts=section, inner_rad = inner_rad,
                            pitch=tooth_pitch, length=plastic_top_screw_height, segments_per_rot=SEGMENTS, 
                            neck_in_degrees=30, neck_out_degrees=30)
                        

    c = cylinder( r=plastic_battery_hole_diameter/2 - pcb_gap, h=plastic_top_screw_height)
    h = up(plastic_top_screw_height/2)(cylinder( r=plastic_battery_hole_diameter/2 - pcb_gap - min_wall_free, h=plastic_top_screw_height/2))
    print plastic_battery_hole_diameter/2 - pcb_gap - min_wall_free
    return c + s - h

def cuff_bottom_bat():
    # lower_bar_length
    b = cylinder( r=min_wire_free, h=lower_bar_length)
    r = cylinder( r=min_wire_free*1.5, h=min_wire_free)
    b = b + r + up(lower_bar_length/4+min_wire_free/2)(r) + up(lower_bar_length-min_wire_free*2+min_wire_free)(r) + up(lower_bar_length-min_wire_free*2-lower_bar_length/4+min_wire_free/2)(r)
    return b

def cuff_bottom_stork():
    # Could be r=min_wire_free/2 but wanted strength
    c = cylinder( r=min_wire_free, h=stork_length)
    b = left(lower_bar_length/2) ( rotate([90,0,90]) ( cuff_bottom_bat() ) )

    # min_wire_free
    return down(stork_length-min_wall_free)(c+b)

def cuff_bottom():
    return cuff_bottom_disc() + cuff_bottom_stork()


def assembly():
   cutaway = down(20)(cube([40,40,40]))
   t = top()
   c = plastic_connector()
   p = color(Green)(up(5.15)(pcb()))
   b = cuff_bottom()

#   return  c + right(22)(c) 
#   return  t + p  - cutaway
   return  b  
#   return  t + b + right(22)(t + b) 

if __name__ == '__main__':    
    parser = argparse.ArgumentParser()
    parser.add_argument('-s', action='store', dest='openscad',
                    help='Openscad file name')
    parser.add_argument('-t', action='store_true', default=False,
                    dest='top',
                    help='Include the top of the cufflink')
    parser.add_argument('-b', action='store_true', default=False,
                    dest='bottom',
                    help='Include the bottom of the cufflink')
    parser.add_argument('-c', action='store_true', default=False,
                    dest='connector',
                    help='Include the connector of the cufflink')
    parser.add_argument('-p', action='store_true', default=False,
                    dest='pcb',
                    help='Include the connector of the cufflink')
    parser.add_argument('-w', action='store_true', default=False,
                    dest='cutaway',
                    help='Cutaway one quarter of the cufflink(s)')
    parser.add_argument('-d', action='store_true', default=False,
                    dest='double',
                    help='Two sets of output')

    parser.add_argument('--version', action='version', version='%(prog)s 1.0')
    options = parser.parse_args()

    a = union()
    if options.top:
        a = a + top()
    if options.bottom:
        a = a + cuff_bottom()
    if options.connector:
        a = a + plastic_connector()
    if options.pcb:
        a = a + color(Green)(up(5.15)(pcb()))
    if options.cutaway:
        a = a - down(20)(cube([40,40,40]))
    if options.double:
        a = a + right(20)(a) 

    scad_render_to_file( a, options.openscad, include_orig_code=True)

