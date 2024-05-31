// UMBC - CMSC 341 - Fall 2023 - Proj2
#include "satnet.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    void getShuffle(vector<int> & array){
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution
};

class Tester{
public:
    double sampleTimeMeasurement(SatNet & aNet, int tempArray[], int arraySize);
    bool insertsampleTimeMeasurement(int arraySize);
    bool removesampleTimeMeasurement(int arraySize);
    bool insertnormaltest();
    bool insertedgetest();
    bool inserttesthelper(const SatNet& testnetwork, int size);
    bool removenormaltest();
    bool removeedgetest();
    bool removebalancedtest();
    int getBalance(Sat *aNode);
    bool checkbalanced(Sat *aNode);
    int height(Sat* node);
    bool findnormaltest();
    bool finderrortest();
    bool inserterrortest();
    bool countSatellitetest();
    bool removedeorbitedttest();
    bool insertbalancedtest();
    bool testBalancedBSTHelper2(Sat* root);
    bool insertbalancedtestBST();
    bool assignmentoperatornormal();
    bool assignmentoperatorerror();
    bool removebalancedtestBST();
    bool assignmentoperatorhelper(const SatNet& net1, const SatNet& net2);
    bool assignmentoperatorhelper2(const Sat* lhs, const Sat* rhs);
    bool removerrortest();


};
int main(){
    Tester tester;
    Random idGen(MINID,MAXID);
    Random inclinGen(0,3);  // there are 4 inclination
    Random altGen(0,3);     // there are 4 altitudes
    {
        SatNet network;
        int teamSize = 10;
        int tempID = 0;
        int ID = 0;
        for(int i=0;i<teamSize;i++){
            ID = idGen.getRandNum();
            if (i == teamSize / 2) tempID = ID;//we store this ID for later use
            Sat satellite(ID,
                          static_cast<ALT>(altGen.getRandNum()),
                          static_cast<INCLIN>(inclinGen.getRandNum()));
            network.insert(satellite);
        }

    }
    SatNet network1;
    int size = 1000;
    int tempIDs[1001] = {0};
    int id = 0;
    for(int i=0;i<size;i++){
        id = idGen.getRandNum();
        tempIDs[i] = id;//we store this ID for later use
        Sat satellite(id,
                      static_cast<ALT>(altGen.getRandNum()),
                      static_cast<INCLIN>(inclinGen.getRandNum()));
        network1.insert(satellite);
    }
    cout << endl << "Calling Tester::sampleTimeMeasurement(...): " << endl;
    cout << "\tFinding 1000 nodes takes " << tester.sampleTimeMeasurement(network1, tempIDs, size) << " seconds." << endl;

    cout<<"-----------------Insert test functions ---------------"<<endl;
    Tester avaltree1;
    if (avaltree1.insertnormaltest()) {
        cout << "The insert normal case test function passed \n";
    } else {
        cout << "The insert normal case test function failed\n";
    }
    Tester avaltree2;
    if (avaltree2.insertedgetest()) {
        cout << "The insert edge case test function passed \n";
    } else {
        cout << "The insert edge case test function failed\n";
    }
    Tester avaltree3;
    if (avaltree3.inserterrortest()) {
        cout << "The insert error case test function passed \n";
    } else {
        cout << "The insert error case test function failed\n";
    }
    Tester avaltree4;
    if (avaltree4.insertbalancedtest()) {
        cout << "The BST property after multiple inserts test function passed \n";
    } else {
        cout << "The BST property after multiple inserts test function failed\n";
    }
    Tester avaltree5;
    if (avaltree5.insertbalancedtestBST()) {
        cout << "The BST property after multiple inserts is passed  \n";
    } else {
        cout << "The BST property after multiple inserts is failed\n";
    }
    Tester avaltree6;
    int size1 = 1000;

    if (avaltree6.insertsampleTimeMeasurement(size1)) {
        cout << "The insertsampleTimeMeasurementhelper test function is passed  \n";
    } else {
        cout << "The insertsampleTimeMeasurementhelper test function is failed\n";
    }
    cout<<"-------------------Remove test functions -----------------"<<endl;
    Tester removetree;
    if(removetree.removenormaltest()){
        cout<<"The remove mormal case test function passed \n";
    } else {
        cout << "The remove normal case test function failed\n";
    }
    Tester removetree2;
   if (removetree2.removeedgetest()) {
       cout<<"The remove error case test function passed \n";
   } else {
       cout << "The remove error case test function failed\n";
   }
    Tester removetree3;
    if (removetree3.removerrortest()) {
        cout<<"The remove edge case test function passed \n";
    } else {
        cout << "The remove edge case test function failed\n";
    }
    Tester removebalanced;
    if (removebalanced.removebalancedtest()) {
        cout<<"The remove balanced (after multiple removes)test function passed \n";
    } else {
        cout << "The remove balanced (after multiple removes)test function failed\n";
    }
    Tester removebalancedBST;
    if (removebalancedBST.removebalancedtestBST()) {
        cout<<"The BST property after multiple remove test function passed \n";
    } else {
        cout << "The BST property after multiple remove test function failed\n";
    }


    Tester tester2;
    int size2 = 1000;
    if (tester2.removesampleTimeMeasurement(size2)) {
        cout << "The removesampleTimeMeasurement test function is passed  \n";
    } else {
        cout << "The removesampleTimeMeasurement test function is failed \n";
    }
    cout<<"-------------------Find test functions -----------------"<<endl;
    Tester findsatellite1;
    if (findsatellite1.findnormaltest()) {
        cout<<"The find satellite normal case test function passed \n";
    } else {
        cout << "The find satellite normal case test function failed\n";
    }
    Tester findsatellite2;
    if (findsatellite2.finderrortest()) {
        cout<<"The find satellite error case test function passed \n";
    } else {
        cout << "The find satellite error case test function failed\n";
    }
    cout<<"-----------------count test functions ------------------"<<endl;
    Tester countsatellite;
    if (countsatellite.countSatellitetest()) {
        cout<<"The count satellite test function passed \n";
    } else {
        cout << "The count satellite test function failed\n";
    }
    cout<<"-----------------Assingment operator test functions ---------------"<<endl;
    Tester assignmenttest;
    if (assignmenttest.assignmentoperatornormal()) {
        cout<<"The assignment operator for normal case test function passed \n";
    } else {
        cout << "The assignment operator for normal case test function failed\n";
    }
    Tester assignmenttest2;
    if (assignmenttest2.assignmentoperatorerror()) {
        cout<<"The assignment operator for error case test function passed \n";
    } else {
        cout << "The assignment operator for error case test function failed\n";
    }
    cout<<"-----------------Remove Deorbited test functions ---------------"<<endl;
    Tester deroiteddgree;
    if (deroiteddgree.removedeorbitedttest()) {
        cout<<"The remove deorbited test function passed \n";
    } else {
        cout << "The remove deorbited test function failed\n";
    }


    return 0;
}

