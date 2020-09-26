#pragma once

#include "quantum.h"
#include "Segments.h"
#define ___ KC_NO

// This a shortcut to help you visually see your layout.
// The first section contains all of the arguments
// The second converts the arguments into a two-dimensional array

#define LAYOUT( \
    K00, K01, K02, K03, K04, \
    K10, K11, K12, K13, K14 \
) \
{ \
    { K00, K01, K02, K03, K04}, \
    { K10, K11, K12, K13, K14} \
}
