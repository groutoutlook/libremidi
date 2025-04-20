# libremidi

libremidi is a cross-platform C++20 library for real-time and MIDI file input and output.

Additionnally, for MIDI 2 parsing support we use [cmidi2](https://github.com/atsushieno/cmidi2)!

Read the documentation [here](https://celtera.github.io/libremidi).

## Changelog 

### Since v5.1

* Report USB device identifiers with ALSA and udev
* PipeWire and JACK UMP support (requires PipeWire v1.4+)

### Since v5
* Use stdx::error for error reporting until C++26 and std::error are widely available :-)
* Hunt exceptions down
* MIDI 2 support on Windows with the Windows MIDI Services.
  * Using Developer Preview 9: https://github.com/microsoft/MIDI/releases/
  * Works on both MSVC and MinGW / MSYS2.
* WinUWP support on MinGW / MSYS2.
* Getters for USB location, etc. in `libremidi::port_information`.
* Reverse-engineered implementation of [Mackie Control Universal & Logic Control protocols](https://github.com/celtera/libremidi/blob/master/include/libremidi/protocols/remote_control.hpp). Tested with [TouchMCU](https://github.com/NicoG60/TouchMCU) and a BCF2000.
* C API for bindings to other languages ([libremidi-c.h](https://github.com/celtera/libremidi/blob/master/include/libremidi/libremidi-c.h)).
* [Python binding](https://github.com/celtera/libremidi/tree/master/bindings/python).
* [Haskell binding](https://github.com/ejconlon/libremidi-haskell) courtesy of @ejconlon λ!
* [Java binding](https://github.com/atsushieno/libremidi-javacpp) courtesy of @atsushieno!
* Support for getting raw, unfiltered MIDI data (e.g. no SYSEX recombination logic).
* Computer Keyboard backend to easily map scancodes to keyboard-like MIDI maps ⌨.
* [Network backend](https://github.com/celtera/libremidi/blob/master/examples/network.cpp) to send MIDI 1 and UMP packets over OSC 🛜.
* libremidi finally supports MIDI 2 on all desktop platforms 🎉!

# They use this library

* [BlastThruReborn](https://github.com/Cacodemon345/BlastThruReborn)
* [BraneEngineDx](https://github.com/MC-117/BraneEngineDx)
* [chordcat](https://github.com/shriramters/chordcat)
* [EraeTouchLite](https://github.com/TheTechnobear/EraeTouchLite)
* [MIDIVisualizer](https://github.com/kosua20/MIDIVisualizer)
* [ossia.io](https://ossia.io)
* [obs-midi-mg](https://github.com/nhielost/obs-midi-mg)
* [SceneSwitcher](https://github.com/WarmUpTill/SceneSwitcher)
* [Senos](https://github.com/RuiVarela/Senos)
* [SynthMania](https://github.com/HyperLan-git/Synthmania)
* [zing](https://github.com/Rezonality/zing)
