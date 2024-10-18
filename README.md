# kr55-midi
Midi modification for Korg KR-55


## Overview

This is a comprehensive mod for Korg KR-55, with:
- USB MIDI input for both sound trigger and time sync 
- Individual out were made as in https://tubbutec.de/blog/individual_outputs_kr_55/
- Amp disable when not playing disable
- Tom/claves pitch knobs

But sorry, no schematics...

## Controller

Controller used is a Teensy 3.2, with Arduino, using Serial+MIDI profile.
Each control line is connected to Teensy using a 74LS245 chip, with out being in either High-Z or low when triggered. Electronics concept is really simple. 
The trigger signals are connected to original wire straps, like Y-cable.

We also take The clock in from the KR55 circuitry and always route it via Teensy. 
Technically its done like the clock section in this: https://tubbutec.de/blog/midi-and-sync-for-korg-kr-55-unipulse-installation-guide/

## Other mods
- Tomtom pitch knobs are made by just putting 220R pots in place of trimmers
- Amp disable: Remove R183, and reuse the resistor to pull up D31 to 5V. See photo. 

## Software

Arduino software as simple as it gets. Made with Teensy's USBMIDI library. Changing MIDI mappings of the sounds would be trivial.

