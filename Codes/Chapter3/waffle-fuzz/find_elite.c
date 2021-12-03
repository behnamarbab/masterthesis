#ifndef R(x)
    # define R(x) (random() % (x))
#endif

struct queue_entry {
  // ...
  u32 TERU;                         /* Total ERU     */
  u32 elite_ERU;                    /* Elite ERU     */
  u32 elite_index;                  /* Elite index   */
};

EXP_ST u32* elite_ERUs;                /* SHM with instrumentation bitmap  */
EXP_ST u32* ERUs;

void find_elite(queue_entry q) {
  u32 max_elite_ERU = 0;
  float ERU_ratio = 1;
  const float eps = 0.000001;
  u32 elite_index = -1;

  for (i = 0; i < MAP_SIZE; i++){
    if(elite_ERUS[i]){
      float oratio = float(ERUs[i]) / elite_ERUs[i];
      /* oratio == ERU_ratio */
      if (ERU_ratio - eps <= oratio && oratio >= ERU_ratio + eps) {
        if (max_elite_ERU < ERUs[i]) {
          max_elite_ERU = ERUs[i];
          elite_index = i;
        }
      }
      /* a higher oratio is found */
      else if (oratio > ERU_ratio + eps) {
        ERU_ratio = oratio;
        elite_index = i;
      }
    } else
    /* Yet the untracked ERUCs may become the elite. 10% chance */
    if (max_elite_ERU && R(100)>10) continue;

    else if (max_elite_ERU < ERUs[i]) {
      max_elite_ERU = ERUs[i];
      elite_index = i;
    }
  }

  q->elite_index = elite_index;
  q->elite_ERU = ERUs[elite_index];
}