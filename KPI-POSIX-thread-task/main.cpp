//
//  main.cpp
//  DSMatrix
//
//  Created by Dmytro Skorokhod on 01/20/2013.
//  Copyright (c) 2013 Dmytro Skorokhod. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <pthread.h>

using namespace std;

int **matrix;
int rowsCount, columnsCount;
pthread_mutex_t readingMutex, writingMutex;

void generateMatrix(int aRowsCount, int aColumnsCount);
void destroyMatrix(void);
void fillMatrixByComputer(void);
void fillMatrixByUser(void);
void showMatrix(void);
void decision(void);
void *decisionForRow(void *aRow);
void *decisionForColumn(void *aColumn);

void generateMatrix(int aRowsCount, int aColumnsCount)
{
    if (matrix != NULL)
    {
        for (int theColumnIndex = 0; theColumnIndex < columnsCount + 3;
             theColumnIndex++)
        {
            delete matrix[theColumnIndex];
        }
        delete matrix;
    }
    rowsCount = aRowsCount;
    columnsCount = aColumnsCount;
    
    matrix = new int*[rowsCount + 3];
    
    for (int theRowIndex = 0; theRowIndex < rowsCount + 3; theRowIndex++)
    {
        matrix[theRowIndex] = new int[columnsCount];
        
        for (int theColumnIndex = 0; theColumnIndex < columnsCount;
             theColumnIndex++)
        {
            matrix[theRowIndex][theColumnIndex] = 0;
        }
    }
}

void destroyMatrix(void)
{
    if (matrix != NULL)
    {
        for (int theColumnIndex = 0; theColumnIndex < columnsCount;
             theColumnIndex++)
        {
            delete matrix[theColumnIndex];
        }
        delete matrix;
    }
}

void fillMatrixByComputer(void)
{
    pthread_mutex_lock(&readingMutex);
    pthread_mutex_lock(&writingMutex);
    
    for (int theRowIndex = 0; theRowIndex < rowsCount; theRowIndex++)
    {
        for (int theColumnIndex = 0; theColumnIndex < columnsCount;
             theColumnIndex++)
        {
            matrix[theRowIndex][theColumnIndex] = rand() % 100;
        }
    }
    
    pthread_mutex_unlock(&readingMutex);
    pthread_mutex_unlock(&writingMutex);
}

void fillMatrixByUser(void)
{
    pthread_mutex_lock(&readingMutex);
    pthread_mutex_lock(&writingMutex);
    
    for (int theRowIndex = 0; theRowIndex < rowsCount; theRowIndex++)
    {
        for (int theColumnIndex = 0; theColumnIndex < columnsCount;
             theColumnIndex++)
        {
            cout << "Input a value for the row " << theRowIndex + 1
            << ", the column " << theColumnIndex + 1 << ": ";
            int theValue;
            cin >> theValue;
            matrix[theRowIndex][theColumnIndex] = theValue;
        }
    }
    
    pthread_mutex_unlock(&readingMutex);
    pthread_mutex_unlock(&writingMutex);
}

void showMatrix(void)
{
    cout << endl << "The matrix:" << endl;
    
    pthread_mutex_lock(&writingMutex);
    pthread_mutex_lock(&readingMutex);
    pthread_mutex_unlock(&readingMutex);
    
    for (int theRowIndex = 0; theRowIndex < rowsCount + 3; theRowIndex++)
    {
        for (int theColumnIndex = 0; theColumnIndex < columnsCount + 3;
             theColumnIndex++)
        {
            cout << setw(10);
            
            if (theRowIndex >= rowsCount && theColumnIndex >= columnsCount)
            {
            }
            else if (theColumnIndex == columnsCount || theRowIndex == rowsCount)
            {
                cout << setw(7) << "Middle " << setw(3);
            }
            else if (theColumnIndex == columnsCount + 1 ||
                     theRowIndex == rowsCount + 1)
            {
                cout << setw(5) << "Less " << setw(5);
            }
            else if (theColumnIndex == columnsCount + 2 ||
                     theRowIndex == rowsCount + 2)
            {
                cout << setw(5) << "More " << setw(5);
            }
            
            if (theRowIndex < rowsCount || theColumnIndex < columnsCount)
            {
                cout << matrix[theRowIndex][theColumnIndex] << " ";
            }
        }
        
        cout << endl;
    }
    
    cout << endl;
    
    pthread_mutex_unlock(&writingMutex);
}

