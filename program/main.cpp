//
//  main.cpp
//  Alternative exam
//  version beta 0.5
//  Created by Matthew Sobolewski on 27.12.2020.
//

#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <vector>
#include <list>

using namespace std;

// VALUABLE CONSTANTS

const int rows = 32; // general number of rows in array (number of items)
const int columns = 10; // general number of columns in array (number of knapsacks)
const int max_bin_sizes = 30; // maximal sizes for bins (days for workers)
const int jobs_number = 32; // general number of jobs (translations)
const int workers_number = 10; // general number of workers (translators)
const int languages_number = 7; // general number of languages

// FUNCTION PROTOTYPES

char** simple_split(char *str, int length, char separator);
int** input_data_file (int local_size, char* local_name);

// INSTRUCTION

// for jobs ("jobs.txt file"):
// english - 1; german - 2; french - 3; spanish - 4; italian - 5; portugal - 6; polish - 7
// second row - size
// third row - complexity
// fourth row - importance
// fifth row - days

// for workers:
// Complexity matrix:
// each row - worker, each column - language, "0" means "doesn't exist"
// Speed matrix:
// each row - worker, each column - language, "0" means "doesn't exist"

int main()
{
    // INITIALIZING ARRAYS FOR EFFICIENT APPROXIMATION
    
    int i;
    int j;
    int x; // current column iteration (sorry, i forgot about 'k' variable)
    
    int current_value;
    int current_row;
    int current_step;
    
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
    
    // BIN SIZES INITIALIZATION (DAYS FOR EACH WORKER)
    
    for (i = 0; i <= workers_number; i++) // it doesn't matter here, from "0" or "1" to start
    {
        bin_sizes[i] = max_bin_sizes;
    }
    
    // VALUES FROM A AUTOMATIC FILE INPUT FOR OUR TASK
    
    int *translation_size = new int[rows + 1];
    int *translation_complexity = new int[rows + 1];
    int *translation_importance = new int[rows + 1];
    int *translation_days = new int[rows + 1];
    float *translator_speed = new float[columns + 1]; // eto vsyo ne nado, mozho udalyat
    
    char jobs_file [] = {'j', 'o', 'b', 's', '.', 't', 'x', 't', '\0'};
    char workers_c_file [] = {'w', 'o', 'r', 'k', 'e', 'r', 's', '_', 'c', '.', 't', 'x', 't', '\0'};
    char workers_s_file [] = {'w', 'o', 'r', 'k', 'e', 'r', 's', '_', 's', '.', 't', 'x', 't', '\0'};
    
    int **jobs = input_data_file(jobs_number, jobs_file); // rows - values, columns - jobs
    int **workers_c = input_data_file(languages_number, workers_c_file); // rows - workers, columns - languages
    int **workers_s = input_data_file(languages_number, workers_s_file); // rows - workers, columns - languages
    float **workers_s_float = new float*[workers_number + 1]; // rows - workers, columns - languages
    
    // TRANSLATION INTO THE FLOAT FORMAT FOR THE WORKERS' SPEED
    
    for (i = 1; i <= workers_number; i++)
    {
        workers_s_float[i] = new float[languages_number + 1];
        for (j = 1; j <= languages_number; j++)
        {
            workers_s_float[i][j] = 8*(float)workers_s[i][j]/100;
        }
    }
    
    // FORMING PROFIT FUNCTION (ROWS - JOBS, COLUMNS - WORKERS)
    
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
            profit_function[i][j] = 1;
        }
    }
    
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
        
        cout << "Step " << x << " items number: ";
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
    
    for (i = 0; i <= 5; i++)
    {
        delete [] jobs[i];
    }
    
    for (i = 0; i <= workers_number; i++)
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
