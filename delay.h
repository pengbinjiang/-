#ifndef DELAY_H_
#define DELAY_H_

#define CPU_F1 ((double) 8000000)
#define delay_us(x) __delay_cycles((long)(CPU_F1*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F1*(double)x/1000.0))

#endif /* DELAY_H_ */
