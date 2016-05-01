#ifndef __utils_h__
#define __utils_h__

#define TEST_VIDEO_PATH2 "~/test11_divx6.1.1.avi"
#define TEST_VIDEO_PATH "/home/carter/road-01.avi"
#define RECT_FRAME_X2 720
#define RECT_FRAME_Y2 (480-220)
#define DELAY_MS 10
#define DEBUG_DELAY_MS 800
#define IMAGE_WIDTH 320
#define IMAGE_HEIGHT 480
#define VANISHING_DIVIDER 5

#define ZERO_F 0.0
#define ZERO_I 0

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d:) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d:) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); }
#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); }
#define check_mem(A) check((A), "Out of memory.")
#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); }

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define TRUE 1
#define FALSE 0
#define SUCCESS 0
#define FAIL -1

#define MAX_BUFF_SIZE 4095
#endif
