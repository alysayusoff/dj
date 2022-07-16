#pragma once
#include <JuceHeader.h>
using namespace juce;

class DJAudioPlayer : public AudioSource {
public:
	DJAudioPlayer(AudioFormatManager& _formatManager);
	~DJAudioPlayer();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;

	void loadURL(URL audioURL);
	void setGain(double gain);
	void setSpeed(double ratio);
	void setPosition(double posInSecs);
	void setPositionRelative(double pos);

	void start();
	void stop();
	void loop();
	void rewind(double pos);
	void forward(double pos);
	/*get the relative position of the playhead*/
	double getPositionRelative();
	std::string getLength(URL audioURL);

private:
	AudioFormatManager& formatManager;
	std::unique_ptr<AudioFormatReaderSource> readerSource;
	AudioTransportSource transportSource;
	ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
};