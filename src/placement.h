#ifndef PLACEMENT_H
#define PLACEMENT_H

enum placement_policy {
  PLACEMENT_ORIGIN,
  PLACEMENT_CASCADE,
};

void
placement_compute(struct swc_rectangle *geometry);


#endif
