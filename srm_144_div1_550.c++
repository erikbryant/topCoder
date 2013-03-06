#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <cmath>

using namespace std;

class Lottery
{
public:
  Lottery( void )
  {
  }
  
  vector<string> sortByOdds( const vector<string> rules );

private:
  unsigned long int odds( int choices, int blanks, bool sorted, bool unique );
  unsigned long int factorial( int choices, int blanks );
  pair<unsigned long int, string> parseRecord( const string &ticket );
};

unsigned long int Lottery::factorial( int choices, int blanks )
{
  int i = 0;
  unsigned long int result = 1;

  for ( i=1; i<=blanks; i++ )
    {
      result *= choices;
      choices--;
    }

  return result;
}

unsigned long int Lottery::odds( int choices, int blanks, bool sorted, bool unique )
{
  unsigned long int combinations = 0;

  if ( unique )
    {
      combinations = this->factorial( choices, blanks );
    }
  else
    {
      combinations = pow( choices, blanks );
    }

  if ( sorted )
    {
      combinations -= (pow( choices, blanks ) - choices) / 2;   // TODO: BROKEN! Work out this calculation
    }

  return combinations;
}

pair<unsigned long int, string> Lottery::parseRecord( const string &record )
{
  string name;
  string ticket = record;
  int choices = 0;
  int blanks = 0;
  bool sorted = false;
  bool unique = false;
  unsigned long int combinations = 0;
  string::size_type pos = ticket.find( ':', 0 );

  name = ticket.substr( 0, pos );
  pos += 2;
  ticket = ticket.substr( pos, ticket.length() );
  choices = stoi( ticket, &pos );
  ticket = ticket.substr( pos, ticket.length() );
  blanks = stoi( ticket, &pos );
  pos++;
  sorted = ticket[pos] == 'T';
  unique = ticket[pos+2] == 'T';
  combinations = odds( choices, blanks, sorted, unique );

  cout << name << ": " << choices << " " << blanks << " " << (sorted ? 'T' : 'F') << " " << (unique ? 'T' : 'F') << "  -> " << combinations << endl;

  pair<unsigned long int, string> entry( combinations, name );

  return entry;
}

vector<string> Lottery::sortByOdds( const vector<string> rules )
{
  vector<string> result;
  list< pair<unsigned long int, string> > games;
  vector<string>::const_iterator it;

  for ( it=rules.begin(); it!=rules.end(); it++ )
    {
      pair<unsigned long int, string> entry = parseRecord( *it );
      games.push_back( entry );
    }

  games.sort();

  list< pair<unsigned long int, string> >::iterator lit;
  for ( lit=games.begin(); lit!=games.end(); lit++ )
    {
      result.push_back( (*lit).second );
    }

  return result;
}

int main( int argc, char *argv[] )
{
  Lottery lottery;
  vector<string> rules;
  vector<string> result;
  vector<string>::const_iterator it;

  cout << "Example 1: " << endl;
  rules.clear();
  rules.push_back( "PICK ANY TWO: 10 2 F F" );
  rules.push_back( "PICK TWO IN ORDER: 10 2 T F" );
  rules.push_back( "PICK TWO DIFFERENT: 10 2 F T" );
  rules.push_back( "PICK TWO LIMITED: 10 2 T T" );
  result = lottery.sortByOdds( rules );
  for ( it=result.begin(); it!=result.end(); it++ )
    {
      cout << *it << ", ";
    }
  cout << endl;

  cout << endl << "Example 2: " << endl;
  rules.clear();
  rules.push_back( "INDIGO: 93 8 T F" );
  rules.push_back( "ORANGE: 29 8 F T" );
  rules.push_back( "VIOLET: 76 6 F F" );
  rules.push_back( "BLUE: 100 8 T T" );
  rules.push_back( "RED: 99 8 T T" );
  rules.push_back( "GREEN: 78 6 F T" );
  rules.push_back( "YELLOW: 75 6 F F" );
  result = lottery.sortByOdds( rules );
  for ( it=result.begin(); it!=result.end(); it++ )
    {
      cout << *it << ", ";
    }
  cout << endl;

  cout << endl << "Example 3: " << endl;
  rules.clear();
  result = lottery.sortByOdds( rules );
  for ( it=result.begin(); it!=result.end(); it++ )
    {
      cout << *it << ", ";
    }
  cout << endl;
}
