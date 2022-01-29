//
//  main.cpp
//  Alternative exam
//  version release 2.1
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

// 32 -> 10, 10 -> 32

const int rows = 32; // general number of rows in array (number of items)
const int columns = 11; // general number of columns in array (number of knapsacks)
const int max_bin_sizes = 30; // maximal sizes for bins (days for workers)
const int jobs_number = 32; // general number of jobs (translations)
const int workers_number = 11; // general number of workers (translators)
const int languages_number = 7; // general number of languages

// VALUABLE VARIABLES

int work_hours = 0; // number for the work hours coefficient switch case
int work_task = 0; // number for the task switch case
int work_function = 0; // number for the forming profit function

// FUNCTION PROTOTYPES

// CLASSES

class Approximation_algorithm
{
private:
    int x; // current column iteration (sorry, i forgot about 'k' variable)
    int answer = 0; // answer variable
    int deadline = 0; // biggest deadline
    
    int current_value;
    int current_row;
    int current_step;
    
    int *bin_sizes; // general capacity of bins (workers)
    int **S; // items (objects) chosed at all/in general
    int *T; // items (objects) passed in general
    
    int **item_sizes; // weight of each item
    int **profit_function; // profit of each item
    //int **profit_function_modified_one; // work function
    //int **profit_function_modified_two; // work function
    
    int P[rows + 1]; // current profit function for algorithm 2
    
    int **jobs; // rows - values, columns - jobs
    int **workers_c; // rows - workers, columns - languages
    int **workers_s; // rows - workers, columns - languages
    float **workers_s_float; // rows - workers, columns - languages
    
public:
    char** simple_split(char *str, int length, char separator);
    int** input_data_file (int local_size, char* local_name);
    
    void forming_profit();
    void forming_profit_reversive();
    void output_current();
    void find_invalid();
    void output_result();
    void algorithm_one();
    void algorithm_two();
    void algorithm_one_custom();
    void algorithm_two_custom();
    void algorithm_reversive();
    
    Approximation_algorithm();
    ~Approximation_algorithm();
};

// ---------------------------------
// ---------- CONSTRUCTOR ----------
// ---------------------------------

