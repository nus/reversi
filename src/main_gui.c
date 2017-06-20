#include "board.h"
#include "libui/ui.h"
#include <stdio.h>
#include <string.h>

#define BORDER_LINE_SIZE (2)
#define MIN(a, b) ((a < b) ? (a) : (b))

static board_t board;
static phase_t phase;
static uiLabel *status_lbl = NULL;

// helper to quickly set a brush color
static void set_solid_brush(uiDrawBrush *brush, uint32_t color, double alpha) {
  uint8_t component;

  brush->Type = uiDrawBrushTypeSolid;
  component = (uint8_t) ((color >> 16) & 0xFF);
  brush->R = ((double) component) / 255;
  component = (uint8_t) ((color >> 8) & 0xFF);
  brush->G = ((double) component) / 255;
  component = (uint8_t) (color & 0xFF);
  brush->B = ((double) component) / 255;
  brush->A = alpha;
}

static int on_closing(uiWindow *w, void *data) {
  uiControlDestroy(uiControl(w));
  uiQuit();
  return 0;
}

static void handler_draw(uiAreaHandler *a, uiArea *area, uiAreaDrawParams *p) {
  const int len = MIN(p->AreaWidth, p->AreaHeight);
  const int cell_len = len / 8;
  uiDrawBrush brush;
  uiDrawPath *border_path, *white_path, *black_path;
  int i;
  uiDrawStrokeParams sp;

  // draw backgournd
  set_solid_brush(&brush, 0x008800, 1.0);
  border_path = uiDrawNewPath(uiDrawFillModeWinding);
  uiDrawPathAddRectangle(border_path, 0, 0, len, len);
  uiDrawPathEnd(border_path);
  uiDrawFill(p->Context, border_path, &brush);
  uiDrawFreePath(border_path);

  // draw border lines
  set_solid_brush(&brush, 0x000000, 1.0);
  border_path = uiDrawNewPath(uiDrawFillModeWinding);
  for (i = 0; i < 9; i++) {
    uiDrawPathNewFigure(border_path, cell_len * i, 0);
    uiDrawPathLineTo(border_path, cell_len * i, len);
    uiDrawPathCloseFigure(border_path);
    uiDrawPathNewFigure(border_path, 0, cell_len * i);
    uiDrawPathLineTo(border_path, len, cell_len * i);
    uiDrawPathCloseFigure(border_path);
  }
  uiDrawPathEnd(border_path);

  memset(&sp, 0, sizeof (uiDrawStrokeParams));
  sp.Cap = uiDrawLineCapFlat;
  sp.Join = uiDrawLineJoinMiter;
  sp.Thickness = BORDER_LINE_SIZE;
  sp.MiterLimit = uiDrawDefaultMiterLimit;
  uiDrawStroke(p->Context, border_path, &brush, &sp);
  uiDrawFreePath(border_path);

  // draw pieces
  black_path = uiDrawNewPath(uiDrawFillModeWinding);
  white_path = uiDrawNewPath(uiDrawFillModeWinding);
  for (i = 0; i < BOARD_LEN; i++) {
    uiDrawPath *path = NULL;
    int x, y;

    switch (board[i]) {
      case CELL_BLACK:
        path = black_path;
        break;
      case CELL_WHITE:
        path = white_path;
        break;
      default:
        continue;
    }

    x = (cell_len / 2) + cell_len * (i % EDGE);
    y = (cell_len / 2) + cell_len * (i / EDGE);
    uiDrawPathNewFigureWithArc(path, x, y, cell_len / 2 - 2, 0, uiPi * 2, 0);
    uiDrawPathCloseFigure(path);
  }
  uiDrawPathEnd(black_path);
  uiDrawPathEnd(white_path);
  set_solid_brush(&brush, 0x000000, 1.0);
  uiDrawFill(p->Context, black_path, &brush);
  set_solid_brush(&brush, 0xffffff, 1.0);
  uiDrawFill(p->Context, white_path, &brush);
  uiDrawFreePath(black_path);
  uiDrawFreePath(white_path);
}

