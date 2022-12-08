#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>

bool readData(std::string& fileName, std::vector<std::vector<float> >& data);
void printData(const std::vector<std::vector<float> >& data);
int nearestNeighbor(int index, const std::vector<int>& featureSet, const std::vector<std::vector<float> >& data );
float findDistance(int index, const std::vector<int>& featureSet, const std::vector<std::vector<float> >& data );
float featureAccuracy(std::vector<int>& featureSet, std::vector<std::vector<float> >& data);
bool inSet(const int& possibleFeature,const std::vector<int>& curSet);

int main(){
    // // Reading data in from text file
    std::vector<std::vector<float> > data; // 2D vector to store data
    std::vector<int> curFeatureSet;
    std::string fileName;
    fileName = "toy_data_set.txt";
    readData(fileName , data);
    printData(data);

    int numFeatures, numInstances;
    float accuracy;

    numFeatures = data.size() - 1;
    numInstances = data[0].size();


    std::cout << "\nThis data set has " << numFeatures << " Features and " << numInstances << " rows!\n";

    //curFeatureSet.push_back(1);
    curFeatureSet.push_back(1);

    accuracy = featureAccuracy(curFeatureSet, data);
    std::cout << std::setprecision(4) << accuracy << '\n';

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

    while(dataFile >> x )
    { 
        data[i].push_back(x);
        if(dataFile.peek() == '\n') break;
        data.push_back(std::vector<float>());
        i++;
    }
    i = 0;
    while(dataFile >> x)
    {
        data[i].push_back(x);
        i++;
        if(dataFile.peek() == '\n') i = 0;
       
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

int nearestNeighbor(int index, const std::vector<int>& featureSet, const std::vector<std::vector<float> >& data )
{
    int numRows = data[0].size();
    int curNearestNeighbor;
    float indexDistance, curPointDistance, curMinDistance, tempDistance;
    curMinDistance = 99999;
    indexDistance = findDistance(index, featureSet, data);

    for(int i = 0; i < numRows; i++)
    {
        if(i != index)
        {
            curPointDistance = findDistance(i, featureSet, data);
            tempDistance = std::abs(indexDistance - curPointDistance);
            if(tempDistance < curMinDistance)
            {
                curMinDistance = tempDistance;
                curNearestNeighbor = i;
            }
            std::cout<< "Testing Index " << i << " against " << index << "  Distance = " << tempDistance << '\n';
        }
    }
    std::cout << "Index " << index << " nearest neighbor is index " << curNearestNeighbor << '\n';
    return curNearestNeighbor;
}

float findDistance(int index, const std::vector<int>& featureSet, const std::vector<std::vector<float> >& data )
{
    int curFeature;
    float tempDistance = 0;
    for(int i = 0; i < featureSet.size(); i++)
    {
        curFeature = featureSet[i];
        tempDistance += std::pow(data[curFeature][index], 2);
    }
    return std::sqrt(tempDistance);
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
    std::cout << "number correct: " << numCorrect << '\n' << "number of rows: " << numRows << '\n';
    std::cout << numCorrect/numRows << '\n';

    return numCorrect/numRows;
}

std::vector<int> forwardSelection(std::vector<std::vector<float> > data)
{
    int numColumns = data.size(),
        numRows = data[0].size();
    
    float highestAccuracy = 0,
          tempAccuracy;


    std::vector<int> curFeatures,
                     testFeatures;


    for(int i = 1; i < numColumns; i++)
    {
     //test feature set not in curFeatures
        if(!inSet(i, curFeatures))
        {
            testFeatures.push_back(i);
            tempAccuracy = featureAccuracy(testFeatures, data);
            std::cout << "Accuracy for feature " << i << " is " << tempAccuracy << '\n';   
        }
    }
    return curFeatures;

}

bool inSet(const int& possibleFeature,const std::vector<int>& curSet)
{
    for(int i = 1; i <= curSet.size(); i++)
    {
        if( possibleFeature == curSet[i]) return true;
    }
    return false;
}