Approximation_algorithm :: Approximation_algorithm()
{
    int i;
    int j;
    
    bin_sizes = new int[columns + 1]; // general capacity of bins (workers)
    S = new int*[rows + 1]; // items (objects) chosed at all/in general
    T = new int[rows + 1]; // items (objects) passed in general
    
    item_sizes = new int*[rows + 1]; // weight of each item
    profit_function = new int*[rows + 1]; // profit of each item
    //profit_function_modified_one = new int*[rows + 1]; // work function
    //profit_function_modified_two = new int*[rows + 1]; // work function
    
    for (i = 0; i <= rows; i++) // creating dynamic double arrays
    {
        S[i] = new int[columns + 1];
        T[i] = -1;
        
        item_sizes[i] = new int[columns + 1];
        profit_function[i] = new int[columns + 1];
        //profit_function_modified_one[i] = new int[columns + 1];
        //profit_function_modified_two[i] = new int[columns + 1];
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
    
    jobs = input_data_file(jobs_number, jobs_file); // rows - values, columns - jobs
    workers_c = input_data_file(languages_number, workers_c_file); // rows - workers, columns - languages
    workers_s = input_data_file(languages_number, workers_s_file); // rows - workers, columns - languages
    workers_s_float = new float*[workers_number + 1]; // rows - workers, columns - languages
    
    // FINDING MAX OF THE JOBS DEADLINES (FOR CASE 9)
    
    for (i = 1; i <= jobs_number; i++) // choosing biggest deadline
    {
        if (jobs[5][i] > deadline)
        {
            deadline = jobs[5][i];
        }
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
}

// --------------------------------
// ---------- DESTRUCTOR ----------
// --------------------------------

Approximation_algorithm :: ~Approximation_algorithm()
{
    int i;
    
    // CLEANING OUR MEMORY FROM DYNAMIC ARRAYS
    
    for (i = 0; i <= rows; i++)
    {
        delete [] S[i];
        
        delete [] item_sizes[i];
        delete [] profit_function[i];
        //delete [] profit_function_modified_one[i];
        //delete [] profit_function_modified_two[i];
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
    //delete [] profit_function_modified_one;
    //delete [] profit_function_modified_two;
    
    delete [] jobs;
    delete [] workers_c;
    delete [] workers_s;
    delete [] workers_s_float;
}

// ---------------------------------------------
// ---------- FORMING PROFIT FUNCTION ----------
// ---------------------------------------------

void Approximation_algorithm :: forming_profit()
{
    int i;
    int j;
    
    // TRANSLATION INTO THE FLOAT FORMAT FOR THE WORKERS' SPEED WITH SWITCH CASE
    
    workers_s_float[0] = new float[languages_number + 1];
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
        case 2: // 24-hours work day
            cout << "---------- 24-hours work day ----------" << endl;
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
                        if ((float)jobs[2][i]/workers_s_float[j][jobs[1][i]] > (float)(int)((float)jobs[2][i]/workers_s_float[j][jobs[1][i]]))
                        {
                            item_sizes[i][j] = item_sizes[i][j] + 1;
                        }
                        cout << item_sizes[i][j] << " ";
                    }
                    else
                    {
                        item_sizes[i][j] = bin_sizes[j] + 1;
                    }
                    switch (work_function)
                    {
                        case 1:
                            profit_function[i][j] = (-1)*(item_sizes[i][j] - bin_sizes[j] - 1);
                            break;
                        case 2:
                            profit_function[i][j] = item_sizes[i][j];
                            break;
                        case 3:
                            profit_function[i][j] = (workers_s[j][jobs[1][i]]);
                            break;
                        case 4:
                            profit_function[i][j] = jobs[2][i];
                            break;
                        default:
                            profit_function[i][j] = (-1)*(item_sizes[i][j] - bin_sizes[j] - 1);
                            break;
                    }
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
                    switch (work_function)
                    {
                        case 1:
                            profit_function[i][j] = (-1)*(item_sizes[i][j] - bin_sizes[j] - 1);
                            break;
                        case 2:
                            profit_function[i][j] = item_sizes[i][j];
                            break;
                        case 3:
                            profit_function[i][j] = (workers_s[j][jobs[1][i]]);
                            break;
                        case 4:
                            profit_function[i][j] = jobs[2][i];
                            break;
                        default:
                            profit_function[i][j] = (-1)*(item_sizes[i][j] - bin_sizes[j] - 1);
                            break;
                    }
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
                    switch (work_function)
                    {
                        case 1:
                            profit_function[i][j] = (-1)*(item_sizes[i][j] - bin_sizes[j] - 1);
                            break;
                        case 2:
                            profit_function[i][j] = item_sizes[i][j];
                            break;
                        case 3:
                            profit_function[i][j] = (workers_s[j][jobs[1][i]]);
                            break;
                        case 4:
                            profit_function[i][j] = jobs[2][i];
                            break;
                        default:
                            profit_function[i][j] = (-1)*(item_sizes[i][j] - bin_sizes[j] - 1);
                            break;
                    }
                }
            }
            break;
        case 6: // maximal quality to do extra important task (extra important tasks) (3 importance)
            cout << "---------- Maximal quality to do extra important task (extra important tasks) (3 importance) ----------" << endl;
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
            cout << "---------- The minimum time for completing all tasks (deputy chief 4-hours day (without 3 importance)) ----------" << endl;
            for (i = 1; i <= jobs_number; i++)
            {
                for (j = 1; j <= workers_number; j++)
                {
                    if (workers_s[j][jobs[1][i]] != 0)
                    {
                        item_sizes[i][j] = (int)((float)jobs[2][i]/workers_s_float[j][jobs[1][i]]);
                        if ((j == 2 || j == 9) && jobs[4][i] != 3)
                        {
                            item_sizes[i][j] = 2*item_sizes[i][j]; // 4-hours work day
                        }
                    }
                    else
                    {
                        item_sizes[i][j] = bin_sizes[j] + 1;
                    }
                    switch (work_function)
                    {
                        case 1:
                            profit_function[i][j] = (-1)*(item_sizes[i][j] - bin_sizes[j] - 1);
                            break;
                        case 2:
                            profit_function[i][j] = item_sizes[i][j];
                            break;
                        case 3:
                            profit_function[i][j] = (workers_s[j][jobs[1][i]]);
                            break;
                        case 4:
                            profit_function[i][j] = jobs[2][i];
                            break;
                        default:
                            profit_function[i][j] = (-1)*(item_sizes[i][j] - bin_sizes[j] - 1);
                            break;
                    }
                }
            }
            break;
        /*case 12:
            cout << "---------- Distribution depending on deadline of taks ----------" << endl;
            for (i = 1; i <= workers_number; i++)
            {
                for (j = 1; j <= jobs_number; j++)
                {
                    if (workers_s[jobs[1][j]][i] != 0)
                    {
                        item_sizes[i][j] = (int)((float)jobs[2][j]/workers_s_float[jobs[1][j]][i]);
                    }
                    else
                    {
                        item_sizes[i][j] = bin_sizes[j] + 1;
                    }
                    profit_function[i][j] = (-1)*(item_sizes[i][j] - deadline - 1);
                }
            }
            break;*/
        default: // standart work of the algorithm (case 1: minimal time to do all the tasks)
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
                    switch (work_function)
                    {
                        case 1:
                            profit_function[i][j] = (-1)*(item_sizes[i][j] - bin_sizes[j] - 1);
                            break;
                        case 2:
                            profit_function[i][j] = item_sizes[i][j];
                            break;
                        case 3:
                            profit_function[i][j] = (workers_s[j][jobs[1][i]]);
                            break;
                        case 4:
                            profit_function[i][j] = jobs[2][i];
                            break;
                        default:
                            profit_function[i][j] = (-1)*(item_sizes[i][j] - bin_sizes[j] - 1);
                            break;
                    }
                }
            }
            break;
    }
}

