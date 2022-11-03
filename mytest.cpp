#include "squeue.h"
#include <vector>
#include <algorithm>
#include <string>
#include <stdlib>
#include <random>
// the followings are sample priority functions to be used by SQueue class
// users can define their own priority functions
// Priority functions compute an integer priority for a post.  Internal
// computations may be floating point, but must return an integer.
int priorityFn1(const Post &post);// works with a MAXHEAP
int priorityFn2(const Post &post);// works with a MINHEAP

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
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
        else{ //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
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
    bool test_normalInsert(HEAPTYPE heaptype);
    bool test_remove(HEAPTYPE heaptype);
    bool test_changeP(HEAPTYPE heaptype);
    bool test_merge(HEAPTYPE heaptype);
    bool testCC(HEAPTYPE heaptype);
    bool testAO(HEAPTYPE heaptype);
    bool testBadDeQueue(HEAPTYPE heaptype);
    bool testBadMerge(HEAPTYPE heaptype);
};

int main(){
    Tester tester;
    tester.test_normalInsert(MINHEAP);
    tester.test_normalInsert(MAXHEAP);
    tester.test_remove(MINHEAP);
    tester.test_remove(MAXHEAP);
    tester.test_changeP(MINHEAP);
    tester.test_changeP(MAXHEAP);
    tester.test_merge(MINHEAP);
    tester.test_merge(MAXHEAP);
    tester.testCC(MINHEAP);
    tester.testCC(MAXHEAP);
    tester.testAO(MINHEAP);
    tester.testAO(MAXHEAP);
    tester.testBadDeQueue(MAXHEAP);
    tester.testBadDeQueue(MINHEAP);
    tester.testBadMerge(MINHEAP);
    tester.testBadMerge(MAXHEAP);
    return 0;
}

int priorityFn1(const Post &post) {
    //needs MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [1-510]
    //the highest priority would be 500+10 = 510
    //the lowest priority would be 0+1 = 1
    //the larger value means the higher priority
    int priority = post.getNumLikes() + post.getInterestLevel();
    return priority;
}

int priorityFn2(const Post &post) {
    //needs MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [2-55]
    //the highest priority would be 1+1 = 2
    //the lowest priority would be 5+50 = 55
    //the smaller value means the higher priority
    int priority = post.getPostTime() + post.getConnectLevel();
    return priority;
}

bool Tester::test_normalInsert(HEAPTYPE heaptype){
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn2, heaptype);
    for (int i=0;i<300;i++) {
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue1.insertPost(aPost);
        if (queue1.m_size != i + 1) {
            cout << "Insert Failed" << endl;
            return false;
        }
        if (queue1.r_heap_property(queue1.m_heap) == false) {
            cout << "Insert Failed" << endl;
            return false;
        }
    }
    cout << "Insert Successful" << endl;
    return true;

}

bool Tester::test_remove(HEAPTYPE heaptype){
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn2, heaptype);
    for (int i=0;i<300;i++) {
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue1.insertPost(aPost);
        if (queue1.m_size != i + 1) {
            cout << "Insert Failed" << endl;
            return false;
        }
        if (queue1.r_heap_property(queue1.m_heap) == false) {
            cout << "Insert Failed" << endl;
            return false;
        }
    }
    for (int i = 300; i > 0; i--){
        queue1.getNextPost();
        if (queue1.m_size != i - 1) {
            cout << "Remove Failed" << endl;
            return false;
        }
        if (queue1.r_heap_property(queue1.m_heap) == false) {
            cout << "Remove Failed" << endl;
            return false;
        }
    }
    cout << "Removal Successful" << endl;
    return true;
}

bool Tester::test_changeP(HEAPTYPE heaptype){
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn2, heaptype);
    vector <Post> v1, v2;
    for (int i=0;i<300;i++) {
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue1.insertPost(aPost);
        v1.push_back(aPost);
        if (queue1.m_size != i + 1) {
            cout << "Insert Failed" << endl;
            return false;
        }
        if (queue1.r_heap_property(queue1.m_heap) == false) {
            cout << "Insert Failed" << endl;
            return false;
        }
    }
    //square brackets is the capture clause
    std::sort(v1.begin(), v1.end(),
              [](const Post& p1, const Post& p2){return p1.getPostID() < p2.getPostID();}); //lambda expression
    queue1.setPriorityFn(priorityFn1, heaptype);
    for (int i = 300; i > 0; i--){
        v2.push_back(queue1.getNextPost());
        if (queue1.m_size != i - 1) {
            cout << "Remove Failed" << endl;
            return false;
        }
        if (queue1.r_heap_property(queue1.m_heap) == false) {
            cout << "Remove Failed" << endl;
            return false;
        }
    }
    std::sort(v2.begin(), v2.end(),
              [](const Post& p1, const Post& p2){return p1.getPostID() < p2.getPostID();});
    if (std::equal(v1.begin(), v1.end(), v2.begin(),
                   [](const Post& p1, const Post& p2) {return p1.getPostID() == p2.getPostID();}) == false){
        cout << "Rebuild Failed" << endl;
        return false;
    }
    cout << "Rebuild Successful" << endl;
    return true;
}

