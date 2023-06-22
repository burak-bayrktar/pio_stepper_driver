

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "stepperdriver.pio.h"

//an enum for direction, clockwise or counter clockwise
typedef enum{
	clockWise = 0,
	counterClockWise = 1
} RotationDirection_T;

typedef enum{
	rotational = 0,
	straight = 1
} Movement_T;

typedef struct{
	uint32_t haltedSteps; // non-performed number of steps of a halted movement
	uint16_t degree;
	uint16_t length; 
} HaltedMovement_T;




class Motion{
	private:
		//singleton 
		//Motion(PIO pio, uint sm, uint driverBasePin, uint driver1_DirectionPin, uint driver2_DirectionPin);
		//Motion* motionObject;

		//Pio definitions
		PIO pio;
		uint stateMachine;
		pio_sm_config stateMachineConfig;
		uint instruction_mem_offset;
		bool running;

		//Wiring Definitions
		uint BasePin;			
		uint directionPin1;
		uint directionPin2;

		//Movement Definitions
		uint32_t step; // migth need to compare the last given steps to number of steps left ,if halt occurs
		void startMovement();

		Movement_T movement;
		HaltedMovement_T haltedMovement;
		/*
			wheel diameter = 80.4mm
			2*40.2*pi = 252.58mm
			wheel to wheel diameter = 150.1mm
			2*75.05*pi = 471.55mm
		*/

		//Calculation Definitions


		uint32_t degreeToSteps(uint16_t degree);
		uint32_t lengthToSteps(uint16_t length);
		uint16_t stepsToDegree(uint32_t steps);
		uint16_t stepsToLength(uint32_t steps);

		//reset
		void resetCore();


	public:
		Motion(PIO pio, uint sm, uint driverBasePin, uint driver1_DirectionPin, uint driver2_DirectionPin);
		//Motion* init(PIO pio, uint sm, uint driverBasePin, uint driver1_DirectionPin, uint driver2_DirectionPin );
		void StraightMovement(uint16_t length);
		void RotateMovement(uint16_t degree, RotationDirection_T dir);
		void HaltMovement(); //returns number of steps left that are not applied for that movement
		bool isRunning();
};


