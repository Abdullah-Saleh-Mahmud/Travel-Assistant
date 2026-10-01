#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Place
{
    string name;
    int planStart, planEnd;
    int openTime, closeTime;
    int cost;
    int enjoyment;

    Place(string n, int ps, int pe, int os, int oe, int c, int e)
    {
        name = n;
        planStart = ps;
        planEnd = pe;
        openTime = os;
        closeTime = oe;
        cost = c;
        enjoyment = e;
    }
};

bool isValidPlan(Place p)
{
    return p.planStart >= p.openTime && p.planEnd <= p.closeTime;
}

bool compareEndTime(Place a, Place b)
{
    return a.planEnd < b.planEnd;
}

vector<Place> selectPlaces(vector<Place> &places, int budget)
{
    int n = places.size();
    sort(places.begin(), places.end(), compareEndTime);

    vector<int> totalEnjoy(n + 1, 0);
    vector<int> totalCost(n + 1, 0);
    vector<vector<int>> selectedIndices(n + 1);

    for (int i = 1; i <= n; i++)
    {
        totalEnjoy[i] = totalEnjoy[i - 1];
        totalCost[i] = totalCost[i - 1];
        selectedIndices[i] = selectedIndices[i - 1];

        int currentStart = places[i - 1].planStart;
        int currentEnd = places[i - 1].planEnd;
        int currentCost = places[i - 1].cost;
        int currentEnjoy = places[i - 1].enjoyment;

        // NEW: Consider taking this place alone
        if (currentCost <= budget)
        {
            if (currentEnjoy > totalEnjoy[i] ||
                (currentEnjoy == totalEnjoy[i] && currentCost < totalCost[i]))
            {
                totalEnjoy[i] = currentEnjoy;
                totalCost[i] = currentCost;
                selectedIndices[i] = {i - 1};
            }
        }

        // Explore combinations with earlier places
        for (int j = i - 1; j >= 0; j--)
        {
            if (places[j].planEnd <= currentStart)
            {
                int newCost = totalCost[j] + currentCost;
                int newEnjoy = totalEnjoy[j] + currentEnjoy;

                if (newCost <= budget)
                {
                    if (newEnjoy > totalEnjoy[i] ||
                        (newEnjoy == totalEnjoy[i] && newCost < totalCost[i]))
                    {
                        totalEnjoy[i] = newEnjoy;
                        totalCost[i] = newCost;
                        selectedIndices[i] = selectedIndices[j];
                        selectedIndices[i].push_back(i - 1);
                    }
                }
            }
        }
    }

    vector<Place> result;
    for (int idx : selectedIndices[n])
    {
        result.push_back(places[idx]);
    }
    return result;
}

void printPlan(vector<Place> &selected)
{
    int totalCost = 0;
    int totalEnjoyment = 0;

    cout << "\n--- Best Plan to Visit Places ---\n";
    for (Place p : selected)
    {
        cout << "Place: " << p.name
             << " | Time: " << p.planStart << "-" << p.planEnd
             << " | Cost: " << p.cost
             << " | Enjoyment: " << p.enjoyment << endl;
        totalCost += p.cost;
        totalEnjoyment += p.enjoyment;
    }

    cout << "Total places visited: " << selected.size() << endl;
    cout << "Total cost: " << totalCost << endl;
    cout << "Total enjoyment: " << totalEnjoyment << endl;
}

int main()
{
    int n, budget;
    cout << "Enter number of places: ";
    cin >> n;
    cout << "Enter maximum budget: ";
    cin >> budget;

    vector<Place> allPlaces;
    for (int i = 0; i < n; i++)
    {
        string name;
        int ps, pe, os, oe, cost, joy;

        cout << "\nEnter details for place " << i + 1 << ":\n";
        cout << "Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Planned start time: ";
        cin >> ps;
        cout << "Planned end time: ";
        cin >> pe;
        cout << "Opening time: ";
        cin >> os;
        cout << "Closing time: ";
        cin >> oe;
        cout << "Cost: ";
        cin >> cost;
        cout << "Enjoyment: ";
        cin >> joy;

        Place p(name, ps, pe, os, oe, cost, joy);
        if (isValidPlan(p))
        {
            allPlaces.push_back(p);
        }
        else
        {
            cout << "Invalid time. Skipping " << name << endl;
        }
    }

    if (allPlaces.empty())
    {
        cout << "No valid places to plan.\n";
        return 0;
    }

    vector<Place> selected = selectPlaces(allPlaces, budget);
    printPlan(selected);

    return 0;
}
