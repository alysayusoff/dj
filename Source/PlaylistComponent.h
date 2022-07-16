#pragma once
#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include <iostream>
#include <fstream>
using namespace juce;

class PlaylistComponent : public juce::Component, public TableListBoxModel, public Button::Listener, public TextEditor::Listener
{
public:
	PlaylistComponent(DeckGUI* deck1, DeckGUI* deck2);
	~PlaylistComponent() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	int getNumRows() override;
	void paintRowBackground(Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
	void paintCell(Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
	Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;
	void buttonClicked(Button* button) override;

	static std::vector<std::string> readCSV(std::string csvFile);
	static void addCSV(int id, std::string data, std::string csvFile);
	static void removeCSV(int id, std::string csvFile);

	static std::vector<std::string> tokenise(std::string csvLine, char separator);

	void textEditorTextChanged(TextEditor&) override;

private:
	TableListBox tableComponent;
	std::vector<std::string> trackTitles;
	std::vector<std::string> trackPath;
	std::vector<std::string> trackLength;

	TextButton addBtn{ "ADD A NEW FILE" };

	DeckGUI* deck1;
	DeckGUI* deck2;

	TextEditor searchTextBox;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};