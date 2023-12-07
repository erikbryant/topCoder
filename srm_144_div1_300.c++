#include <iostream>
#include <vector>
#include <string>

using namespace std;

#define EMPTY ' '

class BinaryCode
{
public:
  vector<string> decode(const string &message);

private:
  bool setCell(string &s, int cell, char value);
  bool tryBit(char bit, const string &message, string &decode);
};

bool BinaryCode::setCell(string &s, int cell, char value)
{
  if (s[cell] != value && s[cell] != EMPTY)
  {
    return false;
  }

  s[cell] = value;
  return true;
}

bool BinaryCode::tryBit(char bit, const string &message, string &decode)
{
  string code = '-' + message + '-';
  decode = code;
  unsigned int i = 0;

  // Initialize the decode string to all EMPTY
  // Then put sentinels on it
  for (i = 0; i < decode.length(); i++)
  {
    decode[i] = EMPTY;
  }
  decode[0] = '0';
  decode[decode.length() - 1] = '0';

  decode[1] = bit;

  //
  // Cells in the code that are '0' or '3' are
  // simple to do. Transcribe their results to
  // the decode string.
  //
  for (i = 1; i < code.length() - 1; i++)
  {
    if (code[i] == '3')
    {
      if (!setCell(decode, i - 1, '1') ||
          !setCell(decode, i, '1') ||
          !setCell(decode, i + 1, '1'))
      {
        decode = "NONE";
        break;
      }
    }
    else if (code[i] == '0')
    {
      if (!setCell(decode, i - 1, '0') ||
          !setCell(decode, i, '0') ||
          !setCell(decode, i + 1, '0'))
      {
        decode = "NONE";
        return false;
      }
    }
  }

  //
  // See if there are any blanks left in the
  // decoded message. If there are, compute
  // values for each.
  //
  for (i = 1; i < decode.length() - 1; i++)
  {
    if (decode[i] == EMPTY)
    {
      decode[i] = (code[i - 1] - '0') - (decode[i - 2] - '0') - (decode[i - 1] - '0') + '0';
      if (decode[i] != '0' && decode[i] != '1')
      {
        decode = "NONE";
        return false;
      }
    }
  }

  // All the cells have been decoded. See if
  // the message is consistent. Start one bit
  // further into the message to avoid a fault.
  for (i = 2; i < decode.length() - 1; i++)
  {
    int expected = (code[i - 1] - '0') - (decode[i - 2] - '0') - (decode[i - 1] - '0') + '0';
    if (decode[i] != expected)
    {
      decode = "NONE";
      return false;
    }
  }

  // Strip the sentinels off of each end
  decode.erase(0, 1);
  decode.erase(decode.length() - 1, 1);

  return true;
}

vector<string> BinaryCode::decode(const string &message)
{
  vector<string> decoded;
  string decode;

  tryBit('0', message, decode);
  decoded.push_back(decode);
  tryBit('1', message, decode);
  decoded.push_back(decode);

  return decoded;
}

int main(int argc, char *argv[])
{
  BinaryCode coder;
  vector<string> result;

  result = coder.decode("123210122");
  cout << "Result[0] = /" << result[0] << "/" << endl;
  cout << "Result[1] = /" << result[1] << "/" << endl
       << endl;

  result = coder.decode("11");
  cout << "Result[0] = /" << result[0] << "/" << endl;
  cout << "Result[1] = /" << result[1] << "/" << endl
       << endl;

  result = coder.decode("22111");
  cout << "Result[0] = /" << result[0] << "/" << endl;
  cout << "Result[1] = /" << result[1] << "/" << endl
       << endl;

  result = coder.decode("12321010");
  cout << "Result[0] = /" << result[0] << "/" << endl;
  cout << "Result[1] = /" << result[1] << "/" << endl
       << endl;

  result = coder.decode("3");
  cout << "Result[0] = /" << result[0] << "/" << endl;
  cout << "Result[1] = /" << result[1] << "/" << endl
       << endl;

  result = coder.decode("12221112222221112221111111112221111");
  cout << "Result[0] = /" << result[0] << "/" << endl;
  cout << "Result[1] = /" << result[1] << "/" << endl
       << endl;
}
