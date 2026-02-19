#ifndef SSG_BACKEND_H
#define SSG_BACKEND_H

#ifdef __cplusplus
extern "C" {
#endif

int  ssg_init(SSG_Canvas canvas, const char *title);
void ssg_shutdown(void);
int  ssg_running(void);
void ssg_begin_frame(void);
void ssg_end_frame(SSG_Canvas canvas);

#ifdef __cplusplus
}
#endif

#endif /* SSG_BACKEND_H */