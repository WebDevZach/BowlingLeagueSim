/*
Zachary Leyes

6/19/2023

MidTerm Project

CIS 1202 201
*/



#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

//4 weeks in a league
const int WEEKS = 4;

//2 games bowled for each match
const int GAMES = 2;

//Structure for a bowler
struct Bowler
{
	string bowlerName;
	int scores[WEEKS][GAMES];
};

//Structure for a team
struct Team
{
	string teamName;
	Bowler bowlerOne;
	Bowler bowlerTwo;
	int points = 0;
};


void addTeams(vector<Team>&, int, fstream&);
void displaySchedule(vector<Team>);
void displayMainMenu(vector<Team>&, fstream&, string, vector<vector<vector<string>>>);
void enterMatchScores(vector<Team>&, fstream&, string, vector<vector<vector<string>>>);
void enterIndividualScores(int, fstream&, string, int, vector<Team>&, vector<vector<vector<string>>>);
void generateMatchSchedule(vector<Team>, vector<vector<vector<string>>>&);
void updatePoints(vector<vector<vector<string>>>, vector<Team>&, int);
void displayLeagueStandings(vector<Team>);
void lookupTeamInfo(vector<Team>);




int main()
{

	string fileName;

	//vector for teams in the league
	vector<Team> teams;

	//Asks user for a file name
	cout << "Enter the file name>";
	cin.ignore();
	getline(cin, fileName);

	//Creates file object for writing
	fstream inputFile(fileName, ios::out);


	int numOfTeams;

	//Asks the user how many teams are in the league (has to be an even number 2 or greater)
	cout << "Enter number of teams in the league (has to be an even number): ";
	cin >> numOfTeams;

	//Validates the user input
	while (numOfTeams <= 1 || numOfTeams % 2 == 1)
	{
		cout << "The of teams in a league has to be greater than 1 and an even number: ";
		cin >> numOfTeams;
	}


	//number of weeks in a league
	int weeks = WEEKS;
	
	//Calculates the number of matches for the amount of teams in the league
	int matches = numOfTeams / 2;

	//Sets the number of teams in a match to 2;
	int teamsPerMatch = 2;

	//Intialized a 3-dimensional vector (chose this over dynamically allocating and deleting a 3D array)
	//3D vector that stores which teams play in a match for a given week 
	vector<vector<vector<string>>> matchSchedule(weeks, vector<vector<string>>(matches, vector<string>(teamsPerMatch)));

	//Adds team objects to the teams vector
	addTeams(teams, numOfTeams, inputFile);

	//Generates the match schedule 
	generateMatchSchedule(teams, matchSchedule);

	//Displays the main menu 
	displayMainMenu(teams, inputFile, fileName, matchSchedule);

	return 0;
}




void addTeams(vector<Team>& teams, int numOfTeams, fstream& inputFile)
{
	string teamName;
	string bowlerName;

	cin.ignore();

	//Asks user to enter all team information
	for (int x = 0; x < numOfTeams; x++)
	{
		Team team;

		//Team name
		cout << "Team " << x + 1 << "#'s name: ";
		getline(cin, teamName);

		//Writes to file
		inputFile << teamName << endl;
		team.teamName = teamName;

		//Bowler one's name
		cout << "Enter the first bowler's name: ";
		getline(cin, bowlerName);

		//Writes to file
		inputFile << bowlerName << endl;
		team.bowlerOne.bowlerName = bowlerName;

		//Bowler two's name 
		cout << "Enter the second bowler's name: ";
		getline(cin, bowlerName);

		//Writes to file
		inputFile << bowlerName << endl;
		team.bowlerTwo.bowlerName = bowlerName;

		//Stores team object in the vector
		teams.push_back(team);
	}


	//Writes week 1 at the end of file
	inputFile << "Week 1" << endl;


	inputFile.close();
}


