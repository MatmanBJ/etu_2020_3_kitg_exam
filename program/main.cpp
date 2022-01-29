//
//  main.cpp
//  Alternative exam
//  version alpha 0.05
//  Created by Matthew Sobolewski on 27.12.2020.
//

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <list>

using namespace std;

int main()
{
    int i;
    int j;
    int k;
    int w[10]; // weight of items
    int v[10]; // capacity of items
    
    int current_value;
    int current_row;
    int current_step;
    int isin[10][15];
    int isin_row[10][15];
    int isin_step[10][15];
    int W = 14; // weight of knapsack
    int n = 4; // number of items
    int Weight[10][15];
    list <int> N;
    
    // ---------- KNAPSACK PROBLEM ALGORITHM (A) ----------
    
    /*w[1] = 5;
    w[2] = 10;
    w[3] = 6;
    w[4] = 5;
    
    v[1] = 3;
    v[2] = 5;
    v[3] = 4;
    v[4] = 2;
    
    for (i = 0; i <= W; i++)
    {
        isin[0][i] = 0;
        Weight[0][i] = 0;
    }
    for (i = 1; i <= n; i++)
    {
        for (j = 0; j <= W; j++)
        {
            if (w[i] > j)
            {
                Weight[i][j] = Weight[i - 1][j];
                isin_row[i][j] = isin_row[i - 1][j];
                isin_step[i][j] = isin_step[i - 1][j];
                isin[i][j] = isin[i - 1][j];
            }
            else
            {
                Weight[i][j] = fmax(Weight[i - 1][j], Weight[i - 1][j - w[i]] + v[i]);
                if (Weight[i - 1][j] >= Weight[i - 1][j - w[i]] + v[i])
                {
                    isin_row[i][j] = isin_row[i - 1][j];
                    isin_step[i][j] = isin_step[i - 1][j];
                    isin[i][j] = isin[i - 1][j];
                }
                else
                {
                    isin_row[i][j] = i - 1;
                    isin_step[i][j] = j - w[i];
                    isin[i][j] = i;
                }
            }
        }
    }
    for (i = 0; i <= n; i++)
    {
        for (j = 0; j <= W; j++)
        {
            cout << Weight[i][j] << " ";
        }
        cout << endl;
    }
    
    current_value = isin[n][W];
    current_row = isin_row[n][W];
    current_step = isin_step[n][W];
    while (current_value != 0)
    {
        N.push_back(current_value);
        current_value = isin[current_row][current_step];
        int a = isin_row[current_row][current_step];
        current_step = isin_step[current_row][current_step];
        current_row = a;
    }
    
    cout << "Items number: ";
    while (!N.empty())
    {
        cout << N.front() << " ";
        N.pop_front();
    }
    cout << endl;*/
    
    // ---------- KNAPSACK PROBLEM ALGORITHM (B) ----------
    
    int item_sizes[10][10];
    int bin_sizes[10];
    int profit_function[10][10];
    int profit_function_modified_one[10][10];
    int profit_function_modified_two[10][10];
    int S[10];
    int x;
    int y;
    int z;
    
    item_sizes[1][1] = 1;
    item_sizes[1][2] = 1;
    item_sizes[1][3] = 1;
    
    item_sizes[2][1] = 2;
    item_sizes[2][2] = 3;
    item_sizes[2][3] = 3;
    
    item_sizes[3][1] = 2;
    item_sizes[3][2] = 3;
    item_sizes[3][3] = 4;
    
    item_sizes[4][1] = 1;
    item_sizes[4][2] = 2;
    item_sizes[4][3] = 3;
    
    profit_function[1][1] = 3;
    profit_function[1][2] = 1;
    profit_function[1][3] = 5;
    
    profit_function[2][1] = 1;
    profit_function[2][2] = 1;
    profit_function[2][3] = 1;
    
    profit_function[3][1] = 5;
    profit_function[3][2] = 15;
    profit_function[3][3] = 25;
    
    profit_function[4][1] = 25;
    profit_function[4][2] = 15;
    profit_function[4][3] = 5;
    
    bin_sizes[0] = 0;
    bin_sizes[1] = 2;
    bin_sizes[2] = 3;
    bin_sizes[3] = 4;
    
    for (i = 0; i <= 4; i++)
    {
        item_sizes[i][0] = 0;
        profit_function[i][0] = 0;
    }
    for (i = 0; i <= 3; i++)
    {
        item_sizes[0][i] = 0;
        profit_function[0][i] = 0;
    }
    
    for (i = 0; i <= 4; i++)
    {
        for (j = 0; j <= 3; j++)
        {
            profit_function_modified_one[i][j] = profit_function[i][j];
            profit_function_modified_two[i][j] = profit_function[i][j];
        }
    }
    
    cout << "Item sizes:" << endl;
    for (i = 1; i <= 4; i++)
    {
        for (j = 1; j <= 3; j++)
        {
            cout << item_sizes[i][j] << " ";
        }
        cout << endl;
    }
    
    cout << "Profit function:" << endl;
    for (i = 1; i <= 4; i++)
    {
        for (j = 1; j <= 3; j++)
        {
            cout << profit_function[i][j] << " ";
        }
        cout << endl;
    }
    
    for (x = 1; x <= 3; x++) // dolzhno byt' tri!!!!!!!!!!!!!!!!
    {
        for (i = 0; i <= bin_sizes[x]; i++)
        {
            isin[0][i] = 0;
            Weight[0][i] = 0;
        }
        for (i = 1; i <= 4; i++)
        {
            for (j = 0; j <= bin_sizes[x]; j++)
            {
                if (item_sizes[i][x] > j)
                {
                    Weight[i][j] = Weight[i - 1][j];
                    isin_row[i][j] = isin_row[i - 1][j];
                    isin_step[i][j] = isin_step[i - 1][j];
                    isin[i][j] = isin[i - 1][j];
                }
                else
                {
                    Weight[i][j] = fmax(Weight[i - 1][j], Weight[i - 1][j - item_sizes[i][x]] + profit_function_modified_two[i][x]);
                    if (Weight[i - 1][j] >= Weight[i - 1][j - item_sizes[i][x]] + profit_function_modified_two[i][x])
                    {
                        isin_row[i][j] = isin_row[i - 1][j];
                        isin_step[i][j] = isin_step[i - 1][j];
                        isin[i][j] = isin[i - 1][j];
                    }
                    else
                    {
                        isin_row[i][j] = i - 1;
                        isin_step[i][j] = j - item_sizes[i][x];
                        isin[i][j] = i;
                    }
                }
            }
        }
        for (i = 0; i <= 4; i++)
        {
            for (j = 0; j <= bin_sizes[x]; j++)
            {
                cout << Weight[i][j] << " ";
            }
            cout << endl;
        }
        
        for (i = 0; i <= 4; i++)
        {
            S[i] = 0;
        }
        
        current_value = isin[4][bin_sizes[x]];
        current_row = isin_row[4][bin_sizes[x]];
        current_step = isin_step[4][bin_sizes[x]];
        while (current_value != 0)
        {
            //N.push_back(current_value);
            S[current_value] = 1;
            current_value = isin[current_row][current_step];
            int a = isin_row[current_row][current_step];
            current_step = isin_step[current_row][current_step];
            current_row = a;
        }
        
        for (i = x; i <= 3; i++)
        {
            if (i != x)
            {
                for (j = 1; j <= 4; j++)
                {
                    profit_function_modified_one[j][i] = 0;
                }
            }
        }
        for (i = 1; i <= 4; i++)
        {
            if (S[i] != 1)
            {
                for (j = x; j <= 3; j++)
                {
                    profit_function_modified_one[i][j] = 0;
                }
            }
        }
        
        for (i = x; i <= 3; i++)
        {
            if (i == x)
            {
                for (j = 1; j <= 4; j++)
                {
                    profit_function_modified_one[j][i] = profit_function_modified_two[j][i];
                }
            }
        }
        for (i = 1; i <= 4; i++)
        {
            if (S[i] == 1)
            {
                for (j = x; j <= 3; j++)
                {
                    profit_function_modified_one[i][j] = profit_function_modified_two[i][x];
                }
            }
        }
        
        cout << "Profit function p^1: " << endl;
        for (i = 1; i <= 4; i++)
        {
            for (j = x; j <= 3; j++)
            {
                cout << profit_function_modified_one[i][j] << " ";
            }
            cout << endl;
        }
        
        for (i = 1; i <= 4; i++)
        {
            for (j = x; j <= 3; j++)
            {
                profit_function_modified_two[i][j] = profit_function_modified_two[i][j] - profit_function_modified_one[i][j];
            }
        }
        
        cout << "Profit function p^2: " << endl;
        for (i = 1; i <= 4; i++)
        {
            for (j = x; j <= 3; j++)
            {
                cout << profit_function_modified_two[i][j] << " ";
            }
            cout << endl;
        }
        
        cout << "Items number: ";
        /*while (!N.empty())
        {
            cout << N.front() << " ";
            N.pop_front();
        }*/
        for (i = 0; i <= 4; i++)
        {
            if (S[i] == 1)
            {
                cout << i << " ";
            }
        }
        cout << endl;
    }
    
    return 0;
}
