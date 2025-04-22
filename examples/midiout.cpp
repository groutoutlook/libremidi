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
    // Yield MIDI event for processing, allowing external control over execution timing
    co_yield std::make_tuple(time, note, velocity);
    // Check if it's time to play the note
    while (std::chrono::steady_clock::now() < start_time + std::chrono::milliseconds(time))
      std::this_thread::yield(); // Allow other tasks to proceed

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

std::vector<std::tuple<int, int, int>> happy_birthday_harmony = {
    // Opening phrase
    {0, 60, 127}, {0, 64, 90},  // C and E (harmony)
    {400, 60, 127}, {400, 67, 80}, // C and G
    {800, 62, 127}, {800, 65, 100},  // D and F
    {1200, 60, 127}, {1200, 64, 90},  // C and E
    {1600, 65, 127}, {1600, 69, 80}, // F and A
    {2000, 64, 127}, {2000, 72, 90}, // E and C (higher)

    // Second phrase
    {2800, 60, 127}, {2800, 64, 85}, // C and E
    {3200, 60, 127}, {3200, 67, 80}, // C and G
    {3600, 62, 127}, {3600, 65, 100}, // D and F
    {4000, 60, 127}, {4000, 64, 90}, // C and E
    {4400, 67, 127}, {4400, 72, 85}, // G and C (higher)
    {4800, 65, 127}, {4800, 69, 80}, // F and A

    // Third phrase - climactic high note
    {5600, 60, 127}, {5600, 64, 85}, // C and E
    {6000, 60, 127}, {6000, 67, 80}, // C and G
    {6400, 72, 127}, {6400, 76, 80}, // High C and F (powerful ending)
    {6800, 69, 127}, {6800, 65, 90}, // A and F
    {7200, 65, 127}, {7200, 72, 85}, // F and C
    {7600, 64, 127}, {7600, 67, 80}, // E and G
    {8000, 62, 127}, {8000, 69, 85}, // D and A

    // Final closing phrase
    {8800, 70, 127}, {8800, 65, 80}, // Bb and F
    {9200, 70, 127}, {9200, 64, 85}, // Bb and E
    {9600, 69, 127}, {9600, 72, 80}, // A and C
    {10000, 65, 127}, {10000, 69, 90}, // F and A
    {10400, 67, 127}, {10400, 72, 85}, // G and C
    {10800, 65, 127}, {10800, 69, 80} // F and A
};

  auto midi_gen = playMidiSequence(midiout, happy_birthday_harmony);

  // Process MIDI events asynchronously
  for (auto [time, note, velocity] : midi_gen)
  {
    std::cout << "Playing: " << note << " at " << time << "ms\n";
  }
  // Control Change: 176, 7, 40
  midiout.send_message(176, 7, 40);

  std::this_thread::sleep_for(500ms);

  // Sysex: 240, 67, 4, 3, 2, 247
  midiout.send_message(std::to_array<unsigned char>({240, 67, 4, 3, 2, 247}));

  return 0;
}
