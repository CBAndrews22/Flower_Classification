#include <iostream>
#include <vector>
#include <fstream>

bool readData(std::string& fileName, std::vector<std::vector<float> >& data);

int main(){
    // // Reading data in from text file
    std::vector<std::vector<float> > data; // 2D vector to store data
    std::string fileName;
    fileName = "toy_data_set.txt";
    readData(fileName , data);
    int numFeatures, numInstances;

    numFeatures = data[0].size() - 1;
    numInstances = data.size();

    for(int i = 0; i < data.size(); i++){
        for(int j = 0; j < data[i].size(); j++){
            std::cout << data[i][j] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\nThis data set has " << numFeatures << " Features and " << numInstances << "rows!\n";




    // Prompt user for forward search or back elimination


    return 0;

}


bool readData(std::string& fileName, std::vector<std::vector<float> >& data){
    std::ifstream dataFile;
    dataFile.open(fileName, std::ios::in);
    if(dataFile.fail()){
        std::cout << "Error: file failed to open.\n";
        return false;
    } 
    
    float x;
    int i =0;
    data.clear();
    data.push_back(std::vector<float>());

    while(dataFile >> x )
    {
        if(dataFile.peek() == '\n')
        {
            data[i].push_back(x);
            data.push_back(std::vector<float>());
            //std::cout << x << ' ';
            i++;
            //std::cout << '\n';
        }
        else
        {
            data[i].push_back(x);
            //std::cout << x << " ";
        }
    }
    return true;
}