#include "../../kernel/standard_lib/standard_lib.h"
#ifndef DRIVERS_VIDEO_RAW_VIDEO
#define DRIVERS_VIDEO_RAW_VIDEO
#define RAW_VIDEO_BLACK           0x00
#define RAW_VIDEO_BLUE            0x01
#define RAW_VIDEO_GREEN           0x02
#define RAW_VIDEO_CYAN            0x03
#define RAW_VIDEO_RED             0X04
#define RAW_VIDEO_MAGENTA         0x05
#define RAW_VIDEO_BROWN           0x06
#define RAW_VIDEO_LIGHT_GRAY      0x07
#define RAW_VIDEO_DARK_GRAY       0x08
#define RAW_VIDEO_LIGHT_BLUE      0x09
#define RAW_VIDEO_LIGHT_GREEN     0x0a
#define RAW_VIDEO_LIGHT_CYAN      0X0b
#define RAW_VIDEO_LIGHT_RED       0X0c
#define RAW_VIDEO_LIGHT_MAGENTA   0x0d
#define RAW_VIDEO_YELLOW          0X0e
#define RAW_VIDEO_WHITE           0x0f

/* this routine creates the format required by ORing
 * together the foreground and the background into a
 * single byte (pretty simple, but useful to 
 * remember which color goes first) */
#define RAW_VIDEO_CREATE_FORMAT(bg,fg) ((bg<<4)|fg)

/* the location where the video memory starts; The memory is
 * organized as a pair of bytes per character: the character 
 * itself and the format (expressed as a combination of background 
 * color and foreground color). You can use the routine 
 * RAW_VIDEO_CREATE_FORMAT to generate the right value */
#define RAW_VIDEO_MEMORY_LOCATION 0xb8000

/* some of the parameters required to perform operations on the
 * video memory area. Accessing each byte is possible only when 
 * we are aware of the boundaries we have */
#define RAW_VIDEO_MAXIMUM_COLUMNS 80
#define RAW_VIDEO_MAXIMUM_ROWS 25
#define RAW_VIDEO_TOTAL_MEMORY \
  ((RAW_VIDEO_MAXIMUM_ROWS*RAW_VIDEO_MAXIMUM_COLUMNS)*2)

/* special characters that has an effect on the position of the
 * cursor/scroll are here mapped */
#define RAW_VIDEO_LINE_FEED 0x0a        // it moves the cursor to a new line 
#define RAW_VIDEO_CARRIAGE_RETURN 0x0d  // it moves the cursor back at the home of the line
#define RAW_VIDEO_DELETE 0x7f
typedef enum e_raw_video_flags {
  e_raw_video_flag_scrollable     = 0x0001,
  e_raw_video_flag_advance_cursor = 0x0002
} e_raw_video_flags;
typedef struct s_raw_video_configuration {
  unsigned char *_memory_video_entry_point, _maximum_rows, _maximum_cols, row, col, format, flags;
} s_raw_video_configuration;
typedef enum e_raw_video_cursor_actions {
  e_raw_video_cursor_action_line_home,
  e_raw_video_cursor_action_screen_home,
  e_raw_video_cursor_action_step_back,
  e_raw_video_cursor_action_step_forward,
  e_raw_video_cursor_action_line_forward
} e_raw_video_cursor_actions;
extern void raw_video_set(s_raw_video_configuration *video_configuration, 
    unsigned char character_format, unsigned char flags);
extern void raw_video_print_byte(s_raw_video_configuration *video_configuration, char data);
extern void raw_video_scroll(s_raw_video_configuration *video_configuration, unsigned int rows);
extern void raw_video_cursor(s_raw_video_configuration *video_configuration, 
    enum e_raw_video_cursor_actions action);
extern void raw_video_print_string(s_raw_video_configuration *video_configuration, const char *buffer);
#endif
