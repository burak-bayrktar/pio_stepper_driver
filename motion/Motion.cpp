/*
	halted movement struct will be filled in halt movement method
	calcultaion functions fill be filled
*/

#include "Motion.hpp"


//public methods

Motion::Motion(PIO pio, uint sm, uint driverBasePin, uint driver1_DirectionPin, uint driver2_DirectionPin){

	directionPin1 = driver1_DirectionPin;
	directionPin2 = driver2_DirectionPin;
	gpio_init(directionPin1);
	gpio_init(directionPin2);
	gpio_set_dir(directionPin1, GPIO_OUT);
	gpio_set_dir(directionPin2, GPIO_OUT);
	//for later use
	this->pio = pio; 
	this->stateMachine = sm;
	//load program and get config
	instruction_mem_offset = pio_add_program(pio, &stepperdriver_program);
	stateMachineConfig = stepperdriver_program_init(pio, stateMachine, instruction_mem_offset, driverBasePin, 62500.f);
}


void Motion::StraightMovement(uint16_t length){

	// registers in state machine keeps their content (like pc, isr and so on), even after halted.
	// To  prevent the next loop to start from the middle of previous one, a re-initialisation
	// must be performed for every new driving sequence.
	// fifos are cleared as well.
	resetCore();
	//calculate required steps
	step = lengthToSteps(length);
	//set directions
	gpio_put(directionPin1, 1);
	gpio_put(directionPin2, 1);
	//movement type
	movement = straight;
	//send steps into pio's fifo
	pio_sm_put(pio, stateMachine, step);
	startMovement();
}

void Motion::RotateMovement(uint16_t degree, RotationDirection_T dir){

	resetCore();
	
	step = degreeToSteps(degree);
	
	if(dir==clockWise){
		gpio_put(directionPin1, 0);
		gpio_put(directionPin2, 1);
	}else if(dir==counterClockWise){
		gpio_put(directionPin1, 1);
		gpio_put(directionPin2, 0);
	}else{
		//print and return
	}
	//movement type
	movement = rotational;
	//send steps into pio's fifo
	pio_sm_put(pio, stateMachine, step);
	startMovement();
}

//Perform a push instruction before stopping the machine and get
//the content of isr(which is number of steps left) into fifo.
void Motion::HaltMovement(){
	//push
	pio_sm_exec(pio, stateMachine, pio_encode_push(false, false)); 
	//stop the machine
	pio_sm_set_enabled(pio, stateMachine, false); 
	//And get the fifo content
	haltedMovement.haltedSteps = pio_sm_get(pio, stateMachine);
	if(movement==rotational){
		haltedMovement.degree = stepsToDegree(haltedMovement.haltedSteps);
	}else if(movement==straight){
		haltedMovement.length = stepsToLength(haltedMovement.haltedSteps);
	}else{
	}
	


}

bool Motion::isRunning(){
	if(pio_sm_get(pio,stateMachine) == 0){
		return false;
	}else{
		return true;
	}
}

void Motion::startMovement(){
	pio_sm_set_enabled(pio, stateMachine, true);
}


//--------------------------------------------
//private methods

uint32_t Motion::degreeToSteps(uint16_t degree){
	uint32_t steps=((float)degree * 1.3097) / .1571 ;
	return steps;
}


uint16_t Motion::stepsToDegree(uint32_t steps){
	uint16_t degree = (steps * .1571) / 1.3097;
	return degree;
}


uint32_t Motion::lengthToSteps(uint16_t length){
	uint32_t steps = length / .1571;
	return steps;
}

uint16_t Motion::stepsToLength(uint32_t steps){
	uint16_t length = steps * .1571 ;
	return length;
}

void Motion::resetCore(){

	pio_sm_init(pio, stateMachine, instruction_mem_offset, &stateMachineConfig);
	pio_sm_set_enabled(pio, stateMachine, false);
}



/*
Motion* Motion::init(PIO pio, uint sm, uint driverBasePin, uint driver1_DirectionPin, uint driver2_DirectionPin){

	if(*motionObject==NULL){
		*motionObject = new Motion(pio, sm, driverBasePin, driver1_DirectionPin, driver2_DirectionPin);	
	}
	return motionObject;
}

*/