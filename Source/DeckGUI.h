#pragma once
#include <JuceHeader.h>
#include "WaveformDisplay.h"
#include "DJAudioPlayer.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace juce;

class DeckGUI : public juce::Component, public Button::Listener, public Slider::Listener, public FileDragAndDropTarget, public Timer
{
public:
	DeckGUI(DJAudioPlayer* player, AudioFormatManager& formatManagerToUse, AudioThumbnailCache& cacheToUse);
	~DeckGUI() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	/*implement Button::Listener*/
	void buttonClicked(Button*) override;
	/*implement Slider::Listener*/
	void sliderValueChanged(Slider*) override;
	/*implement isInterestedInFileDrag*/
	bool isInterestedInFileDrag(const StringArray& files) override;
	/*implement filesDropped*/
	void filesDropped(const StringArray& files, int x, int y) override;
	/*implement timerCallback*/
	void timerCallback() override;
	/*implement updateToggleState*/
	String updateToggleState(juce::Button* button);
	/*implement loadURL*/
	void loadURL(URL audioURL);
	/*implement getLength()*/
	std::string getLength(URL audioURL);

private:
	TextButton loadButton{ "LOAD" };	
	ImageButton imgPlayButton;
	ImageButton imgPauseButton;
	ImageButton imgForwardButton;
	ImageButton imgRewindButton;
	/*ImageButton imgVolumeButton;
	ImageButton imgPlaybackButton;*/
	ToggleButton loopButton;

	Image play;
	Image pause;
	Image forward;
	Image rewind;
	/*Image volume;
	Image playback;*/

	Slider volSlider;
	Slider speedSlider;
	Slider posSlider;

	Label volLabel;
	Label speedLabel;

	DJAudioPlayer* player;

	WaveformDisplay waveformDisplay;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};