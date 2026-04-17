#include "ai_class_labels.h"

const char * const g_ai_class_labels[AI_CLASS_LABEL_COUNT] = {
  "Apple",  /* 0 */
  "Banana", /* 1 */
  "Orange", /* 2 */
  "NFSQ",   /* 3 */
  "Yibao",  /* 4 */
};

const char *ai_class_label_get(uint32_t class_index)
{
    if (class_index < AI_CLASS_LABEL_COUNT)
    {
        return g_ai_class_labels[class_index];
    }

    return "Unknown";
}
