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
#include <coroutine>
#include <generator>
#include <thread>
// Coroutine for scheduling notes
struct MidiEvent
{
  struct promise_type
  {
    MidiEvent get_return_object() { return MidiEvent{}; }
    std::suspend_always initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() { }
    void unhandled_exception() { std::terminate(); }
  };
};

// Generator function to yield scheduled MIDI events
auto playMidiSequence(
    libremidi::midi_out& midiout, const std::vector<std::tuple<int, int, int>>& sequence)
    -> std::generator<std::tuple<int, int, int>>
{
  auto start_time = std::chrono::steady_clock::now();

  for (const auto& [time, note, velocity] : sequence)
  {
    while (std::chrono::steady_clock::now() < start_time + std::chrono::milliseconds(time))
    {
      co_yield std::make_tuple(time, note, velocity); // Yield event for processing
    }
    midiout.send_message(144, note, velocity); // Note On
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    midiout.send_message(128, note, velocity / 2); // Note Off
  }
}

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

  std::vector<std::tuple<int, int, int>> happy_birthday = {
      {0, 60, 100},    // C
      {400, 60, 100},  // C
      {800, 62, 100},  // D
      {1200, 60, 100}, // C
      {1600, 65, 100}, // F
      {2000, 64, 100}, // E

      {2800, 60, 100}, // C
      {3200, 60, 100}, // C
      {3600, 62, 100}, // D
      {4000, 60, 100}, // C
      {4400, 67, 100}, // G
      {4800, 65, 100}, // F

      {5600, 60, 100}, // C
      {6000, 60, 100}, // C
      {6400, 72, 100}, // High C
      {6800, 69, 100}, // A
      {7200, 65, 100}, // F
      {7600, 64, 100}, // E
      {8000, 62, 100}, // D

      {8800, 70, 100},  // Bb
      {9200, 70, 100},  // Bb
      {9600, 69, 100},  // A
      {10000, 65, 100}, // F
      {10400, 67, 100}, // G
      {10800, 65, 100}  // F
  };

  auto midi_gen = playMidiSequence(midiout, happy_birthday);

  // Process MIDI events asynchronously
  for (auto [time, note, velocity] : midi_gen)
  {
    std::cout << "Playing: " << note << " at " << time << "ms\n";
  }
  std::this_thread::sleep_for(500ms);

  // Control Change: 176, 7, 40
  midiout.send_message(176, 7, 40);

  std::this_thread::sleep_for(500ms);

  // Sysex: 240, 67, 4, 3, 2, 247
  midiout.send_message(std::to_array<unsigned char>({240, 67, 4, 3, 2, 247}));

  return 0;
}
