static int total_ERU = 0;

void __attribute__((constructor)) traceBegin(void) {}

void __attribute__((destructor)) traceEnd(void) {
  unsigned char *orc_str = getenv(ERU_ENV_VAR);

  if (mem_str) {
    unsigned int shm_mem_id = atoi(mem_str);
    u64 *TERU;
    
    TERU = shmat(shm_mem_id, NULL, 0);
    if (TERU == (void *)-1) _exit(1);

    *TERU = total_ERU;
  }
}

void addERU(int cnt) {
  total_ERU += cnt;
}