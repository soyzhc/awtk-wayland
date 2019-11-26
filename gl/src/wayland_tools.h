/*
 * wayland_tools.h
 *
 *  Created on: 2019��9��23��
 *      Author: zjm09
 */

#ifndef WAYLAND_TOOLS_H_
#define WAYLAND_TOOLS_H_

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <wayland-client.h>
#include <wayland-cursor.h>
#include <xkbcommon/xkbcommon.h>

#include "pthread_signal.h"
#include "xdg-shell-unstable-v6-client-protocol.h"

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

static void
do_nothing () {}

struct keyboard
{
  struct wl_keyboard *kbd;
  uint32_t buf_size;
  int keymap_fd;
  struct xkb_context *ctx;
  struct xkb_keymap *map;
  struct xkb_state *kb_state;
  void (*kb_xcb)(int,int);
};

struct point{
	struct wl_pointer *pointer;
	struct wl_surface *point_surface;
	struct wl_cursor_theme *cursor_theme;
	void (*point_xcb)(int,int,int,int);
};

struct touch{
	struct wl_touch *pointer;
	void (*point_xcb)(int,int,int,int);
};


struct input_bundle
{
  struct keyboard keyboard;
  struct point mouse;
  struct touch touch;
  struct wl_seat *seat;
};

struct wayland_data
{
  int width,height;
  struct wl_display *display;
  struct wl_registry *registry;
  struct wl_compositor *compositor;
  struct wl_surface *surface;
  struct zxdg_surface_v6 *shell_surface;
  struct zxdg_shell_v6 *shell;
  struct zxdg_toplevel_v6 *top;
  struct wl_shm *shm;
  /* List of outputs */
  struct wl_list *monitors;
  struct input_bundle inputs;
};

struct output_info
{
  int32_t width;
  int32_t height;
  int32_t hertz;
  int32_t transform;
};

struct wayland_output
{
  struct wl_output *out;
  struct output_info info;
  struct wl_list link;
};

struct double_buffer_list
{
  struct wl_buffer *wl_buffer;
  uint32_t *pixels;
  struct double_buffer_list *next;
};

struct buffers{
	  struct wl_buffer *wl_buffer;
	  uint32_t *pixels;
};

struct buffer
{
  ThreadSignal used;
  uint32_t width;
  uint32_t height;
//  struct double_buffer_list *bufs;
  uint32_t idx:1;
  struct buffers bufs[2];
};

typedef enum
{
    SETUP_OK,
    NO_WAY_DISP,
    NO_REG,
    NO_COMP,
    NO_SHELL,
    NO_SEAT,
    NO_SHM,
    NO_MONITORS,
    NO_SURFACE,
    NO_SHELL_SURFACE,
    NO_TOPLEVEL
}WAYLAND_SETUP_ERR;

WAYLAND_SETUP_ERR setup_wayland (struct wayland_data *objs,int fullscreen);
void destroy_wayland_data (struct wayland_data *objs);
void *wayland_create_buffer(struct wl_shm *shm,int width,int height,int num_of_buffers);
void ref_display(struct wl_surface *surface,struct wl_buffer *buffer,int width,int height);
struct buffer *wayland_create_double_buffer(struct wl_shm *shm,int width,int height);

#endif /* WAYLAND_TOOLS_H_ */
