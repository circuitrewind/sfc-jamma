# sfc-jamma

TLDR: SNES/SFC controller support on arcade machines!

This repository contains the Eagle CAD files as well as the Arduino Nano source to to interface a Super Famicom (SFC) or Super Nintendo (SNES) controller with an Arcade Cabinet JAMMA interface via a 40-pin ribbon cable.

The 40-pin ribbon cable pinout is based on the pinout used by the [EZ Switchers by RiddledTV](https://riddledtv.com/arcade/ez.html). The EZ 4-way Switcher and EZ 8-way Switcher provide both a JAMMA edge and 40-pin output connectors with the intent of having two cabinets wired into the same arcade board for head-to-head gameplay. I abused this fact to instead use the 40-pin cable for a secondary set of user input controls.

Because the wiring on the JAMMA interface and 40-pin interface are tied directly together, the Arduino Nano can also read input from the arcade's input panel, and then forward this information over the Arduino Nano's USB interface back to a PC, such as using it for gamepad emulation. This can be useful for running a PC through an arcade cabinet, and forwarding the input controls from the cabinet back into the PC.

![SFC to JAMMA PCB](pcb/pcb.jpg?raw=true "SFC to JAMMA PCB")
