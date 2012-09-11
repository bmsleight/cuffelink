/* Needs a really good clean-up */

/* 
 Traffic Light Cufflinks case_parts
 (c) Brendan M. Sleight 2012
 This file is part of Cuffelink.com project
 
 Cuffelink is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 Cuffelink is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Cuffelink.  If not, see <http://www.gnu.org/licenses/>.
 
 Used Watchdog Sleep Examples 
 * KHM 2008 / Lab3/  Martin Nawrath nawrath@khm.de
 * Kunsthochschule fuer Medien Koeln
 * Academy of Media Arts Cologne
 *
 * Modified on 5 Feb 2011 by InsideGadgets (www.insidegadgets.com)
 * to suit the ATtiny85 and removed the cbi( MCUCR,SE ) section 
 
 */


use <Thread_Library.scad>

$fn=36;
alpha = 0.2725;

width_max = 20;

body_wall_thickness = 1.6;
pcb_gap = 0.25;
pcb_thickness = 1.6;
pcb_diameter = 14.5;
battery_thickness = 2.7;
battery_diameter = 9.5;
wire_diameter = 1.23;
chip_thickness = 1.1;
led_height= 0.6;
need_to_make_it_two_lazer_cut_thickness = 1.2;
plastic_connector_thickness = body_wall_thickness +battery_thickness+.5 + need_to_make_it_two_lazer_cut_thickness;
plastic_connector_inner_thickness = 3;
plastic_connector_battery_space = battery_thickness + wire_diameter/2;
wiggle_room = 2;
plastic_battery_hole_diameter = battery_diameter + wire_diameter + wiggle_room;
cuff_sides_height = 10.5 + need_to_make_it_two_lazer_cut_thickness;;
lower_cuff_width = 5;
lower_stock_length = 12+body_wall_thickness*2;
lower_bar_length = 12;
led_hole = 1.5;
hood_diameter = 2;
hood_height = 1;
touch_pads_height = 3.;
touch_pads_cube_height = 1.6;
touch_pads_diameter = wire_diameter; //Not 3
touch_pads_offset = 5.5;

steps_per_turn = 60;

module eecho(text) {
    echo(text);
}

module triangle(o_len, a_len, depth)
{
    linear_extrude(height=depth)
    {
        polygon(points=[[0,0],[a_len,0],[0,o_len]], paths=[[0,1,2]]);
    }
}

module battery_cr937() {
    color("silver", alpha) {
        translate([0,0,0.6/2]) cylinder(h=2.1, r=battery_diameter/2, center=true);
        translate([0,0,-2.1/2]) cylinder(h=0.6, r=7.4/2, center=true);
    }
}

module pcb_base() {
    color("purple", alpha) {
        cylinder(h=pcb_thickness, r=pcb_diameter/2, center=true);
   }
}

module ground_wire() {
    rotate([0,180,0]) color("olive", alpha) {
        cylinder(h=2.7, r=(wire_diameter/2), center=true);
        translate([0,2.7/2,2.7/2]) rotate([90,90,0]) cylinder(h=2.7, r=(wire_diameter/2), center=true);
   }
}

module chip() {
    // Led = 1.6 x 0.8 x 0.6
    color("black", alpha) {
        cube(size = [3.05, 6.4, chip_thickness], center = true);
    }
}

module led(colour) {
    // Led = 1.6 x 0.8 x 0.6
    color(colour, alpha) {
        cube(size = [1.6, 0.8, led_height], center = true);
    }
}

module led_array(){
    translate([0,3,0])    led("Red");
    translate([0,0,0])    led("DarkOrange");
    translate([0,-3,0])    led("Green");
}

module electronics() {
    electronics_thickness = chip_thickness+pcb_thickness+battery_thickness+wire_diameter/2;

    translate([0,0,electronics_thickness/2-chip_thickness+led_height/2]) led_array();
    translate([-3.5,0,electronics_thickness/2-chip_thickness/2]) chip();
    translate([0,0,electronics_thickness/2-chip_thickness-pcb_thickness/2]) pcb_base();
    translate([0,wire_diameter/2,electronics_thickness/2-chip_thickness-pcb_thickness-battery_thickness/2]) battery_cr937();
    translate([0,-battery_diameter/2,electronics_thickness/2-chip_thickness-pcb_thickness-battery_thickness/2]) ground_wire();
    eecho(-battery_diameter/2,-pcb_thickness/2);
    eecho(electronics_thickness);
}


