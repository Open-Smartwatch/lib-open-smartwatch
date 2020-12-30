#include <stdint.h>
#include <stdio.h>

#include "../../FakeArduino.h"
#include "../../FakeArduinoWindowSDL.h"
#include "../../gfx_2d.h"
#include "../../gfx_util.h"
#include "lvgl/lvgl.h"

// full screen bufffer
#define BUF_W 240
#define BUF_H 240

/*
 * This demo uses the gfx2d as the "display", and lvgl2d as the lvgl buffer.
 */

Graphics2D gfx2d(BUF_W, BUF_H, 16);

Graphics2D lvgl2d(BUF_W, BUF_H, 16);

lv_disp_drv_t disp_drv;
lv_disp_t* disp;

void my_flush_cb(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p) {
  gfx2d.drawGraphics2D(0, 0, &lvgl2d);

  /* IMPORTANT!!!
   * Inform the graphics library that you are ready with the flushing*/
  lv_disp_flush_ready(disp_drv);
}

void my_set_px_cb(lv_disp_drv_t* disp_drv, uint8_t* buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y, lv_color_t color,
                  lv_opa_t opa) {
  lvgl2d.drawPixel(x, y, lv_color_to16(color));
}

void my_monitor_cb(lv_disp_drv_t* disp_drv, uint32_t time, uint32_t px) {
  // printf(px);
  // printf(" px refreshed in");
  // printf(time);
  // printf(" ms\n");
}

class RotationExampleWindow : public SDLWindowRGB565 {
 public:
  RotationExampleWindow(Graphics2D* gfx2d, int w, int h) : SDLWindowRGB565(gfx2d, w, h) {}
  void setup() {
    // init lvgl here

    // src: https://docs.lvgl.io/latest/en/html/porting/display.html#display-buffer

    lv_disp_drv_init(&disp_drv);
    // disp_drv.buffer = &disp_buf;     /*Set an initialized buffer*/
    disp_drv.flush_cb = my_flush_cb;
    disp_drv.set_px_cb = my_set_px_cb;
    disp = lv_disp_drv_register(&disp_drv);

    lv_obj_t* par = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_size(par, BUF_W, BUF_H);

    lv_obj_t* btn1 = lv_btn_create(par, NULL);

    lv_init();
  }

  void loop() {
    static uint16_t counter = 1;
    counter++;

    gfx2d.drawGraphics2D(0, 0, &lvgl2d);

    delay(1000 / 30);
    // lv_tick_inc(1000 / 30);
    // lv_task_handler();
  }
};

int main(int argc, char* argv[]) {
  RotationExampleWindow exampleWindow(&gfx2d, BUF_W, BUF_H);
  exampleWindow.run();
  return 0;
}
