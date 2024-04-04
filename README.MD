# Table of contents
- [Table of contents](#table-of-contents)
- [1.1 Introduction:](#11-introduction)
- [1.2 Demonstration:](#12-demonstration)
  - [1.2.1 English:](#121-english)
  - [1.2.1 Swedish:](#121-swedish)
- [2 Design and process:](#2-design-and-process)
  - [2.1 Arduino](#21-arduino)
  - [2.2 KiCad](#22-kicad)
    - [2.2.1 Schematics:](#221-schematics)
      - [2.2.1.1 PCB rev0](#2211-pcb-rev0)
      - [2.2.1.2 PCB rev1](#2212-pcb-rev1)
    - [2.2.2 Custom Footprint SPDT-Switch:](#222-custom-footprint-spdt-switch)
    - [2.2.3 Custom Footprint 7-segment Display:](#223-custom-footprint-7-segment-display)
  - [2.3 FreeCAD](#23-freecad)
- [3 Assembly:](#3-assembly)
- [4 Programming and bootloading:](#4-programming-and-bootloading)

________________

# 1.1 Introduction:
This project was created to teach my niece the wonders of electicity and programming.
Here is a video trying to give and project overview and explain the process that made me go from 64 bit memory board to a "space-machine":<br>
[![8 Switch RGB project overview and presentation](https://img.youtube.com/vi/f2jGDhYTRMw/0.jpg)](https://www.youtube.com/watch?v=f2jGDhYTRMw)

# 1.2 Demonstration: 
## 1.2.1 English:
Using Revision 0:<br>
[![English demonstration revision 0](https://img.youtube.com/vi/hgyK7DbGRng/0.jpg)](https://www.youtube.com/watch?v=hgyK7DbGRng)


## 1.2.1 Swedish:
Better quality than the english one, using PCB revision 1:
[![Swedish demonstration revision 1](https://img.youtube.com/vi/zpn5mNTmlIY/0.jpg)](https://www.youtube.com/watch?v=zpn5mNTmlIY)

___________
# 2 Design and process:
## 2.1 Arduino

## 2.2 KiCad
### 2.2.1 Schematics:
The only major differences between the two schematics are that I have a filter on the data and clock-lines in revision 1. I also relabeled the decimal-displays from order "dec-2" to "dec-0" starting from upper left corner in revision 0, to the reversed (and ISO-standard) way of counting/indexing from 0 in the upper left corner.

I also left-out the fuse and some other things in order to conserve space.
#### 2.2.1.1 PCB rev0
Will not publish Gerbers because this version should never ever be built or produced, as can be viewed in this 30 second clip:
[![Clip showing the horror of revision 0](./images/thumbnail_clip_rev0.png)](https://www.youtube.com/clip/UgkxvULYqhLVdSH0bBzPMFzRGHbVHcaSfia2)
![alt text](images/schematic_revision_0.png)

#### 2.2.1.2 PCB rev1
Gerbers availible for order in this directory:
https://github.com/MMDavito/8_switch_rgb/tree/master/KiCad/8_switch_rgb_pcb_revision_1/gerber_to_order
![PCB revision 1 Schematic](images/8_switch_rgb_rev1_schematic.svg)

### 2.2.2 Custom Footprint SPDT-Switch:
The SPDT switch was purchased from digikey: https://www.digikey.se/sv/products/detail/adam-tech/SW-T3-2B-A-A3-S1/15284416 <br>
And later electrokit: https://www.electrokit.com/vippomkopplare-sub-miniatyr-1-pol-on-on-lodoron-smts-102

These had to be changed, since the footprints I found where for the larger 6-Amp switch. If I where to refactor the PCB I would probably make the soldering-pads 20% larger for this footprint, or even more if DRC/Design-Rules-Checker would allow it.
The custom footprint can be found here: https://github.com/MMDavito/8_switch_rgb/tree/master/KiCad/resources/Switch_Custom.pretty
### 2.2.3 Custom Footprint 7-segment Display:
The display used was a 14mm CA display purchased in RED here: https://www.electrokit.com/en/led-display-7-seg-14mm-superrod-ca <br>
And green from here: https://www.electrokit.com/en/led-display-7-seg-14mm-gron-ca

The schematics, PCB and footprint label it as a HDSP-511, which is incorrect, buying HDSP-511 would lead to buying a display too tall and wide to fit.
The edited footprint and 3D .step file can be found here:
https://github.com/MMDavito/8_switch_rgb/tree/master/KiCad/resources/HDSP-511Y
## 2.3 FreeCAD

____________
# 3 Assembly:
Will be covered in a later update somewhere before June 2024.
_____________
# 4 Programming and bootloading:
