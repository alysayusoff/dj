#include <JuceHeader.h>
#include "DeckGUI.h"

DeckGUI::DeckGUI(DJAudioPlayer* _player, AudioFormatManager& formatManagerToUse, AudioThumbnailCache& cacheToUse) : player(_player), waveformDisplay(formatManagerToUse, cacheToUse)
{
	addAndMakeVisible(loadButton);
	addAndMakeVisible(imgPlayButton);
	addAndMakeVisible(imgPauseButton);
	addAndMakeVisible(imgForwardButton);
	addAndMakeVisible(imgRewindButton);
	/*addAndMakeVisible(imgVolumeButton);
	addAndMakeVisible(imgPlaybackButton);*/
	addAndMakeVisible(volSlider);
	addAndMakeVisible(speedSlider);
	addAndMakeVisible(posSlider);
	addAndMakeVisible(waveformDisplay);
	addAndMakeVisible(loopButton);
	addAndMakeVisible(volLabel);
	addAndMakeVisible(speedLabel);

	loopButton.onClick = [this] {updateToggleState(&loopButton); };
	loopButton.setButtonText("LOOP");

	loadButton.addListener(this);
	imgPlayButton.addListener(this);
	imgPauseButton.addListener(this);
	imgForwardButton.addListener(this);
	imgRewindButton.addListener(this);
	volSlider.addListener(this);
	speedSlider.addListener(this);
	posSlider.addListener(this);

	volSlider.setValue(1);
	speedSlider.setValue(1);

	volSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 80, 20);
	speedSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 80, 20);
	posSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

	volSlider.setRange(0.0, 10.0);
	speedSlider.setRange(0.0, 2.0);
	posSlider.setRange(0.0, 1.0);

	volLabel.setText("Volume", dontSendNotification);
	volLabel.setJustificationType(Justification::centred);
	speedLabel.setText("Playback Speed", dontSendNotification);
	speedLabel.setJustificationType(Justification::centred);

	play = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("OtoDecksWinImg/imgPlayButton.png"));
	pause = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("OtoDecksWinImg/imgPauseButton.png"));
	forward = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("OtoDecksWinImg/imgForwardButton.png"));
	rewind = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("OtoDecksWinImg/imgRewindButton.png"));
	/*volume = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("OtoDecksWinImg/imgVolume.png"));
	playback = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("OtoDecksWinImg/imgPlayBack.png"));*/

	imgPlayButton.setImages(false, true, false, play, 1.0f, {}, play, 0.5f, {}, play, 0.2f, {});
	imgPauseButton.setImages(false, true, false, pause, 1.0f, {}, pause, 0.5f, {}, pause, 0.2f, {});
	imgForwardButton.setImages(false, true, false, forward, 1.0f, {}, forward, 0.5f, {}, forward, 0.2f, {});
	imgRewindButton.setImages(false, true, false, rewind, 1.0f, {}, rewind, 0.5f, {}, rewind, 0.2f, {});
	/*imgVolumeButton.setImages(false, true, false, volume, 1.0f, {}, volume, 1.0f, {}, volume, 1.0f, {});
	imgPlaybackButton.setImages(false, true, false, playback, 1.0f, {}, playback, 1.0f, {}, playback, 1.0f, {});*/

	startTimer(500);
}

DeckGUI::~DeckGUI()
{
	stopTimer();
}

void DeckGUI::paint(juce::Graphics& g)
{
	int y = getHeight() / 8;
	int x = getWidth() / 32;

	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)); // clear the background
	g.setColour(juce::Colours::grey);
	g.drawRect(getLocalBounds(), 1); // draw an outline around the component
	//g.drawRect(x * 26, 10, x * 6 - 10, getHeight() - 20); //right controls wrapper
	g.drawRect(10, 10, x * 6 - 10, getHeight() - 20); //left controls wrapper
}