module hood() {
// Outside dia = hood_diameter
// Inside dia = led_hole
    difference() {
        cylinder(h=hood_height, r=hood_diameter/2, center=true);
        cylinder(h=hood_height*2, r=led_hole/2, center=true);
        translate([-hood_diameter,-hood_diameter/2,1/2]) rotate([0,90,0]) triangle(hood_diameter,1,hood_diameter*2);
        translate([0,0,-.1+(hood_diameter+hood_height)/2]) cube([hood_diameter,hood_diameter,hood_diameter], center=true);
    }
}

module rag_outline() {
// Width = 3 + 1 +1 + 1
// length = 9
// height = 1
// inner_thickness = 0.5
    difference() {
        cube([6,12,1], center=true);
        cube([6-(0.5*2),12-(0.5*2),1*2], center=true);
    }
}


module touch_pads() {
    cylinder(h=touch_pads_height, r=touch_pads_diameter/2, center=true);
    translate([0,0,(touch_pads_height/2)-touch_pads_cube_height/2])  cube([3,2,touch_pads_cube_height], center=true);
}


module top_cuff() {
    difference() {
        cylinder(h=body_wall_thickness, r=width_max/2, center=true);
        translate([0,0,0]) cylinder(h=body_wall_thickness*2, r=led_hole/2, center=true);
        translate([0,3,0]) cylinder(h=body_wall_thickness*2, r=led_hole/2, center=true);
        translate([0,-3,0]) cylinder(h=body_wall_thickness*2, r=led_hole/2, center=true);    
    }
    // Top marking, hoods and outline
    translate([0,0,(body_wall_thickness+hood_height)/2]) {
        translate([0,0,0]) hood();
        translate([0,3,0]) hood();
        translate([0,-3,0]) hood();
        rag_outline();
    }
    // Bottom marking, touch pad collums
    translate([0,0,-(body_wall_thickness+touch_pads_height)/2]) {
        translate([0,touch_pads_offset,0]) touch_pads();
        translate([0,-touch_pads_offset,0]) touch_pads();
    }


}




module side_cuff() {
    difference() {
        cylinder(h=cuff_sides_height, r=width_max/2, center=true);
        cylinder(h=cuff_sides_height*2, r=(width_max/2)-body_wall_thickness, center=true);
        translate([0,0, cuff_sides_height -1]) cylinder(h=cuff_sides_height, r=width_max/2-1, center=true); // Make top boarder a little less thick
    }

    translate([0,0,-plastic_connector_thickness/2+body_wall_thickness/2]) 
    difference() { 
        cylinder(h=body_wall_thickness, r=(width_max/2)-body_wall_thickness, center=true);

        translate([0,0,-body_wall_thickness/2])  
        trapezoidThreadNegativeSpace(
                length=body_wall_thickness,
                pitch=0.6,	
                pitchRadius=(width_max/2)-body_wall_thickness - .3,  // Opening - (pitch*HeightToPitch) 
                threadHeightToPitch=0.5,
                profileRatio=0.5,
                threadAngle=30,
                RH=true,
                clearance=0.1,
                backlash=0.1,
                stepsPerTurn=steps_per_turn
                );
    }

}


module cuff_body() {
     color("gold", alpha) {
     translate([0,0,+cuff_sides_height/2-body_wall_thickness/2-1]) top_cuff ();
     side_cuff();
    }
}


/*
module plastic_connector() {
    color("navy", alpha) difference() {
        cylinder(h=plastic_connector_thickness, r=width_max/2-body_wall_thickness, center=true);
        cylinder(h=plastic_connector_thickness*2, r=(width_max/2-body_wall_thickness)-1.6, center=true);

    }
    eecho("Plastic");
    eecho(width_max/2-body_wall_thickness);
    eecho(width_max/2-(body_wall_thickness)-plastic_connector_inner_thickness);
}
*/