// -------------------------------------------------------
// ---------- FORMING REVERSIVE PROFIT FUNCTION ----------
// -------------------------------------------------------

void Approximation_algorithm :: forming_profit_reversive()
{
    int i;
    int j;
    
    // TRANSLATION INTO THE FLOAT FORMAT FOR THE WORKERS' SPEED WITH SWITCH CASE
    
    workers_s_float[0] = new float[languages_number + 1];
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
}

// ----------------------------------
// ---------- SIMPLE SPLIT ----------
// ----------------------------------

char** Approximation_algorithm :: simple_split(char *str, int length, char separator)
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

// -------------------------------------
// ---------- INPUT DATA FILE ----------
// -------------------------------------

int** Approximation_algorithm :: input_data_file (int local_size, char* local_name)
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

// -----------------------------------------
// ---------- OUTPUT CURRENT DATA ----------
// -----------------------------------------

void Approximation_algorithm :: output_current()
{
    int i;
    int j;
    
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
}

// ---------------------------------------------------
// ---------- SETTING TO NULL INVALID ITEMS ----------
// ---------------------------------------------------

void Approximation_algorithm :: find_invalid()
{
    int i;
    int j;
    
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
}

// ----------------------------------------------------
// ---------- RESULT OUTPUT AND CLEAN MEMORY ----------
// ----------------------------------------------------

