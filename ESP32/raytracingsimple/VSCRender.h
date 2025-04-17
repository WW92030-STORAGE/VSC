#include "matrices.h"
#include <VSC_everything.h>

#include <cstdint>
#include <vector>

// The BUF_FER is coordinates BUF_FER[x][y] where x goes right and y goes up
// However the col matrix has coordinates col[r][c] where r goes down and c goes right
// So (x, y) --> (r, c) = (n - y - 1, x)
inline void RenderBuffer(Scene& s) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      col[n - i - 1][j] = hex565((s.buffer[j][i].color)>>8);
      col[n - i - 1][j + m] = hex565((s.buffer[j][i].color)>>8);
    }
  }
}