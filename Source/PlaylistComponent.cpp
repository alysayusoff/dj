#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <algorithm>

PlaylistComponent::PlaylistComponent(DeckGUI* deck1, DeckGUI* deck2) : deck1(deck1), deck2(deck2)
{
	trackTitles = readCSV("trackTitle.csv");
	trackLength = readCSV("trackLength.csv");
	trackPath = readCSV("trackPath.csv");

	tableComponent.getHeader().addColumn("File", 1, 312.5, 312.5, 505.4);
	tableComponent.getHeader().addColumn("Duration", 2, 120, 120, 192.15);
	tableComponent.getHeader().addColumn("Load into Deck 1", 3, 130, 130, 208.1625);
	tableComponent.getHeader().addColumn("Load into Deck 2", 4, 130, 130, 208.1625);
	tableComponent.getHeader().addColumn("Remove File", 5, 100, 100, 160.125);

	searchTextBox.setName("search");
	searchTextBox.setTextToShowWhenEmpty("SEARCH FOR A TRACK", Colour(uint8(255), uint8(255), uint8(255), uint8(100)));

	tableComponent.setModel(this);
	searchTextBox.addListener(this);
	addBtn.addListener(this);

	addAndMakeVisible(searchTextBox);
	addAndMakeVisible(tableComponent);
	addAndMakeVisible(addBtn);
}

PlaylistComponent::~PlaylistComponent()
{

}

void PlaylistComponent::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

	g.setColour(juce::Colours::grey);
	g.drawRect(getLocalBounds(), 1);

	g.setColour(juce::Colours::white);
	g.setFont(14.0f);
	g.drawText("PlaylistComponent", getLocalBounds(), juce::Justification::centred, true);
}

void PlaylistComponent::resized()
{
	double col = getWidth() / 10;
	double row = getHeight() / 8;

	searchTextBox.setBounds(0, 0, col * 8, row);
	addBtn.setBounds(col * 8, 0, col * 2, row);

	tableComponent.setBounds(0, row, getWidth(), row * 7);
}

int PlaylistComponent::getNumRows() {
	return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) {
	if (rowIsSelected)
		g.fillAll(Colours::mediumpurple);
	else
		g.fillAll(Colours::darkgrey);
}

void PlaylistComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
	if (rowIsSelected)
		g.setColour(Colours::black);
	else
		g.setColour(Colours::white);
	if (columnId == 1)
		g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, true);
	if (columnId == 2)
		g.drawText(trackLength[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, true);
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) {
	if (columnId == 3) {
		if (existingComponentToUpdate == nullptr) {
			TextButton* btn = new TextButton{ "DECK 1" };
			String id{ std::to_string(rowNumber) };
			btn->setName("deckOne"); //set component name
			btn->setComponentID(id);
			btn->addListener(this);
			existingComponentToUpdate = btn;
		}
	}
	if (columnId == 4) {
		if (existingComponentToUpdate == nullptr) {
			TextButton* btn = new TextButton{ "DECK 2" };
			String id{ std::to_string(rowNumber) };
			btn->setName("deckTwo"); //set component name
			btn->setComponentID(id);
			btn->addListener(this);
			existingComponentToUpdate = btn;
		}
	}
	if (columnId == 5) {
		if (existingComponentToUpdate == nullptr) {
			TextButton* btn = new TextButton{ "REMOVE" };
			String id{ std::to_string(rowNumber) };
			btn->setName("removeBtn"); //set component name
			btn->setComponentID(id);
			btn->addListener(this);
			existingComponentToUpdate = btn;
		}
	}
	return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button) {
	if (button->getName() == "deckOne") {
		int id = std::stoi(button->getComponentID().toStdString());
		deck1->loadURL(URL{ trackPath[id] });
	}
	if (button->getName() == "deckTwo") {
		int id = std::stoi(button->getComponentID().toStdString());
		deck2->loadURL(URL{ trackPath[id] });
	}
	if (button->getName() == "removeBtn") {
		int id = std::stoi(button->getComponentID().toStdString());
		//store vectors into temp vectors
		std::vector<std::string> tempTitles = trackTitles;
		std::vector<std::string> tempPath = trackPath;
		std::vector<std::string> tempLength = trackLength;
		//clear main vectors
		trackTitles.clear();
		trackPath.clear();
		trackLength.clear();
		//rewrite
		for (int i = 0; i < tempTitles.size(); i++) {
			if (id != i) {
				trackTitles.push_back(tempTitles[i]);
				trackPath.push_back(tempPath[i]);
				trackLength.push_back(tempLength[i]);
			}
		}
		//rewrite data
		removeCSV(id, "trackTitle.csv");
		removeCSV(id, "trackPath.csv");
		removeCSV(id, "trackLength.csv");
		//update
		tableComponent.updateContent();
		repaint();
	}
	if (button == &addBtn) {
		int id = trackTitles.size();
		FileChooser chooser{ "Select a file..." };
		if (chooser.browseForFileToOpen()) {
			//get data
			std::string fileName = chooser.getResult().getFileName().toStdString();
			std::string filePath = chooser.getURLResult().toString(true).toStdString();
			std::string fileLength = deck1->getLength(URL{ filePath }); //does not matter which player
			//clear and repaint
			trackTitles.push_back(fileName);
			trackPath.push_back(filePath);
			trackLength.push_back(fileLength);
			//store data
			addCSV(id, fileName, "trackTitle.csv");
			addCSV(id, filePath, "trackPath.csv");
			addCSV(id, fileLength, "trackLength.csv");
			//update
			tableComponent.updateContent();
			repaint();
		}
	}
}