void Approximation_algorithm :: output_result()
{
    int i;
    int j;
    
    // SETTING TO NULL INVALID ITEMS
    
    find_invalid();
    
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
        if (work_task != 6 && work_task != 4)
        {
            cout << "(" << a << " days)";
            if (a > answer)
            {
                answer = a;
            }
        }
        cout << endl;
    }
    cout << endl;
    
    // OUTPUT AN ANSWER
    
    cout << "Answer: " << answer << endl;
}

// ----------------------------------------------------
// ---------- KNAPSACK PROBLEM ALGORITHM (1) ----------
// ----------------------------------------------------

void Approximation_algorithm :: algorithm_one()
{
    int i;
    int j;
    
    int **isin = new int*[rows + 1];
    int **isin_row = new int*[rows + 1];
    int **isin_step = new int*[rows + 1];
    int **Weight = new int*[rows + 1];
    
    for (i = 0; i <= rows; i++)
    {
        isin[i] = new int[max_bin_sizes + 1];
        isin_row[i] = new int[max_bin_sizes + 1];
        isin_step[i] = new int[max_bin_sizes + 1];
        Weight[i] = new int[max_bin_sizes + 1];
    }
    
    int profit_function_modified_one [rows + 1][columns + 1]; // work function
    int profit_function_modified_two [rows + 1][columns + 1]; // work function
    
    // SETTIN' TO INITIAL ARRAY VALUES WORK ARRAYS
    
    for (i = 0; i <= rows; i++)
    {
        for (j = 0; j <= columns; j++)
        {
            profit_function_modified_one[i][j] = profit_function[i][j];
            profit_function_modified_two[i][j] = profit_function[i][j];
        }
    }
    
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
    
    for (i = 0; i <= rows; i++)
    {
        delete [] isin[i];
        delete [] isin_row[i];
        delete [] isin_step[i];
        delete [] Weight[i];
    }
    
    delete [] isin;
    delete [] isin_row;
    delete [] isin_step;
    delete [] Weight;
}

// ----------------------------------------------------
// ---------- KNAPSACK PROBLEM ALGORITHM (2) ----------
// ----------------------------------------------------

void Approximation_algorithm :: algorithm_two()
{
    int i;
    int j;
    
    int **isin = new int*[rows + 1];
    int **isin_row = new int*[rows + 1];
    int **isin_step = new int*[rows + 1];
    int **Weight = new int*[rows + 1];
    
    for (i = 0; i <= rows; i++)
    {
        isin[i] = new int[max_bin_sizes + 1];
        isin_row[i] = new int[max_bin_sizes + 1];
        isin_step[i] = new int[max_bin_sizes + 1];
        Weight[i] = new int[max_bin_sizes + 1];
    }
    
    for (x = 1; x <= columns; x++)
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
    }
    
    for (i = 0; i <= rows; i++)
    {
        delete [] isin[i];
        delete [] isin_row[i];
        delete [] isin_step[i];
        delete [] Weight[i];
    }
    
    delete [] isin;
    delete [] isin_row;
    delete [] isin_step;
    delete [] Weight;
}

// -----------------------------------------------------------
// ---------- KNAPSACK PROBLEM CUSTOM ALGORITHM (1) ----------
// -----------------------------------------------------------

