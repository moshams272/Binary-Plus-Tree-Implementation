#include <iostream>
#include <string>
#include <cstdlib>
#include "../functions.h"

// val=60&op=search
using namespace std;

string getParam(string query, string key)
{
    string keyPattern = key + "=";
    size_t pos = query.find(keyPattern);
    if (pos == string::npos)
        return "";
    pos += keyPattern.length();
    size_t endPos = query.find("&", pos);
    if (endPos == string::npos)
        return query.substr(pos);
    return query.substr(pos, endPos - pos);
}


int main()
{
    init_tree_file();

    cout << "Content-Type: text/html\n\n";

    cout << "<html dir='ltr'><head><meta charset='UTF-8'></head>";
    cout << "<body style='font-family:sans-serif; text-align:center;'>";
    cout << "<h2>The Result of Process in B+ Tree</h2>";

    char *data = getenv("QUERY_STRING");

    if (data != NULL)
    {
        string query(data);
        string valStr = getParam(query, "val");
        string opStr = getParam(query, "op");

        if (!opStr.empty())
        {
            if (opStr == "print") 
            {
                cout << "<h3>🌲 Tree Visualization (Print Only)</h3>";
                print_tree();
            }
            else 
            {
                if (!valStr.empty()) 
                {
                    int key = stoi(valStr);

                    if (opStr == "insert")
                    {
                        cout << "<h3 style='color:green'>Insert: " << key << "</h3>";
                        insert(key);
                        print_tree();
                    }
                    else if (opStr == "search")
                    {
                        cout << "<h3 style='color:green'>Search: " << key << "</h3>";
                        search(key);
                        print_tree();
                    }
                    else
                    {
                        cout << "<p style='color:red'>Error!! <br> We Don't Know That Process!!</p>";
                    }
                }
                else
                {
                    cout << "<p style='color:red'>Please Fill Data</p>";
                }
            }
        }
        else
        {
            cout << "<p style='color:red'>Please Select an Operation.</p>";
        }
    }
    else
    {
        cout << "<p style='color:red'>There is no data sent.</p>";
    }

    cout << "<br><a href='../index.html'><button>Return</button></a>";
    cout << "</body></html>";

    return 0;
}