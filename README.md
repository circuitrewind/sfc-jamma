# sfc-jamma

TLDR: SNES/SFC controller support on arcade machines!

This repository contains the Eagle CAD files as well as the Arduino Nano source to to interface a Super Famicom (SFC) or Super Nintendo (SNES) controller with an Arcade Cabinet JAMMA interface via a 40-pin ribbon cable.

The 40-pin ribbon cable pinout is based on the pinout used by the [EZ Switchers by RiddledTV](https://riddledtv.com/arcade/ez.html). The EZ 4-way Switcher and EZ 8-way Switcher provide both a JAMMA edge and 40-pin output connectors with the intent of having two cabinets wired into the same arcade board for head-to-head gameplay. I abused this fact to instead use the 40-pin cable for a secondary set of user input controls.

Because the wiring on the JAMMA interface and 40-pin interface are tied directly together, the Arduino Nano can also read input from the arcade's input panel, and then forward this information over the Arduino Nano's USB interface back to a PC, such as using it for gamepad emulation. This can be useful for running a PC through an arcade cabinet, and forwarding the input controls from the cabinet back into the PC.

## Tested Controllers
* Nintendo Super Famicom Controller: SHVC-005
* Brook Gaming Wingman SNES Converter: ZPP005Q
* 8BitDo Retro Receiver for SNES
* Capcom CP Soldier: CP-S01CAN
* HORI Fatal Fury 2 Commander Controller: HSG-07
* HORI Fighting Commander: HSJ-14
* Hudson Soft Super Joycard: HC-691
* Asciiware AsciiPad: AS-131-SP
* Asciiware Super Advantage: 4910
* Asciiware Fight Stick: AS-9981-SF
* STD SN ProPad: SV-334
* STD SN ProgramPad: SV-337
* Triax Turbo Touch 360
* Tyco Super NES Power Plug: 1286
* Konami Hissatsu Command Controller: RU-009


![SFC to JAMMA PCB](pcb/pcb.jpg?raw=true "SFC to JAMMA PCB")
