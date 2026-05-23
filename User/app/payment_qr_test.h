#ifndef __PAYMENT_QR_TEST_H
#define __PAYMENT_QR_TEST_H

#include "app/camera_workflow.h"

void payment_qr_test_start(const camera_workflow_state_t *camera_state);
void payment_qr_test_loop(const camera_workflow_state_t *camera_state);

#endif
