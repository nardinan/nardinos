#include "../drivers/ports.h"
#include "../drivers/video/raw_video.h"
void main() {
  s_raw_video_configuration screen;
  raw_video_set(&screen, RAW_VIDEO_CREATE_FORMAT(RAW_VIDEO_BLACK, RAW_VIDEO_WHITE), 
      (e_raw_video_flag_scrollable | e_raw_video_flag_advance_cursor));
  raw_video_clear_screen(&screen);
  raw_video_print_string(&screen, "Hello, I don't know what I am doing!\n");
  raw_video_print_string(&screen, "I have to be honest, I have no clue if this is going to work\n");
  raw_video_print_string(&screen, "What do you think?\x7f");

}