//Displays the schedule for which teams play each other on a certain week
void displaySchedule(vector<Team> teams)
{

	int numOfTeams = teams.size();
	Team temp;

	cout << endl;

	//Round robin algorithm
	//Never moves the team at index 0
	//Then moves the last team to index 1 and then shifts all teams after index 1 over an index


	//Loops for weeks in a league
	for (int x = 0; x < WEEKS; x++)
	{
		//Displays the week 
		cout << "Week " << x + 1 << ":" << endl;

		//Loops for matches 
		for (int i = 0; i < numOfTeams / 2; i++)
		{
			//Displays the teams matchup
			//The first team plays the last team then the second team plays the second to last and so on 
			cout << teams[i].teamName << " vs " << teams[numOfTeams - 1 - i].teamName << endl;
		}

		//Stores the last team in the vector
		temp = teams[numOfTeams - 1];

		//Starts loop at the end of the vector then moves down until index 1 is reached
		for (int i = numOfTeams - 1; i > 1; i--)
		{
			//Sets the value of an index to the index that comes before it 
			teams[i] = teams[i - 1];
		}
		
		//Places the last team in the vector at index 1 
		teams[1] = temp;

		cout << endl;
	}
}

//Displays the main menu 
void displayMainMenu(vector<Team>& teams, fstream& inputFile, string fileName, vector<vector<vector<string>>> matchSchedule)
{
	int selection;

	do
	{
		//Main menu
		cout << "\n1) Enter bowling match scores for a week" << endl;
		cout << "2) Display league schedule" << endl;
		cout << "3) Display league standing" << endl;
		cout << "4) Lookup a particular team's information" << endl;
		cout << "5) Exit the program" << endl;
		cin >> selection;
		
		//Validation for the user input
		while (selection < 1 || selection > 5)
		{
			cout << "Enter a valid selection 1-5: ";
			cin >> selection;
		}

		//Chooses the correct function for user input
		if (selection == 1)
		{
			//Enters scores for each team for the next week
			enterMatchScores(teams, inputFile, fileName, matchSchedule);
		}
		if (selection == 2)
		{
			//Displays the match schedule for each week 
			displaySchedule(teams);
		}
		if (selection == 3)
		{
			//Displays the league standings 
			displayLeagueStandings(teams);
		}
		if (selection == 4)
		{
			//Searches for a team and displays the team's information 
			lookupTeamInfo(teams);
		}
	} while (selection != 5);
}


//Enters the match scores for the next week
void enterMatchScores(vector<Team>& teams, fstream& inputFile, string fileName, vector<vector<vector<string>>> matchSchedule)
{
	int numOfTeams = teams.size();
	string teamName;
	string bowlerOne, bowlerTwo;

	//Opens file for writing in append mode
	inputFile.open(fileName, ios::in | ios::app);

	//Checks if file was open successfully 
	if (!inputFile.is_open())
	{
		cout << "Failed to open :(" << endl;
	}

	//Disclaimer
	//For the program to know what week you are entering I chose to do this by counting the lines on the text file
	//I did this way because orginally I wanted to add an option at the start of the program to where you can chose an existing file 
	//I wanted use files as a sense of storage so you could have multiple different league files
	//You could then enter scores for a week for a league then close this program and the data would be stored
	//Do to time consumption and project length I took this feature out but left this function as is
	

	string line;


	int totalLines = 0;

	//Counts total lines on text file 
	while (getline(inputFile, line))
	{
		totalLines++;
	}

	//Calculates the amount of lines for week one 
	int totalLinesForWeekOne = (numOfTeams * 2) + numOfTeams + 1;


	//Week one
	if (totalLines == totalLinesForWeekOne)
	{
		//Enters score for week one
		enterIndividualScores(0, inputFile, fileName, numOfTeams, teams, matchSchedule);
	}

	//Calculates how many lines are added after each week
	int calculationForLines = (numOfTeams * 4) + (numOfTeams)+1;

	//Calculates the amount of lines for week two
	int totalLinesForWeekTwo = totalLinesForWeekOne + calculationForLines;


	//Week Two
	if (totalLines == totalLinesForWeekTwo)
	{
		//Enters scores for week two
		enterIndividualScores(1, inputFile, fileName, numOfTeams, teams, matchSchedule);
	}

	//Calculates lines for week three
	int totalLinesForWeekThree = calculationForLines * 2 + totalLinesForWeekOne;


	//Week Three
	if (totalLines == totalLinesForWeekThree)
	{
		//Enters scores for week three
		enterIndividualScores(2, inputFile, fileName, numOfTeams, teams, matchSchedule);
	}

	//Calculates lines for week four
	int totalLinesForWeekFour = calculationForLines * 3 + totalLinesForWeekOne;


	//Week Four
	if (totalLines == totalLinesForWeekFour)
	{
		//Enters scores for week four
		enterIndividualScores(3, inputFile, fileName, numOfTeams, teams, matchSchedule);
	}

	//Calculates lines for the whole file
	int totalLinesForWholeTextFile = calculationForLines * 4 + totalLinesForWeekOne;


	//League is over 
	if (totalLines == totalLinesForWholeTextFile)
	{
		cout << "\nLeague play is completed!!!!!!!!!!! :D" << endl;
	}



	inputFile.close();
}