void DeckGUI::resized()
{
	double y = getHeight() / 8;
	double x = getWidth() / 32;

	//ORIGINAL SPOTIFY INSPIRED LAYOUT
	/*imgVolumeButton.setBounds(x, 3, y, y);
	volSlider.setBounds(x * 2, 3, x * 11, y);

	imgRewindButton.setBounds(x * 14, 3, y, y);
	imgPlayButton.setBounds(x * 15, 3, y, y);
	imgPauseButton.setBounds(x * 16, 3, y, y);
	imgForwardButton.setBounds(x * 17, 3, y, y);

	imgPlaybackButton.setBounds(x * 19, 3, y, y);
	speedSlider.setBounds(x * 20, 3, x * 11, y);

	posSlider.setBounds(0, y, getWidth(), y);

	waveformDisplay.setBounds(10, y * 2, getWidth() - 20, y * 5);

	loopButton.setBounds(x * 29, y * 7, x * 3, y);*/

	//RIGHT CONTROLS
	/*posSlider.setBounds(0, 0, x * 26, y);
	waveformDisplay.setBounds(10, y, (x * 26) - 20, (y * 7) - 10);

	imgRewindButton.setBounds(x * 27, y + 3, y, y);
	imgPlayButton.setBounds(x * 28, y + 3, y, y);
	imgPauseButton.setBounds(x * 29, y + 3, y, y);
	imgForwardButton.setBounds(x * 30, y + 3, y, y);

	volLabel.setBounds((x * 26), (y * 2) + 5, (x * 6) - 10, y);
	volSlider.setBounds((x * 26), y * 3, (x * 6) - 10, y);
	speedLabel.setBounds((x * 26), (y * 4) + 5, (x * 6) - 10, y);
	speedSlider.setBounds((x * 26), y * 5, (x * 6) - 10, y);

	loopButton.setBounds(getWidth() - 82, y * 6, 62, y);*/

	//LEFT CONTROLS
	posSlider.setBounds(x * 6, 0, x * 26, y);
	waveformDisplay.setBounds(x * 6 + 10, y, (x * 26) - 20, (y * 7) - 10);

	imgRewindButton.setBounds(x + (0.00625 * getWidth()), y + 3, y, y);
	imgPlayButton.setBounds(x * 2 + (0.00625 * getWidth()), y + 3, y, y);
	imgPauseButton.setBounds(x * 3 + (0.00625 * getWidth()), y + 3, y, y);
	imgForwardButton.setBounds(x * 4 + (0.00625 * getWidth()), y + 3, y, y);

	volLabel.setBounds(10, (y * 2) + 5, (x * 6) - 10, y);
	volSlider.setBounds(10, y * 3, (x * 6) - 10, y);
	speedLabel.setBounds(10, (y * 4) + 5, (x * 6) - 10, y);
	speedSlider.setBounds(10, y * 5, (x * 6) - 10, y);

	loopButton.setBounds(20, y * 6, 62, y);
	loadButton.setBounds(x * 3 + 10, y * 6, x * 2, y);
}

void DeckGUI::buttonClicked(Button* button)
{
	if (button == &imgPlayButton)
		player->start();
	if (button == &imgPauseButton)
		player->stop();
	if (button == &imgRewindButton)
		player->rewind(posSlider.getValue());
	if (button == &imgForwardButton)
		player->forward(posSlider.getValue());
	if (button == &loadButton) {
		FileChooser chooser{ "Select a file..." };
		if (chooser.browseForFileToOpen()) {
			player->loadURL(URL{ chooser.getResult() });
			waveformDisplay.loadURL(URL{ chooser.getResult() });
		}
	}
}

void DeckGUI::sliderValueChanged(Slider* slider)
{
	if (slider == &volSlider)
		player->setGain(slider->getValue());
	if (slider == &speedSlider)
		player->setSpeed(slider->getValue());
	if (slider == &posSlider) {
		player->setPositionRelative(slider->getValue());
		//enable loop
		if (updateToggleState(&loopButton) == "ON") {
			if (slider->getValue() == 1)
				player->loop();
		}
	}
}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
	return true;
}

void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
	if (files.size() == 1)
		player->loadURL(URL{ File{files[0]} });
}

void DeckGUI::timerCallback() {
	waveformDisplay.setPositionRelative(player->getPositionRelative());
	//make slider change position as postitionRelative changes
	if (player->getPositionRelative() >= 0)
		posSlider.setValue(player->getPositionRelative());
}

String DeckGUI::updateToggleState(juce::Button* button) {
	auto state = button->getToggleState();
	juce::String stateString = state ? "ON" : "OFF";
	return stateString;
}

void DeckGUI::loadURL(URL audioURL) {
	player->loadURL(audioURL);
	waveformDisplay.loadURL(audioURL);
}

std::string DeckGUI::getLength(URL audioURL) {
	std::string fileLength = player->getLength(audioURL);
	return fileLength;
}