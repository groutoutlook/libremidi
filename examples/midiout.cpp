//*****************************************//
//  midiout.cpp
//  by Gary Scavone, 2003-2004.
//
//  Simple program to test MIDI output.
//
//*****************************************//

#include "utils.hpp"

#include <libremidi/libremidi.hpp>

#include <array>
#include <chrono>
#include <thread>

int main(int argc, const char** argv)
{
  using namespace std::literals;

  // Read command line arguments
  libremidi::examples::arguments args{argc, argv};

  libremidi::midi_out midiout{{}, libremidi::midi_out_configuration_for(args.api)};

  // Call function to select port.
  if (!args.open_port(midiout))
    return 1;

  // Send out a series of MIDI messages.

  // Program change: 192, 5
  midiout.send_message(192, 5);

  std::this_thread::sleep_for(500ms);

  midiout.send_message(0xF1, 60);

  // Control Change: 176, 7, 100 (volume)
  midiout.send_message(176, 7, 100);

  // Define notes and velocities
  std::vector<std::tuple<int, int>> notes = {
      {60, 100}, // C (Middle C)
      {62, 90},  // D
      {64, 95},  // E
      {65, 110}, // F
      {67, 105}, // G
      {69, 100}, // A
      {71, 95},  // B
      {72, 120}, // C (Octave Higher)
      {74, 90},  // D#
      {76, 85},  // F
      {77, 100}, // G#
      {79, 110}, // B
  };

  for (const auto& [note, velocity] : notes)
  {
    // Note On
    midiout.send_message(144, note, velocity);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Note Off
    midiout.send_message(128, note, velocity / 2);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  std::this_thread::sleep_for(500ms);

  // Control Change: 176, 7, 40
  midiout.send_message(176, 7, 40);

  std::this_thread::sleep_for(500ms);

  // Sysex: 240, 67, 4, 3, 2, 247
  midiout.send_message(std::to_array<unsigned char>({240, 67, 4, 3, 2, 247}));

  return 0;
}
