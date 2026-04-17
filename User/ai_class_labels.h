#ifndef AI_CLASS_LABELS_H
#define AI_CLASS_LABELS_H

#include <stdint.h>

#define AI_CLASS_LABEL_COUNT (5U)

extern const char * const g_ai_class_labels[AI_CLASS_LABEL_COUNT];

const char *ai_class_label_get(uint32_t class_index);

#endif /* AI_CLASS_LABELS_H */
