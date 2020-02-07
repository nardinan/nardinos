#include "../drivers/ports.h"
#include "../drivers/video/raw_video.h"
#include "standard_lib/memory.h"
void print_memory_status(s_raw_video_configuration *screen) {
  char memory_description[1024];
  for (int index = 0; 
      (index < MEMORY_MAXIMUM_FRAGMENTS) &&
      (memory.fragments[index].flags & e_memory_flag_memory_initialized); 
      ++index) {
    int bytes = (int)(memory.fragments[index].end - memory.fragments[index].start);
    string_create(memory_description,
        "[%d] memory from %p to %p (status = %s) %dbytes\n", index, memory.fragments[index].start, memory.fragments[index].end,
        (memory.fragments[index].flags & e_memory_flag_memory_taken)?"taken":"free", bytes);
    raw_video_print_string(screen, memory_description);
  }
  raw_video_print_string(screen, "-----\n");
}
void main() {
  s_raw_video_configuration screen;
  raw_video_set(&screen, RAW_VIDEO_CREATE_FORMAT(RAW_VIDEO_BLACK, RAW_VIDEO_WHITE), 
      (e_raw_video_flag_scrollable | e_raw_video_flag_advance_cursor));
  raw_video_clear_screen(&screen);
  char *string_1 = (char *)memory_alloc(60);
  char *string_2 = (char *)memory_alloc(60);
  char *string_3 = (char *)memory_alloc(80);
  print_memory_status(&screen);
  char buffer[512];
  string_create(buffer, "empty pointer %p\n", string_2);
  raw_video_print_string(&screen, buffer);
  memory_free(string_2);
  print_memory_status(&screen);
  char *string_4 = (char *)memory_alloc(20);
  string_create(buffer, "allocating 20 bytes\n");
  raw_video_print_string(&screen, buffer);
  print_memory_status(&screen);
  string_create(buffer, "emptying last string allocated %p\n", string_4);
  raw_video_print_string(&screen, buffer);
  memory_free(string_4);
  print_memory_status(&screen);
  memory_free(string_1);
  //print_memory_status(&screen);
}