void Approximation_algorithm :: algorithm_one_custom()
{
    int i;
    int j;
    
    int **isin = new int*[rows + 1];
    int **isin_row = new int*[rows + 1];
    int **isin_step = new int*[rows + 1];
    int **Weight = new int*[rows + 1];
    
    for (i = 0; i <= rows; i++)
    {
        isin[i] = new int[max_bin_sizes + 1];
        isin_row[i] = new int[max_bin_sizes + 1];
        isin_step[i] = new int[max_bin_sizes + 1];
        Weight[i] = new int[max_bin_sizes + 1];
    }
    
    int profit_function_modified_one [rows + 1][columns + 1]; // work function
    int profit_function_modified_two [rows + 1][columns + 1]; // work function
    
    // SETTIN' TO INITIAL ARRAY VALUES WORK ARRAYS
    
    for (i = 0; i <= rows; i++)
    {
        for (j = 0; j <= columns; j++)
        {
            profit_function_modified_one[i][j] = profit_function[i][j];
            profit_function_modified_two[i][j] = profit_function[i][j];
        }
    }
    
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
    
    find_invalid(); // find all choosed jobs
    
    int k = 0; // jobs counter
    int l = 0;
    
    for (i = 0; i <= rows; i++) // jobs counting
    {
        for (j = 0; j <= columns; j++)
        {
            if (S[i][x] == 1)
            {
                k++;
            }
        }
    }
    if (k != jobs_number) // find unused jobs
    {
        // SOMETHING HERE
    }
    k = 0; // maximal worker number
    l = 0; // minimal worker number
    int answer_local = 0;
    int minimal_local = max_bin_sizes;
    for (i = 1; i <= columns; i++)
    {
        int a = 0;
        int b = 0;
        for (j = 1; j <= rows; j++)
        {
            if (S[j][i] == 1)
            {
                a = a + item_sizes[j][i];
                b = b + workers_c[i][jobs[1][j]];
            }
        }
        if (work_task == 6 || work_task == 4)
        {
            answer_local = answer_local + b;
        }
        if (work_task != 6 && work_task != 4 && a > answer_local)
        {
            answer_local = a;
            k = i;
        }
        if (work_task != 6 && work_task != 4 && a < minimal_local)
        {
            minimal_local = a;
            l = i;
        }
    }
    int size_local = max_bin_sizes;
    int size_index_local = 0;
    for (i = 0; i <= rows; i++)
    {
        if (item_sizes[i][k] < size_local)
        {
            size_local = item_sizes[i][k];
            size_index_local = i;
        }
    }
    if (item_sizes[size_index_local][l] < max_bin_sizes)
    {
        S[size_index_local][l] = 1;
        S[size_index_local][k] = 0;
    }
    
    for (i = 0; i <= rows; i++)
    {
        delete [] isin[i];
        delete [] isin_row[i];
        delete [] isin_step[i];
        delete [] Weight[i];
    }
    
    delete [] isin;
    delete [] isin_row;
    delete [] isin_step;
    delete [] Weight;
}

// ----------------------------------------------------------
// ---------- KNAPSACK PROBLEM REVERSIVE ALGORITHM ----------
// ----------------------------------------------------------

