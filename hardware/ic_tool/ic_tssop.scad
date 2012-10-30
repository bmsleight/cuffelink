
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

chip();
leg_near_side();
leg_far_side();
