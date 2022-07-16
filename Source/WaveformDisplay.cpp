#include <JuceHeader.h>
#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(AudioFormatManager& formatManagerToUse, AudioThumbnailCache& cacheToUse) : audioThumbnail(1000, formatManagerToUse, cacheToUse), fileLoaded(false), position(0.0)
{
	audioThumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{

}

void WaveformDisplay::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

	g.setColour(juce::Colours::grey);
	g.drawRect(getLocalBounds(), 1);

	g.setColour(juce::Colours::mediumpurple);

	if (fileLoaded) {
		audioThumbnail.drawChannel(g, getLocalBounds(), 0, audioThumbnail.getTotalLength(), 0, 1.0f);
		g.setColour(Colours::lightgreen);
		g.fillRect(position * getWidth(), 0, 1, getHeight());
	}
	else {
		g.setFont(20.0f);
		g.drawText("File not loaded", getLocalBounds(), juce::Justification::centred, true);
	}
}

void WaveformDisplay::resized()
{

}

void WaveformDisplay::loadURL(URL audioURL) {
	audioThumbnail.clear();
	fileLoaded = audioThumbnail.setSource(new URLInputSource(audioURL));
	if (fileLoaded)
		repaint();
	else
		DBG("WaveformDisplay::loadURL not loaded");
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source) {
	repaint();
}

void WaveformDisplay::setPositionRelative(double pos) {
	if (pos != position) {
		position = pos;
		repaint();
	}
}