#ifndef __UI_DIALOG_H
#define __UI_DIALOG_H

#ifdef __cplusplus
extern "C" {
#endif

void ui_dialog_show_message(const char *title, const char *message);
void ui_dialog_close(void);

#ifdef __cplusplus
}
#endif

#endif
