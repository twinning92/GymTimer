
#define BUZZER_PIN 		5
#define LED_DATA_PIN 	15
#define IR_RECEIVER_PIN 4
#define RTC_IO_PIN		21
#define RTC_CLK_PIN		22
#define RTC_CE_PIN		23
#define TEMP_IO_PIN		2

// Debug level 0-none 9-all
#define DEBUG_PRINT_LEVEL	7

#define D_PRINT(level, format, args...) \
	if(level <= DEBUG_PRINT_LEVEL) { \
		Serial.printf("%s[%d]: ", __FILE__, __LINE__); \
		Serial.printf(format, ##args); \
	}

