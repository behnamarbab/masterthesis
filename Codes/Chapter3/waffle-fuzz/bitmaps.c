#define MAP_SIZE_POW2    16
#define MAP_SIZE         (1 << MAP_SIZE_POW2)
#define ICNT_SIZE        MAP_SIZE 

EXP_ST u8* trace_bits;   /* SHM with coverage bitmap    */
EXP_ST u32* icnt_bits;   /* SHM with performance bitmap */