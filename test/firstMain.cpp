


#include "RtAudio.h"
#include <iostream>
#include <cstdlib>

// Two-channel sawtooth wave generator.
int saw(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
	double streamTime, RtAudioStreamStatus status, void *userData)
{
	unsigned int i, j;
	short *buffer = (short *)outputBuffer;
	short *lastValues = (short *)userData;
	if (status)
		std::cout << "Stream underflow detected!" << std::endl;
	// Write interleaved audio data.
	/*
	for (i = 0; i<nBufferFrames; i++) {
		for (j = 0; j<2; j++) {
			*buffer++ = lastValues[j];
			lastValues[j] += 0.005 * (j + 1 + (j*0.1));
			if (lastValues[j] >= 1.0) lastValues[j] -= 2.0;
		}
	}
	*/
	memcpy(outputBuffer, inputBuffer, 4 * nBufferFrames);
	return 0;
}

int main()
{
	RtAudio dac;

	//std::cout << dac.getDeviceCount() << std::endl;   //2
	if (dac.getDeviceCount() < 1) {
		std::cout << "\nNo audio devices found!\n";
		exit(0);
	}
	RtAudio::StreamParameters parameters;
	//std::cout << dac.getDefaultOutputDevice() << std::endl;
	parameters.deviceId = dac.getDefaultOutputDevice();    //0
	parameters.nChannels = 2;
	parameters.firstChannel = 0;
	unsigned int sampleRate = 44100;
	unsigned int bufferFrames = 256; // 256 sample frames

	RtAudio::StreamParameters input;
	input.deviceId = dac.getDefaultInputDevice();
	input.nChannels = 2;
	input.firstChannel = 0;

	double data[2];
	try {
		dac.openStream(&parameters, &input, RTAUDIO_SINT16,
			sampleRate, &bufferFrames, &saw, (void *)&data);
		dac.startStream();
	}
	catch (RtAudioError& e) {
		e.printMessage();
		exit(0);
	}

	char input1;
	std::cout << "\nPlaying ... press <enter> to quit.\n";
	std::cin.get(input1);
	
	try {
		// Stop the stream
		dac.stopStream();
	}
	catch (RtAudioError& e) {
		e.printMessage();
	}
	if (dac.isStreamOpen()) dac.closeStream();
	system("pause");
	
	return 0;
}





/*

#include "RtAudio.h"
#include <iostream>

using namespace std;

typedef signed short MY_TYPE;
#define FORMAT RTAUDIO_SINT16

int inout(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
	double , RtAudioStreamStatus status, void *data)
{
	// Since the number of input and output channels is equal, we can do
	// a simple buffer copy operation here.
	if (status) std::cout << "Stream over/underflow detected." << std::endl;

	unsigned int *bytes = (unsigned int *)data;
	memcpy(outputBuffer, inputBuffer, *bytes);
	return 0;
}

int main()
{
	unsigned int channels = 2;
	unsigned int fs = 44100;
	unsigned int bufferBytes = 0;
	unsigned int oDevice = 0;
	unsigned int iDevice = 0;
	unsigned int iOffset = 0;
	unsigned int oOffset = 0;

	RtAudio adac;
	if (adac.getDeviceCount() < 1) {
		cout << "\nNo audio devices found!\n";
		exit(-1);
	}

	adac.showWarnings(true);

	unsigned int bufferFrames = 441;
	RtAudio::StreamParameters iParams, oParams;
	iParams.deviceId = adac.getDefaultInputDevice();
	iParams.nChannels = channels;
	iParams.firstChannel = iOffset;
	oParams.deviceId = adac.getDefaultOutputDevice();
	oParams.nChannels = channels;
	oParams.firstChannel = oOffset;

	try {
		adac.openStream(&oParams, &iParams, RTAUDIO_SINT16, fs, &bufferFrames, &inout, (void *)&bufferBytes);
	}
	catch (RtAudioError& e) {
		std::cout << '\n' << e.getMessage() << '\n' << std::endl;
		exit(1);
	}

	bufferBytes = bufferFrames * channels * sizeof(MY_TYPE);

	// Test RtAudio functionality for reporting latency.
	std::cout << "\nStream latency = " << adac.getStreamLatency() << " frames" << std::endl;

	try {
		adac.startStream();

		char input;
		std::cout << "\nPlaying ... press <enter> to quit (buffer frames = " << bufferFrames << ").\n";
		std::cin.get(input);

		// Stop the stream.
		adac.stopStream();
	}
	catch (RtAudioError& e) {
		std::cout << '\n' << e.getMessage() << '\n' << std::endl;
		goto cleanup;
	}

cleanup:
	if (adac.isStreamOpen()) adac.closeStream();

	return 0;

	return 0;
}


*/