void Approximation_algorithm :: algorithm_reversive() // unworkable
{
    int i;
    int j;
    int k = 0;
    
    // INITIALIZING LOCAL ARRAYS
    
    int **isin = new int*[columns + 1];
    int **isin_row = new int*[columns + 1];
    int **isin_step = new int*[columns + 1];
    int **Weight = new int*[columns + 1];
    
    int *bin_sizes_reversive = new int[rows + 1]; // general capacity of bins (workers)
    int **S_reversive = new int*[columns + 1]; // items (objects) chosed at all/in general
    int *T_reversive = new int[columns + 1]; // items (objects) passed in general
    
    int **item_sizes_reversive = new int*[columns + 1]; // weight of each item
    int **profit_function_reversive = new int*[columns + 1];
    int *jobs_passed_reversive = new int[rows + 1];
    
    int P_reversive[columns + 1];
    
    for (i = 0; i <= columns; i++) // creating dynamic double arrays
    {
        isin[i] = new int[max_bin_sizes + 1];
        isin_row[i] = new int[max_bin_sizes + 1];
        isin_step[i] = new int[max_bin_sizes + 1];
        Weight[i] = new int[max_bin_sizes + 1];
        
        S_reversive[i] = new int[rows + 1];
        T_reversive[i] = -1;
        
        item_sizes_reversive[i] = new int[rows + 1];
        profit_function_reversive[i] = new int[rows + 1];
    }
    
    // BIN SIZES INITIALIZATION (DAYS FOR EACH WORKER)
    
    for (i = 0; i <= jobs_number; i++) // it doesn't matter here, from "0" or "1" to start
    {
        bin_sizes_reversive[i] = 15; // (max_bin_sizes)
    }
    
    for (i = 1; i <= rows; i++)
    {
        for (j = 0; j <= columns; j++)
        {
            item_sizes_reversive[j][i] = item_sizes[i][j];
            profit_function_reversive[j][i] = profit_function[i][j];
        }
    }
    
    // ALGORITHM BASED ON ALGORITHM 2
    
    for (x = 1; x <= rows; x++)
    {
        for (i = 1; i <= columns; i++)
        {
            if (T_reversive[i] == -1)
            {
                P_reversive[i] = profit_function_reversive[i][x];
            }
            else
            {
                P_reversive[i] = profit_function_reversive[i][x] - profit_function_reversive[i][T_reversive[i]];
            }
        }
        for (i = 0; i <= bin_sizes_reversive[x]; i++)
        {
            isin[0][i] = 0;
            Weight[0][i] = 0;
        }
        for (i = 1; i <= columns; i++)
        {
            for (j = 0; j <= bin_sizes_reversive[x]; j++)
            {
                if (item_sizes_reversive[i][x] > j)
                {
                    Weight[i][j] = Weight[i - 1][j];
                    isin_row[i][j] = isin_row[i - 1][j];
                    isin_step[i][j] = isin_step[i - 1][j];
                    isin[i][j] = isin[i - 1][j];
                }
                else
                {
                    Weight[i][j] = fmax(Weight[i - 1][j], Weight[i - 1][j - item_sizes_reversive[i][x]] + P_reversive[i]);
                    if (Weight[i - 1][j] >= Weight[i - 1][j - item_sizes_reversive[i][x]] + P_reversive[i])
                    {
                        isin_row[i][j] = isin_row[i - 1][j];
                        isin_step[i][j] = isin_step[i - 1][j];
                        isin[i][j] = isin[i - 1][j];
                    }
                    else
                    {
                        isin_row[i][j] = i - 1;
                        isin_step[i][j] = j - item_sizes_reversive[i][x];
                        isin[i][j] = i;
                    }
                }
            }
        }
        
        // FINDING CHOOSED ITEMS (WITH MY "CHAIN METHOD")
        
        for (i = 0; i <= columns; i++)
        {
            S_reversive[i][x] = 0;
        }
        
        current_value = isin[columns][bin_sizes_reversive[x]]; // last added item number in array
        current_row = isin_row[columns][bin_sizes_reversive[x]]; // last added BEFORE last added in array ROW
        current_step = isin_step[columns][bin_sizes_reversive[x]]; // last added BEFORE last added in array COLUMN
        
        while (current_value != 0) // restore item numbers with my "chain method"
        {
            S_reversive[current_value][x] = 1;
            current_value = isin[current_row][current_step];
            int a = isin_row[current_row][current_step];
            current_step = isin_step[current_row][current_step];
            current_row = a;
        }
        
        // UPDATIN' T ARRAY WITH NEW "PASSED" (CHOOSED IN SOME STEP) ITEMS
        
        int z = -1;
        int y = 0;
        int l = 0;
        
        for (i = 1; i <= columns; i++)
        {
            if (S_reversive[i][x] == 1)
            {
                if (profit_function_reversive[i][x] > z)
                {
                    T_reversive[y] = -1;
                    T_reversive[i] = x;
                    y = i;
                    z = profit_function_reversive[i][x];
                }
            }
        }
        
        int q = 0;
        
        for (i = 1; i <= columns; i++)
        {
            if (T_reversive[i] == -1)
            {
                q = 1;
                i = columns;
            }
        }
        
        if (q == 0)
        {
            k = 1;
        }
        
        // CURRENT PROFIT FUNCTION P_j (P_x HERE)
        
        /*cout << "Profit function P_j (P_x): " << endl;
        for (i = 1; i <= columns; i++)
        {
            cout << P_reversive[i];
            cout << endl;
        }
        cout << endl;*/
        
        // TEMPORARY WORK ARRAY (A - KNAPSACK PROBLEM - ALGORITHM) OUTPUT
        
        /*cout << "Temporary work array:" << endl;
        for (i = 0; i <= columns; i++)
        {
            for (j = 0; j <= bin_sizes_reversive[x]; j++)
            {
                cout << Weight[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;*/
        
        // NUMBER OF CHOSEN ITEMS AT CURRENT ITERATION OUTPUT
        
        cout << "Step " << x << " items number: ";
        for (i = 0; i <= columns; i++)
        {
            if (S_reversive[i][x] == 1)
            {
                cout << i << " ";
            }
            //T_reversive[i] = -1;
        }
        cout << endl << endl;
    }
    
    for (i = 0; i <= columns; i++)
    {
        delete [] isin[i];
        delete [] isin_row[i];
        delete [] isin_step[i];
        delete [] Weight[i];
        
        delete [] S_reversive[i];
        
        delete [] item_sizes_reversive[i];
        delete [] profit_function_reversive[i];
    }
    
    delete [] isin;
    delete [] isin_row;
    delete [] isin_step;
    delete [] Weight;
    
    delete [] bin_sizes_reversive;
    delete [] S_reversive;
    delete [] T_reversive;
    
    delete [] item_sizes_reversive;
    delete [] profit_function_reversive;
    delete [] jobs_passed_reversive;
}

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
    
    int work_algorithm;
    
    Approximation_algorithm A;
    
    cout << "Please, choose hours to work:" << endl << "0 -- 8-hours work day (default)" << endl << "1 -- 12-hours work day" << endl << "2 -- 24-hours work day" << endl;
    cin >> work_hours;
    
    cout << "Please, choose the target function:" << endl << "1 -- Minimal time to do all the tasks (default)" << endl << "2 -- Minimal time to do all the immediate tasks" << endl << "3 -- Minimal time to do all the important tasks" << endl << "4 -- Maximal quality to do all the tasks" << endl << "5 -- Maximal fast (minimal time) to do extra important task (extra important tasks) (3 importance)" << endl << "6 -- Maximal quality to do extra important task (extra important tasks) (3 importance)" << endl << "8 -- Distribution depending on importance of tasks" << endl << "9 -- Distribution depending on deadline of tasks" << endl << "11 -- The minimum time for completing all tasks (deputy chief 4-hours day (without 3 importance))" << endl;
    cin >> work_task;
    
    if (work_task == 1 || work_task == 2 || work_task == 5 || work_task == 11)
    {
        cout << "Please, choose the work profit function (for target functions 1, 2, 5 and 11):" << endl << "1 -- Minimal time - max profit (default)" << endl << "2 -- Maximal time - max profit" << endl << "3 -- Maximal speed - max profit" << endl << "4 -- Maximal list size - max profit" << endl;
        cin >> work_function;
    }
    
    cout << "Please, choose the algorithm:" << endl << "1 -- Algorithm 1 (only) (default)" << endl << "2 -- Algorithm 2 (only)" << endl;/* << "3 -- Algorithm 1 (custom) NOT READY" << endl << "4 -- Algorithm 2 (custom) NOT READY" << endl << "5 -- Algorithm 2 (reversive) UNWORKABLE" << endl;*/
    cin >> work_algorithm;
    
    A.forming_profit();
    A.output_current();
    
    switch(work_algorithm)
    {
        case 1:
            A.algorithm_one();
            break;
        case 2:
            A.algorithm_two();
            break;
        /*case 3:
            A.algorithm_one_custom();
            break;
        case 4:
            A.algorithm_two_custom();
            break;
        case 5:
            A.algorithm_reversive();
            break;*/
        default:
            A.algorithm_one();
            break;
    }
    
    A.output_result();
    
    return 0;
}
