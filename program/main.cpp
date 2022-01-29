//
//  main.cpp
//  Alternative exam
//  version release 1.1
//  Created by Matthew Sobolev
//  Team: Sobolev Matvey, Stepovik Viktor, Kashapova Olga

#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <vector>
#include <list>

#include <math.h>
#include <cstring>

using namespace std;

// VALUABLE CONSTANTS (MUST BE CHANGED FOR YOUR CONDITIONS)

const int rows = 32; // general number of rows in array (number of items)
const int columns = 10; // general number of columns in array (number of knapsacks)
const int max_bin_sizes = 30; // maximal sizes for bins (days for workers)
const int jobs_number = 32; // general number of jobs (translations)
const int workers_number = 10; // general number of workers (translators)
const int languages_number = 7; // general number of languages
const int work_hours = 0; // number for the work hours coefficient switch case
const int work_task = 9; // number for the task switch case

// FUNCTION PROTOTYPES

char** simple_split(char *str, int length, char separator);
int** input_data_file (int local_size, char* local_name);

// INSTRUCTION

// for constants (you see them above):
// rows and columns are variablef for the algorithm
// max_bin_sizes are for our task
// jobs_number, workers_number, languages_number are for our task too and used before algorithm!!!

// for jobs ("jobs.txt" file):
// english - 1; german - 2; french - 3; spanish - 4; italian - 5; portugal - 6; polish - 7
// second row - size
// third row - complexity
// fourth row - importance
// fifth row - days

// for workers:
// Complexity matrix ("workers_c.txt" file):
// each row - worker, each column - language, "0" means "doesn't exist"
// Speed matrix ("workers_s.txt" file):
// each row - worker, each column - language, "0" means "doesn't exist"

