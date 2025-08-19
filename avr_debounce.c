 /*
 * Biblioteca de debounce de GPIOs 
 * para Atmel AVR
 *
 * Created: 19/07/2025 23:46:12
 * Author : Amauri Tuoni
 */
 
 
 #include "avr_debounce.h" //header dessa biblioteca
 

inline volatile uint8_t Read_Keys(void)
 {
	 checked_keys = 0;
	 if(GPIO_Read(PB,P4))    //pino em nível alto?
	 {
		 CLEARBIT(checked_keys,0);  //sim, zera o bit 0 da variavel checked_keys
	 }
	 else
	 {
		 SETBIT(checked_keys,0);  //nao, seta o bit 0 da variavel checked_keys 
	 }
/*	 if(GPIO_Read(PD,P7))
	 {
		 SETBIT(debouncedKeys,1);
	 }
	 else
	 {
		 CLEARBIT(debouncedKeys,1);
	 }
*/
	 return checked_keys;
 }

 void Debounce_Keys(void)
 {
	 static uint8_t temp_keys = 0;
	 static uint16_t count_debounce = 0;
	 if(temp_keys != Read_Keys())
	 {
		 count_debounce = DEBOUNCE_COUNT;
		 temp_keys = Read_Keys();
		 
	 }

	 if(temp_keys == Read_Keys())
	 {
		 if(count_debounce > 0)
		 {
			 count_debounce--;
		 }
		 else
			checked_keys = temp_keys;
		 
	 }
	 //if(debounceCount == 0)
	 //{
	//	 checked_keys = temp_keys;
	 //}
 }
 