#include <pthread.h>
#include <unistd.h>

#include "MIDI_IO.h"
#include "Physical_IO.h"

#define SPIDEVICE "/dev/spidev0.0"
#define I2CDEVICE "/dev/i2c-1"

void* Thread_InEncoder(void *arg){

	Physical_IO *phys_io = static_cast<Physical_IO*>(arg);
	uint8_t encoderchanged = 0;
	MidiMessage msg;

	while(1){

		encoderchanged = phys_io->updateEncoderState();

		if(!encoderchanged)
			continue;

		for(int i = 0; i < 4; i++){
			if(encoderchanged & (1 << i)){
				msg = phys_io->encoderToMidiMsg(i);
				phys_io->writeMidiMsg(msg);
			}
		}
	}

}

void* Thread_InButton(void *arg){

	Physical_IO *phys_io = static_cast<Physical_IO*>(arg);
	MidiMessage msg;

	while(1){

		usleep(30000);

		if (phys_io->updateButtonState()){

			for (uint8_t i=0; i<16; i++) {
	        	// if it was pressed...
	      		if (justPressed(i)){
					msg = phys_io->buttonToMidiMsg(i, NOTE_ON);
					phys_io->writeMidiMsg(msg);
				}

				// if it was released, turn it off
				if (justReleased(i)){
					msg = phys_io->buttonToMidiMsg(i, NOTE_OFF);
					phys_io->writeMidiMsg(msg);
				}
	 		}
	    }

	}

}


int main(int argc, char **argv){

	pthread_t thread_handle;

	Physical_IO	*phys_io = new Physical_IO(I2CDEVICE, SPIDEVICE);







	pthread_create(&thread_handle, NULL, Thread_InEncoder, static_cast<void*>(phys_io));
	pthread_create(&thread_handle, NULL, Thread_InButton, static_cast<void*>(phys_io));

	pthread_exit(NULL);

	return 0;
}
