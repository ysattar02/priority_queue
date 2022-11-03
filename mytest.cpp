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