void enterIndividualScores(int index, fstream& inputFile, string fileName, int numOfTeams, vector<Team>& teams, vector<vector<vector<string>>> matchSchedule)
{
	inputFile.close();

	inputFile.open(fileName, ios::out | ios::app);

	Team temp;

	//Displays the week
	cout << "\nWeek " << index + 1 << " scores" << endl;

	//Loops for each team
	for (int i = 0; i < numOfTeams; i++)
	{

		cout << "Entering scores for team " << teams[i].teamName << endl;
		cout << "---------------------------------------------" << endl;

		//Writes team name to file
		inputFile << teams[i].teamName << endl;

		int x = 0;

		//Loop for each game
		for (int x = 0; x < GAMES; x++)
		{
			int score = 0;

			//Enters the first bowler's score 
			cout << "Enter " << teams[i].bowlerOne.bowlerName << "'s score for game " << x + 1 << " : ";
			cin >> score;

			//Validation for score
			while (score < 0 || score > 300)
			{
				cout << "Invalid score (0-300): ";
				cin >> score;
			}

			//Stores score in the bowler object
			teams[i].bowlerOne.scores[index][x] = score;

			//Writes score to file
			inputFile << teams[i].bowlerOne.scores[index][x] << endl;


			//Enters the second bowler's score 
			cout << "Enter " << teams[i].bowlerTwo.bowlerName << "'s score for game " << x + 1 << " : ";
			cin >> score;

			//Validation for score
			while (score < 0 || score > 300)
			{
				cout << "Invalid score (0-300): ";
				cin >> score;
			}

			//Stores score in bowler object
			teams[i].bowlerTwo.scores[index][x] = score;

			//Writes score to file
			inputFile << teams[i].bowlerTwo.scores[index][x] << endl;

			cout << endl;
		}
	}


	//Updates points based on scores for the week 
	updatePoints(matchSchedule, teams, index);

	//Writes the week number on the file for the next week  
	index += 2;
	string printWeek = "Week " + to_string(index);
	inputFile << printWeek << endl;
}


//Generates the match schedule
void generateMatchSchedule(vector<Team> teams, vector<vector<vector<string>>>& matchSchedule)
{

	int numOfTeams = teams.size();
	Team temp;

	//Loops for each week
	for (int x = 0; x < WEEKS; x++)
	{
		//Loops for each match
		for (int i = 0; i < numOfTeams / 2; i++)
		{
			//Stores the name for team one for the match
			matchSchedule[x][i][0] = teams[i].teamName;

			//Stores the name for team two for the match
			matchSchedule[x][i][1] = teams[numOfTeams - 1 - i].teamName;
		}

		//Rotates teams with the round robin algorithm 
		temp = teams[numOfTeams - 1];
		for (int i = numOfTeams - 1; i > 1; i--)
		{
			teams[i] = teams[i - 1];
		}
		teams[1] = temp;
	}


}


