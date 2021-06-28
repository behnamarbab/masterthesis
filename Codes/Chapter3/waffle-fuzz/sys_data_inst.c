static int total_ORC = 0;

void __attribute__((constructor)) traceBegin(void) {}

void __attribute__((destructor)) traceEnd(void) {
  unsigned char *orc_str = getenv(ORC_ENV_VAR);

  if (mem_str) {
    unsigned int shm_mem_id = atoi(mem_str);
    u64 *TORC;
    
    TORC = shmat(shm_mem_id, NULL, 0);
    if (TORC == (void *)-1) _exit(1);

    *TORC = total_ORC;
  }
}

void addORC(int cnt) {
  total_ORC += cnt;
}