void decision(void)
{
    pthread_t pthreadsForRows[rowsCount];
    pthread_t pthreadsForColumns[columnsCount];
    
    pthread_mutex_lock(&writingMutex);
    pthread_mutex_lock(&readingMutex);
    pthread_mutex_unlock(&readingMutex);
    
    for (int theRowIndex = 0; theRowIndex < rowsCount; theRowIndex++)
    {
        pthread_t theThread = pthreadsForRows[theRowIndex];
        pthread_create(&theThread, NULL, decisionForRow, (void *)&theRowIndex);
        pthread_join(theThread, NULL);
    }
    
    for (int theColumnIndex = 0; theColumnIndex < columnsCount;
         theColumnIndex++)
    {
        pthread_t theThread = pthreadsForColumns[theColumnIndex];
        pthread_create(&theThread, NULL, decisionForColumn, (void *)&theColumnIndex);
        pthread_join(theThread, NULL);
    }
    
    pthread_mutex_unlock(&writingMutex);
}

void *decisionForRow(void *aRow)
{
    int theRow = *((int *)aRow);
    int theSum = 0;
    
    for (int theColumnIndex = 0; theColumnIndex < columnsCount;
         theColumnIndex++)
    {
        theSum += matrix[theRow][theColumnIndex];
    }
    
    int theMiddleValue = theSum / columnsCount;
    
    int theLessCount = 0;
    int theMoreCount = 0;
    
    for (int theColumnIndex = 0; theColumnIndex < columnsCount;
         theColumnIndex++)
    {
        int theValue = matrix[theRow][theColumnIndex];
        
        if (theValue < theMiddleValue)
        {
            theLessCount++;
        }
        else if (theValue > theMiddleValue)
        {
            theMoreCount++;
        }
    }
    
    matrix[theRow][columnsCount] = theMiddleValue;
    matrix[theRow][columnsCount + 1] = theLessCount;
    matrix[theRow][columnsCount + 2] = theMoreCount;
    
    return NULL;
}

void *decisionForColumn(void *aColumn)
{
    int theColumn = *((int *)aColumn);
    int theSum = 0;
    
    for (int theRowIndex = 0; theRowIndex < rowsCount; theRowIndex++)
    {
        theSum += matrix[theRowIndex][theColumn];
    }
    
    int theMiddleValue = theSum / rowsCount;
    
    int theLessCount = 0;
    int theMoreCount = 0;
    
    for (int theRowIndex = 0; theRowIndex < rowsCount; theRowIndex++)
    {
        int theValue = matrix[theRowIndex][theColumn];
        
        if (theValue < theMiddleValue)
        {
            theLessCount++;
        }
        else if (theValue > theMiddleValue)
        {
            theMoreCount++;
        }
    }
    
    matrix[rowsCount][theColumn] = theMiddleValue;
    matrix[rowsCount + 1][theColumn] = theLessCount;
    matrix[rowsCount + 2][theColumn] = theMoreCount;
    
    return NULL;
}

void takeMatrixSize(int *aRowCount, int *aColumnCount)
{
    do {
        cout << "How many rows should have a new matrix?" << endl;
        cin >> *aRowCount;
        
    } while (*aRowCount < 1);
    
    do {
        cout << "How many columns should have the matrix?" << endl;
        cin >> *aColumnCount;
        
    } while (*aColumnCount < 1);
}

int main(int argc, const char * argv[])
{
    matrix = NULL;
    int theRowCount = 0;
    int theColumnCount = 0;
    pthread_mutex_init(&readingMutex, NULL);
    pthread_mutex_init(&writingMutex, NULL);
    
    takeMatrixSize(&theRowCount, &theColumnCount);
    generateMatrix(theRowCount, theColumnCount);
    
    int theInputedNumber;
    do {
        cout << "Press 1 to cteate a computer generated matrix." << endl;
        cout << "Press 2 to create your own matrix." << endl;
        cin >> theInputedNumber;
        getchar();
        
        switch (theInputedNumber)
        {
            case 1:
                fillMatrixByComputer();
                break;
                
            case 2:
                fillMatrixByUser();
                break;
                
            default:
                cout << endl;
                break;
        }
    } while (theInputedNumber != 1 && theInputedNumber != 2);
    
    decision();
    showMatrix();
    destroyMatrix();
    
    pthread_mutex_destroy(&readingMutex);
    pthread_mutex_destroy(&writingMutex);
    
    return 0;
}