//Updates the points for each team 
void updatePoints(vector<vector<vector<string>>> matchSchedule, vector<Team>& teams, int weekNumber)
{
	int numOfTeams = teams.size();

	//Loops for each match
	for (int y = 0; y < numOfTeams / 2; y++)
	{
		//Stores first team name for a match
		string teamOne = matchSchedule[weekNumber][y][0];

		//Stores second team name for a match
		string teamTwo = matchSchedule[weekNumber][y][1];


		int positionIndexTeamOne = 0;
		int positionIndexTeamTwo = 0;
		int teamOneScore = 0;
		int teamTwoScore = 0;

		//Sequential search for team names
		for (int x = 0; x < numOfTeams; x++)
		{
			//Stores the index of team one 
			if (teams[x].teamName == teamOne)
			{
				positionIndexTeamOne = x;
			}

			//Stores the index of team two
			if (teams[x].teamName == teamTwo)
			{
				positionIndexTeamTwo = x;
			}
		}

		//Loop for each game
		for (int z = 0; z < GAMES; z++)
		{
			//Calculates team one's score for a game
			teamOneScore = teams[positionIndexTeamOne].bowlerOne.scores[weekNumber][z] + teams[positionIndexTeamOne].bowlerTwo.scores[weekNumber][z];

			//Calculates team two's score for a game 
			teamTwoScore = teams[positionIndexTeamTwo].bowlerOne.scores[weekNumber][z] + teams[positionIndexTeamTwo].bowlerTwo.scores[weekNumber][z];

			//Awards two points to the team that won the game (A match has 2 games so a team can win up to 4 points a match 2 points for each game)
			if (teamOneScore > teamTwoScore)
			{
				teams[positionIndexTeamOne].points = teams[positionIndexTeamOne].points + 2;
			}
			if (teamOneScore < teamTwoScore)
			{
				teams[positionIndexTeamTwo].points = teams[positionIndexTeamTwo].points + 2;
			}
		}
	}
}

//Displays team points and their standings
void displayLeagueStandings(vector<Team> teams)
{
	int numOfTeams = teams.size();

	//Selection sort 
	for (int x = 0; x < numOfTeams - 1; x++)
	{
		int minIndex = x;
		for (int y = x + 1; y < numOfTeams; y++)
		{
			if (teams[y].points > teams[minIndex].points) {
				minIndex = y;
			}
		}
		if (minIndex != x)
		{
			Team temp = teams[x];
			teams[x] = teams[minIndex];
			teams[minIndex] = temp;
		}
	}

	cout << endl;

	//Displays teams and their points to the user in descending order
	for (int z = 0; z < numOfTeams; z++)
	{
		cout << teams[z].teamName << " :" << teams[z].points << endl;
	}

}


//Looks up a team's information 
void lookupTeamInfo(vector<Team>teams)
{
	string search;
	int numOfTeams = teams.size();
	int position = -1;

	cout << "\nTeam name?: ";
	cin >> search;

	//Sequential Search
	for (int x = 0; x < numOfTeams; x++)
	{
		if (teams[x].teamName == search)
		{
			position = x;
		}
	}

	//If the name was found displays the team's info
	if (position != -1)
	{
		cout << endl << teams[position].teamName << endl;
		cout << "Bowler 1: " << teams[position].bowlerOne.bowlerName << endl;
		cout << "Bowler 2: " << teams[position].bowlerTwo.bowlerName << endl;
		cout << "Points: " << teams[position].points;
	}
	else //displays error if team name was not found
	{
		cout << "\nTeam name was not found" << endl;
	}
}