double Tester::sampleTimeMeasurement(SatNet & aNet, int tempArray[], int arraySize){
    double T = 0.0;//stores running times
    clock_t start, stop;//stores the clock ticks while running the program
    start = clock();
    // the algorithm to be measured
    for (int j=0;j<arraySize;j++){
        aNet.findSatellite(tempArray[j]);
    }
    stop = clock(); //stops the clock
    T = stop - start;//number of clock ticks the algorithm took
    double measureTime = T/CLOCKS_PER_SEC;//time in seconds
    return measureTime;
}

bool Tester::insertsampleTimeMeasurement(int arraySize){
    SatNet aNet; //creating a SatNet network
    bool result  =true;
    double T1 = 0.0;
    clock_t start, stop; //Assigning start and stop
    start = clock();
    Random test1(MINID,MAXID); //Creating a random object
    for (int j=0;j<arraySize;j++){ //looping through to insert
        aNet.insert(Sat(test1.getRandNum()));
    }

    stop = clock(); //stops the time after inserting
    T1 = stop - start;//number of clock ticks the algorithm took
    SatNet aNet2;
    Random test(MINID,MAXID);
    double T2 = 0.0;
    clock_t start1, stop1;
    start1 = clock();
    for (int j=0;j<2*arraySize;j++){ //looping throug to insert again
        aNet2.insert(Sat(test.getRandNum()));
    }
    stop1 = clock();
    T2 = stop1 - start1;

    double measureTime = T1;//time in seconds
    double measureTime2 = T2;//time in seconds
    //return measureTime;
    double ratio = measureTime2 / (2 * measureTime); //measuring the ration
    return result && (ratio >= 0.7 && ratio <= 1.5); //comparing within the range and returning boolean if is passsed or failed

}

