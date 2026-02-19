#ifndef SSG_BACKEND_H
#define SSG_BACKEND_H

#ifdef __cplusplus
extern "C" {
#endif

// SDL Backend
int  ssg_window_init(SSG_Canvas canvas, const char *title);
void ssg_window_shutdown(void);
int  ssg_window_running(void);
float ssg_window_get_dt(void);
void ssg_window_begin_frame(void);
void ssg_window_end_frame(SSG_Canvas canvas);

#ifdef __cplusplus
}
#endif

#endif /* SSG_BACKEND_H */