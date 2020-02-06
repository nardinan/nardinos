#include "../drivers/ports.h"
#include "../drivers/video/raw_video.h"
#include "standard_lib/memory.h"
void main() {
  s_raw_video_configuration screen;
  raw_video_set(&screen, RAW_VIDEO_CREATE_FORMAT(RAW_VIDEO_BLACK, RAW_VIDEO_WHITE), 
      (e_raw_video_flag_scrollable | e_raw_video_flag_advance_cursor));
  raw_video_clear_screen(&screen);
  raw_video_print_string(&screen, "Hello, I don't know what I am doing!\n");
  raw_video_print_string(&screen, "I have to be honest, I have no clue if this is going to work\n");
  raw_video_print_string(&screen, "What do you think?\n");
  char *string_1 = (char *)memory_alloc(60),
       *string_2 = (char *)memory_alloc(60),
       *string_3 = (char *)memory_alloc(80);
  string_copy(string_1, "This has been allocated\n");
  string_copy(string_2, "this has been allocated as well\n");
  string_copy(string_3, "another allocation, then we are going to free it\n");
  raw_video_print_string(&screen, string_1);
  raw_video_print_string(&screen, string_2);
  raw_video_print_string(&screen, string_3);
  memory_free(string_2);
  char *string_4 = (char *)memory_alloc(20);
  raw_video_print_string(&screen, string_4);
}
