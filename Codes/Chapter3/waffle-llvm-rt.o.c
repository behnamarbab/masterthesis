// snippet of wafl-llvm-rt.o.c

#define ERU_SIZE (1 << 16)

u32  __wafl_ERU_initial[ERU_SIZE];
u32* __wafl_area_ptr = __wafl_ERU_initial;

static void __afl_map_shm(void) {
  u8 *id_str = getenv(SHM_ENV_VAR);
  
  if (id_str) {
    u32 shm_id = atoi(id_str);
    __wafl_area_ptr = shmat(shm_id, NULL, 0);
    
    if (__wafl_area_ptr == (void *)-1) _exit(1);

    memset(__wafl_area_ptr, 0, sizeof __wafl_ERU_ptr);
  }
}