bool Tester::removesampleTimeMeasurement(int arraySize){
    SatNet aNet; //Creating a new SatNet network
    bool result  =true;
    Random test1(MINID,MAXID);
    for (int j=0;j<arraySize;j++){ //inserting inside the loop
        aNet.insert(Sat(test1.getRandNum()));
    }
    double T1 = 0.0;
    clock_t start, stop;
    start = clock(); //starts the clock
    // the algorithm to be measured
    for (int j=0;j<arraySize;j++){
        aNet.remove(j); //removes the id to measure the time of removing ids
    }
    stop = clock();//stops the clock


    SatNet aNet2; //Create aSatnet object
    Random test(MINID,MAXID);
    double T2 = 0.0;
    for (int j=0;j<2*arraySize;j++){
        aNet2.insert(Sat(test.getRandNum()));
    }
    clock_t start1, stop1;
    start1 = clock();
    for (int j=0;j<2*arraySize;j++){
        aNet.remove(j);
    }
    stop1 = clock();
    T2 = stop1 - start1;
    T1 = stop - start;//number of clock ticks the algorithm took
    double measureTime = T1;//time in second
    double measureTime2 = T2;//time in seconds
    //return measureTime;
    double ratio = measureTime2 / (2 * measureTime); //measure the remove time for the two for loops
    return result && (ratio >= 0.7 && ratio <= 1.5);

}


bool Tester::inserttesthelper(const SatNet& testnetwork, int size){
    int count = 0;

    for (int i = MINID; i <= MAXID; i++) { //loops through the network
        if (testnetwork.findSatellite(i)) { //finds the satellite and counts the total numbers of satellites inserted
            count++;//increments the count if they found the size
            if(count == size){
                return true; //returs true if the size and the count are equal
            }
        }
    }
    //cout<<count<<endl;
    return false; // Return true if all insertions were successful
}
bool Tester::insertnormaltest(){
    SatNet testnetwork; //creating the Satnet netwok
    ALT altinput = DEFAULT_ALT;
    INCLIN inclineinput = DEFAULT_INCLIN;
    Sat data1(10002, altinput,inclineinput); //creating the SAt objects
    Sat data2(10003,altinput,inclineinput);
    Sat data3(10004,altinput,inclineinput);
    testnetwork.insert(data1);//inserting all the datas
    testnetwork.insert(data2);
    testnetwork.insert(data3);

    bool insertbetweenrange = inserttesthelper(testnetwork,3); //returs true or false based on the helper function
    return insertbetweenrange;
}
bool Tester::insertedgetest(){
    SatNet testnetwork;
    ALT altinput = DEFAULT_ALT;
    INCLIN inclineinput = DEFAULT_INCLIN;
    Sat data1(10000, altinput,inclineinput); //inserting the Min and Max values
    Sat data2(99999,altinput,inclineinput);

    testnetwork.insert(data1);
    testnetwork.insert(data2);

    bool insertcheck1 = inserttesthelper(testnetwork,2);//returns true or false based on the helper function

    return insertcheck1;
}
bool Tester::inserterrortest(){
    SatNet testnetwork;
    ALT altinput = DEFAULT_ALT;
    INCLIN inclineinput = DEFAULT_INCLIN;
    Sat data1(3, altinput,inclineinput);
    Sat data2(-1,altinput,inclineinput);
    Sat data3(1000000,altinput,inclineinput); //inserts the values that shouldn't be inserted
    testnetwork.insert(data1);
    testnetwork.insert(data2);
    testnetwork.insert(data3);

    bool insertbetweenrange = inserttesthelper(testnetwork,3);

    return !insertbetweenrange; //if the bool is false it is true

}
bool Tester::removenormaltest(){
    SatNet removetest;
    ALT altinput = DEFAULT_ALT;
    INCLIN inclineinput = DEFAULT_INCLIN;
    Sat data1(10000, altinput,inclineinput);
    Sat data2(20000,altinput,inclineinput);
    Sat data3(15000,altinput,inclineinput);
    removetest.insert(data1);
    removetest.insert(data2);
    removetest.insert(data3);
    int removeid = 15000;
    removetest.remove(removeid);
    bool removed = !removetest.findSatellite(removeid);
    return removed;
}