void PlaylistComponent::textEditorTextChanged(TextEditor& textEditor) {
	std::string search = textEditor.getTextValue().toString().toStdString();
	for (int i = 0; i < trackTitles.size(); i++) {
		if (search == "") {
			tableComponent.deselectAllRows();
		}
		else if (trackTitles[i].find(search) != std::string::npos) {
			tableComponent.selectRow(i, false, false);
		}
	}
	//https://stackoverflow.com/questions/2340281/check-if-a-string-contains-a-string-in-c
}

std::vector<std::string> PlaylistComponent::readCSV(std::string csvFilename)
{
	std::vector<std::string> tracks;
	std::ifstream csvFile{ csvFilename };
	std::string line;
	if (csvFile.is_open()) {
		while (std::getline(csvFile, line)) {
			try {
				std::vector<std::string> tokens = tokenise(line, ',');
				tracks.push_back(tokens[1]);
			}
			catch (const std::exception& e) {
				DBG("Something went wrong trying to read line");
			}
		}
	}
	else
		DBG("Something went wrong trying to load " << csvFilename);
	return tracks;
}

void PlaylistComponent::addCSV(int id, std::string data, std::string csvFilename)
{
	std::ifstream inFile(csvFilename, std::ios::in);
	std::ofstream outFile("temp.csv", std::ios::out | std::ofstream::app);
	std::string line;
	int count = 0;
	if (inFile.is_open()) {
		if (outFile.is_open()) {
			while (std::getline(inFile, line)) {
				count++;
				try {
					outFile << line << "\n";
					if (id == count) {
						std::string newLine = std::to_string(id) + "," + data;
						outFile << newLine << "\n";
					}
				}
				catch (const std::exception& e) {
					DBG("Something went wrong trying to read line");
				}
			}
		}
		inFile.close();
		outFile.close();
		const char* cstr = csvFilename.c_str();
		remove(cstr);
		rename("temp.csv", cstr);
	}
	else
		DBG("inFile not open");
}

void PlaylistComponent::removeCSV(int id, std::string csvFilename)
{
	std::ifstream inFile(csvFilename, std::ios::in);
	std::ofstream outFile("temp.csv", std::ios::out | std::ofstream::app);
	std::string line;
	if (inFile.is_open()) {
		if (outFile.is_open()) {
			while (std::getline(inFile, line)) {
				try {
					std::vector<std::string> token = tokenise(line, ',');
					if (std::stoi(token[0]) < id)
						outFile << line << "\n";
					else if (std::stoi(token[0]) == id)
					{
						//nothing happens
					}
					else {
						int newId = std::stoi(token[0]) - 1;
						std::string newLine = std::to_string(newId) + "," + token[1];
						outFile << newLine << "\n";
					}
				}
				catch (const std::exception& e) {
					DBG("Something went wrong trying to read line");
				}
			}
		}
		inFile.close();
		outFile.close();
		const char* cstr = csvFilename.c_str();
		remove(cstr);
		rename("temp.csv", cstr);
	}
	else
		DBG("inFile not open");

	//https://www.worldbestlearningcenter.com/tips/Cplusplus-remove-line-from-a-file.htm#:~:text=To%20remove%20a%20specific%20line,temporarily%20file%20using%20ofstream%20class
}

std::vector<std::string> PlaylistComponent::tokenise(std::string csvLine, char separator)
{
	std::vector<std::string> tokens;
	signed int start, end;
	std::string token;
	start = csvLine.find_first_not_of(separator, 0);
	do {
		end = csvLine.find_first_of(separator, start);
		if (start == csvLine.length() || start == end) break;
		if (end >= 0) token = csvLine.substr(start, end - start);
		else token = csvLine.substr(start, csvLine.length() - start);
		tokens.push_back(token);
		start = end + 1;
	} while (end > 0);

	return tokens;
}