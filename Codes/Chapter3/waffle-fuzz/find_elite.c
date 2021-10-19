#ifndef R(x)
    # define R(x) (random() % (x))
#endif

struct queue_entry {
  // ...
  u32 TORC;                         /* Total ORC     */
  u32 elite_ORC;                    /* Elite ORC     */
  u32 elite_index;                  /* Elite index   */
};

EXP_ST u32* elite_ORCs;                /* SHM with instrumentation bitmap  */
EXP_ST u32* ORCs;

void find_elite(queue_entry q) {
  u32 max_elite_ORC = 0;
  float ORC_ratio = 1;
  const float eps = 0.000001;
  u32 elite_index = -1;

  for (i = 0; i < MAP_SIZE; i++){
    if(elite_ORCS[i]){
      float oratio = float(ORCs[i]) / elite_ORCs[i];
      /* oratio == ORC_ratio */
      if (ORC_ratio - eps <= oratio && oratio >= ORC_ratio + eps) {
        if (max_elite_ORC < ORCs[i]) {
          max_elite_ORC = ORCs[i];
          elite_index = i;
        }
      }
      /* a higher oratio is found */
      else if (oratio > ORC_ratio + eps) {
        ORC_ratio = oratio;
        elite_index = i;
      }
    } else
    /* Yet the untracked ORCCs may become the elite. 10% chance */
    if (max_elite_ORC && R(100)>10) continue;

    else if (max_elite_ORC < ORCs[i]) {
      max_elite_ORC = ORCs[i];
      elite_index = i;
    }
  }

  q->elite_index = elite_index;
  q->elite_ORC = ORCs[elite_index];
}