bool Tester::removeedgetest(){
    SatNet removetest;
    ALT altinput = DEFAULT_ALT;//setting the alt to default
    INCLIN inclineinput = DEFAULT_INCLIN;//setting the incline to default
    Sat data1(20000, altinput,inclineinput); //inserting values to
    Sat data2(30000,altinput,inclineinput);
    Sat data3(25000,altinput,inclineinput);
    removetest.insert(data1);
    removetest.insert(data2);
    removetest.insert(data3);
    int removeid = 35000;
    removetest.remove(removeid); //removing one id
    bool removed = !removetest.findSatellite(removeid);//if it is not found
    return removed; //then it is true
}

bool Tester::removerrortest(){
    SatNet removetest;
    ALT altinput = DEFAULT_ALT;//setting the alt to default
    INCLIN inclineinput = DEFAULT_INCLIN;//setting the incline to default
    Sat data1(10000, altinput,inclineinput); //inserting values to
    Sat data2(99999,altinput,inclineinput);

    removetest.insert(data1);
    removetest.insert(data2);

    int removeid = 99999;
    removetest.remove(removeid); //removing one id
    bool removed = !removetest.findSatellite(removeid);//if it is not found
    return removed; //then it is true
}
bool Tester::removebalancedtest(){
    SatNet balancedtree;
    ALT altinput = DEFAULT_ALT; //setting the alt to default
    INCLIN inclineinput = DEFAULT_INCLIN; //setting the incline to default
    for(int i = 1;i<=300;i++){
        Sat data1(i, altinput,inclineinput); //looping through to insert
        balancedtree.insert(data1);
    }
    //balancedtree.dumpTree();
    for(int i = 1;i<=150;i++){
        balancedtree.remove(i); //removing inside the for loop

    }
    bool balanced = checkbalanced(balancedtree.m_root); //checks whether the remove is balanced
    return balanced;//returns true or false after checking the tree
}
bool Tester::removebalancedtestBST(){
    SatNet balancedtree;
    ALT altinput = DEFAULT_ALT;
    INCLIN inclineinput = DEFAULT_INCLIN;
    for(int i = 1;i<=5;i++){
        Sat data1(i+10000, altinput,inclineinput);
        balancedtree.insert(data1);
    }

    for(int i = 1;i<=3;i++){
        balancedtree.remove(i);

    }
    bool balanced = testBalancedBSTHelper2(balancedtree.m_root);
    return balanced;
}
bool Tester::insertbalancedtest(){
    SatNet balancedtree;
    ALT altinput = DEFAULT_ALT;
    INCLIN inclineinput = DEFAULT_INCLIN;
    for(int i = 1;i<=300;i++){
        Sat data1(i +10000, altinput,inclineinput);
        balancedtree.insert(data1);
    }
    bool balanced = checkbalanced(balancedtree.m_root);
    return balanced;
}
bool Tester::insertbalancedtestBST(){
    SatNet balancedtree;//creates a Satnet object
    ALT altinput = DEFAULT_ALT;
    INCLIN inclineinput = DEFAULT_INCLIN;
    for(int i = 1;i<=5;i++){
        Sat data1(i +10000, altinput,inclineinput);
        balancedtree.insert(data1); //looping through and inserts the ids

    }
    return testBalancedBSTHelper2(balancedtree.m_root); //Calls the helper function

}
bool Tester::testBalancedBSTHelper2(Sat* root){
    if (root == nullptr){
        return true;
    }
    bool result  = true;
    bool result2 = true;
    if (root->getLeft()!= nullptr){ //checks if the root's left is not nullptr
        result  = root->m_left->m_id < root->m_id;
        result = result && testBalancedBSTHelper2(root->m_left); //then calls the helper function to check if it is balanced

    }
    if (root->getRight()!= nullptr) { //if the rool's right is not nullptr
        result2 = root->m_right->m_id > root->getID();
        result2 = result2 && testBalancedBSTHelper2(root->m_right);//then calls the helper function to check if it balanced

    }
    return result && result2; // both are true then it is true

}


bool Tester::checkbalanced(Sat *aNode){
    if(aNode==nullptr){ //if aNode is equal to nullptr
        return true; //return true
    }
    int balancefactor = getBalance(aNode);
    if (balancefactor >1||balancefactor <-1){ //checks the balancefactor
        return false;
    }
    return checkbalanced(aNode->m_left) && checkbalanced(aNode->m_right); //Calls the check_balanced function recursively
}
int Tester::getBalance(Sat *aNode){
    if (aNode== nullptr)
        return 0;
    return height(aNode->getLeft()) - height(aNode->getRight()); //returns the balance factor of the tree
}

