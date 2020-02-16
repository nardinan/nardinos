#include "../drivers/ports.h"
#include "../drivers/video/raw_video.h"
#include "../drivers/keyboard/keyboard.h"
#include "../interrupts/interrupts_signals.h"
#include "../interrupts/timer.h"
#include "standard_lib/standard_lib.h"
void main() {
  raw_video_set(&standard_output, RAW_VIDEO_CREATE_FORMAT(RAW_VIDEO_BLACK, RAW_VIDEO_WHITE), 
      (e_raw_video_flag_scrollable | e_raw_video_flag_advance_cursor));
  raw_video_clear_screen(&standard_output);
  interrupts_signals_register();
  asm volatile("sti");
  timer_set(50);
  keyboard_set();
}
