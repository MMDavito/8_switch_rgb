# Table of contents:
- [Table of contents:](#table-of-contents)
- [1 Introduction:](#1-introduction)
- [2 Holders:](#2-holders)
  - [2.1 The legacy from pcb revision 0:](#21-the-legacy-from-pcb-revision-0)
  - [2.2 The final version for pcb revision 1](#22-the-final-version-for-pcb-revision-1)
  - [2.3 The holder to display the failings of revision 0:](#23-the-holder-to-display-the-failings-of-revision-0)
- [3 Assembly Tools:](#3-assembly-tools)
  - [3.1 Mounting Frame:](#31-mounting-frame)
  - [3.2 Resistor Lead Bender:](#32-resistor-lead-bender)
  - [3.3 Template to mark drill-holes:](#33-template-to-mark-drill-holes)
  - [3.4 LED-matrix holder:](#34-led-matrix-holder)

__________
# 1 Introduction:
Originally I made cutouts in plywood to make a holder that didn't interfere with the soldering-points. This approach proved inefficient and inaccurate.

I therefore ordered holders that allowed to mount over these holes, these where unnecessarily big, but taught me about wall-thickness and showed me one can place m3-nuts in a hexagon pocket.
The pocket in question is a hexagon constrained by a circle with the diameter 6.5mm:
![hexagon constrained by 6.5mm diameter circle](images/holders/m3_constrained_by_circle.png)

# 2 Holders:
## 2.1 The legacy from pcb revision 0:
Since the first version of the PCB (revision 0) proved to not work very well, and revision 1 being 100x100mm instead of 120x120mm in size I needed something to make the holders/stand I had built and painted useable for revision 1.

Like mentioned in the introduction the first holder was to be mounted above a hole made in the first version of the plywood holders, so it needed to have a flat mounting outside that 100x100mm hole that was there to provide clearance for the 120x120mm pcb_rev0. Therefore this versions baseplate is 150x150mm in size, which is larger than the 100x100mm needed for revision 1:
![150mm with walls](images/holders/revision_1_with_walls.png)
This version also included walls and can be downloaded from [here](./holders/legacy/2-7mm_pocket_hex_1mm_bottom_1.stl).
The walls of this holder where meant to prevent a curious 3-year old from inserting a prying tool between the PCB and the surface it was mounted to.

The likelihood of such prying is minimal, but the added security that came with a minimal addition was something I deemed worthwhile.

## 2.2 The final version for pcb revision 1
Like mentioned in section 2.1 the first holder was bulky, and even though the walls added protection against prying-tools they added cost in materials.

So the version I ordered for the final version is instead meant to be mounted inside a 100x100mm hole cutout into 12mm thick plywood, this approach will make the top of the PCB remain slick against the top of the plywood, and make it seem to be part of the plywood.
![final version of holder, meant for use alongside 12mm plywood](images/holders/100mm_for_12mm_plywood.png)
The STL can be downloaded [here](./holders/2024-03-13_order/holder_100mm_12mm_plywood.stl). My use of measures and notation hint at how little I bothered to perfect my understanding of freecad; but I argue that one only needs to open [this file](./holders/Holder%20100mm.FCStd) and one can print any type of measurements at any time, this is one of the strength of opensource.

## 2.3 The holder to display the failings of revision 0:
Since I want to keep the failings as reminders of my ability to carry on and bring solutions, I also made a holder for the 120x120mm PCB revision 0 for mounting onto walls or holders/displays:
![holder for 120mm revision0](images/holders/120mm_rev0_display_only.png)
[The STL](./holders/2024-03-13_order/holder_120mm-SquareCorners_10mm.stl) and [the CAD](./holders/legacy/holder_120mm.FCStd).
# 3 Assembly Tools:

## 3.1 Mounting Frame:
To help increasing the speed of soldering the PCB, and more importantly, to hold the LEDs and toggle-switches straight I created this frame/holder:
[STL ordered: ](./assembling_tools/7_seg_rev1_100mm_mounting_frame.stl)
![Mounting Frame](images/assembling_tools/7_seg_rev1_mounting_frame.png)

## 3.2 Resistor Lead Bender:
Since I use both 1/4 watt and 1/8 watt resistors with shortest minimal hole distance I am forced to insert the lead one at a time, and then use a needle nose plier to tighten them to the board.

This approach will leave unnecessarily long leads when soldering, since with the frame described in section 3.1 I no longer need them to be long to be held in place.

Instead I will cut them to length (maybe using a 3d printed part not yet created) and then bend them. But this requires me a frame to help me bend them 90 degrees. I therefore created [this Resistor Lead Bender](./assembling_tools/lead_bending/resistor_bending_tool_ver_1_rev0-BodyPocket.stl), to allow me to bend the leads of 16 resistors at a time flush to the case of the resistors:
![Resistor Lead Bender](images/assembling_tools/lead_bending_tool.png)
This tool is tall enough to bend the standard resistor leads without needing to cut the leads to length until one has mounted and soldered them.

## 3.3 Template to mark drill-holes:
To make it more easy to drill holes consistent with the spacing of the LED-strip I had at hand (ws2812b 60 LED/meter), I made a matrix of 5 x 5 2 mm holes (2.5mm would been better), and 3mm holes for screws/nuts holding it in place of the previous line of holes made; allowing scaling infinitely to any direction allowing any size of LED-matrix, be that 1x15, 2x3 etc. 

![Led matrix, drill-frame](images/assembling_tools/led_matrix_drill_frame.png)
[Freecad file](./assembling_tools/led_matrix_drill_frame.FCStd), and the [STL](./assembling_tools/led_matrix_drill_frame-BodyPad.stl)

Here is a example of how to use it to mark drill-holes:
https://www.youtube.com/watch?v=ce8ECqhq-No&t=2s

## 3.4 LED-matrix holder:
To sync with the drill holes created from the markers made using the tool from [3.3 Template to mark drill-holes](#33-template-to-mark-drill-holes) the LED matrix holder was created, to allow for attachment to a surface in a way that aligned with holes made on another surface.
![LED-matrix Holder](./images/assembling_tools/led_matrix_holder.png)

Freecad file is available [here](./assembling_tools/led_matrix_holder_version2.FCStd) and the [STL here](./assembling_tools/led_matrix_holder_version2-BodyPad.stl)

Video explaining how this is used and how a vacuum-cleaner helps solving this useability can be found in this offset:ed video:
[!["Video-segment detailing how to use the LED-matrix-holder"](./images/assembling_tools/thumbnail_led_matrix_holder.png)](https://www.youtube.com/watch?v=ce8ECqhq-No&t=713s)