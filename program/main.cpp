//
//  main.cpp
//  Alternative exam
//  version beta 0.1
//  Created by Matthew Sobolewski on 27.12.2020.
//

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <list>

using namespace std;

int main()
{
    // INITIALIZING ARRAYS FOR EFFICIENT APPROXIMATION
    
    int i;
    int j;
    int x; // current column iteration (sorry, i forgot about 'k' variable)
    
    int current_value;
    int current_row;
    int current_step;
    
    const int rows = 4; // general number of rows in array (number of items)
    const int columns = 3; // general number of columns in array (number of knapsacks)
    const int max_bin_sizes = 30;
    
    int isin[rows + 1][max_bin_sizes + 1];
    int isin_row[rows + 1][max_bin_sizes + 1];
    int isin_step[rows + 1][max_bin_sizes + 1];
    int Weight[rows + 1][max_bin_sizes + 1];
    
    int *bin_sizes = new int[columns + 1]; // general capacity of bins (workers)
    int *S = new int[rows + 1]; // items (objects) chosed at current iteration
    int *T = new int[rows + 1]; // items (objects) passed in general
    
    int **item_sizes = new int*[rows + 1]; // weight of each item
    int **profit_function = new int*[rows + 1]; // profit of each item
    int **profit_function_modified_one = new int*[rows + 1]; // work function
    int **profit_function_modified_two = new int*[rows + 1]; // work function
    
    int P[10]; // current profit function for algorithm 2
    
    for (i = 0; i <= rows; i++) // creating dynamic double arrays
    {
        T[i] = -1;
        
        item_sizes[i] = new int[columns + 1];
        profit_function[i] = new int[columns + 1];
        profit_function_modified_one[i] = new int[columns + 1];
        profit_function_modified_two[i] = new int[columns + 1];
    }
    
    // VALUES FROM A HAND INPUT FOR OUR TASK
    
    int *translation_size = new int[rows + 1];
    int *translation_complexity = new int[rows + 1];
    int *translation_importance = new int[rows + 1];
    int *translation_days = new int[rows + 1];
    float *translator_speed = new float[columns + 1];
    
    translation_size[0] = 0;
    translation_size[1] = 5;
    translation_size[2] = 15;
    translation_size[3] = 10;
    translation_size[4] = 40;
    
    translation_complexity[0] = 0;
    translation_complexity[1] = 5;
    translation_complexity[2] = 6;
    translation_complexity[3] = 9;
    translation_complexity[4] = 5;
    
    translation_importance[0] = 0;
    translation_importance[1] = 3;
    translation_importance[2] = 1;
    translation_importance[3] = 1;
    translation_importance[4] = 2;
    
    translation_days[0] = 0;
    translation_days[1] = 1;
    translation_days[2] = 2;
    translation_days[3] = 5;
    translation_days[4] = 25;
    
    translator_speed[0] = 0;
    translator_speed[1] = 1;
    translator_speed[2] = 0.5;
    translator_speed[3] = 1.5;
    
    bin_sizes[0] = 0;
    bin_sizes[1] = 30;
    bin_sizes[2] = 30;
    bin_sizes[3] = 30;
    
    for (i = 1; i <= rows; i++)
    {
        for (j = 1; j <= columns; j++)
        {
            item_sizes[i][j] = translation_size[i]/translator_speed[j];
            profit_function[i][j] = item_sizes[i][j];
        }
    }
    
    // VALUES FROM AN EXAMPLE HAND INPUT
    
    /*item_sizes[1][1] = 1;
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
    bin_sizes[3] = 4;*/
    
    // SETTIN' TO ZERO ARRAY VALUES (IMPORTANT STAGE FROM ALGORITHM)
    
    for (i = 0; i <= rows; i++)
    {
        item_sizes[i][0] = 0;
        profit_function[i][0] = 0;
    }
    for (i = 0; i <= columns; i++)
    {
        item_sizes[0][i] = 0;
        profit_function[0][i] = 0;
    }
    
    // SETTIN' TO INITIAL ARRAY VALUES WORK ARRAYS
    
    for (i = 0; i <= rows; i++)
    {
        for (j = 0; j <= columns; j++)
        {
            profit_function_modified_one[i][j] = profit_function[i][j];
            profit_function_modified_two[i][j] = profit_function[i][j];
        }
    }
    
    // CURRENT ITEM SIZES OUTPUT
    
    cout << "Item sizes:" << endl;
    for (i = 1; i <= rows; i++)
    {
        for (j = 1; j <= columns; j++)
        {
            cout << item_sizes[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    
    // CURRENT PROFIT FUNCTION OUTPUT
    
    cout << "Profit function:" << endl;
    for (i = 1; i <= rows; i++)
    {
        for (j = 1; j <= columns; j++)
        {
            cout << profit_function[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    
    // ----------------------------------------------------
    // ---------- KNAPSACK PROBLEM ALGORITHM (1) ----------
    // ----------------------------------------------------
    
    for (x = 1; x <= columns; x++)
    {
        for (i = 0; i <= bin_sizes[x]; i++)
        {
            isin[0][i] = 0;
            Weight[0][i] = 0;
        }
        for (i = 1; i <= rows; i++)
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
        
        // FINDING CHOOSED ITEMS (WITH MY "CHAIN METHOD")
        
        for (i = 0; i <= rows; i++)
        {
            S[i] = 0;
        }
        
        current_value = isin[rows][bin_sizes[x]]; // last added item number in array
        current_row = isin_row[rows][bin_sizes[x]]; // last added BEFORE last added in array ROW
        current_step = isin_step[rows][bin_sizes[x]]; // last added BEFORE last added in array COLUMN
        
        while (current_value != 0) // restore item numbers with my "chain method"
        {
            S[current_value] = 1;
            current_value = isin[current_row][current_step];
            int a = isin_row[current_row][current_step];
            current_step = isin_step[current_row][current_step];
            current_row = a;
        }
        
        // WORK PROFIT FUNCTIONS UPDATING AND OUTPUT
        
        for (i = x; i <= columns; i++)
        {
            if (i != x)
            {
                for (j = 1; j <= rows; j++)
                {
                    profit_function_modified_one[j][i] = 0; // j and i, not i and j
                }
            }
        }
        for (i = 1; i <= rows; i++)
        {
            if (S[i] != 1)
            {
                for (j = x; j <= columns; j++)
                {
                    profit_function_modified_one[i][j] = 0;
                }
            }
        }
        
        for (i = x; i <= columns; i++)
        {
            if (i == x)
            {
                for (j = 1; j <= rows; j++)
                {
                    profit_function_modified_one[j][i] = profit_function_modified_two[j][i];
                }
            }
        }
        for (i = 1; i <= rows; i++)
        {
            if (S[i] == 1)
            {
                for (j = x; j <= columns; j++)
                {
                    profit_function_modified_one[i][j] = profit_function_modified_two[i][x];
                }
            }
        }
        
        cout << "Profit function p^1: " << endl;
        for (i = 1; i <= rows; i++)
        {
            for (j = x; j <= columns; j++)
            {
                cout << profit_function_modified_one[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
        
        for (i = 1; i <= rows; i++)
        {
            for (j = x; j <= columns; j++)
            {
                profit_function_modified_two[i][j] = profit_function_modified_two[i][j] - profit_function_modified_one[i][j];
            }
        }
        
        cout << "Profit function p^2: " << endl;
        for (i = 1; i <= rows; i++)
        {
            for (j = x; j <= columns; j++)
            {
                cout << profit_function_modified_two[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
        
        // TEMPORARY WORK ARRAY (A - KNAPSACK PROBLEM - ALGORITHM) OUTPUT
        
        cout << "Temporary work array:" << endl;
        for (i = 0; i <= rows; i++)
        {
            for (j = 0; j <= bin_sizes[x]; j++)
            {
                cout << Weight[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
        
        // NUMBER OF CHOSEN ITEMS AT CURRENT ITERATION OUTPUT
        
        cout << "Items number: ";
        for (i = 0; i <= rows; i++)
        {
            if (S[i] == 1)
            {
                cout << i << " ";
            }
        }
        cout << endl << endl;
    }
    
    // ----------------------------------------------------
    // ---------- KNAPSACK PROBLEM ALGORITHM (2) ----------
    // ----------------------------------------------------
    
    /*for (x = 1; x <= columns; x++)
    {
        for (i = 1; i <= rows; i++)
        {
            if (T[i] == -1)
            {
                P[i] = profit_function[i][x];
            }
            else
            {
                P[i] = profit_function[i][x] - profit_function[i][T[i]];
            }
        }
        for (i = 0; i <= bin_sizes[x]; i++)
        {
            isin[0][i] = 0;
            Weight[0][i] = 0;
        }
        for (i = 1; i <= rows; i++)
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
                    Weight[i][j] = fmax(Weight[i - 1][j], Weight[i - 1][j - item_sizes[i][x]] + P[i]);
                    if (Weight[i - 1][j] >= Weight[i - 1][j - item_sizes[i][x]] + P[i])
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
        
        // FINDING CHOOSED ITEMS (WITH MY "CHAIN METHOD")
        
        for (i = 0; i <= rows; i++)
        {
            S[i] = 0;
        }
        
        current_value = isin[rows][bin_sizes[x]]; // last added item number in array
        current_row = isin_row[rows][bin_sizes[x]]; // last added BEFORE last added in array ROW
        current_step = isin_step[rows][bin_sizes[x]]; // last added BEFORE last added in array COLUMN
        
        while (current_value != 0) // restore item numbers with my "chain method"
        {
            S[current_value] = 1;
            current_value = isin[current_row][current_step];
            int a = isin_row[current_row][current_step];
            current_step = isin_step[current_row][current_step];
            current_row = a;
        }
        
        // UPDATIN' T ARRAY WITH NEW "PASSED" (CHOOSED IN SOME STEP) ITEMS
        
        for (i = 1; i <= rows; i++)
        {
            if (S[i] == 1)
            {
                T[i] = x;
            }
        }
        
        // CURRENT PROFIT FUNCTION P_j (P_x HERE)
        
        cout << "Profit function P_j (P_x): " << endl;
        for (i = 1; i <= rows; i++)
        {
            cout << P[i];
            cout << endl;
        }
        cout << endl;
        
        // TEMPORARY WORK ARRAY (A - KNAPSACK PROBLEM - ALGORITHM) OUTPUT
        
        cout << "Temporary work array:" << endl;
        for (i = 0; i <= rows; i++)
        {
            for (j = 0; j <= bin_sizes[x]; j++)
            {
                cout << Weight[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
        
        // NUMBER OF CHOSEN ITEMS AT CURRENT ITERATION OUTPUT
        
        cout << "Items number: ";
        for (i = 0; i <= rows; i++)
        {
            if (S[i] == 1)
            {
                cout << i << " ";
            }
        }
        cout << endl << endl;
    }*/
    
    // CLEANING OUR MEMORY FROM DYNAMIC ARRAYS
    
    for (i = 0; i <= rows; i++)
    {
        delete [] item_sizes[i];
        delete [] profit_function[i];
        delete [] profit_function_modified_one[i];
        delete [] profit_function_modified_two[i];
    }
    
    delete [] bin_sizes;
    delete [] S;
    delete [] T;
    
    delete [] item_sizes;
    delete [] profit_function;
    delete [] profit_function_modified_one;
    delete [] profit_function_modified_two;
    
    return 0;
}