int Tester::height(Sat* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->m_height; //returns the node's height
}
bool Tester::findnormaltest() {
    SatNet findnetwork;
    //Alt and Incline values to default
    ALT altInput = DEFAULT_ALT;
    INCLIN inclineInput = DEFAULT_INCLIN;
    Sat satellite1(30000, altInput, inclineInput);
    findnetwork.insert(satellite1); //Inserts the Satellite id to the SatNet object

    bool foundSatellite1 = findnetwork.findSatellite(30000); //Calls the find Satellite and cheks if it is found
    return foundSatellite1;
}

bool Tester::finderrortest() {
    SatNet findnetwork;

    ALT altInput = DEFAULT_ALT; //setting values to default
    INCLIN inclineInput = DEFAULT_INCLIN;
    Sat satellite1(30000, altInput, inclineInput);
    findnetwork.insert(satellite1); //inserting the satlelite id

    bool foundSatellite1 = findnetwork.findSatellite(1); //finding the satellite
    return !foundSatellite1;//if not found then the function is passed
}
bool Tester::countSatellitetest(){
    SatNet satnode;
    ALT altinput = DEFAULT_ALT;
    //Inserting the values on different ids
    Sat data1(49871, altinput,I53);
    Sat data2(54513 ,altinput,I53);
    Sat data3(62498 ,altinput,I70);
    satnode.insert(data1);
    satnode.insert(data2);
    satnode.insert(data3);
    INCLIN commondegree= I53;
    int count = satnode.countSatellites(commondegree); //counts the satellite with the similar inline
    int expected = 2;
   if(count ==expected) //if the count and expted is equal
       return true; //returns true
   return false;//if not equal returns false
}
bool Tester::removedeorbitedttest(){
    SatNet satnode;
    ALT altinput = DEFAULT_ALT;
    Sat data1(49871, altinput,I53,ACTIVE);
    Sat data2(54513 ,altinput,I53,ACTIVE);
    Sat data3(62498 ,altinput,I70,DEORBITED);
    satnode.insert(data1);
    satnode.insert(data2);
    satnode.insert(data3);
    satnode.removeDeorbited(); //removes the sattleits that's state is derobited

    if(satnode.countSatellites(I70)==0){ //if the count is 0
        return true;//then it is deleted , and returs true
    }
    return false;
}

bool Tester::assignmentoperatornormal() {
    // Create the original SatNet
    SatNet original;
    ALT altValue = DEFAULT_ALT;
    INCLIN inclinValue = DEFAULT_INCLIN;
    original.insert(Sat(10000, altValue, inclinValue));
    original.insert(Sat(20000, altValue, inclinValue));

    // Create a new SatNet and assign the original to it
    SatNet copyoriginal;
    original = copyoriginal;
    bool equal = assignmentoperatorhelper(original, copyoriginal);//check wthere it is equal
    return equal; //return true or false
}
bool Tester::assignmentoperatorhelper(const SatNet& lhs, const SatNet& rhs) {
    bool result = true;
    if (lhs.m_root != nullptr) {
        result  = assignmentoperatorhelper2(lhs.m_root,rhs.m_root); //checks if the root is equal if the root is not pointing to nullptr
    } else if (lhs.m_root == nullptr) { //if the root is empty
        result = result && (lhs.m_root == nullptr) && (rhs.m_root == nullptr);//check whether both the roots are empty
    }

    return result;//returns the result
}
bool Tester::assignmentoperatorhelper2(const Sat* lhs, const Sat* rhs){
    bool result = true;

    result = result && (lhs == nullptr && rhs ==nullptr);
    result = result &&(lhs->getState()==rhs->getState()); //checks the state
    result = result &&(lhs->getInclin()==rhs->getInclin());//checks the incline
    result = result &&(lhs->getAlt()==rhs->getAlt());//checks the Alt
    result = result &&(lhs->getID()==rhs->getID());//checks the ID
    result = result && assignmentoperatorhelper2(lhs->m_left, rhs->m_left); //recursivly checks all the subtreees on the left
    result = result && assignmentoperatorhelper2(lhs->m_right, rhs->m_right);//recursivly checks all the subtreees on the right
    return result;//returs the result
}

bool Tester::assignmentoperatorerror() {
    SatNet original;
    SatNet copyoriginal;
    original = copyoriginal;
    bool equal = assignmentoperatorhelper(original, copyoriginal);//check wthere it is equal
    return equal; //return true or false
}





