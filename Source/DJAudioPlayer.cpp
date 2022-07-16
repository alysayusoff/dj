#include <JuceHeader.h>
#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager) : formatManager(_formatManager)
{
	
}

DJAudioPlayer::~DJAudioPlayer() {

}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
	//formatManager.registerBasicFormats();
	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
	resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources() {
	transportSource.releaseResources();
	resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL) {
	auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
	if (reader != nullptr) { //good
		std::unique_ptr<AudioFormatReaderSource>newSource(new AudioFormatReaderSource(reader, true));
		transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
		readerSource.reset(newSource.release());
		transportSource.start();
	}
	else
		DBG("Something went wrong loading the file ");
}

void DJAudioPlayer::setGain(double gain) {
	if (gain < 0 || gain > 10)
		DBG("DJAudioPlayer::setGain gain should be between 0 and 10");
	else
		transportSource.setGain(gain);
}

void DJAudioPlayer::setSpeed(double ratio) {
	if (ratio < 0 || ratio > 100)
		DBG("DJAudioPlayer::setSpeed ratio should be between 0 and 100");
	else
		resampleSource.setResamplingRatio(ratio);
}

void DJAudioPlayer::setPosition(double posInSecs) {
	transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos) {
	if (pos < 0 || pos > 1)
		DBG("DJAudioPlayer::setPositionRelative pos should be between 0 and 1");
	else {
		double posInSecs = transportSource.getLengthInSeconds() * pos; //transportSource.getLengthInSeconds() is the file length in seconds
		setPosition(posInSecs);
	}
}

void DJAudioPlayer::start() {
	transportSource.start();
}

void DJAudioPlayer::stop() {
	transportSource.stop();
}

void DJAudioPlayer::loop() {
	setPosition(0);
	start();
}

void DJAudioPlayer::rewind(double pos) {
	double newPos = (pos - 0.1) * transportSource.getLengthInSeconds();
	setPosition(newPos);
}

void DJAudioPlayer::forward(double pos) {
	double newPos = (pos + 0.1) * transportSource.getLengthInSeconds();
	setPosition(newPos);
}

double DJAudioPlayer::getPositionRelative() {
	return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

std::string DJAudioPlayer::getLength(URL audioURL) {
	std::string length = "undefined";
	auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
	if (reader != nullptr) { //good
		std::unique_ptr<AudioFormatReaderSource>newSource(new AudioFormatReaderSource(reader, true));
		transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
		readerSource.reset(newSource.release());
		double lengthInSecs = transportSource.getLengthInSeconds(); //gets length in seconds
		int lengthInMins;
		if (lengthInSecs > 60) { //convert to mins
			lengthInMins = floor(lengthInSecs / 60.0);
			double remainder = lengthInSecs - (lengthInMins * 60.0);
			int sec = (int)remainder;
			if (sec < 10)
				length = std::to_string(lengthInMins) + ":0" + std::to_string(sec);
			else
				length = std::to_string(lengthInMins) + ":" + std::to_string(sec);
		}
		else
			length = "0:" + std::to_string((int)lengthInSecs);
		return length;
	}
	else {
		DBG("Something went wrong loading the file ");
		return length;
	}
}