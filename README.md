# sfc-jamma

TLDR: SNES/SFC controller support on arcade machines!

This repository contains the Eagle CAD files as well as the Arduino Nano source to to interface a Super Famicom (SFC) or Super Nintendo (SNES) controller with an Arcade Cabinet JAMMA interface via a 40-pin ribbon cable.

The 40-pin ribbon cable pinout is based on the pinout used by the [EZ Switchers by RiddledTV](https://riddledtv.com/arcade/ez.html). The EZ 4-way Switcher and EZ 8-way Switcher provide both a JAMMA edge and 40-pin output connectors with the intent of having two cabinets wired into the same arcade board for head-to-head gameplay. I abused this fact to instead use the 40-pin cable for a secondary set of user input controls.

Because the wiring on the JAMMA interface and 40-pin interface are tied directly together, the Arduino Nano can also read input from the arcade's input panel, and then forward this information over the Arduino Nano's USB interface back to a PC, such as using it for gamepad emulation. This can be useful for running a PC through an arcade cabinet, and forwarding the input controls from the cabinet back into the PC.


## Button Mappings
On Player 1, hold SELECT + START for two seconds to toggle TEST / SETTINGS button. This is set as a toggle rather than a momentary press because some games like Soul Edge require the button to remain held while in the settings menu.

On Player 2, hold SELECT + START for two seconds to toggle TILT button. P1 and P2 Arduino Nanos run on the same code, which is why this is also a held toggle function.

Press SELECT + UP to select Profile 1
Press SELECT + DOWN to select Profile 2

|SNES|Profile 1|Profile 2|
|---|---|---|
|SELECT|Insert Coin|Insert Coin|
|START|Player Start|Player Start|
|A|Button 5 (MK)|Button 6 (HK)|
|B|Button 4 (LK)|Button 4 (LK)|
|X|Button 2 (MP)|Button 3 (HP)|
|Y|Button 1 (LP)|Button 1 (LP)|
|L|Button 3 (HP)|Button 2 (MP)|
|R|Button 6 (HK)|Button 5 (MK)|


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
* High Frequency Clear Turbo Controller for SNES


## Images
![SFC to JAMMA PCB](pcb/pcb.jpg?raw=true "SFC to JAMMA PCB")
![SFC to JAMMA Board Layout](pcb/board-1.png?raw=true "SFC to JAMMA Board Layout")
![SFC to JAMMA Schematic](pcb/schematic-1.png?raw=true "SFC to JAMMA Schematic")