int main()
{
    // --------------------------------------------------------------
    // ---------- DATA INPUT, INITIALIZING AND PREPARATION ----------
    // --------------------------------------------------------------
    
    // INITIALIZING ARRAYS FOR EFFICIENT APPROXIMATION
    
    int i;
    int j;
    int x; // current column iteration (sorry, i forgot about 'k' variable)
    int answer = 0; // answer variable
    int deadline = 0; // biggest deadline
    
    int current_value;
    int current_row;
    int current_step;
    
    int isin[rows + 1][max_bin_sizes + 1];
    int isin_row[rows + 1][max_bin_sizes + 1];
    int isin_step[rows + 1][max_bin_sizes + 1];
    int Weight[rows + 1][max_bin_sizes + 1];
    
    int *bin_sizes = new int[columns + 1]; // general capacity of bins (workers)
    int **S = new int*[rows + 1]; // items (objects) chosed at all/in general
    int *T = new int[rows + 1]; // items (objects) passed in general
    
    int **item_sizes = new int*[rows + 1]; // weight of each item
    int **profit_function = new int*[rows + 1]; // profit of each item
    int **profit_function_modified_one = new int*[rows + 1]; // work function
    int **profit_function_modified_two = new int*[rows + 1]; // work function
    
    int P[rows + 1]; // current profit function for algorithm 2
    
    for (i = 0; i <= rows; i++) // creating dynamic double arrays
    {
        S[i] = new int[columns + 1];
        T[i] = -1;
        
        item_sizes[i] = new int[columns + 1];
        profit_function[i] = new int[columns + 1];
        profit_function_modified_one[i] = new int[columns + 1];
        profit_function_modified_two[i] = new int[columns + 1];
    }
    
    // BIN SIZES INITIALIZATION (DAYS FOR EACH WORKER)
    
    for (i = 0; i <= workers_number; i++) // it doesn't matter here, from "0" or "1" to start
    {
        bin_sizes[i] = max_bin_sizes;
    }
    
    // VALUES FROM A AUTOMATIC FILE INPUT FOR OUR TASK
    
    char jobs_file [] = {'j', 'o', 'b', 's', '.', 't', 'x', 't', '\0'};
    char workers_c_file [] = {'w', 'o', 'r', 'k', 'e', 'r', 's', '_', 'c', '.', 't', 'x', 't', '\0'};
    char workers_s_file [] = {'w', 'o', 'r', 'k', 'e', 'r', 's', '_', 's', '.', 't', 'x', 't', '\0'};
    
    int **jobs = input_data_file(jobs_number, jobs_file); // rows - values, columns - jobs
    int **workers_c = input_data_file(languages_number, workers_c_file); // rows - workers, columns - languages
    int **workers_s = input_data_file(languages_number, workers_s_file); // rows - workers, columns - languages
    float **workers_s_float = new float*[workers_number + 1]; // rows - workers, columns - languages
    
    // FINDING MAX OF THE JOBS DEADLINES (FOR CASE 9)
    
    for (i = 1; i <= jobs_number; i++) // choosing biggest deadline
    {
        if (jobs[5][i] > deadline)
        {
            deadline = jobs[5][i];
        }
    }
    
    // TRANSLATION INTO THE FLOAT FORMAT FOR THE WORKERS' SPEED WITH SWITCH CASE
    
    switch (work_hours)
    {
        case 1: // 12-hours work day
            cout << "---------- 12-hours work day ----------" << endl;
            for (i = 1; i <= workers_number; i++)
            {
                workers_s_float[i] = new float[languages_number + 1];
                for (j = 1; j <= languages_number; j++)
                {
                    workers_s_float[i][j] = 12*(float)workers_s[i][j]/100;
                }
            }
            break;
        case 2: // 24-hours work day (Nanomachines, son©)
            cout << "---------- 24-hours work day (Nanomachines, son©) ----------" << endl;
            for (i = 1; i <= workers_number; i++)
            {
                workers_s_float[i] = new float[languages_number + 1];
                for (j = 1; j <= languages_number; j++)
                {
                    workers_s_float[i][j] = 24*(float)workers_s[i][j]/100;
                }
            }
            break;
        default: // 8-hours work day
            cout << "---------- 8-hours work day ----------" << endl;
            for (i = 1; i <= workers_number; i++)
            {
                workers_s_float[i] = new float[languages_number + 1];
                for (j = 1; j <= languages_number; j++)
                {
                    workers_s_float[i][j] = 8*(float)workers_s[i][j]/100;
                }
            }
            break;
    }
    
    // FORMING ITEM AND PROFIT FUNCTION (ROWS - JOBS, COLUMNS - WORKERS) WITH SWITCH CASE
    
    switch (work_task) // PLEASE, CHOOSE THE NUMBER
    {
        case 1: // minimal time to do all the tasks
            cout << "---------- Minimal time to do all the tasks ----------" << endl;
            for (i = 1; i <= jobs_number; i++)
            {
                for (j = 1; j <= workers_number; j++)
                {
                    if (workers_s[j][jobs[1][i]] != 0)
                    {
                        item_sizes[i][j] = (int)((float)jobs[2][i]/workers_s_float[j][jobs[1][i]]);
                    }
                    else
                    {
                        item_sizes[i][j] = bin_sizes[j] + 1;
                    }
                    profit_function[i][j] = (-1)*(item_sizes[i][j] - bin_sizes[j] - 1);
                }
            }
            break;
        case 2: // minimal time to do all the immediate tasks
            cout << "---------- Minimal time to do all the immediate tasks ----------" << endl;
            for (i = 1; i <= jobs_number; i++)
            {
                for (j = 1; j <= workers_number; j++)
                {
                    if (workers_s[j][jobs[1][i]] != 0 && (float)jobs[2][i]/workers_s_float[j][jobs[1][i]] <= jobs[5][i])
                    {
                        item_sizes[i][j] = (int)((float)jobs[2][i]/workers_s_float[j][jobs[1][i]]);
                    }
                    else
                    {
                        item_sizes[i][j] = bin_sizes[j] + 1;
                    }
                    profit_function[i][j] = (-1)*(item_sizes[i][j] - bin_sizes[j] - 1);
                }
            }
            break;
        case 3: // minimal time to do all the important tasks
            cout << "---------- Minimal time to do all the important tasks ----------" << endl;
            for (i = 1; i <= jobs_number; i++)
            {
                for (j = 1; j <= workers_number; j++)
                {
                    if (workers_s[j][jobs[1][i]] != 0 && (float)jobs[2][i]/workers_s_float[j][jobs[1][i]] <= jobs[5][i])
                    {
                        item_sizes[i][j] = (int)((float)jobs[2][i]/workers_s_float[j][jobs[1][i]]);
                    }
                    else
                    {
                        item_sizes[i][j] = bin_sizes[j] + 1;
                    }
                    profit_function[i][j] = jobs[4][i];
                }
            }
            break;
        case 4: // maximal quality to do all the tasks
            cout << "---------- Maximal quality to do all the tasks ----------" << endl;
            for (i = 1; i <= jobs_number; i++)
            {
                for (j = 1; j <= workers_number; j++)
                {
                    if (workers_s[j][jobs[1][i]] != 0)
                    {
                        item_sizes[i][j] = (int)((float)jobs[2][i]/workers_s_float[j][jobs[1][i]]);
                    }
                    else
                    {
                        item_sizes[i][j] = bin_sizes[j] + 1;
                    }
                    profit_function[i][j] = workers_c[j][jobs[1][i]] - jobs[3][i];
                }
            }
            break;
        case 5: // maximal fast (minimal time) to do extra important task (extra important tasks) (3 importance)
            cout << "---------- Maximal fast (minimal time) to do extra important task (extra important tasks) (3 importance) ----------" << endl;
            for (i = 1; i <= jobs_number; i++)
            {
                for (j = 1; j <= workers_number; j++)
                {
                    if (workers_s[j][jobs[1][i]] != 0 && jobs[4][i] == 3)
                    {
                        item_sizes[i][j] = (int)((float)jobs[2][i]/workers_s_float[j][jobs[1][i]]);
                    }
                    else
                    {
                        item_sizes[i][j] = bin_sizes[j] + 1;
                    }
                    profit_function[i][j] = (-1)*(item_sizes[i][j] - bin_sizes[j] - 1);
                }
            }
            break;
        case 6: // maximal quality to do extra important task (extra important tasks) (3 importance)
            cout << "---------- Maximal quality to do extra important task (extra important tasks) (3 importance ----------" << endl;
            for (i = 1; i <= jobs_number; i++)
            {
                for (j = 1; j <= workers_number; j++)
                {
                    if (workers_s[j][jobs[1][i]] != 0 && jobs[4][i] == 3)
                    {
                        item_sizes[i][j] = (int)((float)jobs[2][i]/workers_s_float[j][jobs[1][i]]);
                    }
                    else
                    {
                        item_sizes[i][j] = bin_sizes[j] + 1;
                    }
                    profit_function[i][j] = workers_c[j][jobs[1][i]] - jobs[3][i];
                }
            }
            break;
        case 7: // maximal equal burden on people
            cout << "Not realised" << endl;
            break;
        case 8: // distribution depending on importance of tasks
            cout << "---------- Distribution depending on importance of tasks ----------" << endl;
            for (i = 1; i <= jobs_number; i++)
            {
                for (j = 1; j <= workers_number; j++)
                {
                    if (workers_s[j][jobs[1][i]] != 0)
                    {
                        item_sizes[i][j] = (int)((float)jobs[2][i]/workers_s_float[j][jobs[1][i]]);
                    }
                    else
                    {
                        item_sizes[i][j] = bin_sizes[j] + 1;
                    }
                    profit_function[i][j] = jobs[4][i];
                }
            }
            break;
        case 9: // distribution depending on deadline of taks
            cout << "---------- Distribution depending on deadline of taks ----------" << endl;
            for (i = 1; i <= jobs_number; i++)
            {
                for (j = 1; j <= workers_number; j++)
                {
                    if (workers_s[j][jobs[1][i]] != 0)
                    {
                        item_sizes[i][j] = (int)((float)jobs[2][i]/workers_s_float[j][jobs[1][i]]);
                    }
                    else
                    {
                        item_sizes[i][j] = bin_sizes[j] + 1;
                    }
                    profit_function[i][j] = (-1)*(item_sizes[i][j] - deadline - 1);
                }
            }
            break;
        case 10: // minimal time doing all the tasks without limitation of the work time (but less than 36 hours including time of breaks)
            cout << "Not realised" << endl;
            break;
        case 11: // the minimum time for completing all tasks with the involvement of the deputy head of the department is not more than 4 hours on a standard working day (if the importance of task 3, you can use the maximum)
            cout << "Not realised" << endl;
            break;
        default: // standart work of the algorithm
            cout << "Doesn't exist" << endl;
            break;
    }
    
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
    
    // -----------------------------------------
    // ---------- OUTPUT CURRENT DATA ----------
    // -----------------------------------------
    
    // JOBS VALUES OUTPUT
    
    cout << "Jobs values:" << endl;
    for (i = 1; i <= 5; i++)
    {
        switch (i)
        {
            case 1:
                cout << "Language:   ";
                break;
            case 2:
                cout << "Lists:      ";
                break;
            case 3:
                cout << "Compexity:  ";
                break;
            case 4:
                cout << "Importance: ";
                break;
            case 5:
                cout << "Days:       ";
                break;
        }
        for (j = 1; j <= jobs_number; j++)
        {
            cout << jobs[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    
    // WORKERS COMPLEXITY VALUES OUTPUT
    
    cout << "Workers complexity:" << endl;
    for (i = 1; i <= workers_number; i++)
    {
        for (j = 1; j <= languages_number; j++)
        {
            cout << workers_c[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    
    // WORKERS SPEED VALUES OUTPUT
    
    cout << "Workers speed:" << endl;
    for (i = 1; i <= workers_number; i++)
    {
        for (j = 1; j <= languages_number; j++)
        {
            cout << workers_s_float[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    
    // CURRENT ITEM SIZES OUTPUT
    
    cout << "Item sizes:" << endl;
    for (i = 1; i <= rows; i++)
    {
        cout << i << ":  ";
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
            S[i][x] = 0;
        }
        
        current_value = isin[rows][bin_sizes[x]]; // last added item number in array
        current_row = isin_row[rows][bin_sizes[x]]; // last added BEFORE last added in array ROW
        current_step = isin_step[rows][bin_sizes[x]]; // last added BEFORE last added in array COLUMN
        
        while (current_value != 0) // restore item numbers with my "chain method"
        {
            S[current_value][x] = 1;
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
            if (S[i][x] != 1)
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
            if (S[i][x] == 1)
            {
                for (j = x; j <= columns; j++)
                {
                    profit_function_modified_one[i][j] = profit_function_modified_two[i][x];
                }
            }
        }
        
        /*cout << "Profit function p^1: " << endl;
        for (i = 1; i <= rows; i++)
        {
            for (j = x; j <= columns; j++)
            {
                cout << profit_function_modified_one[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;*/
        
        for (i = 1; i <= rows; i++)
        {
            for (j = x; j <= columns; j++)
            {
                profit_function_modified_two[i][j] = profit_function_modified_two[i][j] - profit_function_modified_one[i][j];
            }
        }
        
        /*cout << "Profit function p^2: " << endl;
        for (i = 1; i <= rows; i++)
        {
            for (j = x; j <= columns; j++)
            {
                cout << profit_function_modified_two[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;*/
        
        // TEMPORARY WORK ARRAY (A - KNAPSACK PROBLEM - ALGORITHM) OUTPUT
        
        /*cout << "Temporary work array:" << endl;
        for (i = 0; i <= rows; i++)
        {
            for (j = 0; j <= bin_sizes[x]; j++)
            {
                cout << Weight[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;*/
        
        // NUMBER OF CHOSEN ITEMS AT CURRENT ITERATION OUTPUT
        
        cout << "Step " << x << " items number: ";
        for (i = 0; i <= rows; i++)
        {
            if (S[i][x] == 1)
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
            S[i][x] = 0;
        }
        
        current_value = isin[rows][bin_sizes[x]]; // last added item number in array
        current_row = isin_row[rows][bin_sizes[x]]; // last added BEFORE last added in array ROW
        current_step = isin_step[rows][bin_sizes[x]]; // last added BEFORE last added in array COLUMN
        
        while (current_value != 0) // restore item numbers with my "chain method"
        {
            S[current_value][x] = 1;
            current_value = isin[current_row][current_step];
            int a = isin_row[current_row][current_step];
            current_step = isin_step[current_row][current_step];
            current_row = a;
        }
        
        // UPDATIN' T ARRAY WITH NEW "PASSED" (CHOOSED IN SOME STEP) ITEMS
        
        for (i = 1; i <= rows; i++)
        {
            if (S[i][x] == 1)
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
        
        cout << "Step " << x << " items number: ";
        for (i = 0; i <= rows; i++)
        {
            if (S[i][x] == 1)
            {
                cout << i << " ";
            }
        }
        cout << endl << endl;
    }*/
    
    // ----------------------------------------------------
    // ---------- RESULT OUTPUT AND CLEAN MEMORY ----------
    // ----------------------------------------------------
    
    // SETTING TO NULL INVALID ITEMS
    
    for (i = rows; i >= 1; i--)
    {
        for (j = columns; j >= 1; j--)
        {
            if (S[i][j] == 1)
            {
                for (x = j - 1; x >= 1; x--)
                {
                    S[i][x] = 0;
                }
            }
        }
    }
    
    // OUTPUT AN ARRAY OF CHOSED ITEMS
    
    cout << "Chosed items:" << endl;
    for (i = 1; i <= columns; i++)
    {
        int a = 0;
        int b = 0;
        cout << "Translator " << i << " jobs: ";
        for (j = 1; j <= rows; j++)
        {
            if (S[j][i] == 1)
            {
                a = a + item_sizes[j][i];
                b = b + workers_c[i][jobs[1][j]];
                cout << j << " ";
            }
        }
        if (work_task == 6 || work_task == 4)
        {
            answer = answer + b;
        }
        if (work_task != 6 && work_task != 4 && a > answer)
        {
            answer = a;
        }
        cout << endl;
    }
    cout << endl;
    
    // OUTPUT AN ANSWER
    
    cout << "Answer: " << answer << endl;
    
    // CLEANING OUR MEMORY FROM DYNAMIC ARRAYS
    
    for (i = 0; i <= rows; i++)
    {
        delete [] S[i];
        
        delete [] item_sizes[i];
        delete [] profit_function[i];
        delete [] profit_function_modified_one[i];
        delete [] profit_function_modified_two[i];
    }
    
    for (i = 0; i <= 5; i++)
    {
        delete [] jobs[i];
    }
    
    for (i = 0; i <= workers_number; i++) // it may start from "1" (may be a mistake with deleting)
    {
        delete [] workers_c[i];
        delete [] workers_s[i];
        delete [] workers_s_float[i];
    }
    
    delete [] bin_sizes;
    delete [] S;
    delete [] T;
    
    delete [] item_sizes;
    delete [] profit_function;
    delete [] profit_function_modified_one;
    delete [] profit_function_modified_two;
    
    delete [] jobs;
    delete [] workers_c;
    delete [] workers_s;
    delete [] workers_s_float;
    
    return 0;
}

char** simple_split(char *str, int length, char separator)
{
    char **string_array;
    int i;
    int j;
    int k;
    int m;
    int key;
    int count;
    
    string_array = nullptr;
    
    for (j = 0, m = 0; j < length; j++)
    {
        if (str[j] == separator)
        {
            m = m + 1;
        }
    }

    key = 0;
    string_array = new char*[m + 1];
    if(string_array != nullptr)
    {
        for (i = 0, count = 0; i <= m; i++, count++)
        {
            string_array[i] = new char[length];
            if (string_array[i] != nullptr)
            {
                key = 1;
            }
            else
            {
                key = 0;
                i = m;
            }
        }
        if (key == 1)
        {
            k = 0;
            m = 0;
            for (j = 0; j < length; j++)
            {
                if (str[j] != separator)
                {
                    string_array[m][j - k] = str[j];
                }
                else
                {
                    string_array[m][j - k] = '\0';
                    k = j + 1;
                    m = m + 1;
                }
            }
        }
        else
        {
            for (i = 0; i < count; i++)
            {
                delete [] string_array[i];
            }
            delete [] string_array;
        }
     }
    return string_array;
}

int** input_data_file (int local_size, char* local_name)
{
    int i;
    int j;
    int n;
    int local_length;
    int **local_numbers;
    FILE* local_file;
    char local_separator;
    char local_buffer[160];
    char **local_buffer_two;
    
    i = 0;
    j = 0;
    n = 0;
    local_length = 0;
    local_numbers = nullptr;
    local_file = nullptr;
    local_separator = ';';
    local_buffer_two = nullptr;
    
    //char local_name[160];
    //fflush(stdin);
    //printf("Please, print the name of the file: ");
    //scanf("%s", local_name);
    
    local_file = fopen(local_name, "r");
    
    if (local_file != nullptr)
    {
        while (fgets(local_buffer, 160, local_file) != nullptr)
        {
            n = n + 1;
        }
        local_numbers = new int*[n + 1];
        rewind(local_file);
        for (i = 1; i <= n; i++)
        {
            fgets(local_buffer, 160, local_file);
            local_length = (int)strlen(local_buffer);
            local_buffer[local_length - 1] = '\0';
            local_buffer_two = simple_split(local_buffer, local_length, local_separator);
            if (local_buffer_two != nullptr)
            {
                local_numbers[i] = new int[local_size + 1];
                for (j = 1; j <= local_size; j++)
                {
                    local_numbers[i][j] = atoi(local_buffer_two[j - 1]);
                    delete [] local_buffer_two[j - 1];
                }
                delete [] local_buffer_two;
            }
            else
            {
                cout << "Error at data reading!\n";
            }
        }
        fclose(local_file);
    }
    else
    {
        cout << "File hasn't been found!\n";
    }
    return local_numbers;
}
