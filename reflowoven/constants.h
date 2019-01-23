// Reflow oven control

#define NUM_PROFILES        3
#define MAX_CURVE_LENGTH    4
#define MAX_NAME_LENGTH     16
#define MAX_ERRORS          100

typedef struct{
  // Name of reflow curve
  char name[MAX_NAME_LENGTH];
  // Names for the steps
  char steps[MAX_CURVE_LENGTH][MAX_NAME_LENGTH];
  // Temperatures for the curve
  uint16_t temps[MAX_CURVE_LENGTH];
  // Times for the curve
  uint16_t times[MAX_CURVE_LENGTH];
} reflow_curve_t;

typedef struct{
  reflow_curve_t profiles[NUM_PROFILES];
} reflow_profiles_t;


typedef enum {
  SETUP,
  RUNNING,
  COOLING,
  STOPPED,
  ERROR
} oven_state_t;

typedef enum {
  UPDATE,
  HOLD
} lcd_state_t;

#define SWITCH 12
#define RELAY_PIN 13
