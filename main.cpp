#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>

bool readData(std::string& fileName, std::vector<std::vector<float> >& data);
void printData(const std::vector<std::vector<float> >& data);
int nearestNeighbor(int index, const std::vector<int>& featureSet, const std::vector<std::vector<float> >& data );
float findDistance(int index1, int index2, const std::vector<int>& featureSet, const std::vector<std::vector<float> >& data );
float featureAccuracy(std::vector<int>& featureSet, std::vector<std::vector<float> >& data);
bool inSet(const int& possibleFeature,const std::vector<int>& curSet);
std::vector<int> forwardSelection(std::vector<std::vector<float> >& data);
void printFeatureSet(const std::vector<int> featureSet);

int main(){
    // // Reading data in from text file
    std::vector<std::vector<float> > data; // 2D vector to store data
    std::vector<int> curFeatureSet;
    std::string fileName;
    fileName = "CS170_Large_Data__96.txt";
    readData(fileName , data);
    printData(data);

    int numFeatures, numInstances;
    float accuracy;

    numFeatures = data.size() - 1;
    numInstances = data[0].size();


    std::cout << "\nThis data set has " << numFeatures << " Features and " << numInstances << " rows!\n";

    //curFeatureSet.push_back(1);
    curFeatureSet.push_back(1);

    forwardSelection(data);

    // Prompt user for forward search or back elimination

    return 0;

}

bool readData(std::string& fileName, std::vector<std::vector<float> >& data)
{
    std::ifstream dataFile;
    dataFile.open(fileName, std::ios::in);
    if(dataFile.fail())
    {
        std::cout << "Error: file failed to open.\n";
        return false;
    } 
    
    float x;
    int i =0;
    data.clear();
    data.push_back(std::vector<float>());
    char peek;

    while(dataFile >> x )
    { 
        data[i].push_back(x);
        peek = dataFile.peek();
        if(dataFile.peek() == '\r') break;
        data.push_back(std::vector<float>());
        i++;
    }
    i = 0;
    while(dataFile >> x)
    {
        data[i].push_back(x);
        i++;
        if(dataFile.peek() == '\r') i = 0;
       
    }
    return true;
};

void printData(const std::vector<std::vector<float> >& data)
{
    int numColumns, numRows;
    numColumns = data.size();
    numRows = data[0].size();

    for(int i = 0; i < numRows; i++)
    {
        for(int j = 0; j < numColumns; j++)
        {
            std::cout << data[j][i] << "\t";
        }
        std::cout << "\n";
    }
}

void printFeatureSet(const std::vector<int> featureSet)
{
    std::cout << "{";
    for(int i = 0; i < featureSet.size(); i++)
    {
        std::cout << featureSet[i];
        if(i == featureSet.size() - 1)
        {
            std::cout << "}";
        }
        else
        {
            std::cout << ", ";
        }
        
    }
}

int nearestNeighbor(int index, const std::vector<int>& featureSet, const std::vector<std::vector<float> >& data )
{
    int numRows = data[0].size();
    int curNearestNeighbor;
    float indexDistance, curPointDistance, curMinDistance, tempDistance;
    curMinDistance = 99999;

    for(int i = 0; i < numRows; i++)
    {
        if(i != index)
        {
            curPointDistance = findDistance(index, i, featureSet, data);
            if(curPointDistance < curMinDistance)
            {
                curMinDistance = curPointDistance;
                curNearestNeighbor = i;
            }
            //std::cout<< "Testing Index " << i << " against " << index << "  Distance = " << tempDistance << '\n';
        }
    }
    //std::cout << "Index " << index << " nearest neighbor is index " << curNearestNeighbor << '\n';
    return curNearestNeighbor;
}

float findDistance(int index1, int index2, const std::vector<int>& featureSet, const std::vector<std::vector<float> >& data )
{
    int curFeature;
    float sumSquared = 0;
    for(int i = 0; i < featureSet.size(); i++)
    {
        sumSquared += std::pow(data[featureSet[i]][index1] - data[featureSet[i]][index2],2);
    }
    return std::sqrt(sumSquared);
}

float featureAccuracy(std::vector<int>& featureSet, std::vector<std::vector<float> >& data)
{
    //Given a feature set and data return accuracy of that feature set
    float numRows = data[0].size();
    float NN,
        numCorrect = 0;
    for(int i = 0; i <= numRows; i++)
    {
        NN = nearestNeighbor(i, featureSet, data);
        if(data[0][i] == data[0][NN])
        {
            numCorrect++;
        }
    }
    std::cout << "Correct: " << numCorrect << '\n' << "Total: " << numRows << '\n';
    std::cout << "Accuracy = " << numCorrect/numRows << '\n';

    return numCorrect/numRows;
}

std::vector<int> forwardSelection(std::vector<std::vector<float> >& data)
{
    int numColumns = data.size(),
        numRows = data[0].size();
    
    float tempAccuracy;
    float highestAccuracy = 0;
    float testHighAccuracy = 0;


    std::vector<int> curFeatures,
                     testFeatures,
                     curBestFeatures,
                     testBestFeatures;
    // test for the number of features
    for(int j = 1; j < numColumns; j++)
    {
        // Add each feature to the set unless already in set
        for(int i = 1; i < numColumns; i++)
        {
        //test feature set not in curFeatures
            testFeatures = curFeatures; // test = cur
            if(!inSet(i, curFeatures)) // if not in set
            {
                testFeatures.push_back(i); // add to test set
                std::cout << "Testing set ";
                printFeatureSet(testFeatures);
                std::cout << "\n";
                tempAccuracy = featureAccuracy(testFeatures, data); //test set accuracy
                if(tempAccuracy > testHighAccuracy)  // if better than current top save
                {
                    testHighAccuracy = tempAccuracy;
                    testBestFeatures = testFeatures;
                }
                std::cout << "Accuracy for feature ";
                printFeatureSet(testFeatures); 
                std::cout << " is " << tempAccuracy << "\n\n";
            }
        }
        std::cout << "Round " << j << " best features set is ";
        printFeatureSet(testBestFeatures);
        std::cout << " with an accuracy of " << testHighAccuracy << '\n';
        if(testHighAccuracy > highestAccuracy)
        {
            curBestFeatures = testBestFeatures;
            highestAccuracy = testHighAccuracy;
        }
        std::cout << "the current best accuracy is " << highestAccuracy << '\n';
        curFeatures = testBestFeatures;
        testHighAccuracy = 0;

    }
    std::cout << "The best feature set is ";
    printFeatureSet(curBestFeatures);
    std::cout << " with an accuracy of " << highestAccuracy << '\n';
    return curBestFeatures;

}

bool inSet(const int& possibleFeature,const std::vector<int>& curSet)
{
    for(int i = 0; i < curSet.size(); i++)
    {
        if( possibleFeature == curSet[i]) return true;
    }
    return false;
}