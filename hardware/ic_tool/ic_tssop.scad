
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
  
 */


// 360 and 180 normally
$fn=36;
steps_per_turn = 18;
//alpha = 0.2725;
alpha = 1;

chip_height = 1.10;
chip_width = 4.4;
chip_length = 3.04;

leg_pitch=0.65;
leg_height = 0.1;
leg_width=0.275;
leg_end_length=0.6;
leg_start_length=0.2;

leg_mid_real_height=(chip_height/2)-(leg_height/2);
leg_mid_length=0.2;
// tan t = opp/adj = leg_mid_length/leg_mid_real_height
leg_mid_angle=atan(leg_mid_length/leg_mid_real_height);
echo(leg_mid_angle);

leg_total_length=leg_end_length + leg_start_length + leg_mid_length;
leg_pitch_total=leg_pitch*3;


module chip() 
{
    color("black", alpha) 
    {
        cube(size = [chip_length, chip_width, chip_height], center = true);
    }
}


module leg_end() 
{
    cube(size = [leg_end_length, leg_width, leg_height], center = true);
}

module leg_start() 
{
    cube(size = [leg_start_length, leg_width, leg_height], center = true);
}

module leg_mid() 
{
    // Cos t = adj/hyp
    // hyp = adj/cos t = leg_mid_real_height / cos (leg_mid_angle)
    rotate(a=[0,90-leg_mid_angle,0]) cube(size = [leg_mid_real_height * cos(leg_mid_angle), leg_width, leg_height], center = true);
}

module leg()
{
    rotate([0,0,270]) 
    {
        translate([leg_total_length/2-leg_end_length/2,0,-chip_height/4]) leg_end();
        translate([-leg_total_length/2+leg_start_length/2,0,+chip_height/4]) leg_start();
        translate([-leg_total_length/2+leg_mid_length/2+leg_start_length,0,0]) leg_mid();
    }
}

module leg_near_side()
{
    translate([0,-chip_width/2-leg_total_length/2,-chip_height/4+leg_height/2])
    {
    //leg_pitch_total
      translate([-leg_pitch*3/2,0,0]) leg();
      translate([-leg_pitch/2,0,0]) leg();
      translate([leg_pitch*3/2,0,0]) leg();
      translate([+leg_pitch/2,0,0]) leg();
    }
}

module leg_far_side()
{
    translate([0,+chip_width/2+leg_total_length/2,-chip_height/4+leg_height/2])  rotate([0,0,180])
    {
    //leg_pitch_total
      translate([-leg_pitch*3/2,0,0])leg();
      translate([-leg_pitch/2,0,0]) leg();
      translate([leg_pitch*3/2,0,0]) leg();
      translate([+leg_pitch/2,0,0]) leg();
    }
}


module wire_inner()
{
    cylinder(h=3, r=(0.2), center=true);
}

/*
module wire_curve()
{
    translate([0,0,5]) rotate(a=[0,90,0])
    {
        difference() 
        {
            rotate_extrude(convexity = 10, $fn = 100) translate([2, 0, 0]) circle(r = 0.2, $fn = 100);
           translate([3,0,0]) cube([6,6,6], center=true);
           translate([-4,-2,0]) rotate(a=[0,0,45]) cube([8,8,8], center=true);
        }
    }
}
*/


module wire_curve()
{
    translate([0,0,5]) rotate(a=[0,90,0])
    {
        difference() 
        {
            rotate_extrude(convexity = 10, $fn = 100) translate([2, 0, 0]) circle(r = 0.2, $fn = 100);
           translate([3,0,0]) cube([6,6,6], center=true);
           translate([-3,-3,0]) cube([6,6,6], center=true);
        }
    }
}


module wire_ends()
{
    translate([-leg_pitch/2,-9.2,2.3]) rotate(a=[-45-leg_mid_angle,180,0])   wire_curve();
    translate([-leg_pitch*3/2,-2.25,1.]) rotate(a=[-leg_mid_angle,0,0])   wire_inner();
    translate([leg_pitch*3/2,-9.2,2.3]) rotate(a=[-45-leg_mid_angle,180,0])   wire_curve();
    translate([leg_pitch/2,-2.25,1.]) rotate(a=[-leg_mid_angle,0,0])   wire_inner();
}

module hollows()
{
wire_ends();
rotate(a=[0,0,180]) wire_ends();

translate([0,0,0]) cube([chip_length+1,chip_width*1.05,chip_height*1.5], center=true);

translate([0,-2.1,0]) rotate(a=[-leg_mid_angle,0,0])   cube([chip_length+1,1,2], center=true);

rotate(a=[0,0,180]) translate([0,-2.1,0]) rotate(a=[-leg_mid_angle,0,0])   cube([chip_length+1,1,2], center=true);
}

module solid_s()
{
    translate([0,0,1.66]) cube([4,7,4], center=true);
}


*chip();
*leg_near_side();
*leg_far_side();

difference() 
{
  solid_s();
  hollows();
}
*wire_curve();