static void handler_mouse_event(uiAreaHandler *a, uiArea *area, uiAreaMouseEvent *e) {
  if (e->Up) {
    const int len = MIN(e->AreaWidth, e->AreaHeight);
    const int cell_len = len / 8;
    int x = (int) e->X;
    int y = (int) e->Y;
    int index = 0;
    eb_t e = EB_OK;
    board_t next;

    if ((x > len) || (y > len)) {
      return;
    }

    // TODO: consider clicking border lines
    x = (x / cell_len);
    y = (y / cell_len);
    index = CELL_INDEX(x, y);

    e = board_put(board, phase, index, next);
    if (e) {
      printf("failed board_put: %d\n", e);
      return;
    }

    board_copy(next, board);
    phase = board_next_phase(board, phase);
    printf("put: %d\n", index);
    board_print(board);
    uiAreaQueueRedrawAll(area);

    if (status_lbl != NULL) {
      char buf[256] = "";
      int black = board_count_cell(board, CELL_BLACK);
      int white = board_count_cell(board, CELL_WHITE);
      char *status = NULL;

      switch (phase) {
        case PHASE_BLACK:
          status = "black";
          break;
        case PHASE_WHITE:
          status = "white";
          break;
        default:
          status = ((black > white) ? "black won" : "white won");
      }

      snprintf(buf, sizeof(buf) / sizeof(buf[0]), "%d, %d\n%s", black, white, status);
      uiLabelSetText(status_lbl, buf);
    }
  }
}

static void handler_mouse_crossed(uiAreaHandler *ah, uiArea *a, int left) {
  // do nothing
}

static void handler_drag_broken(uiAreaHandler *ah, uiArea *a) {
  // do nothing
}

static int handler_key_event(uiAreaHandler *ah, uiArea *a, uiAreaKeyEvent *e) {
  // reject all keys
  return 0;
}

static void reset_clicked(uiButton *b, void *data) {
  puts("clicked");
}

int main() {
  uiInitOptions op;
  const char *err;
  uiWindow *main_win;
  uiBox *hbox, *vbox;
  uiAreaHandler handler;
  uiArea *board_area;
  uiButton *reset_btn;

  board_reset(board);
  phase = PHASE_BLACK;

  memset(&op, 0, sizeof(uiInitOptions));
  err = uiInit(&op);
  if (err != NULL) {
    fprintf(stderr, "error initializing ui: %s\n", err);
    uiFreeInitError(err);
    return 1;
  }

  main_win = uiNewWindow("Reversi", 640, 480, 1);
  uiWindowSetMargined(main_win, 1);
  uiWindowOnClosing(main_win, on_closing, NULL);

  handler.Draw = handler_draw;
  handler.MouseEvent = handler_mouse_event;
  handler.MouseCrossed = handler_mouse_crossed;
  handler.DragBroken = handler_drag_broken;
  handler.KeyEvent = handler_key_event;

  hbox = uiNewHorizontalBox();
  uiBoxSetPadded(hbox, 1);
  uiWindowSetChild(main_win, uiControl(hbox));

  board_area = uiNewArea(&handler);
  uiBoxAppend(hbox, uiControl(board_area), 1);

  vbox = uiNewVerticalBox();
  uiBoxSetPadded(vbox, 2);
  uiBoxAppend(hbox, uiControl(vbox), 0);

  status_lbl = uiNewLabel("2:2\nblack");
  uiBoxAppend(vbox, uiControl(status_lbl), 0);

  reset_btn = uiNewButton("reset");
  uiButtonOnClicked(reset_btn, reset_clicked, NULL);
  uiBoxAppend(vbox, uiControl(reset_btn), 0);

  uiWindowSetChild(main_win, uiControl(hbox));

  uiControlShow(uiControl(main_win));
  uiMain();
  uiUninit(); 

  return 0;
}