module plastic_connector() {
    color("navy", alpha) {
     eecho("Outer r:");
     eecho(width_max/2-body_wall_thickness);
     eecho("Inner r:");
     eecho("4.4");
     eecho("Bottom hole r:");
     eecho(plastic_battery_hole_diameter/2);
     eecho("h=plastic_connector_thickness:");
     eecho(plastic_connector_thickness);
        difference() {
            cylinder(h=plastic_connector_thickness, r=width_max/2-body_wall_thickness, center=true);
            cylinder(h=plastic_connector_thickness*2, r=4.4, center=true);
            translate([0,0,plastic_connector_thickness/2]) cylinder(h=body_wall_thickness*2, r=width_max/2-body_wall_thickness, center=true);
            translate([-8.4,8.4,-plastic_connector_thickness/2+.5])  rotate([0,180,180]) linear_extrude(height = 0.5) import("base_for_text.dxf");
#          translate([0,0,plastic_connector_battery_space-.8]) cylinder(h=plastic_connector_battery_space*2, r=plastic_battery_hole_diameter/2, center=true);
        }

        translate([0,0,-(plastic_connector_thickness-1.6)/2]) difference() {
            cylinder(h=1.6, r=4.75, center=true);

            translate([0,0,-0.8])  trapezoidThreadNegativeSpace(
                length=1.6,
                pitch=0.6,	
                pitchRadius=4.1, 
                threadHeightToPitch=0.5,
                profileRatio=0.5,
                threadAngle=30,
                RH=true,
                clearance=0.1,
                backlash=0.1,
                stepsPerTurn=steps_per_turn
                );
        }

        difference() {
            translate([0,0,(plastic_connector_thickness)/2-body_wall_thickness])  
            cylinder(h=body_wall_thickness, r=(width_max/2)-body_wall_thickness - .3, center=true);
// Remove the tread - let the plastic be thread by the metal case
/*            trapezoidThread(
                length=body_wall_thickness,
                pitch=0.6,	
                pitchRadius=(width_max/2)-body_wall_thickness - .3,  // Opening - (pitch*HeightToPitch) 
                threadHeightToPitch=0.5,
                profileRatio=0.5,
                threadAngle=30,
                RH=true,
                clearance=0.1,
                backlash=0.1,
                stepsPerTurn=steps_per_turn
                );
*/
*            cylinder(h=plastic_connector_thickness*2, r=4.4, center=true);
            cylinder(h=plastic_connector_thickness*2, r=plastic_battery_hole_diameter/2, center=true);
        }
    }
}




module disc_cuff() {
    cylinder(h=body_wall_thickness, r=lower_cuff_width/2, center=true);
translate([0,0,-0.8]) trapezoidThread( 
                length=1.6,
                pitch=0.6,	
                pitchRadius=4.1, 
                threadHeightToPitch=0.5,
                profileRatio=0.5,
                threadAngle=30,
                RH=true,
                clearance=0.1,
                backlash=0.1,
                stepsPerTurn=steps_per_turn
                );
}

module stork_cuff() {
    cylinder(h=lower_stock_length, r=body_wall_thickness, center=true);
}

module bar_cuff() {
    rotate([90,0,90]) {
        cylinder(h=lower_bar_length, r=body_wall_thickness, center=true);
        translate([0,0,lower_bar_length/4]) cylinder(h=body_wall_thickness, r=body_wall_thickness+(body_wall_thickness/2), center=true);
        translate([0,0,lower_bar_length/2]) cylinder(h=body_wall_thickness, r=body_wall_thickness+(body_wall_thickness/2), center=true);
        translate([0,0,-lower_bar_length/4]) cylinder(h=body_wall_thickness, r=body_wall_thickness+(body_wall_thickness/2), center=true);
        translate([0,0,-lower_bar_length/2]) cylinder(h=body_wall_thickness, r=body_wall_thickness+(body_wall_thickness/2), center=true);
    }
}


module cuff_lower() {
    color("gold", alpha) {
        translate([0,0,-body_wall_thickness/2+lower_stock_length/2]) disc_cuff();
        stork_cuff();
        translate([0,0,-lower_stock_length/2]) bar_cuff();
    }
}


translate([0,0,0]) electronics();
translate([0,0,-cuff_sides_height/2+plastic_connector_thickness/2]) plastic_connector();
translate([0,0,0]) cuff_body();
translate([0,0,-cuff_sides_height/2-lower_stock_length/2+body_wall_thickness]) cuff_lower();

/*
translate([22,0,0]) {

translate([0,0,-.75]) electronics();
translate([0,0,-cuff_sides_height/2+plastic_connector_thickness/2]) plastic_connector();
translate([0,0,0]) cuff_body();
translate([0,0,-cuff_sides_height/2-lower_stock_length/2+body_wall_thickness]) cuff_lower();
}
*/