#include "raw_video.h"
void raw_video_set(s_raw_video_configuration *video_configuration, 
    unsigned char character_format, unsigned char flags) {
  memory_set((unsigned char *)video_configuration, 0, sizeof(s_raw_video_configuration));
  video_configuration->_memory_video_entry_point = (unsigned char *)RAW_VIDEO_MEMORY_LOCATION;
  video_configuration->_maximum_cols = RAW_VIDEO_MAXIMUM_COLUMNS;
  video_configuration->_maximum_rows = RAW_VIDEO_MAXIMUM_ROWS;
  video_configuration->format = character_format;
  video_configuration->flags = flags;
}
void raw_video_print_byte(s_raw_video_configuration *video_configuration, char data) {
  if (video_configuration->_maximum_cols > video_configuration->col)
    if (video_configuration->_maximum_rows > video_configuration->row) {
      /* we need to calculate the memory offset but, we need to 
       * keep in mind the fact that each character is a pair of
       * bytes: the first is the ASCII code and the second one is the format */
      unsigned char *memory_position = video_configuration->_memory_video_entry_point +
        (video_configuration->row * (2 * video_configuration->_maximum_cols)) +
        (2 * video_configuration->col);
      memory_position[0] = data;
      memory_position[1] = video_configuration->format;
      if ((video_configuration->flags & e_raw_video_flag_advance_cursor)) {
        raw_video_cursor(video_configuration, e_raw_video_cursor_action_step_forward);
      }
    }
}
void raw_video_scroll(s_raw_video_configuration *video_configuration, unsigned int rows) {
  size_t memory_offset = (rows * (video_configuration->_maximum_cols * 2)), total_memory;
  if ((total_memory = ((video_configuration->_maximum_cols * video_configuration->_maximum_rows) * 2)) >
      memory_offset) {
    memory_move(video_configuration->_memory_video_entry_point,
        (video_configuration->_memory_video_entry_point + memory_offset),
        (total_memory - memory_offset));
    memory_set(video_configuration->_memory_video_entry_point + (total_memory - memory_offset), 
        0, memory_offset);
  } else
    memory_set(video_configuration->_memory_video_entry_point, 0, total_memory);
}
void raw_video_cursor(s_raw_video_configuration *video_configuration, 
    e_raw_video_cursor_actions action) {
  switch (action) {
    case e_raw_video_cursor_action_screen_home:
      video_configuration->row = 0;
    case e_raw_video_cursor_action_line_home:
      video_configuration->col = 0;
      break;
    case e_raw_video_cursor_action_step_back:
      if (video_configuration->col == 0) {
        if (video_configuration->row > 0) {
          --(video_configuration->row);
          video_configuration->col = (video_configuration->_maximum_cols - 1);
        }
      } else
        --(video_configuration->col);
      break;
    case e_raw_video_cursor_action_step_forward:
      if ((++(video_configuration->col)) >= video_configuration->_maximum_cols) {
        ++(video_configuration->row);
        video_configuration->col = 0;
      }
      break;
    case e_raw_video_cursor_action_line_forward:
      ++(video_configuration->row);
      break;
  }
  if ((video_configuration->_maximum_rows <= video_configuration->row) && 
      ((video_configuration->flags & e_raw_video_flag_scrollable))) {
    size_t rows_offset = ((video_configuration->row - video_configuration->_maximum_rows) + 1);
    raw_video_scroll(video_configuration, rows_offset);
    video_configuration->col = 0;
    video_configuration->row = (video_configuration->_maximum_rows - 1);
  }
}
void raw_video_print_string(s_raw_video_configuration *video_configuration, const char *buffer) {
  int backup_flags;
  while (*buffer != 0) {
    switch (*buffer) {
      case RAW_VIDEO_LINE_FEED:
        /* we are considering the line feed as a new line plus the line 
         * home call, as the line feed character on linux and not the 
         * line feed character on BIOS */
        raw_video_cursor(video_configuration, e_raw_video_cursor_action_line_forward);
        raw_video_cursor(video_configuration, e_raw_video_cursor_action_line_home);
        break;
      case RAW_VIDEO_CARRIAGE_RETURN:
        raw_video_cursor(video_configuration, e_raw_video_cursor_action_line_home);
        break;
      case RAW_VIDEO_DELETE:
        raw_video_cursor(video_configuration, e_raw_video_cursor_action_step_back);
        backup_flags = video_configuration->flags;
        /* in order to empty the character, we replace it with a zero and we 
         * temporarly disable the cursor advance option (if enabled). At the 
         * end of the operation, we restore it */
        video_configuration->flags &= ~e_raw_video_flag_advance_cursor;
        raw_video_print_byte(video_configuration, 0);
        video_configuration->flags = backup_flags;
        break;
      default:
        raw_video_print_byte(video_configuration, *buffer);
        break;
    }
    ++buffer;
  }
}
void raw_video_clear_screen(s_raw_video_configuration *video_configuration) {
  size_t total_memory = ((video_configuration->_maximum_cols * 
        video_configuration->_maximum_rows) * 2);
  memory_set(video_configuration->_memory_video_entry_point, 0, total_memory);
}