bool Tester::test_merge(HEAPTYPE heaptype){
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn2, heaptype);
    for (int i=0;i<10;i++) {
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue1.insertPost(aPost);
        if (queue1.m_size != i + 1) {
            cout << "Insert Failed" << endl;
            return false;
        }
        if (queue1.r_heap_property(queue1.m_heap) == false) {
            cout << "Insert Failed" << endl;
            return false;
        }
    }
    SQueue queue2(priorityFn2, heaptype);
    queue1.mergeWithQueue(queue2);
    if (queue1.m_size == 10){
        cout << "Merge Successful" << endl;
        return true;
    }
    cout << "Merge Failed" << endl;
    return false;
}

bool Tester::testCC(HEAPTYPE heaptype){
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn2, heaptype);
    SQueue queue3(queue1); //edge case
    if (queue3.m_size != 0 || queue1.m_size != 0){
        cout << "CC Edge Case Failed" << endl;
        return false;
    }
    cout << "CC Edge Case Test Successful" << endl;
    for (int i=0;i<10;i++) {
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue1.insertPost(aPost);
        if (queue1.m_size != i + 1) {
            cout << "Insert Failed" << endl;
            return false;
        }
        if (queue1.r_heap_property(queue1.m_heap) == false) {
            cout << "Insert Failed" << endl;
            return false;
        }
    }
    SQueue queue2(queue1);
    if (queue1.m_size != queue2.m_size){
        cout << "CC Failed" << endl;
        return false;
    }
    vector <Post> v1, v2;
    for (int i = 0; i < 10; i++){
        v1.push_back(queue1.getNextPost());
        v2.push_back(queue2.getNextPost());
    }
    if (std::equal(v1.begin(), v1.end(), v2.begin(),
                   [](const Post& p1, const Post& p2) {return p1.getPostID() == p2.getPostID();}) == false){
        cout << "CC Failed" << endl;
        return false;
    }
    cout << "CC Successful" << endl;
    return true;
}

bool Tester::testAO(HEAPTYPE heaptype) {
    Random idGen(MINPOSTID,MAXPOSTID);
    Random likesGen(MINLIKES,MAXLIKES);
    Random timeGen(MINTIME,MAXTIME);
    Random conLevelGen(MINCONLEVEL,MAXCONLEVEL);
    Random interestGen(MININTERESTLEVEL,MAXINTERESTLEVEL);
    SQueue queue1(priorityFn2, heaptype);
    SQueue queue3(priorityFn2, heaptype); //edge case
    queue3 = queue1;
    if (queue3.m_size != 0 || queue1.m_size != 0){
        cout << "AO Edge Case Failed" << endl;
        return false;
    }
    cout << "AO Edge Case Test Successful" << endl;
    for (int i=0;i<10;i++) {
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue1.insertPost(aPost);
        if (queue1.m_size != i + 1) {
            cout << "Insert Failed" << endl;
            return false;
        }
        if (queue1.r_heap_property(queue1.m_heap) == false) {
            cout << "Insert Failed" << endl;
            return false;
        }
    }
    SQueue queue2(priorityFn2, heaptype);
    for (int i=0;i<10;i++) {
        Post aPost(idGen.getRandNum(),
                   likesGen.getRandNum(),
                   conLevelGen.getRandNum(),
                   timeGen.getRandNum(),
                   interestGen.getRandNum());
        queue2.insertPost(aPost);
        if (queue2.m_size != i + 1) {
            cout << "Insert Failed" << endl;
            return false;
        }
        if (queue2.r_heap_property(queue2.m_heap) == false) {
            cout << "Insert Failed" << endl;
            return false;
        }
    }
    queue2 = queue1; //checking assignment operator
    queue2 = queue2; //checking self assignment
    if (queue2.m_size != queue1.m_size){
        cout << "AO Failed" << endl;
        return false;
    }
    vector <Post> v1, v2;
    for (int i = 0; i < 10; i++){
        v1.push_back(queue1.getNextPost());
        v2.push_back(queue2.getNextPost());
    }
    if (std::equal(v1.begin(), v1.end(), v2.begin(),
                   [](const Post& p1, const Post& p2) {return p1.getPostID() == p2.getPostID();}) == false){
        cout << "AO Failed" << endl;
        return false;
    }
    cout << "AO Successful" << endl;
    queue1 = queue3;
    if (queue1.m_size != 0){
        cout << "AO Failed" << endl;
        return false;
    }
    return true;
}

bool Tester::testBadDeQueue(HEAPTYPE heaptype){
    SQueue q1(priorityFn2, heaptype);
    cout << "Checking if DeQueue on Empty Queue throw Exception: " << endl;
    try{
        q1.getNextPost();
        cout << "Test Failed" << endl;
        return false;
    }
    catch(out_of_range e){
        cout << e.what() << endl;
        cout << "Test Successful" << endl;
    }
    return true;
}

bool Tester::testBadMerge(HEAPTYPE heaptype){
    SQueue q1(priorityFn2, heaptype);
    SQueue q2(priorityFn1, heaptype);
    cout << "Testing Bad Merge Case (Diff Priority Fnx): " << endl;
    try{
        q1.mergeWithQueue(q2);
        cout << "Test Failed" << endl;
        return false;
    }
    catch(domain_error error){
        cout << error.what() << endl;
        cout << "Bad Merge Test Succesful" << endl;
    }
    return true;
}
